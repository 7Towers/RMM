//
// Created by glenn on 2/24/2025.
//

#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H
#include <QQuickItem>


class ScriptView: public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QStringList scripts READ scripts NOTIFY scriptsChanged)
    Q_PROPERTY(bool runInSeparateWindow READ runInSeparateWindow WRITE setRunInSeparateWindow NOTIFY runInSeparateWindowChanged)
    Q_PROPERTY(QString successMessage READ successMessage NOTIFY successMessageChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
public:
    Q_INVOKABLE void loadScript(QUrl fileURL);
    Q_INVOKABLE void openScriptFolder();
    Q_INVOKABLE void refreshScripts();
    Q_INVOKABLE void runScript(QString filePath);
    Q_INVOKABLE void removeScript(QString scriptPath);
    Q_INVOKABLE bool isPythonInstalled();
    QStringList scripts() const;
    bool runInSeparateWindow() const;
    void setRunInSeparateWindow(bool runInSeparateWindow);
    QString successMessage() const;
    QString errorMessage() const;
private:
    QStringList m_scripts;
    QString m_successMessage;
    QString m_errorMessage;
    bool m_runInSeparateWindow = false;
signals:
    void scriptsChanged();
    void runInSeparateWindowChanged();
    void successMessageChanged();
    void errorMessageChanged();
};


#endif //SCRIPTVIEW_H
