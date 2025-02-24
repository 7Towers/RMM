//
// Created by glenn on 2/23/2025.
//
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <psapi.h>
#include <iostream>
#include <string>

#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "psapi.lib")

class CPUUsageMonitor {
public:
    CPUUsageMonitor(DWORD pid) : processID(pid) {
        // Open a query
        PdhOpenQuery(NULL, NULL, &query);
        // Create a counter for the specific process
        std::wstring counterPath = L"\\Process(" + GetProcessName(pid) + L")\\% Processor Time";
        PdhAddEnglishCounter(query, counterPath.c_str(), NULL, &counter);

        // Initial data collection to set up the counter
        PdhCollectQueryData(query);
    }

    ~CPUUsageMonitor() {
        PdhCloseQuery(query);
    }

    double GetUsage() {
        PDH_FMT_COUNTERVALUE counterVal;

        // Collect initial data
        PdhCollectQueryData(query);

        // Wait for a short interval
        Sleep(1000); // Wait for 1 second

        // Collect data again
        PdhCollectQueryData(query);

        // Get formatted counter value
        PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &counterVal);

        return counterVal.doubleValue;
    }

private:
    DWORD processID;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;

    std::wstring GetProcessName(DWORD pid) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess == NULL) return L"";

        WCHAR processName[MAX_PATH] = L"";
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseNameW(hProcess, hMod, processName, sizeof(processName)/sizeof(WCHAR));
        }

        CloseHandle(hProcess);
        return std::wstring(processName);
    }
};

int main() {
    DWORD pid;
    std::cout << "Enter Process ID: ";
    std::cin >> pid;

    try {
        CPUUsageMonitor monitor(pid);

        while (true) {
            double cpuUsage = monitor.GetUsage();
            std::cout << "CPU Usage for PID " << pid << ": " << cpuUsage << "%" << std::endl;
            Sleep(1000); // Sleep for 1 second
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

