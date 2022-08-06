#include "comm.hpp"
#include "memory.hpp"

//__int64 __fastcall comm::handler(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint32_t a5, uint64_t a6, uint64_t a7, uint64_t a8)
__int64 __fastcall comm::ApiSetEditionCreateDesktopEntryPoint(__int64 a1, __int64 a2, __int64 a3, unsigned int a4, int a5, int a6)
{
	// Check if the call was made from usermode
	if (ExGetPreviousMode() != UserMode)
	{
		//print("not called from usermode\n");
		return func_ptr(a1, a2, a3, a4, a5, a6);
	}


	// Check if a2 (our data) is valid
	if (a1 != comm::magic)
	{
		//print("normal call\n");
		return func_ptr(a1, a2, a3, a4, a5, a6);
	}

	// Typecast a2 to our data format so that it can be use
	COMM* instance = (COMM*)a3;

	// Now we know that the call belongs to us

	switch (instance->id)
	{
	case (int)comm_id::init:
	{
		//print("init requested\n");

		instance->ret = 0x1;

		return STATUS_SUCCESS;

		break;
	}
	case (int)comm_id::exit:
	{
		//print("exit requested\n");

		return STATUS_UNSUCCESSFUL;

		break;
	}
	case (int)comm_id::base:
	{
		//print("base requested\n");

		NTSTATUS status = STATUS_SUCCESS;

		PEPROCESS process = utils::get_process_by_id(instance->pid, &status);
		if (!NT_SUCCESS(status))
		{
			instance->ret = 0x0;
			instance->buffer = 0x0;
			return STATUS_UNSUCCESSFUL;
		}

		instance->ret = 0x1;
		instance->buffer = (void*)utils::PsGetProcessSectionBaseAddress(process);
		ObDereferenceObject(process);

		return STATUS_SUCCESS;

		break;
	}
	case (int)comm_id::read:
	{
		//print("read requested\n");

		if (!NT_SUCCESS(memory::read_process_memory(instance->pid, instance->source_pid, (uint64_t)instance->address, (uint64_t)instance->buffer, (size_t)instance->size, nullptr)))
		{
			instance->ret = 0x0;
			instance->buffer = 0x0;
			//print("read failed\n");
			return STATUS_UNSUCCESSFUL;
		}

		instance->ret = 0x1;

		return STATUS_SUCCESS;

		break;
	}
	case (int)comm_id::write:
	{
		//print("write requested\n");

		if (!NT_SUCCESS(memory::write_process_memory(instance->pid, instance->source_pid, (uint64_t)instance->address, (uint64_t)instance->buffer, (size_t)instance->size, nullptr)))
		{
			instance->ret = 0x0;
			instance->buffer = 0x0;
			//print("write failed\n");
			return STATUS_UNSUCCESSFUL;
		}

		instance->ret = 0x1;

		return STATUS_SUCCESS;

		break;
	}
	case (int)comm_id::readstr:
	{
		//print("readstr requested\n");

		return STATUS_SUCCESS;

		break;
	}
	case (int)comm_id::writestr:
	{
		//print("writestr requested\n");

		return STATUS_SUCCESS;

		break;
	}
	case (int)comm_id::writetoread:
	{
		//print("writeprotected requested\n");

		return STATUS_SUCCESS;

		break;
	}
	default:
	{
		//print("invalid request\n");

		return STATUS_UNSUCCESSFUL;

		break;
	}
	}

	return STATUS_SUCCESS;
}