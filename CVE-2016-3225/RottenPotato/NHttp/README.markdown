# NHttp

LGPL License.

[Download from NuGet](http://nuget.org/packages/NHttp).

## Introduction

NHttp is a simple asynchronous HTTP server written in C# for the .NET framework.

NHttp supports the following features:

* Full request parsing similar to the ASP.net model;

* High performance asynchronous request processing using TcpListener/TcpClient;

* Complete query string parsing;

* Complete form parsing (i.e. application/x-www-form-urlencoded);

* Complete multi-part parsing including file upload (i.e. multipart/form-data);

* Support for parsing and sending cookies.

NHttp specifically does **not** support any kind of utilities producing output.
It for example does not provide a StreamWriter or perform routing. Besides e.g.
the Headers and Cookies collections, only the raw output stream is provided.
The rest is up to you!

## Usage

The following shows how to use NHttp:

    using (var server = new HttpServer())
    {
        server.RequestReceived += (s, e) =>
        {
            using (var writer = new StreamWriter(e.Response.OutputStream))
            {
                writer.Write("Hello world!");
            }
        };

        server.Start();

        Process.Start(String.Format("http://{0}/", server.EndPoint));

        Console.WriteLine("Press any key to continue...");
        Console.ReadKey();
    }

Processing requests in NHttp is done in the RequestReceived event. There you
have access to the request and response information the request. The example
above creates a StreamReader to be able to write text to the response and
outputs the same response for every request.

By default, NHttp listens to a random port. Use the following method to specify
the port NHttp should listen on:

    using (var server = new HttpServer())
    {
        // ...

        server.EndPoint = new IPEndPoint(IPAddress.Loopback, 80);

        server.Start();

        // ...
    }

This method can also be used to change the interface the HttpServer should be
listening to.

## Logging

NHttp does not have a dependency on a logging framework, but uses Common.Logging
if it's available. To enable logging, add a reference to Common.Logging (the
version doesn't matter) to your project.

## Bugs

Bugs should be reported through github at
[http://github.com/pvginkel/NHttp/issues](http://github.com/pvginkel/NHttp/issues).

## License

NHttp is licensed under the LGPL 3.