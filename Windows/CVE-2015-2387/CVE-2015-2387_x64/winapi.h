#ifndef _WINAPI_H
#define _WINAPI_H

#include <windows.h>
#include <Wininet.h>
#include <ShlObj.h>

typedef HMODULE (WINAPI *LOADLIBRARYA)(LPSTR lpFileName);
typedef LOADLIBRARYA *PLOADLIBRARYA;
typedef HMODULE (WINAPI *LOADLIBRARYW)(LPWSTR lpFileName);
typedef LOADLIBRARYW *PLOADLIBRARYW;
typedef HMODULE (WINAPI *GETPROCADDRESS)( HMODULE hModule,  LPSTR lpProcName);
typedef GETPROCADDRESS *PGETPROCADDRESS;

typedef LPVOID (WINAPI *VIRTUALALLOC)( LPVOID lpAddress,  SIZE_T dwSize,  DWORD flAllocationType,  DWORD flProtect);
typedef BOOL (WINAPI *VIRTUALPROTECT)( LPVOID lpAddress,  SIZE_T dwSize,  DWORD flNewProtect,  PDWORD lpflOldProtect);

typedef VOID (WINAPI *EXITTHREAD)(  DWORD dwExitCode);
typedef VOID (WINAPI *EXITPROCESS)( DWORD uExitReason);
typedef DWORD (WINAPI *GETSHORTPATHNAMEW)( LPWSTR lpszLongPath,  LPWSTR lpszShortPath,  DWORD cchBuffer);
typedef DWORD (WINAPI *GETMODULEFILENAMEW)( HMODULE hModule,  LPWSTR lpFilename,  DWORD nSize);
typedef BOOL (WINAPI *SHGETSPECIALFOLDERPATHW)( HWND hwndOwner,  LPWSTR lpszPath,  int csidl,  BOOL fCreate);
typedef DWORD (WINAPI *GETFILEATTRIBUTESA)( LPSTR strFileName);
typedef BOOL (WINAPI *DELETEFILEA)( LPSTR lpFileName);
typedef HINTERNET (WINAPI *INTERNETOPENA)( LPSTR lpszAgent,  DWORD dwAccessType,  LPSTR lpszProxyName,  LPSTR lpszProxyBypass,  DWORD dwFlags);
typedef HINTERNET (WINAPI *INTERNETOPENURLA)( HINTERNET hInternet,  LPSTR lpszUrl,  LPSTR lpszHeaders,   DWORD dwHeadersLength,  DWORD dwFlags,  DWORD_PTR dwContext);
typedef BOOL (WINAPI *HTTPQUERYINFOW)( HINTERNET hRequest,  DWORD dwInfoLevel,  LPVOID lpvBuffer,  LPDWORD lpdwBufferLength,  LPDWORD lpdwIndex);
typedef BOOL (WINAPI *INTERNETREADFILEEXA)( HINTERNET hFile,  LPINTERNET_BUFFERS lpBuffersOut,  DWORD dwFlags,  DWORD dwContext);
typedef HANDLE (WINAPI *CREATEFILEW)( LPWSTR lpFileName,  DWORD dwDesiredAccess,  DWORD dwShareMode,  LPSECURITY_ATTRIBUTES lpSecurityAttributes,  DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes,  HANDLE hTemplateFile);
typedef BOOL (WINAPI *WRITEFILE)( HANDLE hFile,  LPCVOID lpBuffer,  DWORD nNumberOfBytesToWrite,  LPDWORD lpNumberOfBytesWritten,  LPOVERLAPPED lpOverlapped);
typedef UINT (WINAPIV *WTOI)( LPWSTR string);
typedef BOOL (WINAPI *GETUSERPROFILEDIRECTORYW)( HANDLE hToken,  LPWSTR lpProfileDir,  LPDWORD lpcchSize);
typedef BOOL (WINAPI *OPENPROCESSTOKEN)( HANDLE ProcessHandle,  DWORD DesiredAccess,  PHANDLE TokenHandle);
typedef BOOL (WINAPI *DELETEFILEA)( LPSTR strFileName);
typedef BOOL (WINAPI *FREELIBRARY)( HMODULE hMod);
typedef DWORD (WINAPI *WCSTOMBS)( LPSTR lpStr,  LPWSTR lpWstr,  DWORD count);
typedef HANDLE (WINAPI *CREATETHREAD)( LPSECURITY_ATTRIBUTES lpThreadAttributes,  SIZE_T dwStackSize,  LPTHREAD_START_ROUTINE lpStartAddress,  LPVOID lpParameter,  DWORD dwCreationFlags,  LPDWORD lpThreadId);
typedef DWORD (WINAPI *WAITFORSINGLEOBJECT)( HANDLE hHandle,  DWORD dwMilliseconds);
typedef BOOL (WINAPI *TERMINATETHREAD)( HANDLE hThread,  DWORD dwExitCode);
typedef BOOL (WINAPI *CLOSEHANDLE)( HANDLE hFile);
typedef BOOL (WINAPI *GETTEMPPATHW)( DWORD nBufferLength,  LPWSTR lpBuffer);
typedef BOOL (WINAPI *SHGETKNOWNFOLDERPATH)( GUID rfid,  DWORD dwFlags,  HANDLE hToken,  LPWSTR *ppszPath);
typedef BOOL (WINAPI *DELETEFILEW)( LPWSTR lpFileName);

