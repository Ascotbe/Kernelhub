/************************************************************************
*                                                                       *
*   winternl.h -- This module defines the internal NT APIs and data     *
*       structures that are intended for the use only by internal core  *
*       Windows components.  These APIs and data structures may change  *
*       at any time.                                                    *
*                                                                       *
*   These APIs and data structures are subject to changes from one      *
*       Windows release to another Windows release.  To maintain the    *
*       compatiblity of your application, avoid using these APIs and    *
*       data structures.                                                *
*                                                                       *
*   The appropriate mechanism for accessing the functions defined in    *
*       this header is to use LoadLibrary() for ntdll.dll and           *
*       GetProcAddress() for the particular function.  By using this    *
*       approach, your application will be more resilient to changes    *
*       for these functions between Windows releases.  If a function    *
*       prototype does change, then GetProcAddress() for that function  *
*       might detect the change and fail the function call, which your  *
*       application will be able to detect.  GetProcAddress() may not   *
*       be able to detect all signature changes, thus avoid using these *
*       internal functions.  Instead, your application should use the   *
*       appropriate Win32 function that provides equivalent or similiar *
*       functionality.                                                  *
*                                                                       *
*   Copyright (c) Microsoft Corp. All rights reserved.                  *
*                                                                       *
************************************************************************/

#ifndef _WINTERNL_
#define _WINTERNL_

#if (_WIN32_WINNT >= 0x0500)

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// The PEB and TEB structures are subject to changes between Windows
// releases, thus the fields offsets may change as well as the Reserved
// fields.  The Reserved fields are reserved for use only by the Windows
// operating systems.  Do not assume a maximum size for the structures.
//

//
// Instead of using the BeingDebugged field, use the Win32 APIs
//     IsDebuggerPresent, CheckRemoteDebuggerPresent
// Instead of using the SessionId field, use the Win32 APIs
//     GetCurrentProcessId and ProcessIdToSessionId
// Sample x86 assembly code that gets the SessionId (subject to change
//     between Windows releases, use the Win32 APIs to make your application
//     resilient to changes)
//     mov     eax,fs:[00000018]
//     mov     eax,[eax+0x30]
//     mov     eax,[eax+0x1d4]
//
typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[229];
    PVOID Reserved3[59];
    ULONG SessionId;
} PEB, *PPEB;

//
// Instead of using the Tls fields, use the Win32 TLS APIs
//     TlsAlloc, TlsGetValue, TlsSetValue, TlsFree
//
// Instead of using the ReservedForOle field, use the COM API
//     CoGetContextToken
//
typedef struct _TEB {
    BYTE Reserved1[1952];
    PVOID Reserved2[412];
    PVOID TlsSlots[64];
    BYTE Reserved3[8];
    PVOID Reserved4[26];
    PVOID ReservedForOle;  // Windows 2000 only
    PVOID Reserved5[4];
    PVOID TlsExpansionSlots;
} TEB;
typedef TEB *PTEB;

//
// These data structures and type definitions are needed for compilation and
// use of the internal Windows APIs defined in this header.
//
typedef LONG NTSTATUS;

typedef CONST char *PCSZ;

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;
typedef PSTRING PCANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;
typedef CONST STRING* PCOEM_STRING;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );

#if defined(_M_IA64)

typedef struct _FRAME_POINTERS {
    ULONGLONG MemoryStackFp;
    ULONGLONG BackingStoreFp;
} FRAME_POINTERS, *PFRAME_POINTERS;

#define UNWIND_HISTORY_TABLE_SIZE 12

