using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace SharpCifs.Smb
{

/**
 *
 * @author dev
 */
public class QueuedNtlmContext : INtlmContext {
    
    int _state;    
    Queue<byte[]> _queue;
    
    public bool IsEstablished() {
        return _state > 1;
    }
    
    public QueuedNtlmContext(Queue<byte[]> queue) {
        _queue = queue;
    }

    public byte[] GetServerChallenge() {
        throw new Exception("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    public byte[] GetSigningKey() {
        throw new Exception("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }


    public String GetNetbiosName() {
        return "127.0.0.1";
    }

    public byte[] InitSecContext(byte[] token, int offset, int len) {
        //Console.WriteLine("initSecContext - State " + _state);
        if(_state == 0) {                
            _state++;
            return _queue.Dequeue();
        } else if(_state == 1) {                
            _state++;
            //Console.WriteLine("Adding " + Convert.ToBase64String(token) + " to queue");
            _queue.Enqueue(token);
            Config.signalHandlerClient.Set();
            Config.signalHandler.WaitOne();
            byte[] ret = _queue.Dequeue();
            //Console.WriteLine("Got " + Convert.ToBase64String(ret));
            return ret;
        }      
        return new byte[0];
    }
}

}
