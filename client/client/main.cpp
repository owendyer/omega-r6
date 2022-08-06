#include "driver.hpp"
#include "overlay.hpp"
#include "xorstr.hpp"
#include <thread>


int main(int argc, char** argv)
{
	SetConsoleTitleA(XORS("omega - r6"));

	if (comm::init() == false)
	{
		std::cout << XORS("omega -> failed to initialize\n");
		system(XORS("pause"));
		return 0;
	}

	std::cout << XORS("omega -> successfully initialized\n");
	std::cout << XORS("omega -> please start your game now\n\n\n");

	DWORD delay = 0x0;
	while (delay == 0x0)
	{
		GetWindowThreadProcessId(FindWindowA(XORS("R6Game"), NULL), &delay);
	}

	driver::get_process_id("RainbowSix.exe");

	driver::get_module_base(driver::pid);

	system(XORS("cls"));

	std::cout << XORS("omega -> update notes:\n");
	std::cout << XORS("<------> updated for 26/07/2022\n");
	std::cout << XORS("<------> skeletons removed for testing\n");
	std::cout << XORS("<------> temporarily reverted to box esp\n\n\n");
	std::cout << XORS("omega -> keyboard commands:\n");
	std::cout << XORS("<------> press 'END' to close the program\n");

	std::thread update_thread(overlay::update_player_items);
	update_thread.detach();
	std::thread aimbot_thread(overlay::aimbot_loop);
	aimbot_thread.detach();
	std::thread render_thread(overlay::overlay);
	render_thread.join();

	//system("pause"); // remove for live build, its cleaner and retards cant mess something else up
	return 0;
}