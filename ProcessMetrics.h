//
// Created by Glenn Gartner on 2/13/25.
//

#ifndef PROCESSMETRICS_H
#define PROCESSMETRICS_H
#include <QQuickItem>


class ProcessMetrics: public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString processName READ processName WRITE setProcessName NOTIFY processNameChanged)
    Q_PROPERTY(double cpuUsage READ cpuUsage WRITE setCPUUsage NOTIFY cpuUsageChanged)
    Q_PROPERTY(double cpuPercentage READ cpuPercentage WRITE setCPUPercentage NOTIFY cpuPercentageChanged)
    Q_PROPERTY(double RAMUsage READ RAMUsage WRITE setRAMUsage NOTIFY RAMUsageChanged)
    Q_PROPERTY(double RAMPercentage READ RAMPercentage WRITE setRAMPercentage NOTIFY RAMPercentageChanged)
public:
    ProcessMetrics();
    QString processName() const;
    double cpuUsage() const;
    double cpuPercentage() const;
    double RAMUsage() const;
    double RAMPercentage() const;
    void setPid(pid_t pid);
    void setProcessName(QString processName);
    void setCPUUsage(double cpuUsage);
    void setCPUPercentage(double cpuPercentage);
    void setRAMUsage(double RAMUsage);
    void setRAMPercentage(double RAMPercentage);
private:
    QString m_processName = "";
    double m_cpuUsage = 0;
    double m_cpuPercentage = 0;
    double m_RAMUsage = 0;
    double m_RAMPercentage = 0;
    pid_t m_pid = 0;
signals:
    void processNameChanged(QString processName);
    void cpuUsageChanged(double cpuUsage);
    void cpuPercentageChanged(double cpuPercentage);
    void RAMUsageChanged(double RAMUsage);
    void RAMPercentageChanged(double RAMPercentage);
};



#endif //PROCESSMETRICS_H
