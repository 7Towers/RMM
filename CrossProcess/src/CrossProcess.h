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
    static QList<QMap<QString, ProcessInfo>> getProcessInfoList();
private:
#ifdef Q_OS_WIN
    static QList<QString> winGetProcessIDList();
    static QList<QMap<QString, ProcessInfo>> winGetProcessInfoList();
#elif Q_OS_MACOS
    static QList<QString> macGetProcessIDList();
    static QList<QMap<QString, ProcessInfo> macGetProcessInfoList();
#endif
};

#endif //CROSSPROCESS_H
