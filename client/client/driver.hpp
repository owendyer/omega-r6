#pragma once

#ifndef DRIVER_HPP
#define DRIVER_HPP

// Includes
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string>

#include "comm.hpp"

namespace driver
{
	inline std::uint32_t pid = 0x0;
	inline std::uint64_t base = 0x0;

	std::uint32_t get_process_id(const std::string& process_name);
	std::uint64_t get_module_base(std::uint32_t pid = 0x0);

	void read_virtual_memory(std::uint64_t address, std::uint8_t* buffer, size_t size);
	void write_virtual_memory(std::uint64_t address, std::uint8_t* buffer, size_t size);

	template <typename T>
	T read(std::uint64_t address)
	{
		T buffer;
		read_virtual_memory(address, (std::uint8_t*)&buffer, sizeof(T));

		return buffer;
	}

	template <typename T>
	void write(std::uint64_t address, T value)
	{
		write_virtual_memory(address, (std::uint8_t*)&value, sizeof(T));
	}
}

#endif // !DRIVER_HPP