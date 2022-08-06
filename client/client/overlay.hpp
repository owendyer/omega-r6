#pragma once

#ifndef OVERLAY_HPP
#define OVERLAY_HPP

// Includes
#include <Windows.h>
#include <gdiplus.h>
#include <dwmapi.h>
#include <vector>

// Libraries
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "dwmapi.lib")

#include "r6_sdk.hpp"
#include "aimbot.hpp"

typedef struct skeleton_structure
{
	vector2 left_leg[5];
	vector2 right_leg[5];
	vector2 core[6];
	vector2 left_arm[4];
	vector2 right_arm[4];
	vector2 head;
};

typedef struct box_structure
{
	vector2 head;
	vector2 origin;
};

namespace overlay
{
	void overlay();
	//void draw(HDC hdc);
	void draw();
	LRESULT CALLBACK window_messages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void update_player_items();
	void update_world_objects();
	vector2 get_screen_pos(vector3 world);

	void aimbot_loop();

	inline std::vector<skeleton_structure> player_items;
	inline std::vector<box_structure> box_items;
}

#endif // !OVERLAY_HPP