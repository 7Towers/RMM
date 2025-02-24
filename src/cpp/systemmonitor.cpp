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

void SystemMonitor::onRemoveProcessInfo(const ProcessInfo& pi) {
    emit beforeProcessesChanged();
    qDebug() << "Removing process info";
    int indexToRemove = -1;
    for (int i = 0; i < m_processes.size(); i++) {
        if (const auto process = this->m_processes[i]; process->pid() == pi.pid()) {
            indexToRemove = i;
            break;
        }
    }
    if (indexToRemove == -1) {
        return;
    }
    const auto processToRemove = m_processes.takeAt(indexToRemove);
    delete processToRemove;
    emit processesChanged();
    emit afterProcessesChanged();
}

void SystemMonitor::onUpdateProcessInfo(const ProcessInfo& pi) {
    // find the process in processList by pid, then update it
    emit beforeProcessesChanged();
    qDebug() << "Updating process info";
    for (auto& process : m_processes) {
        if (process->pid() == pi.pid()) {
            process->setCPUPercentage(pi.cpu_percentage);
            process->setRAMPercentage(pi.memory_percentage);
            process->setProcessName(pi.name);
            emit processesChanged();
            emit afterProcessesChanged();
            break;
        }
    }
}

void SystemMonitor::onAddProcessInfo(const ProcessInfo& pi) {
    emit beforeProcessesChanged();
    qDebug() << "Adding process info";
    auto pm = new ProcessMetrics();
    pm->setPid(pi.pid());
    pm->setProcessName(pi.name);
    pm->setCPUPercentage(pi.cpu_percentage);
    m_processes.append(pm);
    emit processesChanged();
    emit afterProcessesChanged();
}
