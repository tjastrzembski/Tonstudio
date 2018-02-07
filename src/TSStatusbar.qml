import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ToolBar {
    width: 80
    RowLayout {
        anchors.rightMargin: 0
        anchors.fill: parent
    }

    Text {
        id: status
        x: 29
        y: 13
        text: qsTr("Bereit")
        horizontalAlignment: Text.AlignRight
        wrapMode: Text.WordWrap
        font.pixelSize: 12
    }
}
