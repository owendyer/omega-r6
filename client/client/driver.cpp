#include "driver.hpp"

std::uint32_t driver::get_process_id(const std::string& process_name)
{
	PROCESSENTRY32 process_info;
	process_info.dwSize = sizeof(process_info);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	Process32First(snapshot, &process_info);
	if (!process_name.compare(process_info.szExeFile))
	{
		CloseHandle(snapshot);
		pid = (std::uint32_t)process_info.th32ProcessID;
		return pid;
	}

	while (Process32Next(snapshot, &process_info))
	{
		if (!process_name.compare(process_info.szExeFile))
		{
			CloseHandle(snapshot);
			pid = (std::uint32_t)process_info.th32ProcessID;
			return pid;
		}
	}

	CloseHandle(snapshot);
	return 0;
}

std::uint64_t driver::get_module_base(std::uint32_t pid)
{
	if (!pid)
		return 0x0;

	comm::COMM instance;
	instance.pid = pid;
	instance.id = (std::uint32_t)comm::comm_id::base;
	comm::handler(&instance);

	driver::base = (std::uint64_t)instance.buffer;

	return base;
}

void driver::read_virtual_memory(std::uint64_t address, std::uint8_t* buffer, size_t size)
{
	comm::COMM instance;
	instance.id = (std::uint32_t)comm::comm_id::read;
	instance.pid = driver::pid;
	instance.source_pid = GetCurrentProcessId();
	instance.address = (void*)address;
	instance.buffer = buffer;
	instance.size = size;
	comm::handler(&instance);
}

void driver::write_virtual_memory(std::uint64_t address, std::uint8_t* buffer, size_t size)
{
	comm::COMM instance;
	instance.id = (std::uint32_t)comm::comm_id::write;
	instance.pid = driver::pid;
	instance.source_pid = GetCurrentProcessId();
	instance.address = (void*)address;
	instance.buffer = buffer;
	instance.size = size;
	comm::handler(&instance);
}