import QtQuick
import QtQuick.Controls.Material
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
        monitor.start()
    }

    Column {
        anchors.fill: parent
        width: parent.width
        height: parent.height
        spacing: 10

        RowLayout {
            Label {
                Layout.alignment: Qt.AlignLeft
                text: "Total processes: " + monitor.processes.length
            }
        }

        RowLayout {
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            Label {
                text: "Process Name"
                font.pixelSize: 16
                font.bold: true
            }
            Label {
                text: "CPU Usage"
                font.pixelSize: 16
                font.bold: true
                Layout.alignment: Qt.AlignRight
            }
            Label {
                text: "CPU %"
                font.pixelSize: 16
                font.bold: true
                Layout.alignment: Qt.AlignRight
            }
        }
        ListView {
            id: listView
            property real scrollPosition: 0
            function savePosition(){
                scrollPosition = contentY;
            }

            Connections {
                target: monitor
                function onBeforeProcessesChanged() {
                    console.log("QML Saving scroll position");
                    listView.savePosition()
                }
                function onAfterProcessesChanged() {
                    console.log("QML resetting scroll position");
                   listView.contentY = listView.scrollPosition
                }
            }

            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            height: parent.height - 50
            model: monitor.processes
            spacing: 5
            clip: true
            delegate: ItemDelegate {
                width: listView.width - 20
                anchors.horizontalCenter: parent.horizontalCenter
                height: 50
                RowLayout {
                    anchors.fill: parent
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
