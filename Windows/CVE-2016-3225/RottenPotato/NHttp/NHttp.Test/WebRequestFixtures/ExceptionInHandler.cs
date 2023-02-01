using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using NHttp.Test.Support;
using NUnit.Framework;

namespace NHttp.Test.WebRequestFixtures
{
    [TestFixture]
    public class ExceptionInHandler : FixtureBase
    {
        private const string ErrorMessage = "Error message";

        [Test]
        public void TestExceptionInHandler()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    throw new Exception();
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                ExpectServerError(request);
            }
        }

        [Test]
        public void HandledHandlerException()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    throw new Exception();
                };

                server.UnhandledException += (s, e) =>
                {
                    e.Response.Status = "200 OK";

                    using (var writer = new StreamWriter(e.Response.OutputStream))
                    {
                        writer.Write(ErrorMessage);
                    }
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                Assert.AreEqual(ErrorMessage, GetResponseFromRequest(request));
            }
        }

        [Test]
        public void HandledHandlerExceptionWithoutOutput()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    throw new Exception();
                };

                server.UnhandledException += (s, e) =>
                {
                    e.Response.Status = "200 OK";
                    e.Handled = true;
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    Assert.AreEqual(HttpStatusCode.OK, response.StatusCode);
                    Assert.AreEqual("OK", response.StatusDescription);

                    using (var stream = response.GetResponseStream())
                    using (var reader = new StreamReader(stream))
                    {
                        Assert.IsEmpty(reader.ReadToEnd());
                    }
                }
            }
        }
    }
}
