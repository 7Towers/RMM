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
    Q_PROPERTY(double cpuPercentage READ cpuPercentage WRITE setCPUPercentage NOTIFY cpuPercentageChanged)
    Q_PROPERTY(double RAMUsage READ RAMUsage WRITE setRAMUsage NOTIFY RAMUsageChanged)
public:
    ProcessMetrics();
    QString processName() const;
    QString pid() const;
    double cpuPercentage() const;
    double RAMUsage() const;
    void setPid(QString pid);
    void setProcessName(QString processName);
    void setCPUPercentage(double cpuPercentage);
    void setRAMUsage(double RAMUsage);
private:
    QString m_processName = "";
    double m_cpuPercentage = 0;
    double m_RAMUsage = 0;
    QString m_pid = 0;
signals:
    void processNameChanged(QString processName);
    void cpuPercentageChanged(double cpuPercentage);
    void RAMUsageChanged(double RAMUsage);
};



#endif //PROCESSMETRICS_H
