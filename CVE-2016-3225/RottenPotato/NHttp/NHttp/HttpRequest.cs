using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Globalization;
using System.IO;
using System.Net;
using System.Text;

namespace NHttp
{
    public class HttpRequest
    {
        private static readonly ILog Log = LogManager.GetLogger(typeof(HttpRequest));

        private static readonly string[] EmptyStringArray = new string[0];

        public string[] AcceptTypes { get; private set; }

        public Encoding ContentEncoding { get; private set; }

        public int ContentLength { get; private set; }

        public string ContentType { get; private set; }

        public HttpCookieCollection Cookies { get; private set; }

        public HttpFileCollection Files { get; private set; }

        public NameValueCollection Form { get; private set; }

        public NameValueCollection Headers { get; private set; }

        public string HttpMethod { get; private set; }

        public Stream InputStream { get; private set; }

        public NameValueCollection Params { get; private set; }

        public string Path { get; private set; }

        public NameValueCollection QueryString { get; private set; }

        public string RawUrl { get; private set; }

        public string RequestType { get; private set; }

        public NameValueCollection ServerVariables { get; private set; }

        public Uri Url { get; private set; }

        public Uri UrlReferer { get; private set; }

        public string UserAgent { get; private set; }

        public string UserHostAddress { get; private set; }

        public string UserHostName { get; private set; }

        public string[] UserLanguages { get; private set; }

        internal HttpRequest(HttpClient client)
        {
            ParseHeaders(client);

            Form = CreateCollection(client.PostParameters);

            ParseMultiPartItems(client);

            HttpMethod = RequestType = client.Method;

            ParsePath(client);

            ParseRemoteEndPoint(client);

            BuildServerVariables(client);

            BuildParams();

            InputStream = client.InputStream;

            if (InputStream != null)
                InputStream.Position = 0;
        }

        private void ParseHeaders(HttpClient client)
        {
            Headers = CreateCollection(client.Headers);

            string header;

            // Parse Accept.

            if (client.Headers.TryGetValue("Accept", out header))
            {
                string[] parts = header.Split(',');

                HttpUtil.TrimAll(parts);

                AcceptTypes = parts;
            }
            else
            {
                AcceptTypes = EmptyStringArray;
            }

            // Parse Content-Type.

            if (client.Headers.TryGetValue("Content-Type", out header))
            {
                string[] parts = header.Split(new[] { ';' }, 2);

                ContentType = parts[0].Trim();

                if (parts.Length == 2)
                {
                    string[] encoding = parts[1].Trim().Split(new[] { '=' }, 2);

                    if (encoding.Length == 2 && String.Equals(encoding[0], "charset", StringComparison.OrdinalIgnoreCase))
                    {
                        ContentEncoding = Encoding.GetEncoding(encoding[1]);
                    }
                }
            }

            // Parse Content-Length.

            if (client.Headers.TryGetValue("Content-Length", out header))
            {
                int contentLength;

                if (int.TryParse(header, out contentLength))
                    ContentLength = contentLength;
            }

            // Parse Referer.

            if (client.Headers.TryGetValue("Referer", out header))
                UrlReferer = new Uri(header);

            // Parse User-Agent.

            if (client.Headers.TryGetValue("User-Agent", out header))
                UserAgent = header;

            // Parse Accept-Language.

            if (client.Headers.TryGetValue("Accept-Language", out header))
            {
                string[] parts = header.Split(',');

                HttpUtil.TrimAll(parts);

                UserLanguages = parts;
            }
            else
            {
                UserLanguages = EmptyStringArray;
            }

            // Parse Cookie.

            Cookies = new HttpCookieCollection();

            if (client.Headers.TryGetValue("Cookie", out header))
            {
                string[] parts = header.Split(';');

                foreach (string part in parts)
                {
                    string[] partParts = part.Split(new[] { '=' }, 2);

                    string name = partParts[0].Trim();
                    string value = partParts.Length == 1 ? null : partParts[1];

                    Cookies.AddCookie(new HttpCookie(name, value), true);
                }
            }
        }

        private void ParseMultiPartItems(HttpClient client)
        {
            Files = new HttpFileCollection();

            if (client.MultiPartItems == null)
                return;

            foreach (var item in client.MultiPartItems)
            {
                string contentType = null;
                string name = null;
                string fileName = null;

                string header;

                if (item.Headers.TryGetValue("Content-Disposition", out header))
                {
                    string[] parts = header.Split(';');

                    for (int i = 0; i < parts.Length; i++)
                    {
                        string part = parts[i].Trim();

                        if (part.StartsWith("name="))
                            name = ParseContentDispositionItem(part.Substring(5));
                        else if (part.StartsWith("filename="))
                            fileName = ParseContentDispositionItem(part.Substring(9));
                    }
                }

                if (item.Headers.TryGetValue("Content-Type", out header))
                    contentType = header;

                if (name == null)
                {
                    Log.Info("Received multipart item without name");
                    continue;
                }

                if (item.Value != null)
                {
                    Form[name] = item.Value;
                }
                else
                {
                    Files.AddFile(name, new HttpPostedFile((int)item.Stream.Length, contentType, fileName, item.Stream));
                }
            }
        }

