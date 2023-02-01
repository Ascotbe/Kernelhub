using System;
using System.Collections.Generic;
using System.Text;
using NHttp.Test.Support;
using NUnit.Framework;
using mshtml;

namespace NHttp.Test.WebBrowserFixtures
{
    [TestFixture]
    public class PostRequest : WebBrowserFixtureBase
    {
        [Test]
        public void SimplePost()
        {
            RegisterHandler(new ResourceHandler("/form", GetType().Namespace + ".Resources.PostForm.html"));

            RegisterHandler("/submit", p =>
            {
                Assert.That(p.Request.Form.AllKeys, Is.EquivalentTo(new[] { "key" }));
                Assert.AreEqual("value", p.Request.Form["key"]);
            });

            DocumentCompleted += (s, e) =>
            {
                if (e.Document.Url.AbsolutePath == "/form")
                {
                    var keyElement = (IHTMLInputTextElement)e.Document.GetElementById("key").DomElement;
                    var formElement = (IHTMLFormElement)e.Document.GetElementById("form").DomElement;

                    keyElement.value = "value";
                    formElement.submit();
                }
            };

            Navigate("/form");
        }
    }
}
