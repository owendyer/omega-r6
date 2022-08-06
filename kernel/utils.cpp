#include "utils.hpp"

//#include <ntimage.h>

void utils::sleep(LONG milliseconds)
{
	LARGE_INTEGER interval;
	interval.QuadPart = -(10000ll * milliseconds);

	KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

uint64_t utils::get_system_module(const wchar_t* name)
{
	NTSTATUS status = STATUS_SUCCESS;
	ANSI_STRING s_name;
	UNICODE_STRING su_name;
	RtlInitUnicodeString(&su_name, name);
	RtlUnicodeStringToAnsiString(&s_name, &su_name, TRUE);

	PRTL_PROCESS_MODULES pModules = NULL;
	uint32_t szModules = 0;

	status = ZwQuerySystemInformation(SystemModuleInformation, 0, szModules, (PULONG)&szModules);
	if (!szModules)
	{
		RtlFreeAnsiString(&s_name);
		return 0;
	}

	pModules = (PRTL_PROCESS_MODULES)ExAllocatePool(NonPagedPool, szModules);
	if (!pModules)
	{
		RtlFreeAnsiString(&s_name);
		return 0;
	}
	RtlZeroMemory(pModules, szModules);

	status = ZwQuerySystemInformation(SystemModuleInformation, pModules, szModules, (PULONG)&szModules);
	if (!NT_SUCCESS(status))
	{
		RtlFreeAnsiString(&s_name);
		ExFreePool(pModules);
		return 0;
	}

	uintptr_t modBase = 0;
	PRTL_PROCESS_MODULE_INFORMATION pMods = pModules->Modules;
	for (ULONG i = 0; i < pModules->NumberOfModules && !modBase; i++)
	{
		RTL_PROCESS_MODULE_INFORMATION pMod = pMods[i];
		char* fullPath = (char*)pMod.FullPathName;
		if (fullPath && strlen(fullPath) > 0)
		{
			int32_t lastFound = -1;
			char* baseFullPath = (char*)pMod.FullPathName;
			while (*fullPath != 0)
			{
				if (*fullPath == '\\')
					lastFound = (fullPath - baseFullPath) + 1;
				fullPath++;
			}

			if (lastFound >= 0)
				fullPath = baseFullPath + lastFound;
		}
		else continue;

		ANSI_STRING s_fullPath;
		RtlInitAnsiString(&s_fullPath, fullPath);
		if (RtlEqualString(&s_fullPath, &s_name, TRUE))
			modBase = (uint64_t)pMod.ImageBase;
	}
	RtlFreeAnsiString(&s_name);
	ExFreePool(pModules);
	return modBase;
}

uint64_t utils::get_routine_address(uint64_t image, const char* name)
{
	if (!image || !name)
		return NULL;

	return (uint64_t)RtlFindExportedRoutineByName((PVOID)image, name);
}

PEPROCESS utils::get_process_by_id(uint32_t pid, NTSTATUS* pstatus)
{
	PEPROCESS hProc;
	NTSTATUS status = PsLookupProcessByProcessId((HANDLE)pid, &hProc);

	if (!NT_SUCCESS(status))
	{
		if (pstatus) { *pstatus = status; }

		return nullptr;
	}

	return hProc;
}

PIMAGE_NT_HEADERS get_header(PVOID module)
{
	return (PIMAGE_NT_HEADERS)((PBYTE)module + PIMAGE_DOS_HEADER(module)->e_lfanew);
}

PBYTE utils::find_pattern(PVOID module, DWORD size, LPCSTR pattern, LPCSTR mask)
{

	auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
	{
		for (auto x = buffer; *mask; pattern++, mask++, x++)
		{
			auto addr = *(BYTE*)(pattern);
			if (addr != *x && *mask != '?')
				return FALSE;
		}

		return TRUE;
	};

	for (auto x = 0; x < size - strlen(mask); x++)
	{

		auto addr = (PBYTE)module + x;
		if (checkMask(addr, pattern, mask))
			return addr;
	}

	return NULL;
}

PBYTE utils::find_pattern_work(PVOID base, LPCSTR pattern, LPCSTR mask)
{

	auto header = get_header(base);
	auto section = IMAGE_FIRST_SECTION(header);

	for (auto x = 0; x < header->FileHeader.NumberOfSections; x++, section++)
	{

		if (!memcmp(section->Name, ".text", 5) || !memcmp(section->Name, "PAGE", 4))
		{
			auto addr = find_pattern((PBYTE)base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
			if (addr)
			{
				//DbgPrintEx(0, 0, "[+] Found in Section -> [ %s ]", section->Name);
				return addr;
			}
		}
	}

	return NULL;
}