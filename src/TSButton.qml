import QtQuick 2.10
import QtQml 2.2

Item {

    width: 200
    height: 100

    property string buttonText: "Button"
    property int fontSize: 20
    property string fontColor: "#ffffff"

    property int bP: 0
    property int rP: 0
    property int lP: 0
    property int tP: 0

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
            color: fontColor
            text: buttonText
            bottomPadding: bP
            topPadding: tP
            rightPadding: rP
            leftPadding: lP
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: fontSize
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
