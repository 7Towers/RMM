//
// Created by glenn on 2/23/2025.
//
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>
#include <vector>
#include <string>
#include <psapi.h>

class ProcessCPUMonitor {
private:
    PDH_HQUERY query;
    std::vector<PDH_HCOUNTER> counters;
    std::vector<DWORD> pids;

public:
    ProcessCPUMonitor() {
        PdhOpenQuery(NULL, NULL, &query);
    }

    ~ProcessCPUMonitor() {
        PdhCloseQuery(query);
    }

    void AddProcess(DWORD pid) {
        WCHAR counterPath[PDH_MAX_COUNTER_PATH];
        WCHAR processName[MAX_PATH];
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess) {
            GetProcessImageFileName(hProcess, processName, MAX_PATH);
            CloseHandle(hProcess);

            swprintf_s(counterPath, L"\\Process(%s)\\% Processor Time", wcsrchr(processName, L'\\') + 1);

            PDH_HCOUNTER counter;
            if (PdhAddCounterW(query, counterPath, 0, &counter) == ERROR_SUCCESS) {
                counters.push_back(counter);
                pids.push_back(pid);
            }
        }
    }

    std::vector<double> GetCPUUsage() {
        std::vector<double> usage(pids.size(), 0.0);

        PdhCollectQueryData(query);
        Sleep(1000); // Wait for 1 second
        PdhCollectQueryData(query);

        for (size_t i = 0; i < counters.size(); ++i) {
            PDH_FMT_COUNTERVALUE counterValue;
            PdhGetFormattedCounterValue(counters[i], PDH_FMT_DOUBLE, NULL, &counterValue);
            usage[i] = counterValue.doubleValue / 100.0; // Normalize to 0-100%
        }

        return usage;
    }
};

int main() {
    ProcessCPUMonitor monitor;

    std::vector<DWORD> processIds(1024);
    DWORD bytesReturned;

    if (!EnumProcesses(processIds.data(), sizeof(DWORD) * processIds.size(), &bytesReturned)) {
        std::cerr << "EnumProcesses failed" << std::endl;
        return 1;
    }

    DWORD numProcesses = bytesReturned / sizeof(DWORD);

    std::cout << "Running processes:" << std::endl;
    for (DWORD i = 0; i < numProcesses; i++) {
        if (processIds[i] != 0) {
            std::cout << "PID: " << processIds[i] << std::endl;
            monitor.AddProcess(processIds[i]);
        }
    }

    // Get CPU usage
    std::vector<double> usage = monitor.GetCPUUsage();

    // Print results
    for (size_t i = 0; i < usage.size(); ++i) {
        std::cout << "CPU Usage for PID " << processIds[i] << ": " << usage[i] << "%" << std::endl;
    }

    return 0;
}
