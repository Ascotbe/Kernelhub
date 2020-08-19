using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using NHttp.Test.Support;
using NUnit.Framework;
using mshtml;

namespace NHttp.Test.WebBrowserFixtures
{
    [TestFixture]
    public class FileUpload : WebBrowserFixtureBase
    {
        private const string UploadFileContent = "Upload file content";

        [Test]
        public void FileUploadPost()
        {
            string tempFileName = Path.GetTempFileName();

            try
            {
                using (var tempFile = File.CreateText(tempFileName))
                {
                    tempFile.Write(UploadFileContent);
                    tempFile.Flush();
                }

                PerformFileUpload(tempFileName, "text/plain");
            }
            finally
            {
                File.Delete(tempFileName);
            }
        }

        [Test]
        public void LargeFileUpload()
        {
            string tempFileName = Path.GetTempFileName();

            try
            {
                using (var tempFile = File.Create(tempFileName))
                {
                    WriteRandomData(tempFile, 10 * 1024 * 1024);
                }

                PerformFileUpload(tempFileName, "application/octet-stream");
            }
            finally
            {
                File.Delete(tempFileName);
            }
        }

        private void PerformFileUpload(string tempFileName, string contentType)
        {
            RegisterHandler(new ResourceHandler("/form", GetType().Namespace + ".Resources.FileUploadForm.html"));

            var submittedEvent = new ManualResetEvent(false);

            RegisterHandler("/submit", p =>
            {
                Assert.That(p.Request.Form.AllKeys, Is.EquivalentTo(new[] { "key" }));
                Assert.AreEqual("value", p.Request.Form["key"]);
                Assert.That(p.Request.Files.AllKeys, Is.EquivalentTo(new[] { "file" }));

                using (var tempFile = File.OpenRead(tempFileName))
                {
                    Assert.AreEqual(tempFile, p.Request.Files["file"].InputStream);
                    Assert.AreEqual(tempFile.Length, p.Request.Files["file"].ContentLength);
                }

                Assert.AreEqual(Path.GetFileName(tempFileName), p.Request.Files["file"].FileName);
                Assert.AreEqual(contentType, p.Request.Files["file"].ContentType);

                submittedEvent.Set();
            });

            DocumentCompleted += (s, e) =>
            {
                if (e.Document.Url.AbsolutePath == "/form")
                {
                    SetFileUpload(e.Document, "file", tempFileName);

                    var formElement = (IHTMLFormElement)e.Document.GetElementById("form").DomElement;

                    formElement.submit();
                }
            };

            Navigate("/form");

            Assert.That(submittedEvent.WaitOne(TimeSpan.FromSeconds(5)), "No /submit request received");
        }
    }
}
