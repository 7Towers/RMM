//
// Created by Glenn Gartner on 2/13/25.
//

#include "ProcessMetrics.h"

ProcessMetrics::ProcessMetrics() {
}

QString ProcessMetrics::processName() const {
    return m_processName;
}

void ProcessMetrics::setProcessName(QString processName) {
    if (m_processName == processName)
        return;

    m_processName = processName;
    emit processNameChanged(m_processName);
}

double ProcessMetrics::cpuUsage() const {
    return m_cpuUsage;
}

void ProcessMetrics::setCPUUsage(double cpuUsage) {
    if (m_cpuUsage == cpuUsage)
        return;

    m_cpuUsage = cpuUsage;
    emit cpuUsageChanged(m_cpuUsage);
}

double ProcessMetrics::cpuPercentage() const {
    return m_cpuPercentage;
}

void ProcessMetrics::setCPUPercentage(double cpuPercentage) {
    if (m_cpuPercentage == cpuPercentage)
        return;

    m_cpuPercentage = cpuPercentage;
    emit cpuPercentageChanged(m_cpuPercentage);
}

double ProcessMetrics::RAMUsage() const {
    return m_RAMUsage;
}

void ProcessMetrics::setRAMUsage(double RAMUsage) {
    if (m_RAMUsage == RAMUsage)
        return;

    m_RAMUsage = RAMUsage;
    emit RAMUsageChanged(m_RAMUsage);
}

double ProcessMetrics::RAMPercentage() const {
    return m_RAMPercentage;
}

void ProcessMetrics::setRAMPercentage(double RAMPercentage) {
    if (m_RAMPercentage == RAMPercentage)
        return;

    m_RAMPercentage = RAMPercentage;
    emit RAMPercentageChanged(m_RAMPercentage);
}

void ProcessMetrics::setPid(pid_t pid) {
    m_pid = pid;
}



