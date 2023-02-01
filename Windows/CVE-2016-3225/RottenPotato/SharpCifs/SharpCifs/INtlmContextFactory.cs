using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpCifs.Smb;

namespace SharpCifs
{

    public interface INtlmContextFactory {
        INtlmContext create(NtlmPasswordAuthentication auth, bool doSigning);
    }

}
