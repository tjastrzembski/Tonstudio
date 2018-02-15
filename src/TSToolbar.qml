import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ToolBar {
    id: toolBar

    RecDialog {
        id: recDialog
    }
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        spacing: 1

        ToolButton {
            id: play
            text: qsTr("►")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            font.pointSize: 13
            onClicked: {
                projectManager.currentProject.playSound()
            }
        }
        ToolButton {
            id: stop
            width: 640
            height: 40
            text: qsTr("■")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            bottomPadding: 7
            topPadding: 0
            font.wordSpacing: 0
            Layout.fillHeight: false
            Layout.fillWidth: false
            font.pointSize: 25
            onClicked: {
                projectManager.currentProject.stopSound()
            }
        }

        ToolButton {
            id: pause
            width: 640
            height: 40
            text: qsTr("▌▌")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: {
                projectManager.currentProject.pauseSound()
            }
        }

        ToolButton {
            id: record

            width: 640
            height: 40
            text: qsTr("●")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            topPadding: 0
            font.pointSize: 25
            onClicked: {
                recDialog.open()
            }
        }
    }
}
