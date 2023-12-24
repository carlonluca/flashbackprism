import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import FlashbackPrism

Item {
    FPLoginRequest {
        id: loginRequest
    }

    Column {
        anchors.centerIn: parent
        width: parent.width/2
        spacing: Style.defaultMargin

        Label {
            width: parent.width
            text: qsTr("PhotoPrism server URL:")
        }

        TextField {
            width: parent.width
            placeholderText: "https://my.photoprism.server"
        }

        Label {
            width: parent.width
            text: qsTr("Username:")
        }

        TextField {
            width: parent.width
        }

        Label {
            width: parent.width
            text: qsTr("Password:")
        }

        TextField {
            width: parent.width
        }

        Item {
            width: parent.width
            height: Style.defaultMargin
        }

        Button {
            text: qsTr("Login")
            anchors.horizontalCenter: parent.horizontalCenter
            onPressed: loginRequest.login("abc", "abc")
        }
    }

    // Overlay
    FPPopupMessage {
        title: qsTr("Logging in. Please wait...")
        visible: loginRequest.working
        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