        private string ParseContentDispositionItem(string value)
        {
            if (value.Length == 0)
                return value;

            if (value.Length >= 2 && value[0] == '"' && value[value.Length - 1] == '"')
                value = value.Substring(1, value.Length - 2);

            return HttpUtil.UriDecode(value);
        }

        private void ParsePath(HttpClient client)
        {
            RawUrl = client.Request;

            string[] parts = client.Request.Split(new[] { '?' }, 2);

            Path = parts[0];

            if (parts.Length == 2)
                QueryString = CreateCollection(HttpUtil.UrlDecode(parts[1]));
            else
                QueryString = new NameValueCollection();

            string host;
            string port;
            string hostHeader;

            if (client.Headers.TryGetValue("Host", out hostHeader))
            {
                parts = hostHeader.Split(new[] { ':' }, 2);

                host = parts[0];

                if (parts.Length == 2)
                    port = parts[1];
                else
                    port = null;
            }
            else
            {
                var endPoint = client.Server.EndPoint;

                host = endPoint.Address.ToString();
                
                if (endPoint.Port == 80)
                    port = null;
                else
                    port = endPoint.Port.ToString(CultureInfo.InvariantCulture);
            }

            var sb = new StringBuilder();

            sb.Append("http://");
            sb.Append(host);

            if (port != null)
            {
                sb.Append(':');
                sb.Append(port);
            }

            sb.Append(client.Request);

            Url = new Uri(sb.ToString());
        }

        private void ParseRemoteEndPoint(HttpClient client)
        {
            var endPoint = (IPEndPoint)client.TcpClient.Client.RemoteEndPoint;

            UserHostName = UserHostAddress = endPoint.Address.ToString();
        }

        private void BuildServerVariables(HttpClient client)
        {
            ServerVariables = new NameValueCollection();

            // Add all headers.

            var allHttp = new StringBuilder();
            var allRaw = new StringBuilder();

            foreach (var item in client.Headers)
            {
                ServerVariables[item.Key] = item.Value;

                string httpKey = "HTTP_" + (item.Key.Replace('-', '_')).ToUpperInvariant();

                ServerVariables[httpKey] = item.Value;

                allHttp.Append(httpKey);
                allHttp.Append('=');
                allHttp.Append(item.Value);
                allHttp.Append("\r\n");

                allRaw.Append(item.Key);
                allRaw.Append('=');
                allRaw.Append(item.Value);
                allRaw.Append("\r\n");
            }

            ServerVariables["ALL_HTTP"] = allHttp.ToString();
            ServerVariables["ALL_RAW"] = allRaw.ToString();

            ServerVariables["CONTENT_LENGTH"] = ContentLength.ToString(CultureInfo.InvariantCulture);
            ServerVariables["CONTENT_TYPE"] = ContentType;

            ServerVariables["LOCAL_ADDR"] = client.Server.EndPoint.Address.ToString();
            ServerVariables["PATH_INFO"] = Path;

            string[] parts = client.Request.Split(new[] { '?' }, 2);

            ServerVariables["QUERY_STRING"] = parts.Length == 2 ? parts[1] : "";
            ServerVariables["REMOTE_ADDR"] = UserHostAddress;
            ServerVariables["REMOTE_HOST"] = UserHostName;
            ServerVariables["REMOTE_PORT"] = null;
            ServerVariables["REQUEST_METHOD"] = RequestType;
            ServerVariables["SCRIPT_NAME"] = Path;
            ServerVariables["SERVER_NAME"] = client.Server.ServerUtility.MachineName;
            ServerVariables["SERVER_PORT"] = client.Server.EndPoint.Port.ToString(CultureInfo.InvariantCulture);
            ServerVariables["SERVER_PROTOCOL"] = client.Protocol;
            ServerVariables["URL"] = Path;
        }

        private void BuildParams()
        {
            Params = new NameValueCollection();

            Merge(Params, QueryString);
            Merge(Params, Form);
            Merge(Params, ServerVariables);
        }

        private void Merge(NameValueCollection target, NameValueCollection source)
        {
            foreach (string key in source.AllKeys)
            {
                target[key] = source[key];
            }
        }

        private NameValueCollection CreateCollection(Dictionary<string, string> headers)
        {
            var result = new NameValueCollection();

            if (headers != null)
            {
                foreach (var item in headers)
                {
                    result[item.Key] = item.Value;
                }
            }

            return result;
        }
    }
}
