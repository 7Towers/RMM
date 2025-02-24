//
// Created by glenn on 2/23/2025.
//

#ifndef CROSSPROCESS_H
#define CROSSPROCESS_H


#include <QString>
#include <QList>
#include <QMap>

#include "ProcessInfo.h"

class CrossProcess {
public:
    static QList<QString> getProcessIDList();
    static QMap<QString, ProcessInfo> getProcessInfoList();
    static std::vector<double> getProcessCPUUsage(const QList<QString> &pids);
private:
#ifdef Q_OS_WIN
    static QList<QString> winGetProcessIDList();
    static QMap<QString, ProcessInfo> winGetProcessInfoList();
    static std::vector<double> winGetProcessCPUUsage(const QList<QString> &pids);
#elif Q_OS_MACOS
    static QList<QString> macGetProcessIDList();
    static QMap<QString, ProcessInfo> macGetProcessInfoList();
#endif
};

#endif //CROSSPROCESS_H
