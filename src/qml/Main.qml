import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
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

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainPage
    }

    property bool showToast: false
    property string toastMessage: ""
    property string toastTextColor: "lightgreen"

    Rectangle {
        id: customToast
        width: parent.width
        height: 50
        color: "black"
        opacity: 0.8
        anchors.horizontalCenter: parent.horizontalCenter
        y: -height
        Text {
            anchors.centerIn: parent
            color: toastTextColor
            text: toastMessage
            font.pixelSize: 18
            font.bold: true
        }

        SequentialAnimation on y {
            running: showToast
            PropertyAnimation { to: 0; duration: 300 }
            PauseAnimation { duration: 3000 }
            PropertyAnimation { to: -customToast.height; duration: 300 }
            onStopped: showToast = false
        }
    }

    Component {
        id: mainPage
        Page {
            title: "Main Page"
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
                                    text: Math.round(((modelData.cpuPercentage * .1) + Number.EPSILON) * 100) / 100 + "%";
                                }
                            }
                        }
                    }
                }

            }
        }
    }
    ScriptView{
        id: scriptView
        Component.onCompleted: {
            scriptView.refreshScripts()
        }
    }

    Connections {
        target: scriptView
        function onErrorMessageChanged() {
            if (scriptView.errorMessage !== "") {
                toastTextColor = "red"
                showToast = true
                toastMessage = scriptView.errorMessage
            }
        }

        function onSuccessMessageChanged() {
            if (scriptView.successMessage !== "") {
                toastTextColor = "lightgreen"
                showToast = true
                toastMessage = scriptView.successMessage
            }
        }
    }
    Component {
        id: scriptPage
        Page {
            FileDialog {
                id: fileDialog
                nameFilters: ["Python files (*.py)"]
                onAccepted: {
                    console.log("File selected: ", selectedFile)
                    scriptView.loadScript(selectedFile)
                }
            }
            Column {
                anchors.fill: parent
                spacing: 10
                Item {
                    height: 15
                    width: parent.width
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Run Python Scripts"
                    font.pixelSize: 20
                    font.bold: true
                }
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - 20
                    height: 1
                    color: Material.Blue
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: scriptView.isPythonInstalled() ? "" : "Python is not installed on your system. Please install Python to run scripts."
                    font.pixelSize: 16
                    color: "red"
                }

                Row {
                    width: parent.width - 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    Button {
                        Material.background: Material.Blue
                        Material.elevation: 1
                        text: "Load Script"
                        font.pixelSize: 16
                        font.bold: true
                        onClicked: {
                            // open a file dialog to select a script, only .py type
                            fileDialog.open()
                        }
                    }
                    Button {
                        Material.background: Material.Blue
                        Material.elevation: 1
                        text: "Open Script Folder"
                        font.pixelSize: 16
                        font.bold: true
                        onClicked: {
                            scriptView.openScriptFolder()
                        }
                    }
                }

                CheckBox {
                    id: checkBox
                    text: "Run In Separate Windows"
                    checked: scriptView.runInSeparateWindow
                    onCheckedChanged: {
                        scriptView.runInSeparateWindow = checked
                    }
                }

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    text: scriptView.scripts.length === 0 ? "No scripts loaded. Load them with the <i>Load Script</i> button above" : "Available Scripts"
                    font.bold: true
                    font.pixelSize: 18
                    color: "#888"
                }


                Repeater {
                    model: scriptView.scripts
                    delegate: ItemDelegate {
                        width: parent.width - 20
                        height: 50
                        anchors.horizontalCenter: parent.horizontalCenter
                        Row {
                            anchors.fill: parent
                            width: parent.width - 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 20
                            Label {
                                text: "\u2022 " + modelData
                                anchors.verticalCenter: parent.verticalCenter
                                width: 200
                                font.pixelSize: 16
                                font.bold: true
                            }
                            Button {
                                text: "Run"
                                onClicked: {
                                    scriptView.runScript(modelData)
                                }
                            }
                            Button {
                                text: "Remove"
                                onClicked: {
                                    scriptView.removeScript(modelData)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TabBar {
        id: tabBar
        width: parent.width
        height: 50
        anchors.bottom: parent.bottom
        currentIndex: 0
        TabButton {
            text: "Processes"
            onClicked: stackView.push(mainPage)
        }
        TabButton {
            text: "Script Runner"
            onClicked: stackView.push(scriptPage)
        }
    }
}
