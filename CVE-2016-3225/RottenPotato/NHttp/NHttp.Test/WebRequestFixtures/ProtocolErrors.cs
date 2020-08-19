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
    public class ProtocolErrors : FixtureBase
    {
        [Test]
        public void MultiPartWithoutBoundary()
        {
            using (var server = new HttpServer())
            {
                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                request.Method = "POST";
                request.ContentLength = 0;
                request.ContentType = "multipart/form-data";

                ExpectServerError(request);
            }
        }

        [Test]
        public void MultiPartWithoutContent()
        {
            using (var server = new HttpServer())
            {
                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                request.Method = "POST";
                request.ContentLength = 0;
                request.ContentType = "multipart/form-data; boundary=x";

                ExpectServerError(request);
            }
        }

        [Test]
        public void MultiPartWithoutContentEnd()
        {
            TestMultiPartUpload(
                "--x\r\n" +
                "Content-Disposition: form-data; name=\"key\"\r\n" +
                "\r\n" +
                "value\r\n"
                // There should be a "--x--" here
            );
        }

        [Test]
        public void MultiPartWithIllegalEndBoundary()
        {
            TestMultiPartUpload(
                "--x\r\n" +
                "Content-Disposition: form-data; name=\"key\"\r\n" +
                "\r\n" +
                "value\r\n" +
                "--x??"
            );
        }

        [Test]
        public void MultiPartWithoutContentDisposition()
        {
            TestMultiPartUpload(
                "--x\r\n" +
                "\r\n" +
                "value\r\n" +
                "--x--"
            );
        }

        [Test]
        public void MultiPartWithIllegalStartBoundary()
        {
            TestMultiPartUpload(
                "??x\r\n" +
                "Content-Disposition: form-data; name=\"key\"\r\n" +
                "\r\n" +
                "value\r\n" +
                "--x--"
            );
        }

        private void TestMultiPartUpload(string post)
        {
            using (var server = new HttpServer())
            {
                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                    );

                request.Method = "POST";
                request.ContentLength = 0;
                request.ContentType = "multipart/form-data; boundary=x";

                var postData = Encoding.ASCII.GetBytes(post);

                request.ContentLength = postData.Length;

                using (var stream = request.GetRequestStream())
                {
                    stream.Write(postData, 0, postData.Length);
                }

                ExpectServerError(request);
            }
        }
    }
}
