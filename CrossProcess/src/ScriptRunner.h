//
// Created by glenn on 2/24/2025.
//

#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include <QString>
#include <QProcess>
#include <QFileInfo>
#include <QStandardPaths>

const QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/RMM/scripts/";

class ScriptRunner {
public:
    static bool pythonInstalled();
    static bool runPythonScript(const QString &scriptName);
    static bool copyFileToDataPath(const QString &sourcePath);
    static QString scriptPath();
    static bool verifyScriptPathExists();
    static QList<QUrl> getScripts();
    static bool removeScript(const QString &scriptName);
};



#endif //SCRIPTRUNNER_H
