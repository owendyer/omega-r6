#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

// Includes
#include "imports.hpp"

namespace utils
{
	void sleep(LONG milliseconds);

	uint64_t get_system_module(const wchar_t* name);
	uint64_t get_routine_address(uint64_t image, const char* name);
	PEPROCESS get_process_by_id(uint32_t pid, NTSTATUS* pstatus = nullptr);

	PBYTE find_pattern(PVOID module, DWORD size, LPCSTR pattern, LPCSTR mask);
	PBYTE find_pattern_work(PVOID base, LPCSTR pattern, LPCSTR mask);

	extern "C" PVOID PsGetProcessSectionBaseAddress(PEPROCESS Process);
}

#endif // !UTILS_HPP