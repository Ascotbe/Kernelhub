using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SharpCifs.Smb
{

    public interface INtlmContext {
        bool IsEstablished();
        byte[] GetServerChallenge();
        byte[] GetSigningKey();
        String GetNetbiosName();
        byte[] InitSecContext(byte[] token, int offset, int len);
    }

}
