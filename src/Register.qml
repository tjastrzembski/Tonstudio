import QtQuick 2.10
import QtQuick.Window 2.3
import QtQuick.Controls 2.3
import QtQml.Models 2.3

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Tonstudio Registration")

    Text {
        id: labelUser
        y: 170
        width: 39
        height: 17
        text: qsTr("Nutzer:")
        anchors.left: image.left
        anchors.leftMargin: 0
        anchors.verticalCenterOffset: 2
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
    }

    Image {
        id: image
        x: 280
        y: 62
        width: 200
        height: 100
        anchors.bottom: boxUser.top
        anchors.bottomMargin: 5
        clip: false
        sourceSize.height: 623
        sourceSize.width: 1256
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.Stretch
        source: "Logo.png"
    }

    Text {
        id: labelPasswort
        y: 198
        width: 39
        height: 17
        text: qsTr("Passwort:")
        anchors.left: image.left
        anchors.leftMargin: 0
        anchors.verticalCenterOffset: 25
        anchors.verticalCenter: labelUser.verticalCenter
        font.pixelSize: 12
    }

    Text {
        id: labelStatus
        x: 309
        y: 359
        color: "#a61414"
        text: qsTr("")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 30
        anchors.verticalCenter: buttonRegister.verticalCenter
        font.pixelSize: 12
    }

    TSButton {
        id: buttonRegister
        x: 220
        y: 401
        width: 200
        height: 34
        anchors.verticalCenter: checkBox.verticalCenter
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenterOffset: 45
        anchors.horizontalCenter: parent.horizontalCenter

        buttonText: qsTr("Registrieren")
    }

    Text {
        id: labelPasswortValidate
        x: -4
        y: 194
        width: 39
        height: 17
        text: qsTr("Passwort  bestätigen:")
        wrapMode: Text.WordWrap
        anchors.verticalCenterOffset: 25
        anchors.leftMargin: 0
        font.pixelSize: 12
        anchors.verticalCenter: labelPasswort.verticalCenter
        anchors.left: image.left
    }
    Rectangle {
        id: boxUser
        y: 231
        height: 20
        color: inputUser.cursorVisible ? "#dddddd" : "#ffffff"
        implicitWidth: 120
        implicitHeight: 40
        border.color: "lightblue"
        border.width: inputUser.cursorVisible ? 2 : 1
        radius: 1
        anchors.left: boxPW.left
        anchors.leftMargin: 0
        anchors.right: image.right
        anchors.rightMargin: 0
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: labelUser.verticalCenter

        TextInput {
            id: inputUser
            text: qsTr("")
            anchors.fill: parent
            echoMode: TextInput.Normal
            font.pixelSize: 12
        }
    }

    CheckBox {
        id: checkBox
        y: 355
        height: 40
        text: qsTr("Ich habe diesen Satz gelesen.")
        anchors.left: comboBox.left
        anchors.leftMargin: 0
        anchors.right: comboBox.right
        anchors.rightMargin: 0
        anchors.verticalCenterOffset: 45
        anchors.verticalCenter: comboBox.verticalCenter
        checked: false
    }

    ComboBox {
        id: comboBox
        y: 310
        height: 40
        anchors.right: boxPWValidate.right
        anchors.rightMargin: 0
        anchors.left: labelPasswortValidate.left
        anchors.leftMargin: 0
        anchors.verticalCenterOffset: 40
        anchors.verticalCenter: boxPWValidate.verticalCenter
        textRole: "key"

        model: ListModel {
            ListElement {
                // @disable-check M16
                key: "Admin"
                // @disable-check M16
                value: 0
            }
            ListElement {
                // @disable-check M16
                key: "Composer"
                // @disable-check M16
                value: 1
            }
            ListElement {
                // @disable-check M16
                key: "Producer"
                // @disable-check M16
                value: 2
            }
        }
        background: Rectangle {
            implicitWidth: 120
            implicitHeight: 40
            border.color: comboBox.pressed ? "#17a81a" : "lightblue"
            border.width: comboBox.visualFocus ? 2 : 1
            radius: 2
        }

        indicator: Canvas {
            id: canvas
            x: comboBox.width - width - comboBox.rightPadding
            y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
            width: 12
            height: 8
            contextType: "2d"

            Connections {
                target: comboBox
                onPressedChanged: canvas.requestPaint()
            }

            onPaint: {
                context.reset()
                context.moveTo(0, 0)
                context.lineTo(width, 0)
                context.lineTo(width / 2, height)
                context.closePath()
                context.fillStyle = comboBox.pressed ? "#17a81a" : "lightblue"
                context.fill()
            }
        }
    }

    Rectangle {
        id: boxPW
        x: -5
        y: 233
        height: 20
        color: inputPW.cursorVisible ? "#dddddd" : "#ffffff"
        radius: 1
        anchors.left: boxPWValidate.left
        border.width: inputPW.cursorVisible ? 2 : 1
        anchors.verticalCenter: labelPasswort.verticalCenter
        border.color: "#add8e6"
        implicitWidth: 120
        implicitHeight: 40
        anchors.verticalCenterOffset: 0
        anchors.leftMargin: 0
        TextInput {
            id: inputPW
            text: qsTr("")
            echoMode: TextInput.Password
            anchors.verticalCenterOffset: 27
            font.pixelSize: 12
            anchors.fill: parent
        }
        anchors.right: image.right
        anchors.rightMargin: 0
    }

    Rectangle {
        id: boxPWValidate
        x: -12
        y: 230
        height: 20
        color: inputPWValidate.cursorVisible ? "#dddddd" : "#ffffff"
        radius: 1
        anchors.left: labelPasswortValidate.right
        border.width: inputPWValidate.cursorVisible ? 2 : 1
        anchors.verticalCenter: labelPasswortValidate.verticalCenter
        border.color: "#add8e6"
        implicitWidth: 120
        anchors.verticalCenterOffset: 5
        implicitHeight: 40
        anchors.leftMargin: 25
        TextInput {
            id: inputPWValidate
            text: qsTr("")
            echoMode: TextInput.Password
            font.pixelSize: 12
            anchors.fill: parent
        }
        anchors.right: image.right
        anchors.rightMargin: 0
    }
}
