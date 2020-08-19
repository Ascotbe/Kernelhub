using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Globalization;
using System.Text;

namespace NHttp
{
    public class HttpCookie
    {
        private const string DateFormat = "ddd, dd-MMM-yyyy HH':'mm':'ss 'GMT'";
        private static readonly CultureInfo DateCulture = new CultureInfo("en-US");

        public HttpCookie(string name)
            : this(name, null)
        {
        }

        public HttpCookie(string name, string value)
        {
            if (name == null)
                throw new ArgumentNullException("name");

            Name = name;
            Expires = DateTime.MinValue;
            Path = "/";

            Values = new NameValueCollection();

            if (value != null)
            {
                if (value.Contains("&") || value.Contains("="))
                {
                    string[] parts = value.Split('&');

                    foreach (string part in parts)
                    {
                        name = null;
                        value = null;

                        if (part.Length > 0)
                        {
                            string[] partParts = part.Split(new[] { '=' }, 2);

                            name = partParts[0];
                            value = partParts.Length == 1 ? null : partParts[1];
                        }

                        Values.Add(name, value);
                    }
                }
                else
                {
                    Value = value;
                }
            }
        }

        public string Domain { get; set; }

        public DateTime Expires { get; set; }

        public bool HasKeys
        {
            get { return !(Values.Keys.Count == 1 && Values.Keys[0] == null); }
        }

        public bool HttpOnly { get; set; }

        public string Name { get; set; }

        public string Path { get; set; }

        public bool Secure { get; set; }

        public string Value
        {
            get { return SerializeValues(); }
            set
            {
                Values.Clear();

                Values.Add(null, value ?? String.Empty);
            }
        }

        private string SerializeValues()
        {
            if (Values.Keys.Count == 0)
            {
                return String.Empty;
            }
            else if (!HasKeys)
            {
                return Values[0];
            }
            else
            {
                var sb = new StringBuilder();

                for (int i = 0; i < Values.Keys.Count; i++)
                {
                    if (i > 0)
                        sb.Append('&');

                    sb.Append(Values.Keys[i]);

                    string value = Values[i];

                    if (value != null)
                    {
                        sb.Append('=');
                        sb.Append(value);
                    }
                }

                return sb.ToString();
            }
        }

        public NameValueCollection Values { get; private set; }

        internal string GetHeaderValue()
        {
            var sb = new StringBuilder();

            sb.Append(Name);
            sb.Append('=');
            sb.Append(Value);

            if (!String.IsNullOrEmpty(Domain))
            {
                sb.Append("; domain=");
                sb.Append(Domain);
            }

            if (Expires != DateTime.MinValue)
            {
                sb.Append("; expires=");
                sb.Append(Expires.ToString(DateFormat, DateCulture));
            }

            if (!String.IsNullOrEmpty(Path))
            {
                sb.Append("; path=");
                sb.Append(Path);
            }

            if (Secure)
                sb.Append("; secure");

            if (HttpOnly)
                sb.Append("; HttpOnly");

            return sb.ToString();
        }
    }
}
