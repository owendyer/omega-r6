#pragma once

#ifndef R6_SDK_HPP
#define R6_SDK_HPP

// Includes
#include <emmintrin.h>

#include "driver.hpp"
#include "hexrays_defs.hpp"
#include "vectors.hpp"

namespace sdk
{
	inline int display_width;
	inline int display_height;

	struct bone_entry
	{
		uint32_t hash;
		uint16_t id;
		uint16_t group;
	};

	class game_manager
	{
	public:

		static game_manager* get()
		{
			return (game_manager*)(__ROL8__((driver::read<std::uint64_t>(driver::base + 0x7A4BE60) - 0x2DFBC9ECA85C0230) ^ 0x8776BB84854170C4, 0x37));
		}
	};

	class round_manager
	{
	public:

		static round_manager* get()
		{
			return (round_manager*)(__ROR8__((driver::read<std::uint64_t>(driver::base + 0x758C6D8) ^ 0x12) - 0x74, 0x1));
		}

		std::uint32_t state()
		{
			return __ROL4__((driver::read<std::uint32_t>((std::uint64_t)get() + 0xF8) ^ 0xFBF03A1D) - 0x7E2A89AB, 0x8);
		}
	};

	class entity
	{
	public:

		static entity* get()
		{
			return (entity*)(game_manager::get());
		}

		std::uint64_t list()
		{
			return ((driver::read<std::uint64_t>((std::uint64_t)get() + 0xD0) - 0x5B) ^ 0x13) - 0x770AEB7514380744;
		}

		std::uint32_t count()
		{
			return (std::uint32_t)(((driver::read<std::uint64_t>((std::uint64_t)get() + 0xD8) - 0x5B) ^ 0x13) - 0x770AEB7514380744) & 0x3FFFFFFF;
		}

		std::uint64_t local()
		{
			uint64_t ProfileManager = driver::read<uint64_t>(driver::base + 0x739DBA8);
			uint64_t v3 = __ROL__<uint64_t>(__ROL__<uint64_t>(driver::read<uint64_t>((((ProfileManager ^ 0x53) - 0x3D621731D4A299C5i64) ^ 0x437043787E3C5D99i64) + 0x30), 62) - 101i64, 62);
			uint64_t v4 = driver::read<uint64_t>(v3);
			return __ROL__<uint64_t>((driver::read<uint64_t>(v4 + 32) ^ 0x15i64) - 33, 5);
		}

		std::uint64_t component(int index)
		{
			return driver::read<std::uint64_t>((std::uint64_t)get()->list() + sizeof(void*) * index);
		}

		std::uint64_t pawn(std::uint64_t component)
		{
			// "(playercontroller)0x%p;(pawn)0x%p; %s" <- search for string
			return driver::read<std::uint64_t>(component + 0x38) ^ 0xA5A9F4BA79A2A1C6;
		}

		std::uint64_t actor(std::uint64_t pawn)
		{
			// aGadgetCatcher <- jump to name
			return __ROL8__((driver::read<std::uint64_t>(pawn + 0x18) - 0x275B64C73359300F) ^ 0x2102372E900268A1, 0x2E);
		}

		std::uint64_t replication(std::uint64_t component)
		{
			return __ROL8__(__ROL8__(driver::read<std::uint64_t>(component + 0x90), 0x17) - 0x73, 0x1D);
		}

		std::uint8_t team(std::uint64_t replication)
		{
			/*std::uint64_t rax = 0ull, rbx = 0ull, rcx = 0ull, rdx = 0ull, rdi = 0ull, rsi = 0ull, r8 = 0ull, r9 = 0ull, r10 = 0ull, r11 = 0ull, r12 = 0ull, r13 = 0ull, r14 = 0ull, r15 = replication, rbp = 0ull;

			rax = driver::read<std::uint64_t>(r15 + 0x710);
			rax ^= 0x1B;
			rcx = 0xD580150D315F57E7;
			rcx += rax;
			rcx = _rotl64(rcx, 0x1A);
			rax = 0x9EB0ED70;
			rax = (std::uint32_t)rax + driver::read<std::uint32_t>(rcx + 0xB0);
			rax = _rotl(rax, 0x14);
			rax ^= 0x4DFA889A;

			return rax;*/

			return driver::read<std::uint8_t>(replication + 0x850) ^ 0xE0;
		}

