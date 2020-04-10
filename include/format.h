#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // DONE: See src/format.cpp
std::string formatTimeUnit(int timeUnit);
};                                    // namespace Format

#endif