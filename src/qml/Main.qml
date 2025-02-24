import QtQuick
import QtQuick.Controls
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
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                Layout.alignment: Qt.AlignLeft
                text: "Total processes: " + monitor.processes.length
                font.bold: true
                color: "#333"
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
            Item {
                Layout.alignment: Qt.AlignCenter
                width: 100
                Label {
                    anchors.centerIn: parent
                    text: "Memory (MB)"
                    font.pixelSize: 16
                    font.bold: true
                }
            }
            Item {
                Layout.alignment: Qt.AlignRight
                width: 100
                Label {
                    anchors.centerIn: parent
                    text: "CPU %"
                    width: 100
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
        Rectangle {
            height: 1
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            color: '#333'
        }

        ListView {
            id: listView
            property real scrollPosition: 0

            function savePosition() {
                scrollPosition = contentY;
            }

            Connections {
                target: monitor

                function onBeforeProcessesChanged() {
                    listView.savePosition()
                }

                function onAfterProcessesChanged() {
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
                width: listView.width
                anchors.horizontalCenter: parent.horizontalCenter
                height: 50
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    Label {
                        Layout.alignment: Qt.AlignLeft
                        text: modelData.processName
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Item {
                        Layout.alignment: Qt.AlignCenter
                        width: 100
                        Label {
                            anchors.centerIn: parent
                            text: Math.round(modelData.RAMPercentage) + " MB";
                        }
                    }
                    Item {
                        Layout.alignment: Qt.AlignRight
                        width: 100
                        Label {
                            anchors.centerIn: parent
                            text: Math.round((modelData.cpuPercentage + Number.EPSILON) * 100) / 100 + "%";
                        }
                    }
                }
            }
        }
    }
}