typedef struct _RUNTIME_FUNCTION {
    ULONG BeginAddress;
    ULONG EndAddress;
    ULONG UnwindInfoAddress;
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;

typedef struct _UNWIND_HISTORY_TABLE_ENTRY {
    ULONG64 ImageBase;
    ULONG64 Gp;
    PRUNTIME_FUNCTION FunctionEntry;
} UNWIND_HISTORY_TABLE_ENTRY, *PUNWIND_HISTORY_TABLE_ENTRY;

typedef struct _UNWIND_HISTORY_TABLE {
    ULONG Count;
    UCHAR Search;
    ULONG64 LowAddress;
    ULONG64 HighAddress;
    UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
} UNWIND_HISTORY_TABLE, *PUNWIND_HISTORY_TABLE;

#endif // _M_IA64

typedef struct _PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER Reserved1[2];
    ULONG Reserved2;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    BYTE Reserved1[52];
    PVOID Reserved2[3];
    HANDLE UniqueProcessId;
    PVOID Reserved3;
    ULONG HandleCount;
    BYTE Reserved4[4];
    PVOID Reserved5[11];
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER Reserved6[6];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION {
    ULONG RegistryQuotaAllowed;
    ULONG RegistryQuotaUsed;
    PVOID Reserved1;
} SYSTEM_REGISTRY_QUOTA_INFORMATION, *PSYSTEM_REGISTRY_QUOTA_INFORMATION;

typedef struct _SYSTEM_BASIC_INFORMATION {
    BYTE Reserved1[24];
    PVOID Reserved2[4];
    CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_TIMEOFDAY_INFORMATION {
    BYTE Reserved1[48];
} SYSTEM_TIMEOFDAY_INFORMATION, *PSYSTEM_TIMEOFDAY_INFORMATION;

typedef struct _SYSTEM_PERFORMANCE_INFORMATION {
    BYTE Reserved1[312];
} SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_EXCEPTION_INFORMATION {
    BYTE Reserved1[16];
} SYSTEM_EXCEPTION_INFORMATION, *PSYSTEM_EXCEPTION_INFORMATION;

typedef struct _SYSTEM_LOOKASIDE_INFORMATION {
    BYTE Reserved1[32];
} SYSTEM_LOOKASIDE_INFORMATION, *PSYSTEM_LOOKASIDE_INFORMATION;

typedef struct _SYSTEM_INTERRUPT_INFORMATION {
    BYTE Reserved1[24];
} SYSTEM_INTERRUPT_INFORMATION, *PSYSTEM_INTERRUPT_INFORMATION;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation = 1
} FILE_INFORMATION_CLASS;

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
    ProcessWow64Information = 26
} PROCESSINFOCLASS;

typedef enum _THREADINFOCLASS {
    ThreadIsIoPending = 16
} THREADINFOCLASS;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation = 0,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3,
    SystemProcessInformation = 5,
    SystemProcessorPerformanceInformation = 8,
    SystemInterruptInformation = 23,
    SystemExceptionInformation = 33,
    SystemRegistryQuotaInformation = 37,
    SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

#if (_WIN32_WINNT >= 0x0501)
//
// use the WTS API instead
//     WTSGetActiveConsoleSessionId
// The active console id is cached as a volatile ULONG in a constant
// memory location.  This x86 memory location is subject to changes between
// Windows releases.  Use the WTS API to make your application resilient to
// changes.
//
#define INTERNAL_TS_ACTIVE_CONSOLE_ID ( *((volatile ULONG*)(0x7ffe02d8)) )
#endif // (_WIN32_WINNT >= 0x0501)

//
// These functions are intended for use by internal core Windows components
// since these functions may change between Windows releases.
//


#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))


//
// use the Win32 API instead
//     CloseHandle
//
NTSTATUS
NtClose (
    IN HANDLE Handle
    );

//
// use the Win32 API instead
//     CreateFile
//
NTSTATUS
NtCreateFile (
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
    );

//
// use the Win32 API instead
//     CreateFile
//
NTSTATUS
NtOpenFile (
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess,
    IN ULONG OpenOptions
    );

//
// use the Win32 API instead
//     DeviceIoControl
//
NTSTATUS
NtDeviceIoControlFile (
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
    );

