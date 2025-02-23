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
    this->processInfoList.clear();
}

void MetricsThread::cleanupProcessRefs() {
    this->processInfoList.clear();
}

void MetricsThread::getInitialProcessList() {
    auto processInfoList = CrossProcess::getProcessInfoList();
    for (auto& processInfo : processInfoList) {
        auto process = new ProcessMetrics();
        process->setPid(processInfo["PID"].pid);
        process->setProcessName(processInfo["PID"].name);
        process->setCPUPercentage(processInfo["PID"].cpu_percentage);
        emit addProcessInfo(processInfo["PID"]);
    }
}