/* exploit api */

/* kernel32  */
typedef BOOL (WINAPI *ISWOW64PROCESS)(   HANDLE hProcess,    PBOOL Wow64Process);
typedef HANDLE (WINAPI *GETCURRENTPROCESS)(VOID);
typedef BOOL (WINAPI *GETVERSIONEXA)(   LPOSVERSIONINFO lpVersionInfo);
typedef HMODULE (WINAPI *GETMODULEHANDLE)(  LPCTSTR lpModuleName);
typedef HLOCAL (WINAPI *LOCALALLOC)(   UINT uFlags,   SIZE_T uBytes);
typedef HLOCAL (WINAPI *LOCALFREE)(   HLOCAL hMem );
typedef UINT (WINAPI *GETWINDOWSDIRECTORY)(   LPSTR lpBuffer,    UINT uSize );
typedef BOOL (WINAPI *DUPLICATEHANDLE)(  HANDLE hSourceProcessHandle,    HANDLE hSourceHandle,     HANDLE hTargetProcessHandle,    LPHANDLE lpTargetHandle,     DWORD dwDesiredAccess,     BOOL bInheritHandle,     DWORD dwOptions);
typedef BOOL (WINAPI* READFILE)(  HANDLE hFile,    LPVOID lpBuffer,    DWORD nNumberOfBytesToRead,     LPDWORD lpNumberOfBytesRead,   LPOVERLAPPED lpOverlapped);
typedef DWORD (WINAPI* GETFILESIZE)(   HANDLE hFile,    LPDWORD lpFileSizeHigh);
typedef VOID (WINAPI* OUTPUTDEBUGSTRINGA)(  LPCSTR lpOutputString);
typedef HANDLE (WINAPI *GETPROCESSHEAP)(VOID);
typedef LPVOID (WINAPI *HEAPALLOC)(  HANDLE hHeap,   DWORD dwFlags,   SIZE_T dwBytes);
typedef BOOL (WINAPI *HEAPFREE)(  HANDLE hHeap,   DWORD dwFlags,   LPVOID lpMem);
typedef ULONGLONG (WINAPI *VERSETCONDITIONMASK)(  ULONGLONG dwlConditionMask,   DWORD dwTypeBitMask,   BYTE dwConditionMask);
typedef BOOL (WINAPI *VERIFYVERSIONINFOA)(  LPOSVERSIONINFOEX lpVersionInfo,   DWORD dwTypeMask,   DWORDLONG dwlConditionMask);
typedef VOID (WINAPI *SLEEP)( DWORD dwMilliseconds);

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

typedef DWORD (WINAPI *GETCURRENTPROCESSID)(void);

/* gdi32 */
typedef LONG (WINAPI *SETBITMAPBITS)(  HBITMAP hbmp,   DWORD cBytes,   const VOID *lpBits);
typedef LONG (WINAPI *GETBITMAPBITS)(   HBITMAP hbmp,    LONG cbBuffer,    LPVOID lpvBits);
typedef BOOL (WINAPI *DELETEOBJECT) (   HGDIOBJ hObject);
typedef HBITMAP (WINAPI *CREATEBITMAP)(   int nWidth,   int nHeight,   UINT cPlanes,    UINT cBitsPerPel,    const VOID *lpvBits);
typedef HANDLE (WINAPI *ADDFONTMEMRESOURCEEX)(    PVOID pbFont,    DWORD cbFont,    PVOID pdv,    DWORD *pcFonts);


