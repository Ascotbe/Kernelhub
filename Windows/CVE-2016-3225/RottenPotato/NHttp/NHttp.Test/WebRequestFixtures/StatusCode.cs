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
    public class StatusCode : FixtureBase
    {
        [Test]
        public void NotFound()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    e.Response.Status = "404 Not Found";
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                try
                {
                    GetResponseFromRequest(request);
                }
                catch (WebException ex)
                {
                    var response = (HttpWebResponse)ex.Response;

                    Assert.AreEqual(HttpStatusCode.NotFound, response.StatusCode);
                    Assert.AreEqual("Not Found", response.StatusDescription);
                }
            }
        }

        [Test]
        public void ExceptionOnRequestReceived()
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

                try
                {
                    GetResponseFromRequest(request);
                }
                catch (WebException ex)
                {
                    var response = (HttpWebResponse)ex.Response;

                    Assert.AreEqual(HttpStatusCode.InternalServerError, response.StatusCode);
                    Assert.AreEqual("Internal Server Error", response.StatusDescription);
                }
            }
        }
    }
}
