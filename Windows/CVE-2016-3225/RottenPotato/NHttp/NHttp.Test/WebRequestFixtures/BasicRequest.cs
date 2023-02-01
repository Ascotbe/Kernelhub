using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using NHttp.Test.Support;
using NUnit.Framework;

namespace NHttp.Test.WebRequestFixtures
{
    [TestFixture]
    public class BasicRequest : FixtureBase
    {
        private const string ResponseText = "Response text";

        [Test]
        public void SingleRequest()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    Assert.That(e.Request.QueryString.AllKeys, Is.EquivalentTo(new[] { "key" }));
                    Assert.AreEqual("value", e.Request.QueryString["key"]);

                    using (var writer = new StreamWriter(e.Response.OutputStream))
                    {
                        writer.Write(ResponseText);
                    }
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/?key=value", server.EndPoint)
                );

                Assert.AreEqual(ResponseText, GetResponseFromRequest(request));
            }
        }

        [Test]
        public void NoInputStreamWithGet()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) => Assert.IsNull(e.Request.InputStream);

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                GetResponseFromRequest(request);
            }
        }
    }
}
