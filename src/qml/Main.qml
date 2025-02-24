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

        Item {
            height: 20
            width: parent.width
        }

        Item {
            width: parent.width - 20
            height: 25
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                text: "Total processes: " + monitor.processes.length
                font.bold: true
                font.pixelSize: 16
                color: "#333"
            }
        }

        Item {
            height: 20
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            Row {
                spacing: 20
                Label {
                    text: "<b>Total CPU Usage: </b>" + Math.round(((monitor.totalCPUUsage) + Number.EPSILON) * 100) / 100 + "%"
                    font.pixelSize: 16
                    color: "darkgreen"
                }
                Label {
                    text: "<b>Total RAM Usage: </b>" + Math.round(monitor.totalMemoryUsage) + " MB " + "(" + Math.round(monitor.totalMemoryPercentUsage) + "%" + ")"
                    font.pixelSize: 16
                    color: "darkblue"
                }
            }
        }
        Item {
            width: parent.width - 20
            height: 25
            anchors.horizontalCenter: parent.horizontalCenter
            Label {
                text: "Select a header button to sort"
                font.pixelSize: 12
                font.italic: true
                color: Material.Blue
            }
        }

        Item {
            width: parent.width - 20
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                Material.background: monitor.sortType === text ? Material.Blue : Material.Gray
                Material.elevation: 1
                anchors.left: parent.left
                text: "Process Name"
                font.pixelSize: 16
                font.bold: true
                onClicked: {
                    monitor.sortByName()
                    monitor.sortType = text;
                }
            }
            Button {
                Material.background: monitor.sortType === text ? Material.Blue : Material.Gray
                Material.elevation: 1
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Memory (MB)"
                font.pixelSize: 16
                font.bold: true
                onClicked: {
                    monitor.sortByRAM()
                    monitor.sortType = text;
                }
            }
            Button {
                Material.background: monitor.sortType === text ? Material.Blue : Material.Gray
                Material.elevation: 1
                anchors.right: parent.right
                text: "CPU %"
                font.pixelSize: 16
                font.bold: true
                onClicked: {
                    monitor.sortByCPU()
                    monitor.sortType = text;
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
            height: parent.height - 225
            model: monitor.processes

            spacing: 5
            clip: true
            delegate: ItemDelegate {
                width: listView.width
                anchors.horizontalCenter: parent.horizontalCenter
                height: 50
                Item {
                    anchors.fill: parent
                    anchors.margins: 20
                    Label {
                        anchors.left: parent.left
                        text: modelData.processName
                        width: 200
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Item {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 100
                        Label {
                            anchors.centerIn: parent
                            text: Math.round(modelData.RAMUsage) + " MB";
                        }
                    }
                    Item {
                        anchors.right: parent.right
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
