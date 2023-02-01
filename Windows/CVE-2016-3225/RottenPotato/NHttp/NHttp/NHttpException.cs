using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Text;

namespace NHttp
{
    [Serializable]
    public class NHttpException : Exception
    {
        public NHttpException()
        {
        }

        public NHttpException(string message)
            : base(message)
        {
        }

        public NHttpException(string message, Exception innerException)
            : base(message, innerException)
        {
        }

        protected NHttpException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }
    }
}
