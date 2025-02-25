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
public:
    Q_INVOKABLE void loadScript(QUrl fileURL);
    Q_INVOKABLE void openScriptFolder();
    Q_INVOKABLE void refreshScripts();
    Q_INVOKABLE void runScript(QString filePath);
    Q_INVOKABLE void removeScript(QString scriptPath);
    QStringList scripts() const;
private:
    QStringList m_scripts;
signals:
    void scriptsChanged();
};



#endif //SCRIPTVIEW_H
