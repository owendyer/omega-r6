#include "comm.hpp"

#include <Windows.h>
#include <TlHelp32.h>

comm::func_ptr hook_ptr = (comm::func_ptr)NULL;

HMODULE ensure_dll_load()
{
#define LOAD_DLL(str) LoadLibrary(str)

	LOAD_DLL("user32.dll");

#undef LOAD_DLL
	return LoadLibrary("win32u.dll");
}

bool comm::init()
{
	HMODULE dll_handle = GetModuleHandleA("win32u.dll");

	if (!dll_handle)
	{
		dll_handle = ensure_dll_load();

		if (!dll_handle)
		{
			std::cout << "omega -> failed to get module handle\n";
			return false;
		}
	}

	hook_ptr = (comm::func_ptr)GetProcAddress(dll_handle, "NtUserCreateDesktopEx");

	if (!hook_ptr)
	{
		std::cout << "omega -> failed to get pointer to function address\n";
		return false;
	}

	comm::COMM instance;
	instance.id = (std::uint32_t)comm::comm_id::init;
	handler(&instance);

	return instance.ret;
}

bool comm::abort()
{
	comm::COMM instance;
	instance.id = (std::uint32_t)comm::comm_id::exit;
	handler(&instance);

	return instance.ret;
}

std::uint32_t comm::handler(COMM* instance)
{
	hook_ptr(comm::magic, 0x0, (uint64_t)instance, 0x0, 0x0, 0x0);
	return instance->ret;
}