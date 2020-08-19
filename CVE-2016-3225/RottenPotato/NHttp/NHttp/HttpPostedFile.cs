using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace NHttp
{
    public class HttpPostedFile
    {
        public HttpPostedFile(int contentLength, string contentType, string fileName, Stream inputStream)
        {
            if (fileName == null)
                throw new ArgumentNullException("fileName");
            if (inputStream == null)
                throw new ArgumentNullException("inputStream");

            ContentLength = contentLength;
            ContentType = contentType;
            FileName = fileName;
            InputStream = inputStream;
        }

        public int ContentLength { get; private set; }

        public string ContentType { get; private set; }

        public string FileName { get; private set; }

        public Stream InputStream { get; private set; }
    }
}
