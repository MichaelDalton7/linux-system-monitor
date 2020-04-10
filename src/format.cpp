#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "format.h"

using std::string;
using std::to_string;
using std::setw;
using std::setfill;

string Format::formatTimeUnit(int timeUnit) {
    if (timeUnit < 10) {
        return "0" + to_string(timeUnit);
    }
    return to_string(timeUnit);
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    // Catch invalid values 
    if (seconds <= 0) return string();

    long time = seconds;
	int hour = 0, min = 0, sec = 0;

	hour = time/3600;
	time = time%3600;
	min = time/60;
	time = time%60;
	sec = time;

    return  Format::formatTimeUnit(hour) + ":" + Format::formatTimeUnit(min) + ":" + Format::formatTimeUnit(sec);
}