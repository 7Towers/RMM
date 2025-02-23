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
    while (m_running) {
        qDebug() << "Doing work";
        sleep(1);
    }
}

void MetricsThread::updateProcessList()
{
    // get a list of current processes
    auto processInfoList = CrossProcess::getProcessInfoList();
    // when you loop through, see if process info has changed compared to this->m_proce
}

void MetricsThread::cleanupProcessRefs() {
    this->m_processInfoList.clear();
}

void MetricsThread::getInitialProcessList() {
    auto processInfoList = CrossProcess::getProcessInfoList();
    this->m_processInfoList = processInfoList;
    for (auto& processInfo : this->m_processInfoList) {
        auto process = new ProcessMetrics();
        process->setPid(processInfo.pid);
        process->setProcessName(processInfo.name);
        process->setCPUPercentage(processInfo.cpu_percentage);
        emit addProcessInfo(processInfo);
    }
}
