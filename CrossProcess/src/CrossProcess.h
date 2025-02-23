//
// Created by glenn on 2/23/2025.
//

#ifndef CROSSPROCESS_H
#define CROSSPROCESS_H


#include <QString>
#include <QList>
#include <QMap>

class CrossProcess {
public:
    static QList<QString> getProcessIDList();
    static QList<QMap<QString, QString>> getProcessInfoList();
private:
#ifdef Q_OS_WIN
    static QList<QString> winGetProcessIDList();
    static QList<QMap<QString, QString>> winGetProcessInfoList();
#elif Q_OS_MACOS
    static QList<QString> macGetProcessIDList();
    static QList<QMap<QString, QString>> macGetProcessInfoList();
#endif
};

#endif //CROSSPROCESS_H
