import QtQuick
import QtQuick.Controls
import FlashbackPrism

Rectangle {
    default property alias data: actionRow.data
    property bool backVisible: true

    color: "black"
    height: backButton.height + 2*Style.defaultMargin
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        Button {
            id: backButton
            text: "\uf060"
            font.family: fontAwesomeFreeSolid.family
            font.styleName: fontAwesomeFreeSolid.styleName
            font.weight: fontAwesomeFreeSolid.weight
            anchors.verticalCenter: parent.verticalCenter
            onClicked: mainStackView.pop()
            visible: backVisible
        }
    }

    Row {
        id: actionRow
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }
}
