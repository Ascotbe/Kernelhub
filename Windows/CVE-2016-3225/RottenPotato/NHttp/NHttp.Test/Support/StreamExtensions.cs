using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace NHttp.Test.Support
{
    internal static class StreamExtensions
    {
        public static int GetLength(this Stream stream)
        {
            if (stream == null)
                throw new ArgumentNullException("stream");

            int length = 0;
            int read = 0;
            byte[] buffer = new byte[4096];

            while ((read = stream.Read(buffer, 0, buffer.Length)) != 0)
            {
                length += read;
            }

            return length;
        }
    }
}
