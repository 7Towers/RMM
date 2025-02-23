//
// Created by glenn on 2/23/2025.
//

#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QString>

struct ProcessInfo {
    QString pid = "";
    QString name = "";
    double cpu_percentage = 0.0;
    double memory_percentage = 0.0;
};



#endif //PROCESSINFO_H
