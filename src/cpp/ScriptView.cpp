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
    for (auto file: files) {
        fileNames.append(file.fileName());
    }
    this->m_scripts = fileNames;
    emit scriptsChanged();
}

void ScriptView::runScript(QString fileName) {
    bool success = false;
    success = ScriptRunner::pythonInstalled();
    if (!success) {
        this->m_successMessage = "";
        this->m_errorMessage = "Python is not installed or not found in PATH";
        emit this->successMessageChanged();
        emit this->errorMessageChanged();
        return;
    }
    if (this->m_runInSeparateWindow) {
        success = ScriptRunner::runPythonScriptInTerminal(fileName);
        if (success) {
            this->m_successMessage = "Script launched in Terminal window";
            this->m_errorMessage = "";
        } else {
            this->m_successMessage = "";
            this->m_errorMessage = "Failed to open script in Terminal window";
        }
    } else {
        success = ScriptRunner::runPythonScript(fileName);
        if (success) {
            this->m_successMessage = "Script ran successfully";
            this->m_errorMessage = "";
        } else {
            this->m_successMessage = "";
            this->m_errorMessage = "Failed to run script";
        }
    }
    emit this->successMessageChanged();
    emit this->errorMessageChanged();
}

void ScriptView::removeScript(QString scriptName) {
    bool success = ScriptRunner::removeScript(scriptName);
    if (success) {
        qDebug() << "Script " << scriptName << " removed successfully";
        this->refreshScripts();
    } else {
        qWarning() << "Failed to remove script " << scriptName;
    }
}

bool ScriptView::isPythonInstalled() {
    return ScriptRunner::pythonInstalled();
}

QStringList ScriptView::scripts() const {
    return m_scripts;
}

bool ScriptView::runInSeparateWindow() const {
    return this->m_runInSeparateWindow;
}

void ScriptView::setRunInSeparateWindow(bool runInSeparateWindow) {
    this->m_runInSeparateWindow = runInSeparateWindow;
}

QString ScriptView::successMessage() const {
    return this->m_successMessage;
}

QString ScriptView::errorMessage() const {
    return this->m_errorMessage;
}
