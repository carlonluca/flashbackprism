import QtQuick
import QtQuick.Controls
import FlashbackPrism

Rectangle {
    default property alias data: actionRow.data

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
            text: "\uf060"
            font.family: fontAwesomeFreeSolid.family
            font.styleName: fontAwesomeFreeSolid.styleName
            font.weight: fontAwesomeFreeSolid.weight
            anchors.verticalCenter: parent.verticalCenter
            onClicked: mainStackView.pop()
        }
    }

    Row {
        id: actionRow
        anchors.margins: Style.defaultMargin
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
