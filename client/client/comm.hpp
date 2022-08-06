#pragma once

#ifndef COMM_HPP
#define COMM_HPP

// Includes
#include <cstdint>
#include <iostream>

namespace comm
{
	const uint64_t magic = 0x4F4D4741;
	typedef __int64(__fastcall* func_ptr)(uint64_t a1, uint64_t a2, uint64_t a3, unsigned int a4, int a5, int a6);

	enum class comm_id
	{
		init = 0x1000,
		exit = 0x1001,
		base = 0x1002,
		read = 0x1003,
		write = 0x1004,
		readstr = 0x1005,
		writestr = 0x1006,
		writetoread = 0x1007
	};

	typedef struct _COMM
	{
		uint32_t id;
		void* address;
		void* output;
		void* buffer;
		uint64_t size;
		uint32_t pid;
		uint32_t source_pid;
		uint64_t base;
		const char* name;
		uint32_t ret;

	} COMM;

	bool init();
	bool abort();

	//bool handler(COMM* instance);
	std::uint32_t handler(COMM* instance);
}

#endif // !COMM_HPP