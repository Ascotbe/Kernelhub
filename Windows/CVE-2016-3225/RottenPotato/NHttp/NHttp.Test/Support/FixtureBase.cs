using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using Common.Logging;
using Common.Logging.Simple;
using NUnit.Framework;

namespace NHttp.Test.Support
{
    public abstract class FixtureBase
    {
        private readonly Random _random = new Random();

        [TestFixtureSetUp]
        public void SetupFixture()
        {
            LogManager.Adapter = new ConsoleOutLoggerFactoryAdapter();
        }

        protected void WriteRandomData(Stream stream, int bytes)
        {
            var randomBytes = new byte[128];

            _random.NextBytes(randomBytes);

            for (int i = 0; i < bytes / randomBytes.Length; i++)
            {
                stream.Write(randomBytes, 0, randomBytes.Length);
            }
        }

        protected string GetResponseFromRequest(HttpWebRequest request)
        {
            using (var response = request.GetResponse())
            using (var stream = response.GetResponseStream())
            using (var reader = new StreamReader(stream))
            {
                return reader.ReadToEnd();
            }
        }

        protected void ExpectServerError(HttpWebRequest request)
        {
            try
            {
                GetResponseFromRequest(request);
            }
            catch (WebException ex)
            {
                var response = (HttpWebResponse)ex.Response;

                Assert.AreEqual(HttpStatusCode.InternalServerError, response.StatusCode);
            }
        }
    }
}
