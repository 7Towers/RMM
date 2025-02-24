//
// Created by Glenn Gartner on 2/13/25.
//

#include "ProcessMetrics.h"

ProcessMetrics::ProcessMetrics() {
}

QString ProcessMetrics::processName() const {
    return m_processName;
}

QString ProcessMetrics::pid() const {
    return this->m_pid;
}

void ProcessMetrics::setProcessName(QString processName) {
    if (m_processName == processName)
        return;

    m_processName = processName;
    emit processNameChanged(m_processName);
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

double ProcessMetrics::RAMPercentage() const {
    return m_RAMPercentage;
}

void ProcessMetrics::setRAMPercentage(double RAMPercentage) {
    if (m_RAMPercentage == RAMPercentage)
        return;

    m_RAMPercentage = RAMPercentage;
    emit RAMPercentageChanged(m_RAMPercentage);
}

void ProcessMetrics::setPid(QString pid) {
    m_pid = pid;
}



