using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace NHttp.Test.Support
{
    public partial class WebBrowserForm : Form
    {
        public event EventHandler DocumentCompleted;

        protected virtual void OnDocumentCompleted(EventArgs e)
        {
            var ev = DocumentCompleted;

            if (ev != null)
                ev(this, e);
        }

        public WebBrowserForm()
        {
            InitializeComponent();
        }

        public void Navigate(string location)
        {
            _webBrowser.Navigate(location);
        }

        private void _webBrowser_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            OnDocumentCompleted(EventArgs.Empty);
        }

        public WebBrowser WebBrowser
        {
            get { return _webBrowser; }
        }
    }
}
