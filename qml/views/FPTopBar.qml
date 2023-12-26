import QtQuick
import QtQuick.Controls
import FlashbackPrism

Rectangle {
    color: "black"
    height: Style.topBarHeight
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }

    Row {
        anchors.margins: Style.defaultMargin
        anchors.fill: parent
        Button {
            text: "Back"
            anchors.verticalCenter: parent.verticalCenter
            onClicked: mainStackView.pop()
        }
    }
}
