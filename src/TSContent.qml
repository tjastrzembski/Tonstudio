import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Window 2.3

Item {
    id: item1

    ListView {
        id: list
        //static c++ long?
        contentWidth: projectManager.currentProject.maxSBWidth
        //contentHeight:
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        spacing: 2
        anchors.fill: parent

        delegate: Tonspur {
            x: 0
            y: 3
            height: 150

            //Tonspur != Soundbackend
            name: colorCode
        }
        //model: projectManager.currentProject.model
        model:myModel
    }
}
