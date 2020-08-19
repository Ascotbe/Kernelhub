using System;
using System.Security.Principal;
using System.Runtime.InteropServices;
using System.Text;
using System.ComponentModel;
using QHelper;
using System.Threading;
using Potato;

// SSPIHelper code from PInvoke
// modified slightly and cleaned a little.
namespace SSPITest
{
    public enum SecBufferType
    {
        SECBUFFER_VERSION = 0,
        SECBUFFER_EMPTY = 0,
        SECBUFFER_DATA = 1,
        SECBUFFER_TOKEN = 2
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SecHandle
    {
        IntPtr dwLower;
        IntPtr dwUpper;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SecBuffer : IDisposable
    {
        public int cbBuffer;
        public int BufferType;
        public IntPtr pvBuffer;


        public SecBuffer(int bufferSize)
        {
            cbBuffer = bufferSize;
            BufferType = (int)SecBufferType.SECBUFFER_TOKEN;
            pvBuffer = Marshal.AllocHGlobal(bufferSize);
        }

        public SecBuffer(byte[] secBufferBytes)
        {
            cbBuffer = secBufferBytes.Length;
            BufferType = (int)SecBufferType.SECBUFFER_TOKEN;
            pvBuffer = Marshal.AllocHGlobal(cbBuffer);
            Marshal.Copy(secBufferBytes, 0, pvBuffer, cbBuffer);
        }

        public SecBuffer(byte[] secBufferBytes, SecBufferType bufferType)
        {
            cbBuffer = secBufferBytes.Length;
            BufferType = (int)bufferType;
            pvBuffer = Marshal.AllocHGlobal(cbBuffer);
            Marshal.Copy(secBufferBytes, 0, pvBuffer, cbBuffer);
        }

        public void Dispose()
        {
            if (pvBuffer != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pvBuffer);
                pvBuffer = IntPtr.Zero;
            }
        }
    }

    public struct MultipleSecBufferHelper
    {
        public byte[] Buffer;
        public SecBufferType BufferType;

        public MultipleSecBufferHelper(byte[] buffer, SecBufferType bufferType)
        {
            if (buffer == null || buffer.Length == 0)
            {
                throw new ArgumentException("buffer cannot be null or 0 length");
            }

            Buffer = buffer;
            BufferType = bufferType;
        }
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct SecBufferDesc : IDisposable
    {

        public int ulVersion;
        public int cBuffers;
        public IntPtr pBuffers;

        public SecBufferDesc(int bufferSize)
        {
            ulVersion = (int)SecBufferType.SECBUFFER_VERSION;
            cBuffers = 1;
            SecBuffer ThisSecBuffer = new SecBuffer(bufferSize);
            pBuffers = Marshal.AllocHGlobal(Marshal.SizeOf(ThisSecBuffer));
            Marshal.StructureToPtr(ThisSecBuffer, pBuffers, false);
        }

        public SecBufferDesc(byte[] secBufferBytes)
        {
            ulVersion = (int)SecBufferType.SECBUFFER_VERSION;
            cBuffers = 1;
            SecBuffer ThisSecBuffer = new SecBuffer(secBufferBytes);
            pBuffers = Marshal.AllocHGlobal(Marshal.SizeOf(ThisSecBuffer));
            Marshal.StructureToPtr(ThisSecBuffer, pBuffers, false);
        }

        public SecBufferDesc(MultipleSecBufferHelper[] secBufferBytesArray)
        {
            if (secBufferBytesArray == null || secBufferBytesArray.Length == 0)
            {
                throw new ArgumentException("secBufferBytesArray cannot be null or 0 length");
            }

            ulVersion = (int)SecBufferType.SECBUFFER_VERSION;
            cBuffers = secBufferBytesArray.Length;

            //Allocate memory for SecBuffer Array....
            pBuffers = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SecBuffer)) * cBuffers);

