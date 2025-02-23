import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RMM

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("RMM")

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
                RowLayout {
                    width: parent.width - 20
                    anchors.centerIn: parent
                    Label {
                        Layout.alignment: Qt.AlignLeft
                        text: modelData.processName
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: modelData.cpuUsage
                        width: 50
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: modelData.cpuPercentage
                        width: 50
                    }
                }
            }
        }
    }
}
