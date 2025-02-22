import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RMM

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    SystemMonitor {
        id: monitor
    }

    Component.onCompleted: {
        monitor.updateProcessList()
    }

    ListView {
        id: listView
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        height: parent.height
        model: monitor.processes
        spacing: 5
        delegate: Item {
            width: listView.width
            height: 50
            Rectangle {
                width: parent.width - 20
                anchors.centerIn: parent
                height: parent.height
                radius: 5
                border.color: "black"
                color: "#444444"
                RowLayout {
                    width: parent.width - 20
                    anchors.centerIn: parent
                    Label {
                        text: modelData.processName
                        font.bold: true
                    }
                    Label {
                        text: modelData.cpuUsage
                    }
                    Label {
                        text: modelData.cpuPercentage
                    }
                }
            }
        }
    }
}
