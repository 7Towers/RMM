//
// Created by glenn on 2/23/2025.
//

#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QString>

#include "CPUUsageMonitor.h"

class ProcessInfo {
public:
    QString name = "";
    double cpu_percentage = 0.0;
    double memory_percentage = 0.0;
    void setPID(QString pid);

    void initCPUUsageMonitor();

    QString pid() const;
    void updateCPUUsage();
    bool hasCPUChanged();
private:
    QString m_pid = "";
    double priorCPU = 0.0;
    CPUUsageMonitor cpuUsageMonitor = CPUUsageMonitor();
};



#endif //PROCESSINFO_H
