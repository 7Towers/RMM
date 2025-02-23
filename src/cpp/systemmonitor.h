#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QQuickItem>
#include <thread>
#include <unistd.h>
#include <QThread>
#include "ProcessMetrics.h"
#include "CrossProcess.h"
#include "ProcessInfo.h"

class MetricsThread: public QThread
{
    Q_OBJECT
public:
    bool running = true;
    public slots:
        void run() override {
        while (running) {
            // emit UpdatedProcess(ProcessMetrics());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    signals:
        void UpdatedProcess(QList<ProcessMetrics> pm);
};

class SystemMonitor : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QQmlListProperty<ProcessMetrics> processes READ processes NOTIFY processesChanged)
public:
    SystemMonitor();
    ~SystemMonitor();
    QQmlListProperty<ProcessMetrics> processes();
    Q_INVOKABLE void updateProcessList();

public slots:
    // void processUpdated(ProcessMetrics pm);
private:
    void cleanupProcessRefs();
    QList<ProcessMetrics*> m_processes = QList<ProcessMetrics*>();
    MetricsThread metricsThread = MetricsThread();

signals:
    void processesChanged();
};

#endif // SYSTEMMONITOR_H
