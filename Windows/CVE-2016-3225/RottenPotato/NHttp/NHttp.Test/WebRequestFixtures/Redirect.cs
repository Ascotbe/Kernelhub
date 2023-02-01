using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using NHttp.Test.Support;
using NUnit.Framework;

namespace NHttp.Test.WebRequestFixtures
{
    [TestFixture]
    public class Redirect : FixtureBase
    {
        [Test]
        public void RelativeRedirect()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) => e.Response.Redirect("target");

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/source/page", server.EndPoint)
                );

                request.AllowAutoRedirect = false;

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    string redirectLocation = String.Format(
                        "http://{0}/source/target", server.EndPoint
                    );

                    Assert.AreEqual(HttpStatusCode.Found, response.StatusCode);
                    Assert.AreEqual(redirectLocation, response.Headers["Location"]);
                }
            }
        }

        [Test]
        public void AbsoluteRedirect()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) => e.Response.Redirect("/target");

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/source/page", server.EndPoint)
                );

                request.AllowAutoRedirect = false;

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    string redirectLocation = String.Format(
                        "http://{0}/target", server.EndPoint
                    );

                    Assert.AreEqual(HttpStatusCode.Found, response.StatusCode);
                    Assert.AreEqual(redirectLocation, response.Headers["Location"]);
                }
            }
        }

        [Test]
        public void PermanentRedirect()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) => e.Response.RedirectPermanent("target");

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/source/page", server.EndPoint)
                );

                request.AllowAutoRedirect = false;

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    string redirectLocation = String.Format(
                        "http://{0}/source/target", server.EndPoint
                    );

                    Assert.AreEqual(HttpStatusCode.MovedPermanently, response.StatusCode);
                    Assert.AreEqual(redirectLocation, response.Headers["Location"]);
                }
            }
        }
    }
}
