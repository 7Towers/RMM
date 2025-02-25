//
// Created by glenn on 2/24/2025.
//

#include "ScriptRunner.h"

#include <QDir>
#include <qurl.h>

bool ScriptRunner::pythonInstalled() {
    QProcess checkPython;
    checkPython.start("python", QStringList() << "--version");
    checkPython.waitForFinished();
    if (checkPython.exitStatus() != QProcess::NormalExit) {
        qWarning() << "Python is not installed or not found in PATH.";
        return false;
    }
#ifdef Q_OS_WIN
    // with windows, the console will not print an error, but something like "Python was not found". Handle that as an error
    QString output = checkPython.readAllStandardOutput();
    if (output.contains("not found") || output == "") {
        qWarning() << "Python is not installed or not found in PATH.";
        return false;
    }
#endif
    return true;
}

bool ScriptRunner::verifyScriptPathExists() {
    if (!QFileInfo(dataPath).isDir()) {
        QDir dir;
        auto pathMade = dir.mkpath(dataPath);
        if (!pathMade) {
            qDebug() << "Failed to create data directory:" << dataPath;
            return false;
        }
    }
    return true;
}

QList<QUrl> ScriptRunner::getScripts() {
    QDir dir(dataPath);
    QStringList filters;
    filters << "*.py";
    dir.setNameFilters(filters);
    auto files = dir.entryList();
    QList<QUrl> urls;
    for (auto file : files) {
        urls.append(QUrl::fromLocalFile(dataPath + file));
    }
    return urls;
}

bool ScriptRunner::removeScript(const QString &scriptName) {
    // delete the script from the data path
    QString scriptPath = dataPath + scriptName;
    QFileInfo checkFile(scriptPath);
    if (!verifyScriptPathExists()) {
        return false;
    }
    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << "Script file does not exist:" << scriptPath;
        return false;
    }
    if (!QFile::remove(scriptPath)) {
        qDebug() << "Failed to remove script:" << scriptPath;
        return false;
    }
    return true;
}

bool ScriptRunner::runPythonScript(const QString &scriptName) {
    if (!pythonInstalled()) {
        return false;
    }

    QString scriptPath = dataPath + scriptName;
    QFileInfo checkFile(scriptPath);
    if (!verifyScriptPathExists()) {
        return false;
    }
    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << "Script file does not exist:" << scriptPath;
        return false;
    }

    // Run the Python script
    QProcess process;
    process.start("python", QStringList() << scriptPath);
    process.waitForFinished();

    if (process.exitStatus() != QProcess::NormalExit) {
        qDebug() << "Failed to run the script:" << scriptPath;
        return false;
    }

    // Print the output of the script
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();
    qDebug() << "Script output:" << output;
    if (!errorOutput.isEmpty()) {
        qDebug() << "Script error output:" << errorOutput;
        return false;
    }

    qDebug() << "Script ran successfully:" << scriptPath;
    return true;
}

bool ScriptRunner::runPythonScriptInTerminal(const QString &scriptName) {
    if (!pythonInstalled()) {
        return false;
    }

    QString scriptPath = dataPath + scriptName;
    QFileInfo checkFile(scriptPath);
    if (!verifyScriptPathExists()) {
        return false;
    }
    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << "Script file does not exist:" << scriptPath;
        return false;
    }
#ifdef Q_OS_WIN
    // Run the Python script in a new terminal window
    QStringList arguments;
    arguments << "/C" << "start" << "cmd.exe" << "/K" << "python" << scriptPath;

    QProcess process;
    process.startDetached("cmd.exe", arguments);
#elif
    qWarning() << "runPythonScriptInTerminal() not implemented for this platform";
#endif

    qDebug() << "Script is running in a new terminal window:" << scriptPath;
    return true;
}

bool ScriptRunner::copyFileToDataPath(const QString &sourcePath) {
    // copy the given file at the fileName to the data path
    QString fileName = QFileInfo(sourcePath).fileName();
    QString destPath = scriptPath() + fileName;
    if (!verifyScriptPathExists()) {
        return false;
    }
    if (QFile::exists(destPath)) {
        if (!QFile::remove(destPath)) {
            qDebug() << "Failed to remove existing file:" << destPath;
            return false;
        }
    }
    if (!QFile::copy(sourcePath, destPath)) {
        qDebug() << "Failed to copy file:" << fileName << "to:" << destPath;
        return false;
    }
    return true;
}

QString ScriptRunner::scriptPath() {
    return dataPath;
}

