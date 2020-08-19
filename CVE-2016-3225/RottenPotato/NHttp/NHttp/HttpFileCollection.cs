using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.Specialized;

namespace NHttp
{
    public class HttpFileCollection : NameObjectCollectionBase
    {
        private string[] _allKeys;

        internal HttpFileCollection()
            : base(StringComparer.OrdinalIgnoreCase)
        {
        }

        public HttpPostedFile Get(string name)
        {
            return (HttpPostedFile)BaseGet(name);
        }

        public HttpPostedFile this[string name]
        {
            get { return Get(name); }
        }

        public HttpPostedFile Get(int index)
        {
            return (HttpPostedFile)BaseGet(index);
        }

        public string GetKey(int index)
        {
            return BaseGetKey(index);
        }

        public HttpPostedFile this[int index]
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

        internal void AddFile(string name, HttpPostedFile httpPostedFile)
        {
            BaseAdd(name, httpPostedFile);
        }
    }
}
