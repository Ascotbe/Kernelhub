#pragma once
#include <cstdint>
extern "C" void _DoSyscall();
extern "C" void _GetKernelCallbackTb(PDWORD CallbackTb);

template< typename ReturnType = void, typename... Args,
	typename T1 = void*, typename T2 = void*, typename T3 = void*, typename T4 = void* >
	ReturnType Syscall(const uint64_t Index, T1 A1 = {}, T2 A2 = {}, T3 A3 = {}, T4 A4 = {}, Args... Arguments)
{
	static_assert(sizeof(void*) == 8, "Only x64 is supported.");

	return reinterpret_cast< ReturnType(*)(T1, T2, T3, T4, uint64_t, uint64_t, Args...) >(_DoSyscall)(
		A1, A2, A3, A4, Index, 0, Arguments... // Stack must be aligned to 16 byte boundary.
		);
}