// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifdef ARRAYSIZE
#define countof(a) (ARRAYSIZE(a)) // (sizeof((a))/(sizeof(*(a))))
#else
#define countof(a) (sizeof((a)) / (sizeof(*(a))))

#endif






#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define RPC_USE_NATIVE_WCHAR



#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <bits.h>
#include <bits4_0.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <strsafe.h>
#include <comdef.h>
#include <winternl.h>
#include <Shlwapi.h>
#include <AclAPI.h>
#include <Shlwapi.h>
#include <Sddl.h>
#include <atlbase.h>
#include <shellapi.h>



typedef LUID OXID;
typedef LUID OID;
typedef GUID	IPID;

typedef struct tagDUALSTRINGARRAY    {
	unsigned short wNumEntries;     // Number of entries in array.
	unsigned short wSecurityOffset; // Offset of security info.
	unsigned short aStringArray[];
} DUALSTRINGARRAY;

typedef struct tagSTDOBJREF    {
	DWORD   flags;
	DWORD   cPublicRefs;
	OXID           oxid;
	OID            oid;
	IPID           ipid;
} STDOBJREF;

typedef struct tagOBJREF    {
	unsigned long signature;//MEOW
	unsigned long flags;
	GUID          iid;
	union        {
		struct            {
			STDOBJREF       std;
			DUALSTRINGARRAY saResAddr;
		} u_standard;
		struct            {
			STDOBJREF       std;
			CLSID           clsid;
			DUALSTRINGARRAY saResAddr;
		} u_handler;
		struct            {
			CLSID           clsid;
			unsigned long   cbExtension;
			unsigned long   size;
			ULONGLONG pData;
		} u_custom;
	} u_objref;
} OBJREF;
