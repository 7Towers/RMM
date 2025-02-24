//
// Created by glenn on 2/23/2025.
//

#include "MetricsThread.h"

MetricsThread::MetricsThread(QObject *parent) {
}

MetricsThread::~MetricsThread() {
    this->cleanupProcessRefs();
}

void MetricsThread::stop() {
    this->m_running = false;
}

void MetricsThread::run() {
    this->getInitialProcessList();
    emit this->finishedUpdateCycle();
    while (m_running) {
        this->updateProcessList();
        emit this->finishedUpdateCycle();
        sleep(1);
    }
}

void MetricsThread::updateProcessList()
{
    // get a list of current processes
    auto newProcessInfoList = CrossProcess::getProcessInfoList();
    // when you loop through, see if process info has changed compared to this->m_processInfoList
    for (auto& processInfo : newProcessInfoList) {
        if (this->m_processInfoList.contains(processInfo.pid())) {
            auto oldProcessInfo = this->m_processInfoList[processInfo.pid()];
            if (oldProcessInfo.hasCPUChanged()) {
                emit updateProcessInfo(oldProcessInfo);
            }
            if (oldProcessInfo.memory_percentage != processInfo.memory_percentage) {
                emit updateProcessInfo(processInfo);
            }
            if (oldProcessInfo.name != processInfo.name) {
                emit updateProcessInfo(processInfo);
            }
        } else {
            this->m_processInfoList[processInfo.pid()] = processInfo;

            if (!processInfo.pid().isEmpty()) {
                processInfo.initCPUUsageMonitor();
            }
            emit addProcessInfo(processInfo);
        }
    }
    // if a process is no longer in the list, emit removeProcessInfo
    for (auto it = this->m_processInfoList.begin(); it != this->m_processInfoList.end(); it++) {
        auto processInfo = it.value();
        if (!newProcessInfoList.contains(processInfo.pid())) {
            emit removeProcessInfo(processInfo);
            it = this->m_processInfoList.erase(it);
            qDebug() << "Removed process info with pid: " << processInfo.pid();
        }
    }
    this->updateAllUsages();
}

void MetricsThread::cleanupProcessRefs() {
    this->m_processInfoList.clear();
}

void MetricsThread::getInitialProcessList() {
    auto processInfoList = CrossProcess::getProcessInfoList();
    this->m_processInfoList = processInfoList;
    for (auto& processInfo : this->m_processInfoList) {
        auto process = new ProcessMetrics();
        process->setPid(processInfo.pid());
        process->setProcessName(processInfo.name);
        process->setCPUPercentage(processInfo.cpu_percentage);
        emit addProcessInfo(processInfo);
    }
    this->updateAllUsages();
}

void MetricsThread::updateAllUsages() {
    QList<QString> pids = this->m_processInfoList.keys();
    auto cpuUsages = CrossProcess::getProcessCPUUsage(pids);
    auto memoryUsages = CrossProcess::getProcessMemoryUsage(pids);
    for (int i = 0; i < pids.size(); i++) {
        auto processInfo = this->m_processInfoList[pids[i]];
        processInfo.cpu_percentage = cpuUsages[i];
        processInfo.memory_percentage = memoryUsages[i];
        emit updateProcessInfo(processInfo);
        msleep(10);
    }
}

