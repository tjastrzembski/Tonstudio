import QtQuick 2.10
import io.qt.examples.SoundComponentGraphic 1.0

Item {
    id: scRoot
    height: 200
    width: height + scb.width
    property string name: ""

    Rectangle {
        id: scbBorder
        color: "grey"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        border.width: 1
        border.color: "black"

        Rectangle {
            id: scbID
            width: scRoot.height
            color: "grey"
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            border.width: 1
            border.color: "black"

            Rectangle {
                id: scbDelete
                color: parent.color
                width: 15
                height: 15
                border.width: 1
                border.color: "black"
                Text {
                    text: qsTr("x")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "black"
                }

                //TODO: implement delete
                //TODO: implement mute? -> bool bPlayStream
            }
            Rectangle {
                id: scbName
                color: parent.color
                height: 15
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: scbDelete.right
                anchors.leftMargin: 5
                border.width: 1
                border.color: "black"
                Text {
                    text: qsTr(scRoot.name)
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "black"
                }
            }
        }
        Rectangle {
            id: scbGraphic
            color: "grey"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: scbID.right
            anchors.leftMargin: 0
            border.width: 1
            border.color: "black"
            width: scb.width
            SoundComponentGraphic {
                id: scb
                //it needs to be initialized
                width: 13000
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
            }
        }
    }
}
