using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace NHttp
{
    internal class HttpUrlEncodedRequestParser : HttpRequestParser
    {
        private readonly MemoryStream _stream;

        public HttpUrlEncodedRequestParser(HttpClient client, int contentLength)
            : base(client, contentLength)
        {
            _stream = new MemoryStream();
        }

        public override void Parse()
        {
            Client.ReadBuffer.CopyToStream(_stream, ContentLength);

            if (_stream.Length == ContentLength)
            {
                ParseContent();

                EndParsing();
            }
        }

        private void ParseContent()
        {
            _stream.Position = 0;

            string content;

            using (var reader = new StreamReader(_stream, Encoding.ASCII))
            {
                content = reader.ReadToEnd();
            }

            Client.PostParameters = HttpUtil.UrlDecode(content);
        }
    }
}
