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
            onClicked: stream.play()
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
            onClicked: stream.stop()
        }

        ToolButton {
            id: pause
            width: 640
            height: 40
            text: qsTr("▌▌")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: stream.pause()
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

                wState.setDialogState(0)
                recDialog.open()

                //projectManager.currentProject.checkNameAvailability(recName)
                //win = component.createObject(root)
                //win.con
                //win.show()

                //                //get name
                //                //projectManager.currentProject.recordSound

                //                //stream.record()
            }
        }
    }
}
