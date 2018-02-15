import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Window 2.3

Item {
    id: item1

    ListView {
        id: list
        contentWidth: projectManager.currentProject.maxSBWidth
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        spacing: 2
        anchors.fill: parent

        delegate: Tonspur {
            x: 0
            y: 3
            height: 150

            name: Name
            soundlength: Size

        }
        model: SoundListModel
        MouseArea {
            anchors.fill: parent
            onClicked: list.model = SoundListModel
        }
        //onDataChanged: console.debug(SoundListModel.rowCount())
    }
}
