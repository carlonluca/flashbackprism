import QtQuick
import QtQuick.Controls
import FlashbackPrism

Rectangle {
    default property alias data: actionRow.data
    property bool backVisible: true
    property bool iconVisible: false
    property bool appNameVisible: false

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
        anchors.margins: Style.defaultMargin
        spacing: Style.defaultMargin
        FPTopBarButton {
            id: backButton
            text: "\uf060"
            font.family: fontAwesomeFreeSolid.family
            font.styleName: fontAwesomeFreeSolid.styleName
            font.weight: fontAwesomeFreeSolid.weight
            anchors.verticalCenter: parent.verticalCenter
            onClicked: mainStackView.pop()
            visible: backVisible
            ToolTip.text: qsTr("Copy")
        }
        Image {
            id: iconButton
            source: "qrc:/qt/qml/FlashbackPrism/assets/icon.svg"
            fillMode: Image.PreserveAspectFit
            width: height
            height: backButton.height
            visible: iconVisible
            anchors.verticalCenter: parent.verticalCenter
        }
        FPText {
            id: appName
            text: Qt.application.name
            font.pixelSize: iconButton.height*0.4
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            visible: appNameVisible
        }
    }

    Row {
        id: actionRow
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: Style.defaultMargin
        spacing: Style.defaultMargin
    }
}
