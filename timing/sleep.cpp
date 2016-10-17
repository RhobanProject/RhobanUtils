#include <thread>
#include <chrono>

void ms_sleep(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}  

void us_sleep(long us)
{
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}

