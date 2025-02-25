#include "systemmonitor.h"
#include <QProcess>
#include <QDir>


SystemMonitor::SystemMonitor() {
    this->m_totalCPUUsage = CrossProcess::totalCPUUsage();
    this->m_totalMemoryUsage = CrossProcess::totalMemoryUsage();
    this->m_totalMemoryPercentUsage = CrossProcess::totalPercentMemoryUsed();
    emit totalCPUUsageChanged();
    emit totalMemoryUsageChanged();
    emit totalPercentMemoryUsageChanged();
};

SystemMonitor::~SystemMonitor() {
    cleanupProcessRefs();
    metricsThread.stop();
    metricsThread.wait();
}

QQmlListProperty<ProcessMetrics> SystemMonitor::processes() {
    return {this, &m_processes};
}

double SystemMonitor::totalCPUUsage() const {
    return m_totalCPUUsage;
}

double SystemMonitor::totalMemoryUsage() const {
    return m_totalMemoryUsage;
}

double SystemMonitor::totalPercentMemoryUsage() const {
    return this->m_totalMemoryPercentUsage;
}

void SystemMonitor::cleanupProcessRefs() {
    for (auto &process: m_processes) {
        delete process;
    }
    m_processes.clear();
}

void SystemMonitor::sort()
{
    if (this->m_sortByCPU) {
        std::sort(m_processes.begin(), m_processes.end(), [](const ProcessMetrics *a, const ProcessMetrics *b) {
            return a->cpuPercentage() > b->cpuPercentage();
        });
    } else if (this->m_sortByRAM) {
        std::sort(m_processes.begin(), m_processes.end(), [](const ProcessMetrics *a, const ProcessMetrics *b) {
            return a->RAMUsage() > b->RAMUsage();
        });
    } else if (this->m_sortByName) {
        std::sort(m_processes.begin(), m_processes.end(), [](const ProcessMetrics *a, const ProcessMetrics *b) {
            return a->processName() < b->processName();
        });
    }
}


void SystemMonitor::start() {
    connect(&metricsThread, &MetricsThread::updateProcessInfo, this, &SystemMonitor::onUpdateProcessInfo);
    connect(&metricsThread, &MetricsThread::removeProcessInfo, this, &SystemMonitor::onRemoveProcessInfo);
    connect(&metricsThread, &MetricsThread::addProcessInfo, this, &SystemMonitor::onAddProcessInfo);
    connect(&metricsThread, &MetricsThread::finishedUpdateCycle, this, &SystemMonitor::onFinishedUpdateCycle);
    metricsThread.start();
}

void SystemMonitor::sortByRAM() {
    emit this->beforeProcessesChanged();
    this->m_sortByName = false;
    this->m_sortByCPU = false;
    this->m_sortByRAM = true;
    sort();
    emit this->processesChanged();
    emit this->afterProcessesChanged();
}

void SystemMonitor::sortByCPU() {
    emit this->beforeProcessesChanged();
    this->m_sortByName = false;
    this->m_sortByCPU = true;
    this->m_sortByRAM = false;
    sort();
    emit this->processesChanged();
    emit this->afterProcessesChanged();
}

void SystemMonitor::sortByName() {
    emit this->beforeProcessesChanged();
    this->m_sortByName = true;
    this->m_sortByCPU = false;
    this->m_sortByRAM = false;
    sort();
    emit this->processesChanged();
    emit this->afterProcessesChanged();
}

QString SystemMonitor::sortType() const {
    return this->m_sortType;
}

void SystemMonitor::sortType(const QString &sortType) {
    this->m_sortType = sortType;
    emit this->sortTypeChanged();
}

void SystemMonitor::onRemoveProcessInfo(const ProcessInfo &pi) {
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
}

void SystemMonitor::onUpdateProcessInfo(const ProcessInfo &pi) {
    // find the process in processList by pid, then update it
    emit beforeProcessesChanged();
    for (auto &process: m_processes) {
        if (process->pid() == pi.pid()) {
            process->setCPUPercentage(pi.cpu_percentage);
            process->setRAMUsage(pi.memory_percentage);
            process->setProcessName(pi.name);
            break;
        }
    }
    sort();
}

void SystemMonitor::onAddProcessInfo(const ProcessInfo &pi) {
    emit beforeProcessesChanged();
    auto pm = new ProcessMetrics();
    pm->setPid(pi.pid());
    pm->setProcessName(pi.name);
    pm->setCPUPercentage(pi.cpu_percentage);
    m_processes.append(pm);
    emit processesChanged();
    emit afterProcessesChanged();
}

void SystemMonitor::onFinishedUpdateCycle() {
    this->m_totalCPUUsage = CrossProcess::totalCPUUsage();
    this->m_totalMemoryUsage = CrossProcess::totalMemoryUsage();
    this->m_totalMemoryPercentUsage = CrossProcess::totalPercentMemoryUsed();
    emit totalCPUUsageChanged();
    emit totalMemoryUsageChanged();
    emit totalPercentMemoryUsageChanged();

    emit processesChanged();
    emit afterProcessesChanged();
}
