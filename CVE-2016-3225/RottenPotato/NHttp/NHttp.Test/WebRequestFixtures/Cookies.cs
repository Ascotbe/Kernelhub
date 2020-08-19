using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Text;
using NHttp.Test.Support;
using NUnit.Framework;

namespace NHttp.Test.WebRequestFixtures
{
    [TestFixture]
    public class Cookies : FixtureBase
    {
        private const string DateFormat = "ddd, dd-MMM-yyyy HH':'mm':'ss 'GMT'";
        private static readonly CultureInfo DateCulture = new CultureInfo("en-US");

        [Test]
        public void SetCookie()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    e.Response.Cookies["a"].Value = "b";
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                request.CookieContainer = new CookieContainer();

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    Assert.AreEqual("a=b; path=/", response.Headers["Set-Cookie"]);
                    Assert.AreEqual(1, response.Cookies.Count);

                    var cookie = response.Cookies[0];

                    Assert.AreEqual("a", cookie.Name);
                    Assert.AreEqual("b", cookie.Value);
                    Assert.AreEqual("/", cookie.Path);
                    Assert.AreEqual(server.EndPoint.Address.ToString(), cookie.Domain);
                    Assert.AreEqual(DateTime.MinValue, cookie.Expires);
                    Assert.AreEqual(false, cookie.HttpOnly);
                    Assert.AreEqual(false, cookie.Secure);
                }
            }
        }

        [Test]
        public void SetCookieWithDetails()
        {
            using (var server = new HttpServer())
            {
                var expires = DateTime.Now + TimeSpan.FromDays(1);

                expires = expires - TimeSpan.FromMilliseconds(expires.Millisecond);

                server.RequestReceived += (s, e) =>
                {
                    var cookie = new HttpCookie("a", "b");

                    cookie.Path = "/path";
                    cookie.HttpOnly = true;
                    cookie.Secure = true;
                    cookie.Expires = expires;

                    e.Response.Cookies.Add(cookie);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/path", server.EndPoint)
                );

                request.CookieContainer = new CookieContainer();

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    Assert.AreEqual("a=b; expires=" + expires.ToString(DateFormat, DateCulture) + "; path=/path; secure; HttpOnly", response.Headers["Set-Cookie"]);
 
                    Assert.AreEqual(1, response.Cookies.Count);

                    var cookie = response.Cookies[0];

                    Assert.AreEqual("a", cookie.Name);
                    Assert.AreEqual("b", cookie.Value);
                    Assert.AreEqual("/path", cookie.Path);
                    Assert.AreEqual(expires.ToString(CultureInfo.InvariantCulture), cookie.Expires.ToUniversalTime().ToString(CultureInfo.InvariantCulture));
                    Assert.AreEqual(true, cookie.HttpOnly);
                    Assert.AreEqual(true, cookie.Secure);
                }
            }
        }

        [Test]
        public void SetCookieWithDomain()
        {
            using (var server = new HttpServer())
            {
                var expires = DateTime.Now + TimeSpan.FromDays(1);

                expires = expires - TimeSpan.FromMilliseconds(expires.Millisecond);

                server.RequestReceived += (s, e) =>
                {
                    var cookie = new HttpCookie("a", "b");

                    cookie.Domain = "example.com";

                    e.Response.Cookies.Add(cookie);
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/path", server.EndPoint)
                );

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    Assert.AreEqual("a=b; domain=example.com; path=/", response.Headers["Set-Cookie"]);
                }
            }
        }

        [Test]
        public void SetMultipleCookies()
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    e.Response.Cookies["a"].Value = "b";
                    e.Response.Cookies["c"].Value = "d";
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/", server.EndPoint)
                );

                request.CookieContainer = new CookieContainer();

                using (var response = (HttpWebResponse)request.GetResponse())
                {
                    Assert.AreEqual("a=b; path=/,c=d; path=/", response.Headers["Set-Cookie"]);

                    Assert.AreEqual(2, response.Cookies.Count);

                    for (int i = 0; i < response.Cookies.Count; i++)
                    {
                        var cookie = response.Cookies[i];

                        switch (i)
                        {
                            case 0:
                                Assert.AreEqual("a", cookie.Name);
                                Assert.AreEqual("b", cookie.Value);
                                break;

                            case 1:
                                Assert.AreEqual("c", cookie.Name);
                                Assert.AreEqual("d", cookie.Value);
                                break;
                        }
                    }
                }
            }
        }

        [Test]
        public void CookieRoundTrip()
        {
            TestCookieRoundtrip(
                p =>
                {
                    p.Response.Cookies["a"].Value = "b";
                },
                p =>
                {
                    Assert.That(p.Request.Cookies.AllKeys, Is.EquivalentTo(new[] { "a" }));
                    Assert.AreEqual("b", p.Request.Cookies["a"].Value);
                }
            );
        }

        [Test]
        public void DuplicateCookiesRoundTrip()
        {
            TestCookieRoundtrip(
                p =>
                {
                    p.Response.Cookies.Add(new HttpCookie("a", "b"));
                    p.Response.Cookies.Add(new HttpCookie("a", "c"));
                },
                p =>
                {
                    Assert.That(p.Request.Cookies.AllKeys, Is.EquivalentTo(new[] { "a" }));
                    Assert.AreEqual("c", p.Request.Cookies["a"].Value);
                }
            );
        }

        [Test]
        public void ValueToCollectionRoundTrip()
        {
            TestCookieRoundtrip(
                p =>
                {
                    p.Response.Cookies["a"].Value = "a=b&&c";
                },
                p =>
                {
                    Assert.That(p.Request.Cookies.AllKeys, Is.EquivalentTo(new[] { "a" }));
                    Assert.That(p.Request.Cookies["a"].Values.AllKeys, Is.EquivalentTo(new[] { "a", null, "c" }));
                    Assert.AreEqual("b", p.Request.Cookies["a"].Values["a"]);
                    Assert.AreEqual(null, p.Request.Cookies["a"].Values["c"]);
                }
            );
        }

        [Test]
        public void CollectionToCollectionRoundTrip()
        {
            TestCookieRoundtrip(
                p =>
                {
                    p.Response.Cookies["a"].Values["a"] = "b";
                    p.Response.Cookies["a"].Values["c"] = "d";
                },
                p =>
                {
                    Assert.That(p.Request.Cookies.AllKeys, Is.EquivalentTo(new[] { "a" }));
                    Assert.That(p.Request.Cookies["a"].Values.AllKeys, Is.EquivalentTo(new[] { "a", "c" }));
                    Assert.AreEqual("b", p.Request.Cookies["a"].Values["a"]);
                    Assert.AreEqual("d", p.Request.Cookies["a"].Values["c"]);
                }
            );
        }

        [Test]
        public void InvalidlyParsedCollectionRoundTrip()
        {
            TestCookieRoundtrip(
                p =>
                {
                    p.Response.Cookies["a"].Values["a"] = "b&c=d";
                },
                p =>
                {
                    Assert.That(p.Request.Cookies.AllKeys, Is.EquivalentTo(new[] { "a" }));
                    Assert.That(p.Request.Cookies["a"].Values.AllKeys, Is.EquivalentTo(new[] { "a", "c" }));
                    Assert.AreEqual("b", p.Request.Cookies["a"].Values["a"]);
                    Assert.AreEqual("d", p.Request.Cookies["a"].Values["c"]);
                }
            );
        }

        private void TestCookieRoundtrip(Action<HttpContext> fromCallback, Action<HttpContext> toCallback)
        {
            using (var server = new HttpServer())
            {
                server.RequestReceived += (s, e) =>
                {
                    switch (e.Request.Path)
                    {
                        case "/from":
                            fromCallback(e.Context);

                            e.Response.RedirectPermanent("/to");
                            break;

                        case "/to":
                            toCallback(e.Context);
                            break;
                    }
                };

                server.Start();

                var request = (HttpWebRequest)WebRequest.Create(
                    String.Format("http://{0}/from", server.EndPoint)
                );

                request.CookieContainer = new CookieContainer();

                GetResponseFromRequest(request);
            }
        }
    }
}
