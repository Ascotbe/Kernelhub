using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;

namespace NHttp
{
    public class HttpCookieCollection : NameObjectCollectionBase
    {
        private string[] _allKeys;

        internal HttpCookieCollection()
            : base(StringComparer.OrdinalIgnoreCase)
        {
        }

        internal void AddCookie(HttpCookie cookie, bool append)
        {
            if (cookie == null)
                throw new ArgumentNullException("cookie");

            _allKeys = null;

            if (append)
                BaseAdd(cookie.Name, cookie);
            else
                BaseSet(cookie.Name, cookie);
        }

        public void Add(HttpCookie cookie)
        {
            AddCookie(cookie, true);
        }

        public void Set(HttpCookie cookie)
        {
            AddCookie(cookie, false);
        }

        public void Remove(string name)
        {
            RemoveCookie(name);
        }

        private void RemoveCookie(string name)
        {
            _allKeys = null;

            BaseRemove(name);
        }

        public void Clear()
        {
            Reset();
        }

        private void Reset()
        {
            _allKeys = null;

            BaseClear();
        }

        public HttpCookie Get(string name)
        {
            var result = (HttpCookie)BaseGet(name);

            if (result == null)
            {
                result = new HttpCookie(name);

                AddCookie(result, true);
            }

            return result;
        }

        public HttpCookie this[string name]
        {
            get { return Get(name); }
        }

        public HttpCookie Get(int index)
        {
            return (HttpCookie)BaseGet(index);
        }

        public string GetKey(int index)
        {
            return BaseGetKey(index);
        }

        public HttpCookie this[int index]
        {
            get { return Get(index); }
        }

        public string[] AllKeys
        {
            get
            {
                if (_allKeys == null)
                    _allKeys = BaseGetAllKeys();

                return _allKeys;
            }
        }
    }
}
