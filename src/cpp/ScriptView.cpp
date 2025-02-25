//
// Created by glenn on 2/24/2025.
//

#include "ScriptView.h"
#include "ScriptRunner.h"
#include <QDesktopServices>

void ScriptView::loadScript(QUrl fileURL) {
    QString filePath = fileURL.toLocalFile();
    bool success = ScriptRunner::copyFileToDataPath(filePath);
    if (success) {
        qDebug() << "Script " << fileURL << " loaded successfully";
        this->refreshScripts();
    } else {
        qWarning() << "Failed to load script " << fileURL;
    }
}

void ScriptView::openScriptFolder() {
    ScriptRunner::verifyScriptPathExists();
    QDesktopServices::openUrl(QUrl::fromLocalFile(ScriptRunner::scriptPath()));
}

void ScriptView::refreshScripts() {
    auto files = ScriptRunner::getScripts();
    QStringList fileNames;
    for (auto file : files) {
        fileNames.append(file.fileName());
    }
    this->m_scripts = fileNames;
    emit scriptsChanged();
}

void ScriptView::runScript(QString fileName) {
    ScriptRunner::runPythonScript(fileName);
}

void ScriptView::removeScript(QString scriptName) {
    bool success = ScriptRunner::removeScript(scriptName);
    if (success) {
        qDebug() << "Script " << scriptName<< " removed successfully";
        this->refreshScripts();
    } else {
        qWarning() << "Failed to remove script " << scriptName;
    }
}

QStringList ScriptView::scripts() const {
    return m_scripts;
}
