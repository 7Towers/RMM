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

QList<QMap<QString, ProcessInfo>> CrossProcess::getProcessInfoList() {
#ifdef Q_OS_WIN
    return winGetProcessInfoList();
#elif Q_OS_MACOS
    return macGetProcessInfoList();
#else
    qWarning() << "CrossProcess::getProcessInfoList() not implemented for this platform";
    return QList<QMap<QString, QString>>();
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

QList<QMap<QString, ProcessInfo>> CrossProcess::winGetProcessInfoList() {
        auto pList = QList<QMap<QString, ProcessInfo>>();

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

                    QMap<QString, ProcessInfo> processInfoMap;

                    ProcessInfo pInfo;
                    pInfo.pid = QString::number(processIds[i]);
                    pInfo.name = QString::fromWCharArray(processName);
                    processInfoMap["PID"] = pInfo;
                    pList.append(processInfoMap);

                    CloseHandle(hProcess);
                }
            }
        }

        return pList;
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

st<QMap<QString, ProcessInfo> CrossProcess::winGetProcessInfoList() {
    auto pList = QList<QMap<QString, ProcessInfo>();

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
                pList.append(processInfoMap);

                CloseHandle(hProcess);
            }
        }
    }

    return pList;
#endif


