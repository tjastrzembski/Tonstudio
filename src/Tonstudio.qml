import QtQuick 2.10
import QtQuick.Controls 2.3
import io.qt.examples.SoundComponentGraphic 1.0
import io.qt.examples.ProjectManager 1.0
import io.qt.examples.WindowState 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Tonstudio")

    ProjectManager {
        id: projectManager
    }

    WindowState {
        id: wState
    }

    menuBar: TSMenuBar {
        id: tSMenuBar
    }

    //important for manipulate Soundtracks
    //toolBar
    header: TSToolbar {
        id: toolbar
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
    }

    TSContent {
        id: content
        anchors.fill: parent
        visible: true
    }


    //statusBar
    footer: TSStatusbar {
        id: statusbar
        position: ToolBar.Footer
        anchors.right: parent.right
        anchors.left: parent.left
    }
    //style : Component
}
