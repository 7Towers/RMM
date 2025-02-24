//
// Created by glenn on 2/23/2025.
//

#include "CrossProcess.h"

QList<QString> CrossProcess::getProcessIDList() {
#ifdef Q_OS_WIN
    return winGetProcessIDList();
#elif Q_OS_MACOS
    return macGetProcessIDList();
#else
    qWarning() << "CrossProcess::getProcessIDList() not implemented for this platform";
    return QList<QString>();
#endif
}

QMap<QString, ProcessInfo> CrossProcess::getProcessInfoList() {
#ifdef Q_OS_WIN
    return winGetProcessInfoList();
#elif Q_OS_MACOS
    return macGetProcessInfoList();
#else
    qWarning() << "CrossProcess::getProcessInfoList() not implemented for this platform";
    return QList<QMap<QString, QString>>();
#endif
}

std::vector<double> CrossProcess::getProcessCPUUsage(const QList<QString> &pids) {
#ifdef Q_OS_WIN
    return winGetProcessCPUUsage(pids);
#elif
    qWarning() << "CrossProcess::getProcessCPUUsage() not implemented for this platform";
    return -1.0;
#endif
}

std::vector<double> CrossProcess::getProcessMemoryUsage(const QList<QString> &pids) {
#ifdef Q_OS_WIN
    return winGetProcessMemoryUsage(pids);
#elif
    qWarning() << "CrossProcess::getProcessMemoryUsage() not implemented for this platform";
    return -1.0;
#endif
}

double CrossProcess::totalCPUUsage() {
#ifdef Q_OS_WIN
    return winTotalCPUUsage();
#elif
    qWarning() << "CrossProcess::totalCPUUsage() not implemented for this platform";
    return -1.0;
#endif
}

double CrossProcess::totalMemoryUsage() {
#ifdef Q_OS_WIN
    return winTotalMemoryUsage();
#elif
    qWarning() << "CrossProcess::totalMemoryUsage() not implemented for this platform";
    return -1.0;
#endif
}

double CrossProcess::totalPercentMemoryUsed() {
#ifdef Q_OS_WIN
    return winTotalPercentMemoryUsed();
#elif
    qWarning() << "CrossProcess::totalPercentMemoryUsed() not implemented for this platform";
    return -1.0;
#endif
}

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
QList<QString> CrossProcess::winGetProcessIDList() {
    auto pList = QList<QString>();

    DWORD processIds[1024], bytesReturned;
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
        return pList;
    }

    int numProcesses = bytesReturned / sizeof(DWORD);
    for (int i = 0; i < numProcesses; i++) {
        if (processIds[i] != 0) {
            pList.append(QString::number(processIds[i]));
        }
    }

    return pList;
}

QMap<QString, ProcessInfo> CrossProcess::winGetProcessInfoList() {
        auto pList = QMap<QString, ProcessInfo>();

        DWORD processIds[1024], bytesReturned;
        if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
            return pList;
        }

        int numProcesses = bytesReturned / sizeof(DWORD);
        for (int i = 0; i < numProcesses; i++) {
            if (processIds[i] != 0) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
                if (hProcess) {
                    TCHAR processName[MAX_PATH] = TEXT("<unknown>");
                    HMODULE hMod;
                    DWORD cbNeeded;

                    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                        GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));
                    }


                    ProcessInfo pInfo;
                    pInfo.setPID(QString::number(processIds[i]));
                    pInfo.name = QString::fromWCharArray(processName);
                    pList[pInfo.pid()] = pInfo;

                    CloseHandle(hProcess);
                }
            }
        }

        return pList;
}


