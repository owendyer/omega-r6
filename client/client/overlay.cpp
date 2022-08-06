#include "overlay.hpp"
#include "xorstr.hpp"

HWND game_window;
HWND overlay_window;

static HBRUSH body_color = CreateSolidBrush(RGB(250, 130, 235));
static HBRUSH head_color = CreateSolidBrush(RGB(255, 255, 255));
static HBRUSH health_color;

void drawRect(HDC* hdc, int x, int y, int width, int height, HBRUSH col)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;

	FillRect(*hdc, &rect, col);
}

void drawBox(HDC* hdcDesktop, HDC* hdcMem, int startX, int startY, int width, int height, int thickness, HBRUSH color)
{
	// Top
	drawRect(&(*hdcMem), 0, 0, width, thickness, color);
	BitBlt(*hdcDesktop, startX, startY, width, thickness, *hdcMem, 0, 0, SRCCOPY);

	//Left
	drawRect(&(*hdcMem), 0, 0, thickness, height, color);
	BitBlt(*hdcDesktop, startX, startY, thickness, height, *hdcMem, 0, 0, SRCCOPY);

	//Right
	drawRect(&(*hdcMem), width, 0, thickness, height, color);
	BitBlt(*hdcDesktop, startX + width, startY, thickness, height, *hdcMem, 0, 0, SRCCOPY);

	//Bottom
	drawRect(&(*hdcMem), 0, height, width, thickness, color);
	BitBlt(*hdcDesktop, startX, startY + height, width + thickness, thickness, *hdcMem, 0, 0, SRCCOPY);
}

void draw_filled_box(HDC* hdc_live, HDC* hdc_buffer, int x1 /* top left corner x */, int y1 /* top left corner y */, int x2 /* width */, int y2 /* height */, HBRUSH color)
{
	drawRect(&(*hdc_buffer), 0, 0, x2, y2, color);
	BitBlt(*hdc_live, x1, y1, x2, y2, *hdc_buffer, 0, 0, SRCCOPY);
}

void draw_health(HDC* hdc_live, HDC* hdc_buffer, int x1, int y1, int x2, int y2, int health)
{
	float factor = health / 100.0f;

	float r = (factor >= 0.5f) ? static_cast<std::uint8_t>(255 - 255 * (factor - 0.5f) / 0.5f) : 255;
	float g = (factor >= 0.5f) ? 255 : static_cast<std::uint8_t>(255 * factor / 0.5f);
	float b = 0.00f;

	health_color = CreateSolidBrush(RGB(r, g, b));

	draw_filled_box(&(*hdc_live), &(*hdc_buffer), x1, y1, x2, y2, health_color);

	DeleteObject(health_color);
}

void overlay::overlay()
{
	game_window = FindWindowA(XORS("R6Game"), NULL);
	sdk::display_width = GetSystemMetrics(SM_CXSCREEN);
	sdk::display_height = GetSystemMetrics(SM_CYSCREEN);

	/*
	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Register the window class
	const char* CLASS_NAME = XORS("Chrome_WidgetWin_0");
	WNDCLASS wc{};
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	wc.lpfnWndProc = window_messages;
	RegisterClass(&wc);

	// Create the window
	overlay_window = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, CLASS_NAME, XORS("Spotify"),
		WS_POPUP | WS_VISIBLE,						// Window style
		1, 2,										// Window initial position
		sdk::display_width - 2, sdk::display_height - 1,      // Window size
		nullptr, nullptr, nullptr, nullptr);

	MARGINS margin{ -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(overlay_window, &margin);
	SetWindowLong(overlay_window, GWL_EXSTYLE, GetWindowLong(overlay_window, GWL_EXSTYLE) | WS_EX_LAYERED);

	// Window loop
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0) && (!GetAsyncKeyState(VK_END) & 1))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	DeleteObject(wc.hbrBackground);
	return;
	*/
	
	while (!GetAsyncKeyState(VK_END) & 1)
	{
		draw();
	}

	return;
}