/* Ws2_32 */
typedef unsigned long (WINAPI *HTONL)( unsigned long hostlong);
typedef unsigned short (WINAPI *HTONS)( unsigned short hostshort); 


/* Wtsapi32 */
typedef BOOL (WINAPI *LPFWTSQueryUserToken)(    ULONG SessionId,   PHANDLE phToken);


/* msvcrt */
struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;

typedef void *(__cdecl *MALLOC)(   size_t size );
typedef void (__cdecl *FREE)(    void *memblock );
typedef size_t (__cdecl *FREAD)(void *buffer,   size_t size,   size_t count,   FILE *stream );
typedef FILE *(__cdecl *FOPEN)( const char *filename,   const char *mode );
typedef int (__cdecl *FSEEK)(    FILE *stream,   long offset,   int origin );
typedef long (__cdecl *FTELL)(    FILE *stream );
typedef int (__cdecl *FCLOSE)(    FILE *stream );
typedef int (__cdecl *PRINTF)( const char *format, ... );
typedef int (__cdecl *SNPRINTF)(  char *buffer,  size_t count,  const char *format, ... );

//
typedef int (_stdcall *fpNamedEscape)(HDC hdc, wchar_t *pDriver, int nEscape, int cbInput, void* lpszInData, int cbOutput, void* lpszOutData);



typedef struct {
	unsigned int handle0;
	unsigned int unk0[4];
	unsigned int handle1;
	unsigned int unk1[2];
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int address0;
	unsigned int address1;
	unsigned int scansize;
	unsigned int unk2;
	unsigned int bmpformat;
	unsigned int surftype;
	unsigned int unk3;
	unsigned int surflags;
	unsigned int unk4[12];
} SURFACE;

typedef struct {
	unsigned int handle0;
	unsigned int unk0[7];
	unsigned int handle1;
	unsigned int unk1[5];
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int sizeh;
	unsigned int address0low;
	unsigned int address0high;
	unsigned int address1low;
	unsigned int address1high;
	unsigned int scansize;
	unsigned int unk2;
	unsigned int bmpformat;
	unsigned int surftype;
	unsigned int unk3[2];
	unsigned int surflags;
	unsigned int unk4[12];
} SURFACE64;


typedef LONG R_NTSTATUS;


typedef enum _R_SYSTEM_INFORMATION_CLASS {
	_r_SystemModuleInformation = 11,
	_r_SystemHandleInformation = 16
} R_SYSTEM_INFORMATION_CLASS;

typedef struct _R_SYSTEM_HANDLE
{
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} R_SYSTEM_HANDLE, *R_PSYSTEM_HANDLE;

typedef struct _R_SYSTEM_HANDLE_INFORMATION
{
    ULONG HandleCount; /* Or NumberOfHandles if you prefer. */
    R_SYSTEM_HANDLE Handles[1];
} R_SYSTEM_HANDLE_INFORMATION, *R_PSYSTEM_HANDLE_INFORMATION;


typedef R_NTSTATUS (WINAPI *_R_NtQuerySystemInformation)(R_SYSTEM_INFORMATION_CLASS SystemInformationClass,	PVOID SystemInformation,	ULONG SystemInformationLength,	PULONG ReturnLength		);

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
} R_SYSTEM_MODULE_INFORMATION_ENTRY, *R_PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct {
	ULONG   Count;
	R_SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} R_SYSTEM_MODULE_INFORMATION, *R_PSYSTEM_MODULE_INFORMATION;

#define _VER_SET_CONDITION(_m_,_t_,_c_)  ( (_m_)=lpTable->fpVerSetConditionMask((_m_),(_t_),(_c_)) )

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;

typedef UNICODE_STRING *PUNICODE_STRING;

typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef VOID (NTAPI *PPS_POST_PROCESS_INIT_ROUTINE) (   VOID    );

typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[2];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[2];
    PVOID DllBase;
    PVOID Reserved3[2];
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    } DUMMYUNIONNAME;
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB, *PPEB;

#endif // _WINAPI_H