//
// use the Win32 API instead
//     WaitForSingleObjectEx
//
NTSTATUS
NtWaitForSingleObject (
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

//
// use the Win32 API instead
//     CheckNameLegalDOS8Dot3
//
BOOLEAN
RtlIsNameLegalDOS8Dot3 (
    IN PUNICODE_STRING Name,
    IN OUT POEM_STRING OemName OPTIONAL,
    IN OUT PBOOLEAN NameContainsSpaces OPTIONAL
    );

//
// This function might be needed for some of the internal Windows functions,
// defined in this header file.
//
ULONG
RtlNtStatusToDosError (
   NTSTATUS Status
   );

//
// use the Win32 APIs instead
//     GetProcessHandleCount
//     GetProcessId
//
NTSTATUS
NtQueryInformationProcess (
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

//
// use the Win32 API instead
//     GetThreadIOPendingFlag
//
NTSTATUS
NtQueryInformationThread (
    IN HANDLE ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

//
// use the Win32 APIs instead
//     GetSystemRegistryQuota
//     GetSystemTimes
// use the CryptoAPIs instead for generating random data
//     CryptGenRandom
//
NTSTATUS
NtQuerySystemInformation (
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

//
// use the Win32 API instead
//     GetSystemTimeAsFileTime
//
NTSTATUS
NtQuerySystemTime (
    OUT PLARGE_INTEGER SystemTime
    );

//
// use the Win32 API instead
//     LocalFileTimeToFileTime
//
NTSTATUS
RtlLocalTimeToSystemTime (
    IN PLARGE_INTEGER LocalTime,
    OUT PLARGE_INTEGER SystemTime
    );

//
// use the Win32 API instead
//     SystemTimeToFileTime to convert to FILETIME structures
//     copy the resulting FILETIME structures to ULARGE_INTEGER structures
//     perform the calculation
//
BOOLEAN
RtlTimeToSecondsSince1970 (
    PLARGE_INTEGER Time,
    PULONG ElapsedSeconds
    );

//
// These APIs might be need for some of the internal Windows functions,
// defined in this header file.
//
VOID
RtlFreeAnsiString (
    PANSI_STRING AnsiString
    );

VOID
RtlFreeUnicodeString (
    PUNICODE_STRING UnicodeString
    );

VOID
RtlFreeOemString(
    POEM_STRING OemString
    );

VOID
RtlInitString (
    PSTRING DestinationString,
    PCSZ SourceString
    );

VOID
RtlInitAnsiString (
    PANSI_STRING DestinationString,
    PCSZ SourceString
    );

VOID
RtlInitUnicodeString (
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

NTSTATUS
RtlAnsiStringToUnicodeString (
    PUNICODE_STRING DestinationString,
    PCANSI_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

NTSTATUS
RtlUnicodeStringToAnsiString (
    PANSI_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

NTSTATUS
RtlUnicodeStringToOemString(
    POEM_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );

//
// Use the Win32 API instead
//     WideCharToMultiByte
//     set CodePage to CP_ACP
//     set cbMultiByte to 0
//
NTSTATUS
RtlUnicodeToMultiByteSize(
    __out PULONG BytesInMultiByteString,
    __in_bcount(BytesInUnicodeString) PWCH UnicodeString,
    __in ULONG BytesInUnicodeString
    );

//
// Use the C runtime function instead
//     strtol
//
NTSTATUS
RtlCharToInteger (
    PCSZ String,
    ULONG Base,
    PULONG Value
    );

//
// use the Win32 API instead
//     ConvertSidToStringSid
//
NTSTATUS
RtlConvertSidToUnicodeString (
    PUNICODE_STRING UnicodeString,
    PSID Sid,
    BOOLEAN AllocateDestinationString
    );

//
// use the CryptoAPIs instead
//     CryptGenRandom
//
ULONG
RtlUniform (
    PULONG Seed
    );

//
// Use the default built-in system exception handling instead of these
// functions
//
// JB - commented
//VOID
//RtlUnwind (
//    IN PVOID TargetFrame OPTIONAL,
//    IN PVOID TargetIp OPTIONAL,
//    IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
//    IN PVOID ReturnValue
//    );

#if defined(_M_IA64)

VOID
RtlUnwind2 (
    IN FRAME_POINTERS TargetFrame OPTIONAL,
    IN PVOID TargetIp OPTIONAL,
    IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
    IN PVOID ReturnValue,
    IN PCONTEXT ContextRecord
    );

VOID
RtlUnwindEx (
    IN FRAME_POINTERS TargetFrame OPTIONAL,
    IN PVOID TargetIp OPTIONAL,
    IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
    IN PVOID ReturnValue,
    IN PCONTEXT ContextRecord,
    IN PUNWIND_HISTORY_TABLE HistoryTable OPTIONAL
    );

#endif // _M_IA64


#define LOGONID_CURRENT     ((ULONG)-1)
#define SERVERNAME_CURRENT  ((HANDLE)NULL)

typedef enum _WINSTATIONINFOCLASS {
    WinStationInformation = 8
} WINSTATIONINFOCLASS;


typedef struct _WINSTATIONINFORMATIONW {
    BYTE Reserved2[70];
    ULONG LogonId;
    BYTE Reserved3[1140];
} WINSTATIONINFORMATIONW, * PWINSTATIONINFORMATIONW;

//
// this function is implemented in winsta.dll (you need to loadlibrary to call this function)
// this internal function retrives the LogonId (also called SessionId) for the current process
// You should avoid using this function as it can change. you can retrieve the same information 
// Using public api WTSQuerySessionInformation. Pass WTSSessionId as the WTSInfoClass parameter
// 
typedef BOOLEAN (WINAPI * PWINSTATIONQUERYINFORMATIONW)(
    HANDLE, ULONG, WINSTATIONINFOCLASS, PVOID, ULONG, PULONG );


#ifdef __cplusplus
}
#endif

#endif // (_WIN32_WINNT >= 0x0500)

#endif // _WINTERNL_
