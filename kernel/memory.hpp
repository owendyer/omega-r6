#pragma once

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "utils.hpp"

namespace memory
{
	NTSTATUS read_process_memory(uint32_t pid, uint32_t user_pid, uintptr_t addr, uintptr_t buffer, size_t size, size_t* bytes_read);
	NTSTATUS write_process_memory(uint32_t pid, uint32_t user_pid, uintptr_t addr, uintptr_t buffer, size_t size, size_t* bytes_written);
}

#endif // !MEMORY_HPP