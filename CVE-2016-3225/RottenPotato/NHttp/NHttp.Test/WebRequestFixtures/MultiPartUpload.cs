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
    public class MultiPartUpload : FixtureBase
    {
        public const int UploadSize = 1 * 1024 * 1024;

        [Test]
        public void BasicUpload()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    Assert.That(e.Request.Form.AllKeys, Is.EquivalentTo(new[] { "key" }));
                    Assert.AreEqual("value", e.Request.Form["key"]);
                    Assert.AreEqual("multipart/form-data", e.Request.ContentType);
                    Assert.AreEqual("POST", e.Request.HttpMethod);
                    Assert.IsNull(e.Request.InputStream);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                using (var writer = new MultiPartWriter(request))
                {
                    writer.WriteValue("key", "value");
                }

                GetResponseFromRequest(request);
            }
        }

        [Test]
        public void BasicFileUpload()
        {
            using (var server = new HttpServer())
            using (var uploadStream = new MemoryStream())
            {
                WriteRandomData(uploadStream, UploadSize);

                server.RequestReceived += (s, e) =>
                {
                    uploadStream.Position = 0;

                    Assert.That(e.Request.Files.AllKeys, Is.EquivalentTo(new[] { "file" }));
                    Assert.AreEqual(uploadStream, e.Request.Files["file"].InputStream);
                    Assert.AreEqual("filename", e.Request.Files["file"].FileName);
                    Assert.AreEqual("application/octet-stream", e.Request.Files["file"].ContentType);
                    Assert.AreEqual(UploadSize, e.Request.Files["file"].ContentLength);
                    Assert.AreEqual("multipart/form-data", e.Request.ContentType);
                    Assert.AreEqual("POST", e.Request.HttpMethod);
                    Assert.IsNull(e.Request.InputStream);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                using (var writer = new MultiPartWriter(request))
                {
                    uploadStream.Position = 0;

                    writer.WriteFile("file", "filename", "application/octet-stream", uploadStream);
                }

                GetResponseFromRequest(request);
            }
        }

        [Test]
        public void PartialBoundaryMatch()
        {
            var uploadData = Encoding.ASCII.GetBytes(new String('\r', UploadSize));

            using (var server = new HttpServer())
            using (var uploadStream = new MemoryStream(uploadData))
            {
                server.RequestReceived += (s, e) =>
                {
                    uploadStream.Position = 0;

                    Assert.That(e.Request.Files.AllKeys, Is.EquivalentTo(new[] { "file" }));
                    Assert.AreEqual(uploadStream, e.Request.Files["file"].InputStream);
                    Assert.AreEqual("filename", e.Request.Files["file"].FileName);
                    Assert.AreEqual("application/octet-stream", e.Request.Files["file"].ContentType);
                    Assert.AreEqual(UploadSize, e.Request.Files["file"].ContentLength);
                    Assert.AreEqual("multipart/form-data", e.Request.ContentType);
                    Assert.AreEqual("POST", e.Request.HttpMethod);
                    Assert.IsNull(e.Request.InputStream);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                using (var writer = new MultiPartWriter(request))
                {
                    uploadStream.Position = 0;

                    writer.WriteFile("file", "filename", "application/octet-stream", uploadStream);
                }

                GetResponseFromRequest(request);
            }
        }
    }
}
