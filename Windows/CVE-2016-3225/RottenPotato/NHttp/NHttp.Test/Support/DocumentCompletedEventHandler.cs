using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace NHttp.Test.Support
{
    public class DocumentCompletedEventArgs : EventArgs
    {
        public DocumentCompletedEventArgs(WebBrowser webBrowser)
        {
            if (webBrowser == null)
                throw new ArgumentNullException("webBrowser");

            WebBrowser = webBrowser;
        }

        public WebBrowser WebBrowser { get; private set; }

        public HtmlDocument Document { get { return WebBrowser.Document; } }
    }

    public delegate void DocumentCompletedEventHandler(object sender, DocumentCompletedEventArgs e);
}
