import QtQuick 2.10
import QtQml 2.2

Item {

    width: 200
    height: 100

    property string buttonText: "Button"
    QtObject {
        id: internalSettings
        property color buttonColor: "lightblue"
        property color onHoverColor: "gold"
        property color borderColor: "white"
    }

    Rectangle {
        id: simpleButton
        radius: 50
        anchors.fill: parent

        Text {
            id: buttonLabel
            color: "#ffffff"
            text: buttonText
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onEntered: parent.border.color = internalSettings.onHoverColor
            onExited: parent.border.color = internalSettings.borderColor
        }

        // Determines the color of the button by using the conditional operator
        color: buttonMouseArea.pressed ? Qt.darker(
                                             internalSettings.buttonColor,
                                             1.5) : internalSettings.buttonColor
    }
}