//void overlay::draw(HDC hdc)
void overlay::draw()
{
	//HWND window_handle = WindowFromDC(hdc);
	/*
	HDC hdc_buffer = CreateCompatibleDC(hdc);
	int buffer_dc = SaveDC(hdc_buffer);

	HBITMAP bitmap = CreateCompatibleBitmap(hdc, sdk::display_width, sdk::display_height);
	SelectObject(hdc_buffer, bitmap);

	Gdiplus::Graphics gf(hdc_buffer);
	Gdiplus::Pen pen(Gdiplus::Color(255, 250, 130, 235), 2);

	SetWindowPos(game_window, overlay_window, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	*/

	/*
	for (auto render_item : overlay::player_items)
	{
		vector2 current_bone;
		vector2 next_bone;

		for (int i = 0; i < 4; ++i)
		{
			current_bone = render_item.left_leg[i];
			if (current_bone == vector2())
				continue;

			next_bone = render_item.left_leg[i + 1];
			if (next_bone == vector2())
				continue;

			gf.DrawLine(&pen, Gdiplus::Point(current_bone.x, current_bone.y), Gdiplus::Point(next_bone.x, next_bone.y));
		}

		for (int i = 0; i < 4; ++i)
		{
			current_bone = render_item.right_leg[i];
			if (current_bone == vector2())
				continue;

			next_bone = render_item.right_leg[i + 1];
			if (next_bone == vector2())
				continue;

			gf.DrawLine(&pen, Gdiplus::Point(current_bone.x, current_bone.y), Gdiplus::Point(next_bone.x, next_bone.y));
		}

		for (int i = 0; i < 4; ++i)
		{
			current_bone = render_item.core[i];
			if (current_bone == vector2())
				continue;

			next_bone = render_item.core[i + 1];
			if (next_bone == vector2())
				continue;

			gf.DrawLine(&pen, Gdiplus::Point(current_bone.x, current_bone.y), Gdiplus::Point(next_bone.x, next_bone.y));
		}

		for (int i = 0; i < 3; ++i)
		{
			current_bone = render_item.left_arm[i];
			if (current_bone == vector2())
				continue;

			next_bone = render_item.left_arm[i + 1];
			if (next_bone == vector2())
				continue;

			gf.DrawLine(&pen, Gdiplus::Point(current_bone.x, current_bone.y), Gdiplus::Point(next_bone.x, next_bone.y));
		}

		for (int i = 0; i < 3; ++i)
		{
			current_bone = render_item.right_arm[i];
			if (current_bone == vector2())
				continue;

			next_bone = render_item.right_arm[i + 1];
			if (next_bone == vector2())
				continue;

			gf.DrawLine(&pen, Gdiplus::Point(current_bone.x, current_bone.y), Gdiplus::Point(next_bone.x, next_bone.y));
		}

		current_bone = render_item.head;
		if (current_bone == vector2())
			continue;

		next_bone = render_item.core[1];
		if (next_bone == vector2())
			continue;

		float distance = distance_vec_2(next_bone, current_bone);
		Gdiplus::Rect head_rect;
		head_rect.X = current_bone.x - (distance / 4);
		head_rect.Y = current_bone.y - (distance / 4);
		head_rect.Width = distance / 2;
		head_rect.Height = distance / 2;
		gf.DrawEllipse(&pen, head_rect);
	}
	*/

/*
function_end:

	BitBlt(hdc, 0, 0, sdk::display_width, sdk::display_height, hdc_buffer, 0, 0, SRCCOPY);
	RestoreDC(hdc_buffer, buffer_dc);
	DeleteDC(hdc_buffer);
	DeleteObject(bitmap);

	gf.Flush();
	gf.Clear(Gdiplus::Color(0, 0, 0));
	*/


	// Get desktop window
	HDC desktop_hdc = GetDC(0);
	HWND desktop_hwnd = WindowFromDC(desktop_hdc); // Only need to release desktop_hdc
	// Create back buffer
	HDC buffer_hdc = CreateCompatibleDC(desktop_hdc);
	// Create bitmap compatible with local monitor
	HBITMAP desktop_bitmap = CreateCompatibleBitmap(desktop_hdc, sdk::display_width, sdk::display_height);
	HBITMAP buffer_bitmap = (HBITMAP)SelectObject(buffer_hdc, desktop_bitmap);

	for (auto i : box_items)
	{
		if (i.head.x <= 0 || i.head.x > sdk::display_width || i.head.y <= 0 || i.head.y > sdk::display_height)
			continue;

		if (i.origin.x <= 0 || i.origin.x > sdk::display_width || i.origin.y <= 0 || i.origin.y > sdk::display_height)
			continue;

		int box_height = i.origin.y - i.head.y;
		int box_width = box_height / 2.2;

		RECT player_box;
		player_box.left = i.head.x - (box_width / 2);
		player_box.top = i.head.y - (box_width / 3);
		player_box.right = box_width;
		player_box.bottom = box_height + (box_width / 2);
		// Draw player bounding box
		drawBox(&desktop_hdc, &buffer_hdc, player_box.left, player_box.top, player_box.right, player_box.bottom, 2, body_color);

		int head_radius = box_width / 2.6;

		RECT head_box;
		head_box.left = i.head.x - (head_radius / 2);
		head_box.top = i.head.y - (head_radius / 2);
		head_box.right = head_radius;
		head_box.bottom = head_radius;

		// Draw head bounding box
		drawBox(&desktop_hdc, &buffer_hdc, head_box.left, head_box.top, head_box.right, head_box.bottom, 1, head_color);


		/*
		int box_height = i.origin.y - i.head.y;
		int box_width = box_height / 2.2f;
		int head_box = box_width / 2.5f;

		RECT player_box;
		player_box.left = i.head.x - (box_width / 2);
		player_box.top = i.head.y - (box_width / 3);
		player_box.right = box_width;
		player_box.bottom = box_height * 1.15f;

		// Draw player bounding box
		drawBox(&desktop_hdc, &buffer_hdc, player_box.left, player_box.top, player_box.right, player_box.bottom, 2, body_color);
		// Draw head bounding box
		drawBox(&desktop_hdc, &buffer_hdc, i.head.x - (head_box / 2), i.head.y - (head_box / 2), head_box, head_box, 1, head_color);
		*/
	}

	// Cleanup
	ReleaseDC(desktop_hwnd, desktop_hdc);
	DeleteDC(desktop_hdc);
	DeleteDC(buffer_hdc);
	DeleteObject(desktop_bitmap);
	DeleteObject(buffer_bitmap);
}

