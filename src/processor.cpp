#include "processor.h"
#include "linux_parser.h"

#include <iostream>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // IdleJiffies = idle + iowait
    long idleJiffies = LinuxParser::IdleJiffies();

    // ActiveJiffies = user + nice + system + irq + softirq + steal
    long activeJiffies = LinuxParser::ActiveJiffies();

    // PrevTotalJiffies = PrevIdleJiffies + PrevNonIdleJiffies
    long prevTotalJiffies = prevIdleJiffies_ + prevActiveJiffies_;
    // TotalJiffies = IdleJiffies + ActiveJiffies
    long totalJiffies = idleJiffies + activeJiffies;

    // # differentiate: actual value minus the previous one
    long totald = totalJiffies - prevTotalJiffies;
    long idled = idleJiffies - prevIdleJiffies_;

    // Set previous states for next calculation
    prevIdleJiffies_ = idleJiffies;
    prevActiveJiffies_ = activeJiffies;

    if (totald > 0) {
        // CPU_Percentage = (totald - idled)/totald
        return (double) (totald - idled) / (double) totald;
    }
    return 0.0; 
}