using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using NUnit.Framework;
using mshtml;

namespace NHttp.Test.Support
{
    public abstract class WebBrowserFixtureBase : FixtureBase
    {
        private WebBrowserFixtureProxy _proxy;

        [SetUp]
        public void Setup()
        {
            _proxy = new WebBrowserFixtureProxy();
        }

        [TearDown]
        public void TearDown()
        {
            _proxy.Dispose();
            _proxy = null;
        }

        protected void Navigate(string location)
        {
            _proxy.Navigate(location);
        }

        public event DocumentCompletedEventHandler DocumentCompleted
        {
            add { _proxy.DocumentCompleted += value; }
            remove { _proxy.DocumentCompleted -= value; }
        }

        protected void RegisterHandler(IRequestHandler handler)
        {
            _proxy.RegisterHandler(handler);
        }

        protected void RegisterHandler(string path, RequestHandlerCallback callback)
        {
            RegisterHandler(new RequestHandler(path, callback));
        }

        protected void SetFileUpload(HtmlDocument document, string name, string fileName)
        {
            var fileElement = (HTMLInputElement)document.GetElementById(name).DomElement;

            fileElement.focus();

            // The first space is to open the file open dialog. The
            // remainder of the spaces is to have some messages for
            // until the open dialog actually opens.

            SendKeys.SendWait("       " + fileName + "{ENTER}");
        }
    }
}
