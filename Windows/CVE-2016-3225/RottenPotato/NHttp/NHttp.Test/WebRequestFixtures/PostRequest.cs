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
    public class PostRequest : FixtureBase
    {
        private const string ResponseText = "Response text";
        private const int UploadSize = 1 * 1024 * 1024;

        [Test]
        public void SimplePost()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    Assert.That(e.Request.Form.AllKeys, Is.EquivalentTo(new[] { "key" }));
                    Assert.AreEqual("value", e.Request.Form["key"]);
                    Assert.AreEqual("application/x-www-form-urlencoded", e.Request.ContentType);
                    Assert.AreEqual("POST", e.Request.HttpMethod);
                    Assert.IsNull(e.Request.InputStream);

                    using (var writer = new StreamWriter(e.Response.OutputStream))
                    {
                        writer.Write(ResponseText);
                    }
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                byte[] postBytes = Encoding.ASCII.GetBytes("key=value");

                request.Method = "POST";
                request.ContentLength = postBytes.Length;
                request.ContentType = "application/x-www-form-urlencoded";

                using (var stream = request.GetRequestStream())
                {
                    stream.Write(postBytes, 0, postBytes.Length);
                }

                Assert.AreEqual(ResponseText, GetResponseFromRequest(request));
            }
        }

        [Test]
        public void UploadBinaryData()
        {
            using (var server = new HttpServer())
            using (var uploadStream = new MemoryStream())
            {
                WriteRandomData(uploadStream, UploadSize);

                server.RequestReceived += (s, e) =>
                {
                    uploadStream.Position = 0;

                    Assert.AreEqual(uploadStream, e.Request.InputStream);
                    Assert.AreEqual(UploadSize, e.Request.ContentLength);
                    Assert.AreEqual("application/octet-stream", e.Request.ContentType);
                    Assert.AreEqual("POST", e.Request.HttpMethod);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                request.Method = "POST";
                request.ContentType = "application/octet-stream";
                request.ContentLength = UploadSize;

                using (var stream = request.GetRequestStream())
                {
                    uploadStream.Position = 0;

                    uploadStream.CopyTo(stream);
                }

                GetResponseFromRequest(request);
            }
        }

        [Test]
        public void PostWithoutContent()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    Assert.AreEqual("POST", e.Request.HttpMethod);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                request.Method = "POST";

                GetResponseFromRequest(request);
            }
        }

        [Test]
        public void PostWithContentWithoutType()
        {
            using (var server = new HttpServer())
            using (var uploadStream = new MemoryStream())
            {
                WriteRandomData(uploadStream, 1 * 1024 * 1024);

                server.RequestReceived += (s, e) =>
                {
                    uploadStream.Position = 0;

                    Assert.AreEqual("POST", e.Request.HttpMethod);
                    Assert.AreEqual(uploadStream, e.Request.InputStream);
                    Assert.IsNull(e.Request.ContentType);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                uploadStream.Position = 0;

                request.Method = "POST";
                request.ContentLength = uploadStream.Length;

                using (var stream = request.GetRequestStream())
                {
                    uploadStream.CopyTo(stream);
                }

                GetResponseFromRequest(request);
            }
        }
    }
}
