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
    Q_PROPERTY(QString sortType READ sortType WRITE sortType NOTIFY sortTypeChanged)
public:
    SystemMonitor();
    ~SystemMonitor() override;
    QQmlListProperty<ProcessMetrics> processes();
    Q_INVOKABLE void start();
    Q_INVOKABLE void sortByRAM();
    Q_INVOKABLE void sortByCPU();
    Q_INVOKABLE void sortByName();

    QString sortType() const;
    void sortType(const QString &sortType);


public slots:
    void onUpdateProcessInfo(const ProcessInfo& pi);
    void onRemoveProcessInfo(const ProcessInfo& pi);
    void onAddProcessInfo(const ProcessInfo& pi);
    void onFinishedUpdateCycle();
private:
    void cleanupProcessRefs();
    QList<ProcessMetrics*> m_processes = QList<ProcessMetrics*>();
    MetricsThread metricsThread = MetricsThread(this);
    void sort();
    bool m_sortByRAM = false;
    bool m_sortByCPU = false;
    bool m_sortByName = false;
    QString m_sortType = "";

signals:
    void beforeProcessesChanged();
    void processesChanged();
    void afterProcessesChanged();
    void sortTypeChanged();
};

#endif // SYSTEMMONITOR_H