std::vector<double> CrossProcess::winGetProcessCPUUsage(const QList<QString> &pids) {
    std::vector<double> cpuUsages(pids.size(), -1.0);
    std::vector<HANDLE> handles(pids.size(), NULL);
    std::vector<ULARGE_INTEGER> kernelStart(pids.size()), userStart(pids.size()), sysStart(pids.size());

    FILETIME ftCreation, ftExit, ftKernel, ftUser, ftSys;
    ULARGE_INTEGER ulSysStart;

    // Open process handles and get initial times
    for (int i = 0; i < pids.size(); ++i) {
        DWORD processID = pids[i].toUInt();
        handles[i] = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        if (handles[i] != NULL) {
            GetProcessTimes(handles[i], &ftCreation, &ftExit, &ftKernel, &ftUser);
            GetSystemTimeAsFileTime(&ftSys);

            kernelStart[i].LowPart = ftKernel.dwLowDateTime;
            kernelStart[i].HighPart = ftKernel.dwHighDateTime;
            userStart[i].LowPart = ftUser.dwLowDateTime;
            userStart[i].HighPart = ftUser.dwHighDateTime;
            sysStart[i].LowPart = ftSys.dwLowDateTime;
            sysStart[i].HighPart = ftSys.dwHighDateTime;
        }
    }

    Sleep(1000); // Wait for 1 second

    // Get end times and calculate CPU usage
    for (int i = 0; i < pids.size(); ++i) {
        if (handles[i] != NULL) {
            GetProcessTimes(handles[i], &ftCreation, &ftExit, &ftKernel, &ftUser);
            GetSystemTimeAsFileTime(&ftSys);

            ULARGE_INTEGER kernelEnd, userEnd, sysEnd;
            kernelEnd.LowPart = ftKernel.dwLowDateTime;
            kernelEnd.HighPart = ftKernel.dwHighDateTime;
            userEnd.LowPart = ftUser.dwLowDateTime;
            userEnd.HighPart = ftUser.dwHighDateTime;
            sysEnd.LowPart = ftSys.dwLowDateTime;
            sysEnd.HighPart = ftSys.dwHighDateTime;

            ULONGLONG kernelDiff = kernelEnd.QuadPart - kernelStart[i].QuadPart;
            ULONGLONG userDiff = userEnd.QuadPart - userStart[i].QuadPart;
            ULONGLONG sysDiff = sysEnd.QuadPart - sysStart[i].QuadPart;

            cpuUsages[i] = (kernelDiff + userDiff) * 100.0 / sysDiff;

            CloseHandle(handles[i]);
        }
    }

    return cpuUsages;
}

std::vector<double> CrossProcess::winGetProcessMemoryUsage(const QList<QString> &pids) {
    std::vector<double> memoryUsages(pids.size(), -1.0);

    for (int i = 0; i < pids.size(); ++i) {
        DWORD processID = pids[i].toUInt();
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        if (hProcess != NULL) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                memoryUsages[i] = static_cast<double>(pmc.WorkingSetSize) / (1024 * 1024); // Convert to MB
            }
            CloseHandle(hProcess);
        }
    }

    return memoryUsages;
}

double CrossProcess::winTotalCPUUsage() {
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime) == 0) {
        return -1.0;
    }

    ULARGE_INTEGER sysIdle, sysKernel, sysUser;
    sysIdle.LowPart = idleTime.dwLowDateTime;
    sysIdle.HighPart = idleTime.dwHighDateTime;
    sysKernel.LowPart = kernelTime.dwLowDateTime;
    sysKernel.HighPart = kernelTime.dwHighDateTime;
    sysUser.LowPart = userTime.dwLowDateTime;
    sysUser.HighPart = userTime.dwHighDateTime;

    static ULARGE_INTEGER prevSysIdle, prevSysKernel, prevSysUser;
    ULONGLONG sysIdleDiff = sysIdle.QuadPart - prevSysIdle.QuadPart;
    ULONGLONG sysKernelDiff = sysKernel.QuadPart - prevSysKernel.QuadPart;
    ULONGLONG sysUserDiff = sysUser.QuadPart - prevSysUser.QuadPart;

    prevSysIdle = sysIdle;
    prevSysKernel = sysKernel;
    prevSysUser = sysUser;

    ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
    double cpuUsage = (sysTotal - sysIdleDiff) * 100.0 / sysTotal;

    return cpuUsage;
}

double CrossProcess::winTotalMemoryUsage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo) == 0) {
        return -1.0;
    }

    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = totalPhysMem - memInfo.ullAvailPhys;

    return static_cast<double>(physMemUsed) / (1024 * 1024); // Convert to MB
}

double CrossProcess::winTotalPercentMemoryUsed() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo) == 0) {
        return -1.0;
    }

    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = totalPhysMem - memInfo.ullAvailPhys;

    return (static_cast<double>(physMemUsed) / totalPhysMem) * 100.0;
}


#endif

#ifdef Q_OS_MACOS

#include <libproc.h>
QList<QString> CrossProcess::macGetProcessIDList() {
    auto pList = QList<QString>();

    int numberOfProcesses = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t pids[numberOfProcesses];
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

    for (int i = 0; i < numberOfProcesses; i++) {
        pList.append(QString::number(pids[i]));
    }

    return pList;
}

QMap<QString, ProcessInfo> CrossProcess::winGetProcessInfoList() {
    auto pList = QMap<QString, ProcessInfo>();

    DWORD processIds[1024], bytesReturned;
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
        return pList;
    }

    int numProcesses = bytesReturned / sizeof(DWORD);
    for (int i = 0; i < numProcesses; i++) {
        if (processIds[i] != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
            if (hProcess) {
                TCHAR processName[MAX_PATH] = TEXT("<unknown>");
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR));
                }

                QMap<QString, ProcessInfo processInfoMap;
                ProcessInfo pInfo;
                processInfoMap["PID"] = QString::number(processIds[i]);
                pInfo.pid = QString::number(processIds[i]);
                pInfo.name = QString::fromWCharArray(processName);
                pList[pInfo.pid] = pInfo;

                CloseHandle(hProcess);
            }
        }
    }

    return pList;
#endif