		std::uint32_t health(std::uint64_t actor)
		{
			std::uint64_t entity_info = driver::read<std::uint64_t>(actor + 0xD8);
			std::uint64_t main_component = driver::read<std::uint64_t>(entity_info + 0x8);
			return driver::read<std::uint32_t>(main_component + 0x1BC);
		}
	};

	class camera_manager
	{
	public:

		static camera_manager* get()
		{
			std::uint64_t chain = __ROL8__(driver::read<std::uint64_t>(driver::read<std::uint64_t>(driver::base + 0x676AE78) + 0x40), 0x1) + 0x4C869499115F734A;

			return (camera_manager*)(driver::read<std::uint64_t>(chain));
		}

		bool w2s(vector3 world, vector2* screen)
		{
			std::uint64_t v1 = 0xEFB8CBEAB4138A79i64, v2 = 0x2C4271B184C37EFDi64;

			auto decrypt_view_data = [&v1, &v2](std::uint64_t address)
			{
				auto encoded = (address + v1) ^ v2;
				v1 += 0xFC232120A243611i64;
				v2 -= 0x633FE7A7F870CD7Fi64;
				return encoded << 0x27 | encoded >> 0x19;
			};

			std::uint64_t offset_fov_x = 0x2FC, offset_fov_y = 0x300;

			__m128i right = driver::read<__m128i>((std::uint64_t)get() + 0x1D0);
			right.m128i_i64[0] = decrypt_view_data(right.m128i_i64[0]);
			right.m128i_i64[1] = decrypt_view_data(right.m128i_i64[1]);
			vector3 vec_right = *(vector3*)&(right);

			__m128i up = driver::read<__m128i>((std::uint64_t)get() + 0x1E0);
			up.m128i_i64[0] = decrypt_view_data(up.m128i_i64[0]);
			up.m128i_i64[1] = decrypt_view_data(up.m128i_i64[1]);
			vector3 vec_up = *(vector3*)&(up);

			__m128i forward = driver::read<__m128i>((std::uint64_t)get() + 0x1F0);
			forward.m128i_i64[0] = decrypt_view_data(forward.m128i_i64[0]);
			forward.m128i_i64[1] = decrypt_view_data(forward.m128i_i64[1]);
			vector3 vec_forward = *(vector3*)&(forward);

			__m128i translation = driver::read<__m128i>((std::uint64_t)get() + 0x200);
			translation.m128i_i64[0] = decrypt_view_data(translation.m128i_i64[0]);
			translation.m128i_i64[1] = decrypt_view_data(translation.m128i_i64[1]);
			vector3 vec_translation = *(vector3*)&(translation);

			vector3 temp = world - vec_translation;
			float x = temp.dot(vec_right);
			float y = temp.dot(vec_up);
			float z = temp.dot(vec_forward * -1.0f);

			std::uint32_t fov_x = (driver::read<std::uint32_t>((std::uint64_t)get() + offset_fov_x) + 0x5A90F23D) ^ 0x18;
			std::uint32_t fov_y = _rotr(driver::read<std::uint32_t>((std::uint64_t)get() + offset_fov_y) - 0x6A, 1) - 0x0E6DA57D2;

			float ffov_x = *(float*)&fov_x;
			float ffov_y = *(float*)&fov_y;

			screen->x = (display_width / 2.f) * (1.f + x / -ffov_x / z);
			screen->y = (display_height / 2.f) * (1.f - y / -ffov_y / z);

			if (screen->x < 0 || screen->x > display_width)
				return false;

			return z >= 1.0f ? true : false;
		}
	};

	class skeleton
	{
	public:

		static skeleton* get()
		{
			return NULL;
		}

		std::uint64_t bones_structure(std::uint64_t pawn)
		{
			return _rotl64(driver::read<std::uint64_t>(pawn + 0x9D8), 60) + 4984952518767947254;
		}

