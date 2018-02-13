import QtQuick 2.10
import QtQuick.Controls 2.3
import io.qt.examples.WindowState 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Tonstudio Login")

    WindowState {
        id: wState
    }

    Text {
        id: labelUser
        y: 170
        width: 39
        height: 17
        text: qsTr("Nutzer:")
        anchors.left: image.left
        anchors.leftMargin: 0
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: boxUser.verticalCenter
        font.pixelSize: 12
    }

    Rectangle {
        id: boxUser
        height: 20
        color: inputUser.cursorVisible ? "#dddddd" : "#ffffff"
        implicitWidth: 120
        implicitHeight: 40
        border.color: "lightblue"
        border.width: inputUser.cursorVisible ? 2 : 1
        radius: 1
        anchors.top: image.bottom
        anchors.topMargin: 10
        anchors.left: boxPW.left
        anchors.leftMargin: 0
        anchors.right: image.right
        anchors.rightMargin: 0

        TextInput {
            id: inputUser
            text: qsTr("")
            anchors.fill: parent
            echoMode: TextInput.Normal
            font.pixelSize: 12
        }
    }

    Image {
        id: image
        x: 280
        y: 62
        width: 200
        height: 100
        clip: false
        sourceSize.height: 623
        sourceSize.width: 1256
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: -100
        anchors.verticalCenter: parent.verticalCenter
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

    Rectangle {
        id: boxPW
        y: 233
        height: 20
        color: inputPW.cursorVisible ? "#dddddd" : "#ffffff"
        radius: 1
        anchors.left: labelPasswort.right
        border.width: inputPW.cursorVisible ? 2 : 1
        anchors.verticalCenter: labelPasswort.verticalCenter
        border.color: "#add8e6"
        implicitWidth: 120
        implicitHeight: 40
        anchors.verticalCenterOffset: 0
        anchors.leftMargin: 20
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
        id: buttonLogin
        x: 220
        y: 283
        width: 200
        height: 36
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 30
        anchors.verticalCenter: labelPasswort.verticalCenter

        buttonText: qsTr("Einloggen")

        signal buttonClick
        onButtonClick: {
            wState.setState(2)

            window.close()
        }
        MouseArea {
            id: buttonLoginArea
            anchors.fill: parent
            onClicked: buttonLogin.buttonClick()
        }
    }

    TSButton {
        id: buttonRegister
        x: 220
        y: 325
        width: 200
        height: 34
        anchors.verticalCenterOffset: 40
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: buttonLogin.verticalCenter

        buttonText: qsTr("Registrieren...")

        signal buttonClick
        onButtonClick: {
            wState.setState(1)
            window.close()
        }
        MouseArea {
            id: buttonRegisterArea
            anchors.fill: parent
            onClicked: buttonRegister.buttonClick()
        }
    }
}
