#include <string>
#include <chrono>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    // Catch invalid values 
    if (seconds <= 0) return string();

    // TODO: calculate the elapsed time
    // auto now = std::chrono::system_clock::now().time_since_epoch();
    // long long currentTime = (long long) std::chrono::duration_cast<std::chrono::seconds>(now).count();

    long time = seconds;
	int hour = 0, min = 0, sec = 0;

	hour = time/3600;
	time = time%3600;
	min = time/60;
	time = time%60;
	sec = time;
    return to_string(hour) + ":" + to_string(min) + ":" + to_string(sec);
}