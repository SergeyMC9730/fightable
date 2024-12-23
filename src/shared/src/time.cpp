#include <fightable/time.h>
#include <chrono>
#include <thread>

void _fSleep(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}