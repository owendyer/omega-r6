#include "comm.hpp"
#include "utils.hpp"

extern "C" NTSTATUS driver_entry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	//print("driver starting\n");

	uint64_t win32kbase = utils::get_system_module(L"win32kbase.sys");
	if (!win32kbase)
	{
		//print("win32kbase.sys not found\n");
		return STATUS_UNSUCCESSFUL;
	}

	//print("win32kbase.sys found. address: 0x%llx", win32kbase);

	// 74 2E 8B 84 24 ? ? ? ?
	PBYTE function_address = utils::find_pattern_work((PVOID)win32kbase, "\x74\x2E\x8B\x84\x24\x00\x00\x00\x00", "xxxxx????");
	if (!function_address)
	{
		//print("function not found\n");
		return STATUS_UNSUCCESSFUL;
	}
	
	//print("ApiSetEditionCreateDesktopEntryPoint found. address: 0x%llx", (uint64_t)function_address);

	uint64_t ApiSetEditionCreateDesktopEntryPoint = (uint64_t)(function_address) - 0xA;

	comm::hook_ptr = (uint64_t)ApiSetEditionCreateDesktopEntryPoint + *(PINT)((PBYTE)ApiSetEditionCreateDesktopEntryPoint + 3) + 7;
	*(PVOID*)&comm::func_ptr = InterlockedExchangePointer((PVOID*)comm::hook_ptr, (PVOID)&comm::ApiSetEditionCreateDesktopEntryPoint);
	
	//print("comm successfully initialized\n");

	return STATUS_SUCCESS;
}