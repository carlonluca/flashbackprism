import QtQuick
import QtQuick.Effects
import FlashbackPrism

Item {
    property alias text: yearElement.text

    width: yearElement.width
    height: yearElement.height

    Text {
        id: yearElement
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: Style.defaultMargin
        color: "white"
        font.bold: true
        font.pointSize: 16
        visible: false
    }
    MultiEffect {
        source: yearElement
        anchors.fill: yearElement
        shadowColor: "black"
        shadowEnabled: true
        shadowScale: 1
    }
}
