using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using NUnit.Framework;

namespace NHttp.Test.Support
{
    public class WebBrowserFixtureProxy : IDisposable
    {
        private bool _disposed;
        private WebBrowserForm _form;
        private readonly Thread _thread;
        private readonly ManualResetEvent _startEvent = new ManualResetEvent(false);
        private readonly AutoResetEvent _navigatedEvent = new AutoResetEvent(false);
        private readonly List<IRequestHandler> _handlers = new List<IRequestHandler>();

        public HttpServer Server { get; private set; }

        public WebBrowserFixtureProxy()
        {
            Server = new HttpServer();
            Server.RequestReceived += Server_RequestReceived;
            Server.Start();

            _thread = new Thread(ThreadProc);
            _thread.SetApartmentState(ApartmentState.STA);
            _thread.Start();

            _startEvent.WaitOne();
        }

        void Server_RequestReceived(object sender, HttpRequestEventArgs e)
        {
            foreach (var handler in _handlers)
            {
                if (handler.Path == e.Request.Path)
                {
                    handler.Handle(e.Context);
                    return;
                }
            }

            Assert.Fail("No handler matched '{0}'", e.Request.Path);
        }

        private void ThreadProc()
        {
            _form = new WebBrowserForm();

            _form.DocumentCompleted += _form_DocumentCompleted;

            _form.Shown += (s, e) => _startEvent.Set();

            _form.ShowDialog();
        }

        void _form_DocumentCompleted(object sender, EventArgs e)
        {
            OnDocumentCompleted(new DocumentCompletedEventArgs(_form.WebBrowser));

            _navigatedEvent.Set();
        }

        public void Navigate(string location)
        {
            if (location == null)
                throw new ArgumentNullException("location");

            if (!location.Contains(":"))
                location = "http://" + Server.EndPoint + location;

            _form.Navigate(location);

            _navigatedEvent.WaitOne();
        }

        public event DocumentCompletedEventHandler DocumentCompleted;

        protected virtual void OnDocumentCompleted(DocumentCompletedEventArgs e)
        {
            var ev = DocumentCompleted;

            if (ev != null)
                ev(this, e);
        }

        public void RegisterHandler(IRequestHandler handler)
        {
            if (handler == null)
                throw new ArgumentNullException("handler");

            _handlers.Add(handler);
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                _form.BeginInvoke(new Action(_form.Dispose));

                _thread.Join();

                Server.Stop();

                _disposed = true;
            }
        }

        private delegate void Action();
    }
}