            for (int Index = 0; Index < secBufferBytesArray.Length; Index++)
            {
                //Allocate memory for the individual SecBuffers and just copy the bit values to the SecBuffer array!!!
                SecBuffer ThisSecBuffer = new SecBuffer(secBufferBytesArray[Index].Buffer, secBufferBytesArray[Index].BufferType);

                //We will write out bits in the following order: int cbBuffer; int BufferType; pvBuffer;
                //Note that we won't be releasing the memory allocated by ThisSecBuffer until we are disposed...
                int CurrentOffset = Index * Marshal.SizeOf(typeof(SecBuffer));
                Marshal.WriteInt32(pBuffers, CurrentOffset, ThisSecBuffer.cbBuffer);
                Marshal.WriteInt32(pBuffers, CurrentOffset + Marshal.SizeOf(ThisSecBuffer.cbBuffer), ThisSecBuffer.BufferType);
                Marshal.WriteIntPtr(pBuffers, CurrentOffset + Marshal.SizeOf(ThisSecBuffer.cbBuffer) + Marshal.SizeOf(ThisSecBuffer.BufferType), ThisSecBuffer.pvBuffer);
            }
        }

        public void Dispose()
        {
            if (pBuffers != IntPtr.Zero)
            {
                if (cBuffers == 1)
                {
                    SecBuffer ThisSecBuffer = (SecBuffer)Marshal.PtrToStructure(pBuffers, typeof(SecBuffer));
                    ThisSecBuffer.Dispose();
                }
                else
                {
                    for (int Index = 0; Index < cBuffers; Index++)
                    {
                        int CurrentOffset = Index * Marshal.SizeOf(typeof(SecBuffer));
                        IntPtr SecBufferpvBuffer = Marshal.ReadIntPtr(pBuffers, CurrentOffset + Marshal.SizeOf(typeof(int)) + Marshal.SizeOf(typeof(int)));
                        Marshal.FreeHGlobal(SecBufferpvBuffer);
                    }
                }

                Marshal.FreeHGlobal(pBuffers);
                pBuffers = IntPtr.Zero;
            }
        }

