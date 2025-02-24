// // Created by glenn on 2/23/2025.
//

#ifndef METRICSTHREAD_H
#define METRICSTHREAD_H

#include <QThread>
#include "ProcessInfo.h"
#include "CrossProcess.h"
#include "ProcessMetrics.h"


class MetricsThread: public QThread {

    Q_OBJECT
public:
    MetricsThread(QObject *parent = nullptr);
    ~MetricsThread() override;
    void stop();
protected:
        void run() override;
private:
    QMap<QString, ProcessInfo> m_processInfoList = QMap<QString, ProcessInfo>();
    void updateProcessList();
    void cleanupProcessRefs();
    void getInitialProcessList();
    void updateAllUsages();
    std::atomic<bool> m_running = true;
signals:
        void updateProcessInfo(ProcessInfo pi);
        void removeProcessInfo(ProcessInfo pi);
        void addProcessInfo(ProcessInfo pi);
        void finishedUpdateCycle();
};



#endif //METRICSTHREAD_H
