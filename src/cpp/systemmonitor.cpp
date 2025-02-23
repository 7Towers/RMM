#include "systemmonitor.h"
#include <QProcess>
#include <QDir>


SystemMonitor::SystemMonitor()= default;

SystemMonitor::~SystemMonitor()
{
    cleanupProcessRefs();
    metricsThread.stop();
    metricsThread.wait();
}

QQmlListProperty<ProcessMetrics> SystemMonitor::processes()
{
    return {this, &m_processes};
}

void SystemMonitor::cleanupProcessRefs()
{
    for (auto& process : m_processes) {
        delete process;
    }
    m_processes.clear();
}


void SystemMonitor::start() {
    connect(&metricsThread, &MetricsThread::updateProcessInfo, this, &SystemMonitor::onUpdateProcessInfo);
    connect(&metricsThread, &MetricsThread::removeProcessInfo, this, &SystemMonitor::onRemoveProcessInfo);
    connect(&metricsThread, &MetricsThread::addProcessInfo, this, &SystemMonitor::onAddProcessInfo);
    metricsThread.start();
}

void SystemMonitor::onRemoveProcessInfo(ProcessInfo pi) {
    for (auto& process : m_processes) {
        if (process->pid() == pi.pid) {
            m_processes.removeOne(process);
            delete process;
            break;
        }
    }
}

void SystemMonitor::onUpdateProcessInfo(ProcessInfo pi) {
    // find the process in processList by pid, then update it
    bool processFound = false;
    for (auto& process : m_processes) {
        if (process->pid() == pi.pid) {
            process->setCPUPercentage(pi.cpu_percentage);
            process->setRAMPercentage(pi.memory_percentage);
            process->setProcessName(pi.name);
            processFound = true;
            break;
        }
    }
    if (!processFound) {
        auto process = new ProcessMetrics();
        process->setPid(pi.pid);
        process->setCPUPercentage(pi.cpu_percentage);
        process->setRAMPercentage(pi.memory_percentage);
        process->setProcessName(pi.name);
        m_processes.append(process);
    }
}

void SystemMonitor::onAddProcessInfo(ProcessInfo pi) {
    auto pm = new ProcessMetrics();
    pm->setPid(pi.pid);
    pm->setProcessName(pi.name);
    pm->setCPUPercentage(pi.cpu_percentage);
    m_processes.append(pm);
    emit processesChanged();
}
