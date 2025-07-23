#include <bits/stdc++.h>
#include "main-window.hpp"
#include <windows.h>
#include <psapi.h>

void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // WorkingSetSize is the physical memory (RAM) currently in use by the process.
        // It's a good indicator of actual RAM consumption.
        double workingSetSizeMB = static_cast<double>(pmc.WorkingSetSize) / (1024 * 1024);
        
        // PrivateUsage is the amount of virtual memory that is currently committed 
        // to the process. This memory is not shared with other processes.
        double privateUsageMB = static_cast<double>(pmc.PrivateUsage) / (1024 * 1024);

        std::cout << "Current Process Memory Usage:" << std::endl;
        std::cout << "  Working Set (RAM): " << workingSetSizeMB << " MB" << std::endl;
        std::cout << "  Private Bytes (Virtual): " << privateUsageMB << " MB" << std::endl;
    } else {
        std::cerr << "Failed to get process memory info. Error code: " << GetLastError() << std::endl;
    }
}

int main() {
    mario::MainWindow mainWindow;
    mainWindow.run();

    // Memory usage after window closes (resources might be deallocated)
    std::cout << "\n--- Memory Usage After Window Closes ---" << std::endl;
    printMemoryUsage();
    std::cout << "--------------------------------------" << std::endl;

    return 0;
}