//LRESULT CALLBACK overlay::window_messages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	RECT screen_rect{ 0, 0, sdk::display_width, sdk::display_height };
//
//	switch (msg)
//	{
//	case WM_ERASEBKGND:
//		return TRUE;
//	case WM_PAINT:
//		hdc = BeginPaint(hwnd, &ps);
//		draw(hdc);
//		EndPaint(hwnd, &ps);
//		InvalidateRect(hwnd, &screen_rect, TRUE);
//		ReleaseDC(hwnd, hdc);
//		DeleteDC(hdc);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	default:
//		return DefWindowProc(hwnd, msg, wparam, lparam);
//	}
//}

void overlay::update_player_items()
{
	//std::vector<skeleton_structure> buffer;
	std::vector<box_structure> buffer;

	while (true)
	{
		//std::cout << "count: " << sdk::entity::get()->count() << "\n";

		std::uint32_t round_state = sdk::round_manager::get()->state();
		if (round_state != 2 && round_state != 3)
		{
			for (int i = 0; i < sdk::entity::get()->count(); ++i)
			{
				//skeleton_structure ps;

				//buffer.push_back(ps);

				box_structure bs;
				buffer.push_back(bs);
			}

			//player_items = buffer;
			box_items = buffer;
			buffer.clear();
			continue;
		}

		std::uintptr_t local_controller = sdk::entity::get()->local();

		if (local_controller == NULL)
			continue;

		std::uintptr_t local_replication = sdk::entity::get()->replication(local_controller);
		std::uintptr_t target_replication = 0x0;





		std::uint8_t local_team = sdk::entity::get()->team(local_replication);
		std::uint8_t target_team = sdk::entity::get()->team(target_replication);

		if (target_team == local_team)
			continue;

		for (int i = 0; i < sdk::entity::get()->count(); ++i)
		{
			//skeleton_structure ps;
			box_structure bs;

			std::uintptr_t controller = sdk::entity::get()->component(i);

			if (controller == NULL || controller == local_controller)
				continue;

			std::uintptr_t replication = sdk::entity::get()->replication(controller);
			if (replication == NULL) continue;

			std::uint8_t team = sdk::entity::get()->team(replication);

			if (team == local_team) continue;

			std::uintptr_t pawn = sdk::entity::get()->pawn(controller);
			std::uintptr_t actor = sdk::entity::get()->actor(pawn);

			std::uint32_t health = sdk::entity::get()->health(actor);
			if (health <= 0 || health > 165)
				continue;

			/*std::uintptr_t bone_chain = sdk::skeleton::get()->get_bone_chain(actor);
			std::uintptr_t bone_array, size, bones, data;
			sdk::skeleton::get()->get_hash_skeleton(pawn, bone_chain, &bone_array, &size, &bones, &data);*/

			// Head
			vector3 pos = sdk::skeleton::get()->bone_pos_by_id(pawn, 11);
			vector2 screen = get_screen_pos(pos);

			bs.head = screen;

			// Origin
			pos = sdk::skeleton::get()->bone_pos_by_id(pawn, 0);
			screen = get_screen_pos(pos);

			bs.origin = screen;

			/*

			// Left leg
			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xB95094E1);
			screen = get_screen_pos(pos);

			ps.left_leg[0] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x58988870);
			screen = get_screen_pos(pos);

			ps.left_leg[1] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x7F9D9777);
			screen = get_screen_pos(pos);

			ps.left_leg[2] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xCEFA24D8);
			screen = get_screen_pos(pos);

			ps.left_leg[3] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xDED10611);
			screen = get_screen_pos(pos);

			ps.left_leg[4] = screen;

			// Right leg
			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x42BE0FCB);
			screen = get_screen_pos(pos);

			ps.right_leg[0] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x9B14362C);
			screen = get_screen_pos(pos);

			ps.right_leg[1] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x863D09FC);
			screen = get_screen_pos(pos);

			ps.right_leg[2] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xCC559E90);
			screen = get_screen_pos(pos);

			ps.right_leg[3] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xDED10611);
			screen = get_screen_pos(pos);

			ps.right_leg[4] = screen;

			// Core
			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xDED10611);
			screen = get_screen_pos(pos);

			ps.core[0] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x530EC1CB);
			screen = get_screen_pos(pos);

			ps.core[1] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x8F39FA4E);
			screen = get_screen_pos(pos);

			ps.core[2] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x1630ABF4);
			screen = get_screen_pos(pos);

			ps.core[3] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x8023796D);
			screen = get_screen_pos(pos);

			ps.core[4] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x7C159A2);
			screen = get_screen_pos(pos);

			ps.core[5] = screen;

			// Left arm
			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xB675F36C);
			screen = get_screen_pos(pos);

			ps.left_arm[0] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x89B93A80);
			screen = get_screen_pos(pos);

			ps.left_arm[1] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xEB830ADA);
			screen = get_screen_pos(pos);

			ps.left_arm[2] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x2D4660A8);
			screen = get_screen_pos(pos);

			ps.left_arm[3] = screen;

			// Right arm
			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x75F94D30);
			screen = get_screen_pos(pos);

			ps.right_arm[0] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x7257A1AA);
			screen = get_screen_pos(pos);

			ps.right_arm[1] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0x6BB3F727);
			screen = get_screen_pos(pos);

			ps.right_arm[2] = screen;

			pos = sdk::skeleton::get()->get_bone_by_hash(bone_array, size, bones, data, 0xF60647E5);
			screen = get_screen_pos(pos);

			ps.right_arm[3] = screen;
			*/

			//buffer.push_back(ps);
			buffer.push_back(bs);
		}

		//player_items = buffer;
		box_items.clear();
		box_items = buffer;
		buffer.clear();
	}
}

vector2 overlay::get_screen_pos(vector3 world)
{
	vector2 screen;
	if (!sdk::camera_manager::get()->w2s(world, &screen))
		return vector2(0, 0);

	return screen;
}

void overlay::aimbot_loop()
{
	while (!GetAsyncKeyState(VK_END) & 1)
	{
		aimbot::aimbot();

		Sleep(1);
	}
}