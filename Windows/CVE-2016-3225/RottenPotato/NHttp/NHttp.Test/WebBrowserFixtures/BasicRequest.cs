using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using NUnit.Framework;

namespace NHttp.Test.WebBrowserFixtures
{
    [TestFixture]
    public class BasicRequest : Support.WebBrowserFixtureBase
    {
        private const string ResponseText = "Response text";

        [Test]
        public void SimpleRequest()
        {
            RegisterHandler("/", p =>
            {
                using (var writer = new StreamWriter(p.Response.OutputStream))
                {
                    writer.Write(ResponseText);
                }
            });

            bool navigated = false;

            DocumentCompleted += (s, e) =>
            {
                Assert.AreEqual(ResponseText, e.WebBrowser.DocumentText);

                navigated = true;
            };

            Navigate("/");

            Assert.That(navigated);
        }
    }
}
