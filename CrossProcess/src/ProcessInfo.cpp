//
// Created by glenn on 2/23/2025.
//

#include "ProcessInfo.h"

void ProcessInfo::setPID(QString pid) {
    this->m_pid = pid;
}

void ProcessInfo::initCPUUsageMonitor() {
    this->cpuUsageMonitor.init(this->m_pid);
}

QString ProcessInfo::pid() const {
    return this->m_pid;
}

void ProcessInfo::updateCPUUsage() {
    this->cpu_percentage = this->cpuUsageMonitor.GetUsage();
}

bool ProcessInfo::hasCPUChanged() {
    if (priorCPU != cpu_percentage) {
        priorCPU = cpu_percentage;
        return true;
    } else {
        return false;
    }
}
