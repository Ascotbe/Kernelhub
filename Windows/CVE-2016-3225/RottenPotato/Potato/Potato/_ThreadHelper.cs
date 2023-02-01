using System;
using System.Collections;
using System.Threading;

// To build on 3.5 you cant use BlockingConnection, so we add our own.
namespace QHelper
{
    public class BlockingCollection<T>
    {
        protected Object _queueLock = new Object();
        protected Queue _queue = new Queue();
        protected AutoResetEvent _objectAvailableEvent = new AutoResetEvent(false);
        

        public T Take()
        {
            lock (_queueLock)
            {
                if (_queue.Count > 0)
                {
                    return (T)_queue.Dequeue();
                }
            }
            _objectAvailableEvent.WaitOne();
            return Take();
        }
       

        public void Add(Object obj)
        {
            lock (_queueLock)
            {
                _queue.Enqueue(obj);
            }
            _objectAvailableEvent.Set();
        }
    }
}
