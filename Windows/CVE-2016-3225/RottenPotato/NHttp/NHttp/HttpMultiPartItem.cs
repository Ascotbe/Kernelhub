using System.IO;
using System.Text;
using System.Collections.Generic;
using System;

namespace NHttp
{
    internal class HttpMultiPartItem
    {
        public HttpMultiPartItem(Dictionary<string, string> headers, string value, Stream stream)
        {
            if (headers == null)
                throw new ArgumentNullException("headers");

            Headers = headers;
            Value = value;
            Stream = stream;
        }

        public Dictionary<string, string> Headers { get; private set; }

        public string Value { get; private set; }

        public Stream Stream { get; private set; }
    }
}
