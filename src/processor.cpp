#include "processor.h"
#include "linux_parser.h"

#include <iostream>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // PrevIdleJiffies = previdle + previowait
    // IdleJiffies = idle + iowait

    // PrevActiveJiffies = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    // ActiveJiffies = user + nice + system + irq + softirq + steal

    // PrevTotalJiffies = PrevIdleJiffies + PrevNonIdleJiffies
    // TotalJiffies = IdleJiffies + ActiveJiffies

    // # differentiate: actual value minus the previous one
    // totald = TotalJiffies - PrevTotalJiffies
    // idled = IdleJiffies - PrevIdleJiffies

    // CPU_Percentage = (totald - idled)/totald

    long activeJiffies = LinuxParser::ActiveJiffies();
    long idleJiffies = LinuxParser::IdleJiffies();
    long totalJiffies = idleJiffies + activeJiffies;
    if (totalJiffies > 0) {
        return (double) activeJiffies / (double) totalJiffies;
    }
    return 0.0; 
}