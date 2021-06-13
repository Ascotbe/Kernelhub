#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define BUFSIZE 4096

class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	int connectTCP(const char* hostname, const char* port);

private:
	static DWORD WINAPI ReceiveDataFromSocket(LPVOID lpvParam);
	static DWORD WINAPI SendDataFromConsole(LPVOID lpvParam);
};
