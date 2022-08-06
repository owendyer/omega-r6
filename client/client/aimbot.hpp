#pragma once

#ifndef AIMBOT_HPP
#define AIMBOT_HPP

// Includes
#include "r6_sdk.hpp"

namespace aimbot
{
	inline vector3 calc_euler(vector4 quat)
	{
		float x = atan2f(2.0f * (quat.w * quat.z + quat.x * quat.y), (1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z))) * (180.0f / M_PI);
		float y = atan2f(2.0f * (quat.w * quat.x + quat.y * quat.z), (1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y))) * (180.0f / M_PI);
		return vector3(x, y, 0.0f);
	}

	inline vector4 calc_quat(vector3 euler)
	{
		vector4 quaternion;
		float yaw = (euler.z * 0.01745329251f) * 0.5f;
		float sy = sinf(yaw);
		float cy = cosf(yaw);
		float roll = (euler.x * 0.01745329251f) * 0.5f;
		float sr = sinf(roll);
		float cr = cosf(roll);
		constexpr auto sp = 0.0f;
		constexpr auto cp = 1.0f;
		quaternion.x = cy * sr * cp - sy * cr * sp;
		quaternion.y = cy * cr * sp + sy * sr * cp;
		quaternion.z = sy * cr * cp - cy * sr * sp;
		quaternion.w = cy * cr * cp + sy * sr * sp;

		return quaternion;
	}

	inline vector3 calc_vector(vector3 source, vector3 target)
	{
		vector3 Delta = target - source;
		float x = asinf(Delta.z / Delta.length()) * (180.0f / M_PI);
		float z = atanf(Delta.y / Delta.x) * (180.0f / M_PI);
		if (Delta.x >= 0.0f)
		{
			z += 180.0f;
		}
		if (x > 180.0f)
		{
			x -= 360.0f;
		}
		if (x < -180.0f)
		{
			x += 360.0f;
		}
		return vector3(x, 0.0f, z + 90.0f);
	}

	inline std::uint64_t find_best_target(std::uint64_t local_component)
	{
		float best_dist = FLT_MAX;
		std::uint64_t best_target = {};

		std::uint64_t local_replication = sdk::entity::get()->replication(local_component);
		std::uint8_t local_team = sdk::entity::get()->team(local_replication);

		for (int i = 0; i < sdk::entity::get()->count(); ++i)
		{
			std::uint64_t component = sdk::entity::get()->component(i);
			if (component == NULL || component == local_component)
				continue;

			std::uint64_t replication = sdk::entity::get()->replication(component);
			std::uint8_t team = sdk::entity::get()->team(replication);

			/*if (team == local_team)
				continue;*/

			std::uint64_t pawn = sdk::entity::get()->pawn(component);
			std::uint64_t actor = sdk::entity::get()->actor(pawn);
			std::uint32_t health = sdk::entity::get()->health(actor);

			if (health <= 0 || health > 165)
				continue;


			vector3 world = sdk::skeleton::get()->bone_pos_by_id(pawn, 7);
			if (world.x == 0 && world.y == 0 && world.z == 0)
				continue;

			vector2 screen;
			if (!sdk::camera_manager::get()->w2s(world, &screen))
				continue;

			float x = screen.x - sdk::display_width / 2.f; // screen width
			float y = screen.y - sdk::display_height / 2.f; // screen height
			float crosshair_dist = sqrtf((x * x) + (y * y));

			if (crosshair_dist <= FLT_MAX && crosshair_dist <= best_dist)
			{
				if (crosshair_dist > 60)
					continue;

				best_dist = crosshair_dist;
				best_target = pawn;
			}
		}

		return best_target;
	}

	inline vector4 get_view_angles(std::uint64_t local_component)
	{
		std::uint64_t pawn = sdk::entity::get()->pawn(local_component);

		std::uint64_t unk1 = driver::read<std::uint64_t>(pawn + 0xAF0);

		vector4 angles = driver::read<vector4>(unk1 + 0x95);
		__m128i quaternion = _mm_load_si128(reinterpret_cast<const __m128i*>(&angles));

		quaternion.m128i_u64[0] = __ROL8__(quaternion.m128i_u64[0], 44) - 0x40B034073308638Fi64;
		quaternion.m128i_u64[1] = __ROL8__(quaternion.m128i_u64[1], 44) + 0x16ED2082AB7DDD08i64;

		return *(vector4*)&quaternion;
	}

	inline void set_view_angles(std::uint64_t local_component, vector4 angles)
	{
		std::uint64_t pawn = sdk::entity::get()->pawn(local_component);

		std::uint64_t unk1 = driver::read<std::uint64_t>(pawn + 0xAF0);

		__m128i quaternion = _mm_load_si128(reinterpret_cast<const __m128i*>(&angles));

		quaternion.m128i_u64[0] = __ROR8__(quaternion.m128i_u64[0] + 0x40B034073308638F, 44);
		quaternion.m128i_u64[1] = __ROR8__(quaternion.m128i_u64[1] - 0x16ED2082AB7DDD08, 44);

		vector4 encoded_angle = *(vector4*)(&quaternion);

		driver::write<vector4>(unk1 + 0x95, encoded_angle);
	}

	inline void aimbot()
	{
		std::uint64_t local_component = sdk::entity::get()->local();
		std::uint64_t local_pawn = sdk::entity::get()->pawn(local_component);

		std::uint64_t target_pawn = find_best_target(local_component);

		vector3 local_world = sdk::skeleton::get()->bone_pos_by_id(local_pawn, 11);
		vector3 target_world = sdk::skeleton::get()->bone_pos_by_id(target_pawn, 0);

		vector3 aim_vector = calc_vector(local_world, target_world);
		aim_vector.clamp();

		//std::cout << "x: " << aim_vector.x << " y: " << aim_vector.y << " z: " << aim_vector.z << "\n";

		vector4 vector_quat = calc_quat(aim_vector);

		if (GetAsyncKeyState(VK_XBUTTON1))
		{
			set_view_angles(local_component, vector_quat);
		}
	}
}

#endif // !AIMBOT_HPP