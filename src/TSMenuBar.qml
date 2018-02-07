import QtQuick 2.10
import QtQuick.Controls 2.3

MenuBar {
    id: menuBar
    height: 20
    font.pointSize: 10
    TSMenu {
        title: qsTr("&File")
        Action {
            text: qsTr("&New...")
        }
        Action {
            text: qsTr("&Open...")
        }
        Action {
            text: qsTr("&Save")
        }
        Action {
            text: qsTr("Save &As...")
        }
        MenuSeparator {
        }
        Action {
            text: qsTr("&Quit")
            onTriggered: Qt.quit()
        }
    }

    TSMenu {
        title: qsTr("&Edit")
        Action {
            text: qsTr("Cu&t")
        }
        Action {
            text: qsTr("&Copy")
        }
        Action {
            text: qsTr("&Paste")
        }
    }
    TSMenu {
        title: qsTr("&Help")
        Action {
            text: qsTr("&About")
        }
    }

    delegate: MenuBarItem {
        id: menubarItem
        height: 20
        anchors.top: parent.top
        font.pointSize: 10
    }
}