        public byte[] GetSecBufferByteArray()
        {
            byte[] Buffer = null;

            if (pBuffers == IntPtr.Zero)
            {
                throw new InvalidOperationException("Object has already been disposed!!!");
            }

            if (cBuffers == 1)
            {
                SecBuffer ThisSecBuffer = (SecBuffer)Marshal.PtrToStructure(pBuffers, typeof(SecBuffer));

                if (ThisSecBuffer.cbBuffer > 0)
                {
                    Buffer = new byte[ThisSecBuffer.cbBuffer];
                    Marshal.Copy(ThisSecBuffer.pvBuffer, Buffer, 0, ThisSecBuffer.cbBuffer);
                }
            }
            else
            {
                int BytesToAllocate = 0;

                for (int Index = 0; Index < cBuffers; Index++)
                {
                    //calculate the total number of bytes we need to copy...
                    int CurrentOffset = Index * Marshal.SizeOf(typeof(SecBuffer));
                    BytesToAllocate += Marshal.ReadInt32(pBuffers, CurrentOffset);
                }

                Buffer = new byte[BytesToAllocate];

                for (int Index = 0, BufferIndex = 0; Index < cBuffers; Index++)
                {
                    //Now iterate over the individual buffers and put them together into a byte array...
                    int CurrentOffset = Index * Marshal.SizeOf(typeof(SecBuffer));
                    int BytesToCopy = Marshal.ReadInt32(pBuffers, CurrentOffset);
                    IntPtr SecBufferpvBuffer = Marshal.ReadIntPtr(pBuffers, CurrentOffset + Marshal.SizeOf(typeof(int)) + Marshal.SizeOf(typeof(int)));
                    Marshal.Copy(SecBufferpvBuffer, Buffer, BufferIndex, BytesToCopy);
                    BufferIndex += BytesToCopy;
                }
            }

            return (Buffer);
        }
    }
    //
    [StructLayout(LayoutKind.Sequential)]
    public struct SECURITY_INTEGER
    {
        public uint LowPart;
        public int HighPart;
        public SECURITY_INTEGER(int dummy)
        {
            LowPart = 0;
            HighPart = 0;
        }
    };
    //
    [StructLayout(LayoutKind.Sequential)]
    public struct SECURITY_HANDLE
    {
        public IntPtr LowPart;
        public IntPtr HighPart;
        public SECURITY_HANDLE(int dummy)
        {
            LowPart = HighPart = IntPtr.Zero;
        }
    };
    //
    [StructLayout(LayoutKind.Sequential)]
    public struct SecPkgContext_Sizes
    {
        public uint cbMaxToken;
        public uint cbMaxSignature;
        public uint cbBlockSize;
        public uint cbSecurityTrailer;
    };
    //
    public class SSPIHelper
    {
        public static AutoResetEvent signalHandlerClient = new AutoResetEvent(false);
        bool bContinueServer = true;

        public const int TOKEN_QUERY = 0x00008;
        public const int SEC_E_OK = 0;
        public const int SEC_I_CONTINUE_NEEDED = 0x90312;
        const int SECPKG_CRED_BOTH = 3;
        const int SECPKG_CRED_OUTBOUND = 2;
        const int SECPKG_CRED_INBOUND = 1;
        const int SECURITY_NATIVE_DREP = 0x10;
        const int MAX_TOKEN_SIZE = 12288;
        //
        SECURITY_HANDLE _hInboundCred = new SECURITY_HANDLE(0);
        public SECURITY_HANDLE _hServerContext = new SECURITY_HANDLE(0);
        //
        SECURITY_HANDLE _hOutboundCred = new SECURITY_HANDLE(0);
        public SECURITY_HANDLE _hClientContext = new SECURITY_HANDLE(0);
        //
        public const int ISC_REQ_DELEGATE = 0x00000001;
        public const int ISC_REQ_MUTUAL_AUTH = 0x00000002;
        public const int ISC_REQ_REPLAY_DETECT = 0x00000004;
        public const int ISC_REQ_SEQUENCE_DETECT = 0x00000008;
        public const int ISC_REQ_CONFIDENTIALITY = 0x00000010;
        public const int ISC_REQ_USE_SESSION_KEY = 0x00000020;
        public const int ISC_REQ_PROMPT_FOR_CREDS = 0x00000040;
        public const int ISC_REQ_USE_SUPPLIED_CREDS = 0x00000080;
        public const int ISC_REQ_ALLOCATE_MEMORY = 0x00000100;
        public const int ISC_REQ_USE_DCE_STYLE = 0x00000200;
        public const int ISC_REQ_DATAGRAM = 0x00000400;
        public const int ISC_REQ_CONNECTION = 0x00000800;
        public const int ISC_REQ_CALL_LEVEL = 0x00001000;
        public const int ISC_REQ_FRAGMENT_SUPPLIED = 0x00002000;
        public const int ISC_REQ_EXTENDED_ERROR = 0x00004000;
        public const int ISC_REQ_STREAM = 0x00008000;
        public const int ISC_REQ_INTEGRITY = 0x00010000;
        public const int ISC_REQ_IDENTIFY = 0x00020000;
        public const int ISC_REQ_NULL_SESSION = 0x00040000;
        public const int ISC_REQ_MANUAL_CRED_VALIDATION = 0x00080000;
        public const int ISC_REQ_RESERVED1 = 0x00100000;
        public const int ISC_REQ_FRAGMENT_TO_FIT = 0x00200000;

        public const int SECPKG_ATTR_SIZES = 0;

        public const int STANDARD_CONTEXT_ATTRIBUTES = ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_CONNECTION;

        bool _bGotClientCredentials = false;
        bool _bGotServerCredentials = false;
        bool _bGotServerContext = false;

        // START
        [StructLayout(LayoutKind.Sequential, Pack = 0)]
        public struct SecPkgContext_SessionKey
        {
            //[MarshalAs(UnmanagedType.SysUInt)] internal uint SessionKeyLength;
            public uint SessionKeyLength;
            public IntPtr Sessionkey;
            public static int Size = Marshal.SizeOf(typeof(SecPkgContext_SessionKey));
            public static int SessionkeyOffset = (int)Marshal.OffsetOf(typeof(SecPkgContext_SessionKey), "Sessionkey");
        }

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern int QueryContextAttributes(
            ref SECURITY_HANDLE phContext,
            uint ulAttribute,
            out SecPkgContext_SessionKey pSessKeyOut);

        [DllImport("advapi32.dll", CharSet = CharSet.Auto)]
        public static extern bool GetUserName(StringBuilder sb, ref Int32 length);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern int QuerySecurityContextToken(
            ref SECURITY_HANDLE phContext,
            out IntPtr hToken);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int ExportSecurityContext(
            ref SECURITY_HANDLE phContext,
            uint fFlags,
            IntPtr pPackedContent,
            IntPtr pToken);

        [StructLayout(LayoutKind.Sequential)]
        public struct SecBuff
        {
            public int cbBuffer;
            public int BufferType;
            public IntPtr pvBuffer;
        }

        [DllImport("advapi32.dll", SetLastError = true)]
        static extern bool ImpersonateLoggedOnUser(IntPtr hToken);

        // END
        [DllImport("secur32.dll", CharSet = CharSet.Auto)]
        static extern int AcquireCredentialsHandle(
            string pszPrincipal,                                                   //SEC_CHAR*
            string pszPackage,                                                     //SEC_CHAR* //"Kerberos","NTLM","Negotiative"
            int fCredentialUse,
            IntPtr PAuthenticationID,                                              //_LUID AuthenticationID,//pvLogonID, //PLUID
            IntPtr pAuthData,                                                      //PVOID
            int pGetKeyFn,                                                         //SEC_GET_KEY_FN
            IntPtr pvGetKeyArgument,                                               //PVOID
            ref SECURITY_HANDLE phCredential,                                      //SecHandle //PCtxtHandle ref
            ref SECURITY_INTEGER ptsExpiry);                                       //PTimeStamp //TimeStamp ref

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern int InitializeSecurityContext(
            ref SECURITY_HANDLE phCredential,                                       //PCredHandle
            IntPtr phContext,                                                       //PCtxtHandle
            string pszTargetName,
            int fContextReq,
            int Reserved1,
            int TargetDataRep,
            IntPtr pInput,                                                          //PSecBufferDesc SecBufferDesc
            int Reserved2,
            out SECURITY_HANDLE phNewContext,                                       //PCtxtHandle
            out SecBufferDesc pOutput,                                              //PSecBufferDesc SecBufferDesc
            out uint pfContextAttr,                                                 //managed ulong == 64 bits!!!
            out SECURITY_INTEGER ptsExpiry);                                        //PTimeStamp

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern int InitializeSecurityContext(
            ref SECURITY_HANDLE phCredential,                                       //PCredHandle
            ref SECURITY_HANDLE phContext,                                          //PCtxtHandle
            string pszTargetName,
            int fContextReq,
            int Reserved1,
            int TargetDataRep,
            ref SecBufferDesc SecBufferDesc,                                        //PSecBufferDesc SecBufferDesc
            int Reserved2,
            out SECURITY_HANDLE phNewContext,                                       //PCtxtHandle
            out SecBufferDesc pOutput,                                              //PSecBufferDesc SecBufferDesc
            out uint pfContextAttr,                                                 //managed ulong == 64 bits!!!
            out SECURITY_INTEGER ptsExpiry);                                        //PTimeStamp

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        static extern int AcceptSecurityContext(
            ref SECURITY_HANDLE phCredential,
            IntPtr phContext,
            ref SecBufferDesc pInput,
            uint fContextReq,
            uint TargetDataRep,
            out SECURITY_HANDLE phNewContext,
            out SecBufferDesc pOutput,
            out uint pfContextAttr,
            out SECURITY_INTEGER ptsTimeStamp);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        static extern int AcceptSecurityContext(
            ref SECURITY_HANDLE phCredential,
            ref SECURITY_HANDLE phContext,
            ref SecBufferDesc pInput,
            uint fContextReq,
            uint TargetDataRep,
            out SECURITY_HANDLE phNewContext,
            out SecBufferDesc pOutput,
            out uint pfContextAttr,
            out SECURITY_INTEGER ptsTimeStamp);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int ImpersonateSecurityContext(ref SECURITY_HANDLE phContext);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int QueryContextAttributes(
            ref SECURITY_HANDLE phContext,
            uint ulAttribute,
            out SecPkgContext_Sizes pContextAttributes);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int EncryptMessage(
            ref SECURITY_HANDLE phContext,
            uint fQOP,
            ref SecBufferDesc pMessage,
            uint MessageSeqNo);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int DecryptMessage(
            ref SECURITY_HANDLE phContext,
            ref SecBufferDesc pMessage,
            uint MessageSeqNo,
            out uint pfQOP);

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int MakeSignature(
            ref SECURITY_HANDLE phContext,                                         // Context to use
            uint fQOP,                                                             // Quality of Protection
            ref SecBufferDesc pMessage,                                            // Message to sign
            uint MessageSeqNo);                                                    // Message Sequence Num.

        [DllImport("secur32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern int VerifySignature(
            ref SECURITY_HANDLE phContext,                                         // Context to use
            ref SecBufferDesc pMessage,                                            // Message to sign
            uint MessageSeqNo,                                                     // Message Sequence Num.
            out uint pfQOP);

        string _sAccountName = WindowsIdentity.GetCurrent().Name;

        public SSPIHelper()
        {

        }

        public SSPIHelper(string sRemotePrincipal)
        {
            _sAccountName = sRemotePrincipal;
        }

        public void InitializeClient(out byte[] clientToken, byte[] serverToken, out bool bContinueProcessing)
        {
            clientToken = null;
            bContinueProcessing = true;
            SECURITY_INTEGER ClientLifeTime = new SECURITY_INTEGER(0);
            if (!_bGotClientCredentials) // check if we already have a cred handle, if not call it.
            {
                if (AcquireCredentialsHandle(
                    _sAccountName,             // NULL // or _sAccountName = WindowsIdentity.GetCurrent().Name;
                    "Negotiate",               // lpPackageName = "Negotiate"
                    SECPKG_CRED_OUTBOUND,      // SECPKG_CRED_OUTBOUND
                    IntPtr.Zero,               // NULL
                    IntPtr.Zero,               // NULL
                    0,                         // NULL
                    IntPtr.Zero,               // NULL
                    ref _hOutboundCred,        // &hCred
                    ref ClientLifeTime) != SEC_E_OK) //&Lifetime
                {
                    throw new Exception("Couldn't acquire server credentials handle!!!");
                }
                _bGotClientCredentials = true;
            }

            int ss = -1;
            SecBufferDesc ClientToken = new SecBufferDesc(MAX_TOKEN_SIZE);
            try
            {
                uint ContextAttributes = 0;
                if (serverToken == null)
                {
                    ss = InitializeSecurityContext(ref _hOutboundCred,   // hCred
                        IntPtr.Zero,                                     // NULL first time
                        _sAccountName,                                   // name of target "self",
                        STANDARD_CONTEXT_ATTRIBUTES,                     // ISC_REQ_CONFIDENTIALITY // "null/sess options in flags!!!"
                        0,                                               // 0   // Reserved1,
                        SECURITY_NATIVE_DREP,                            // SECURITY_NATIVE_DREP
                        IntPtr.Zero,                                     // NULL // only first time
                        0,                                               // 0 Reserved2,
                        out _hClientContext,                             // pHandle CtxtHandle = SecHandle
                        out ClientToken,                                 // ref SecBufferDesc // output "Hash"
                        out ContextAttributes,                           // ref int pfContextAttr, // out attributes [int]
                        out ClientLifeTime);                             //ref IntPtr ptsExpiry ); //PTimeStamp
                }
                else
                {
                    SecBufferDesc ServerToken = new SecBufferDesc(serverToken);
                    try
                    {
                        ss = InitializeSecurityContext(ref _hOutboundCred,
                            ref _hClientContext,
                            _sAccountName,                          // null string pszTargetName,
                            STANDARD_CONTEXT_ATTRIBUTES,
                            0,                                      //int Reserved1,
                            SECURITY_NATIVE_DREP,                   //int TargetDataRep
                            ref ServerToken,                        //Always zero first time around...
                            0,                                      //int Reserved2,
                            out _hClientContext,                    //pHandle CtxtHandle = SecHandle
                            out ClientToken,                        //ref SecBufferDesc pOutput, //PSecBufferDesc
                            out ContextAttributes,                  //ref int pfContextAttr,
                            out ClientLifeTime);                    //ref IntPtr ptsExpiry ); //PTimeStamp
                    }
                    finally
                    {
                        ServerToken.Dispose();
                    }
                }
                //
                if (ss != SEC_E_OK && ss != SEC_I_CONTINUE_NEEDED)
                {
                    Console.WriteLine("InitializeSecurityContext() failed!!!");
                    Console.WriteLine(new Win32Exception(Marshal.GetLastWin32Error()));
                }
                clientToken = ClientToken.GetSecBufferByteArray();
            }
            finally
            {
                ClientToken.Dispose();
            }

            bContinueProcessing = ss != SEC_E_OK;
        }
        // This is what we use for all the token stuff.
        public void InitializeServer(byte[] clientToken, out byte[] serverToken, out bool bContinueProcessing)
        {
            serverToken = null;
            bContinueProcessing = true;
            SECURITY_INTEGER NewLifeTime = new SECURITY_INTEGER(0);

            if (!_bGotServerCredentials)
            {
                if (AcquireCredentialsHandle(
                    _sAccountName,
                    "Negotiate",
                    SECPKG_CRED_INBOUND,
                    IntPtr.Zero,
                    IntPtr.Zero,
                    0,
                    IntPtr.Zero,
                    ref _hInboundCred,
                    ref NewLifeTime) != SEC_E_OK)
                {
                    throw new Exception("Couldn't acquire server credentials handle!!!");
                }

                _bGotServerCredentials = true;
            }
            //
            SecBufferDesc ServerToken = new SecBufferDesc(MAX_TOKEN_SIZE);
            SecBufferDesc ClientToken = new SecBufferDesc(clientToken);
            //
            try
            {
                int ss = -1; 
                uint uNewContextAttr = 0;

                if (!_bGotServerContext) // check if we have the context yet
                {
                    ss = AcceptSecurityContext(ref _hInboundCred,        // [in] handle to the credentials
                        IntPtr.Zero,                                     // [in/out] handle partially formed context. NULL the first time
                        ref ClientToken,                                 // [in] pointer to the input buffers
                        STANDARD_CONTEXT_ATTRIBUTES,                     // [in] required context attributes
                        SECURITY_NATIVE_DREP,                            // [in] data representation on the target
                        out _hServerContext,                             // [in/out] receives the new context handle    
                        out ServerToken,                                 // [in/out] pointer to the output buffers
                        out uNewContextAttr,                             // [out] receives the context attributes        
                        out NewLifeTime);                                // [out] receives the life span of the security context
                }
                else
                {
                    ss = AcceptSecurityContext(ref _hInboundCred,        // [in] handle to the credentials
                        ref _hServerContext,                             // [in/out] handle of partially formed context. NULL the first time
                        ref ClientToken, //NOT a token[InBuffDesc]       // [in] pointer to the input buffers
                        STANDARD_CONTEXT_ATTRIBUTES,                     // [in] required context attributes
                        SECURITY_NATIVE_DREP,                            // [in] data representation on the target
                        out _hServerContext,                             // [in/out] receives the new context handle    
                        out ServerToken,                                 // [in/out] pointer to the output buffers
                        out uNewContextAttr,                             // [out] receives the context attributes        
                        out NewLifeTime);                                // [out] receives the life span of the security context
                }

                if (ss != SEC_E_OK && ss != SEC_I_CONTINUE_NEEDED)
                {
                    Console.WriteLine("AcceptSecurityContext() failed!!!");
                    Console.WriteLine(new Win32Exception(Marshal.GetLastWin32Error()));
                }

                if (!_bGotServerContext)
                {
                    _bGotServerContext = true;
                }

                serverToken = ServerToken.GetSecBufferByteArray();

                bContinueProcessing = ss != SEC_E_OK;
            }
            finally
            {
                ClientToken.Dispose();
                ServerToken.Dispose();
            }
        }
        //
        public static void mygetuser()
        {
            StringBuilder Buffer = new StringBuilder(64);
            int nSize = 64;
            GetUserName(Buffer, ref nSize);
            Console.WriteLine("You are now: {0}", Buffer.ToString());
        }
        //
        public void TokenRelay(BlockingCollection<byte[]> hashesIn, BlockingCollection<byte[]> hashesOut)
        {
            while (bContinueServer)
            {

                byte[] out_buffer = null;
                byte[] hash = hashesIn.Take();
                InitializeServer(hash, out out_buffer, out bContinueServer);
                hashesOut.Add(out_buffer);
                if (bContinueServer)
                {
                    hash = hashesIn.Take();
                    InitializeServer(hash, out out_buffer, out bContinueServer);

                    ImpersonateSecurityContext(ref _hServerContext);
                    // The above is just easier
                    //IntPtr hToken = IntPtr.Zero;
                    mygetuser();

                    /*
                    if (V.have_seimpersonate)
                    {
                        QuerySecurityContextToken(ref _hServerContext, out hToken);
                        ImpersonateLoggedOnUser(hToken);
                    }
                    */

                    Console.WriteLine("BOOM!... Sleep: {0}msec", V.sleep_time);
                    Thread.Sleep(V.sleep_time);

                    hashesOut.Add(new byte[] { 99 }); // if finished pass 99
                }
            }
        }
    }
}