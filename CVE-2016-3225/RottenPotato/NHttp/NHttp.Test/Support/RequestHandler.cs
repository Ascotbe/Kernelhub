using System;
using System.Collections.Generic;
using System.Text;

namespace NHttp.Test.Support
{
    public interface IRequestHandler
    {
        string Path { get; }
        void Handle(HttpContext context);
    }

    public class RequestHandler : IRequestHandler
    {
        public RequestHandler(string path, RequestHandlerCallback callback)
        {
            if (path == null)
                throw new ArgumentNullException("path");
            if (callback == null)
                throw new ArgumentNullException("callback");

            Path = path;
            Callback = callback;
        }

        public string Path { get; private set; }

        public RequestHandlerCallback Callback { get; private set; }

        public void Handle(HttpContext context)
        {
            Callback(context);
        }
    }

    public delegate void RequestHandlerCallback(HttpContext context);

    public class ResourceHandler : IRequestHandler
    {
        private readonly string _resourceName;

        public ResourceHandler(string path, string resourceName)
        {
            if (path == null)
                throw new ArgumentNullException("path");
            if (resourceName == null)
                throw new ArgumentNullException("resourceName");

            Path = path;
            _resourceName = resourceName;
        }

        public string Path { get; private set; }

        public void Handle(HttpContext context)
        {
            using (var stream = GetType().Assembly.GetManifestResourceStream(_resourceName))
            {
                byte[] buffer = new byte[4096];
                int read;

                while ((read = stream.Read(buffer, 0, buffer.Length)) > 0)
                {
                    context.Response.OutputStream.Write(buffer, 0, read);
                }
            }
        }
    }
}
