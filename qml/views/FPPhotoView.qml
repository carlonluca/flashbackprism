import QtQuick
import QtQuick.Controls
import FlashbackPrism

Item {
    property alias source: imageElement.source

    Image {
        id: imageElement
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        onStatusChanged: function(status) {
            switch (status) {
            case Image.Null:
                // TODO
                break
            case Image.Ready:
                loadingDialog.close()
                break
            case Image.Loading:
                loadingDialog.open()
                break
            case Image.Error:
                // TODO
                break
            }
        }
    }

    // Waiting layer
    FPPopupMessage {
        id: loadingDialog
        title: qsTr("Preparing data. Please wait...")

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
