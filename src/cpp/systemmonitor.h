#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QQuickItem>
#include <thread>
#include <unistd.h>
#include <QThread>
#include "ProcessMetrics.h"
#include "CrossProcess.h"
#include "ProcessInfo.h"
#include "MetricsThread.h"


class SystemMonitor : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QQmlListProperty<ProcessMetrics> processes READ processes NOTIFY processesChanged)
public:
    SystemMonitor();
    ~SystemMonitor() override;
    QQmlListProperty<ProcessMetrics> processes();
    Q_INVOKABLE void start();

public slots:
    void onUpdateProcessInfo(const ProcessInfo& pi);
    void onRemoveProcessInfo(const ProcessInfo& pi);
    void onAddProcessInfo(const ProcessInfo& pi);
private:
    void cleanupProcessRefs();
    QList<ProcessMetrics*> m_processes = QList<ProcessMetrics*>();
    MetricsThread metricsThread = MetricsThread(this);

signals:
    void beforeProcessesChanged();
    void processesChanged();
    void afterProcessesChanged();
};

#endif // SYSTEMMONITOR_H