		void transforms_calculation(__int64 pBones, __m128* ResultPosition, __m128* BoneInfo)
		{
			__m128 v5; // xmm2
			__m128 v6; // xmm3
			__m128 v7; // xmm0
			__m128 v8; // xmm4
			__m128 v9; // xmm1

			__m128 v10 = { 0.500f, 0.500f, 0.500f, 0.500f };
			__m128 v11 = { 2.000f, 2.000f, 2.000f, 0.000f };

			__m128 v12 = driver::read<__m128>(pBones);
			__m128 v13 = driver::read<__m128>(pBones + 0x10);

			v5 = v13;

			v6 = _mm_mul_ps(*(__m128*)BoneInfo, v5);
			v6.m128_f32[0] = v6.m128_f32[0]
				+ (float)(_mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0x55)) + _mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0xAA)));
			v7 = _mm_shuffle_ps(v13, v5, 0xFF);
			v8 = _mm_sub_ps(
				_mm_mul_ps(_mm_shuffle_ps(v5, v5, 0xD2), *(__m128*)BoneInfo),
				_mm_mul_ps(_mm_shuffle_ps(*(__m128*)BoneInfo, *(__m128*)BoneInfo, 0xD2), v5));
			v9 = _mm_shuffle_ps(v12, v12, 0x93);
			v9.m128_f32[0] = 0.0;
			*(__m128*)ResultPosition = _mm_add_ps(
				_mm_shuffle_ps(v9, v9, 0x39),
				_mm_mul_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(_mm_shuffle_ps(v8, v8, 0xD2), v7),
							_mm_mul_ps(_mm_shuffle_ps(v6, v6, 0), v5)),
						_mm_mul_ps(
							_mm_sub_ps(_mm_mul_ps(v7, v7), (__m128)v10),
							*(__m128*)BoneInfo)),
					(__m128)v11));
		}

		vector3 bone_pos_by_id(std::uint64_t pawn, int index)
		{
			__m128 Output{};

			std::uint64_t pBonesChain2 = bones_structure(pawn);
			pBonesChain2 = driver::read<std::uint64_t>(pBonesChain2);

			std::uint64_t pBones = driver::read<std::uint64_t>(pBonesChain2 + 0x250);

			std::uint64_t pBonesData = driver::read<std::uint64_t>(pBones + 0x58);

			__m128 BoneInfo = driver::read<__m128>((0x20 * index) + pBonesData);
			transforms_calculation(pBones, &Output, &BoneInfo);

			return { Output.m128_f32[0], Output.m128_f32[1], Output.m128_f32[2] };
		}

		std::uint64_t get_bone_chain(std::uint64_t actor)
		{
			return driver::read<std::uint64_t>(driver::read<std::uint64_t>(actor + 0xD8) + driver::read<std::uint8_t>(actor + 0x22B) * 0x8);
		}

		void get_hash_skeleton(std::uint64_t pawn, std::uint64_t bone_chain, std::uint64_t* bone_array, std::uint64_t* size, std::uint64_t* bones, std::uint64_t* data)
		{
			bone_chain = driver::read<std::uint64_t>(bone_chain + 0x208);
			*bone_array = driver::read<uint64_t>(bone_chain + 0x48);
			*size = driver::read<std::uint16_t>(bone_chain + 0x50) * 0x8;

			std::uint64_t entity_skeleton = bones_structure(pawn);
			entity_skeleton = driver::read<std::uint64_t>(entity_skeleton);

			*bones = driver::read<std::uint64_t>(entity_skeleton + 0x250);
			*data = driver::read<std::uint64_t>(*bones + 0x58);
		}

		std::uint16_t get_bone_id_from_hash(bone_entry* bonedata, const uint64_t& size, const uint32_t& hash)
		{
			bone_entry* first_entry = bonedata;
			bone_entry* current_entry = first_entry;

			uint64_t Offset = size >> 3;

			do
			{
				auto PreviousOffset = Offset;
				Offset >>= 1;
				if (Offset > size || Offset < 0)
				{
					return -1;
				}
				if (current_entry[Offset].hash < hash)
				{
					current_entry += Offset + 1;
					Offset = PreviousOffset + ~Offset;
				}
			} while (Offset & ~(1 << 61));

			if (current_entry != first_entry + size / 8 && current_entry->hash == hash && current_entry->id != 0xffff)
			{
				return current_entry->id;
			}

			return -1;
		}

		vector3 get_bone_by_hash(std::uintptr_t bone_array, std::uintptr_t size, std::uintptr_t bones, std::uintptr_t data, std::uint32_t hash)
		{
			auto skeleton_entry = std::make_unique<std::uint8_t[]>(size);
			driver::read_virtual_memory(bone_array, skeleton_entry.get(), size);

			std::uint16_t id = get_bone_id_from_hash(reinterpret_cast<bone_entry*>(&skeleton_entry[0]), size, hash);

			__m128 bone_info = driver::read<__m128>(data + id * 0x20);

			__m128 result;
			transforms_calculation(bones, &result, &bone_info);

			return { result.m128_f32[0], result.m128_f32[1], result.m128_f32[2] };
		}
	};
}

#endif // !R6_SDK_HPP