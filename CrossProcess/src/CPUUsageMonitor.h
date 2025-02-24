#include <qdebug.h>
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>
#include <psapi.h>
#include <QString>

#pragma comment(lib, "pdh.lib")

class CPUUsageMonitor {
public:
    CPUUsageMonitor() {};
    void init(QString processID ) {
        this->processID = processID.toInt();
        // Open a query
        PdhOpenQuery(NULL, NULL, &query);
        // Create a counter for the specific process
        std::wstring counterPath = L"\\Process(" + GetProcessName(this->processID) + L")\\% Processor Time";
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
        Sleep(500); // Wait for 1 second

        // Collect data again
        PdhCollectQueryData(query);

        // Get formatted counter value
        PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &counterVal);

        qDebug() << "CPU Usage for pid: " << this->processID << " " << counterVal.doubleValue;
        return counterVal.doubleValue;
    }

private:
    DWORD processID;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;

    std::wstring GetProcessName(DWORD pid) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess == NULL) return L"";

        TCHAR processName[MAX_PATH];
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleFileNameEx(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));
        }

        CloseHandle(hProcess);

        // Return only the executable name
        return std::wstring(processName).substr(std::wstring(processName).find_last_of(L"\\") + 1);
    }
};
