using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Threading;
using NHttp.Test.Support;
using NUnit.Framework;

namespace NHttp.Test.WebRequestFixtures
{
    [TestFixture]
    public class LoadTest : FixtureBase
    {
        private const string Payload = "Request payload";
        private const int LargeResponseSize = 1 * 1024 * 1024;
        private const int ParallelCount = 20;
        private const int RequestCount = 100;

        [Test]
        public void MultipleSmallConsecutiveRequests()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    using (var writer = new StreamWriter(e.Response.OutputStream))
                    {
                        writer.Write(Payload);
                    }
                };

                server.Start();

                for (int i = 0; i < RequestCount; i++)
                {
                    var request = (HttpWebRequest)WebRequest.Create(
                        String.Format("http://{0}/", server.EndPoint)
                    );

                    Assert.AreEqual(Payload, GetResponseFromRequest(request));
                }
            }
        }

        [Test]
        public void MultipleLargeConsecutiveRequests()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) => WriteRandomData(e.Response.OutputStream, LargeResponseSize);

                server.Start();

                for (int i = 0; i < RequestCount; i++)
                {
                    var request = (HttpWebRequest)WebRequest.Create(
                        String.Format("http://{0}/", server.EndPoint)
                    );

                    using (var response = request.GetResponse())
                    using (var stream = response.GetResponseStream())
                    {
                        Assert.AreEqual(LargeResponseSize, stream.GetLength());
                    }
                }
            }
        }

        [Test]
        public void MultipleSmallParallelRequests()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    using (var writer = new StreamWriter(e.Response.OutputStream))
                    {
                        writer.Write(Payload);
                    }
                };

                server.Start();

                object syncLock = new object();
                int runningThreads = ParallelCount;
                var waitEvent = new AutoResetEvent(false);

                for (int i = 0; i < ParallelCount; i++)
                {
                    ThreadPool.QueueUserWorkItem(p =>
                    {
                        for (int j = 0; j < RequestCount; j++)
                        {
                            var request = (HttpWebRequest)WebRequest.Create(
                                String.Format("http://{0}/", server.EndPoint)
                            );

                            Assert.AreEqual(Payload, GetResponseFromRequest(request));
                        }

                        lock (syncLock)
                        {
                            runningThreads--;
                            waitEvent.Set();
                        }
                    });
                }

                while (true)
                {
                    lock (syncLock)
                    {
                        if (runningThreads == 0)
                            break;
                    }

                    waitEvent.WaitOne();
                }
            }
        }

        [Test]
        public void MultipleLargeParallelRequests()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) => WriteRandomData(e.Response.OutputStream, LargeResponseSize);

                server.Start();

                object syncLock = new object();
                int runningThreads = ParallelCount;
                var waitEvent = new AutoResetEvent(false);

                for (int i = 0; i < ParallelCount; i++)
                {
                    ThreadPool.QueueUserWorkItem(p =>
                    {
                        for (int j = 0; j < RequestCount; j++)
                        {
                            var request = (HttpWebRequest)WebRequest.Create(
                                String.Format("http://{0}/", server.EndPoint)
                            );

                            using (var response = request.GetResponse())
                            using (var stream = response.GetResponseStream())
                            {
                                Assert.AreEqual(LargeResponseSize, stream.GetLength());
                            }
                        }

                        lock (syncLock)
                        {
                            runningThreads--;
                            waitEvent.Set();
                        }
                    });
                }

                while (true)
                {
                    lock (syncLock)
                    {
                        if (runningThreads == 0)
                            break;
                    }

                    waitEvent.WaitOne();
                }
            }
        }
    }
}
