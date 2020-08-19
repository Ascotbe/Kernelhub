using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;

namespace NHttp.Test.Support
{
    public class MultiPartWriter : IDisposable
    {
        private readonly HttpWebRequest _request;
        private readonly string _boundary;
        private readonly MemoryStream _stream;
        private bool _disposed;

        public MultiPartWriter(HttpWebRequest request)
            : this(request, Guid.NewGuid().ToString())
        {
        }

        public MultiPartWriter(HttpWebRequest request, string boundary)
        {
            if (request == null)
                throw new ArgumentNullException("request");
            if (boundary == null)
                throw new ArgumentNullException("boundary");

            _request = request;
            _boundary = boundary;

            _stream = new MemoryStream();
        }

        public void WriteValue(string name, string value)
        {
            WriteBoundary();

            Write(String.Format("Content-Disposition: form-data; name=\"{0}\"\r\n", name));
            Write("\r\n");
            Write(Uri.EscapeDataString(value));
            Write("\r\n");
        }

        public void WriteFile(string name, string fileName, string contentType, Stream stream)
        {
            WriteBoundary();

            Write(String.Format("Content-Disposition: form-data; name=\"{0}\"; filename=\"{1}\"\r\n", name, fileName));
            Write(String.Format("Content-Type: {0}\r\n", contentType));
            Write("\r\n");

            stream.Position = 0;

            stream.CopyTo(_stream);

            Write("\r\n");
        }

        private void Write(string data)
        {
            var buffer = Encoding.ASCII.GetBytes(data);

            _stream.Write(buffer, 0, buffer.Length);
        }

        private void WriteBoundary()
        {
            Write("--" + _boundary + "\r\n");
        }

        private void WriteEndBoundary()
        {
            Write("--" + _boundary + "--");
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                WriteEndBoundary();

                _stream.Position = 0;

                _request.Method = "POST";
                _request.ContentType = "multipart/form-data; boundary=" + _boundary;
                _request.ContentLength = _stream.Length;

                using (var stream = _request.GetRequestStream())
                {
                    _stream.CopyTo(stream);
                }

                _stream.Dispose();

                _disposed = true;
            }
        }
    }
}
