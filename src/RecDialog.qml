import QtQuick 2.10
import QtQuick.Controls 2.3
import io.qt.examples.WindowState 1.0

Dialog {
    id: dialog
    title: qsTr("Tonaufnahme")
    width: 300
    height: 150
    clip: true
    visible: false
    modal: true
    dim: true

    closePolicy: Popup.NoAutoClose

    Dialog {
        id: notAvailable
        title: qsTr("Name '"+ inputRecName.text +"' ist nicht verfügbar")
        width: 300
        height: 150
        clip: true
        visible: false
        modal: true
        dim: true

        standardButtons: Dialog.Ok
        closePolicy: Popup.NoAutoClose
    }

    QtObject
    {
        id: internal
        property bool recording: false
    }


    contentItem: Rectangle {
        id: content
        anchors.fill: parent

        Rectangle {
            id: recName
            width: 128
            height: 20
            color: inputRecName.cursorVisible ? "#dddddd" : "#ffffff"
            radius: 1
            anchors.left: recNameLabel.right
            anchors.leftMargin: 0
            anchors.verticalCenterOffset: -20
            anchors.verticalCenter: parent.verticalCenter
            border.width: inputRecName.cursorVisible ? 2 : 1
            border.color: "#add8e6"
            implicitWidth: 120
            implicitHeight: 40

            TextInput {
                id: inputRecName
                text: qsTr("")
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                font.pixelSize: 12
                anchors.fill: parent
            }
        }
        TSButton {
            id: rec

            width: 68
            height: 50
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.verticalCenterOffset: 25
            anchors.verticalCenter: parent.verticalCenter
            bP: 10
            tP: 0
            rP: 0
            lP: 0
            fontSize: 50
            fontColor: "Red"
            buttonText: "\u25cf"
            MouseArea {
                id: recArea
                anchors.fill: parent
                onClicked:
                {
                    if(!internal.recording )
                    {
                    if(projectManager.currentProject.checkNameAvailability(inputRecName.text))
                    {
                        internal.recording = true
                        inputRecName.readOnly = true
                        projectManager.currentProject.recordSound(inputRecName.text)
                    }
                    else
                    {
                       notAvailable.open()
                    }
                    }
                    else
                    {

                    }
                }
            }
        }

        TSButton {
            id: stop
            width: 68
            height: 50
            anchors.horizontalCenterOffset: -10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: 25
            anchors.verticalCenter: parent.verticalCenter
            bP: 10
            fontColor: "black"
            buttonText: "\u25a0"
            fontSize: 40
            MouseArea {
                id: stopArea
                anchors.fill: parent
                onClicked: {
                    if(internal.recording)
                    {
                        internal.recording = false
                        inputRecName.readOnly = false
                        projectManager.currentProject.finishRecording(inputRecName.text)
                        dialog.accept()
                    }
                }
            }
        }

        TSButton {
            id: cancel
            width: 83
            height: 50
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.verticalCenterOffset: 25
            anchors.verticalCenter: parent.verticalCenter
            fontColor: "black"
            bP: 0
            buttonText: "Abbruch"
            fontSize: 15

            signal buttonClick
            onButtonClick: {

            }
            MouseArea {
                id: cancelArea
                anchors.fill: parent
                onClicked: {
                    if(internal.recording)
                    {
                        internal.recording = false
                        inputRecName.readOnly = false
                        projectManager.currentProject.abortRecording()
                    }
                    else
                    {
                        dialog.reject()
                    }
                }
            }
        }

        Label {
            id: recNameLabel
            text: qsTr("Tonspurbezeichner:")
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.verticalCenterOffset: -20
            anchors.verticalCenter: parent.verticalCenter

        }
    }
}
