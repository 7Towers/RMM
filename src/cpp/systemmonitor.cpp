#include "systemmonitor.h"
#include <QProcess>
#include <QDir>

SystemMonitor::SystemMonitor()
{
    qDebug() << "SystemMonitor::SystemMonitor()";
}

SystemMonitor::~SystemMonitor()
{
    cleanupProcessRefs();
    metricsThread.running = false;
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

void SystemMonitor::updateProcessList()
{
    cleanupProcessRefs();
    auto processInfoList = CrossProcess::getProcessInfoList();
    for (auto& processInfo : processInfoList) {
        auto process = new ProcessMetrics();
        process->setPid(processInfo["PID"].pid);
        process->setProcessName(processInfo["PID"].name);
        m_processes.append(process);
    }
    emit processesChanged();
}

