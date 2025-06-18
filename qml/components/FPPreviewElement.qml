import QtQuick
import QtQuick.Controls
import FlashbackPrism

Item {
    property alias source: imageElement.source
    property alias iconMediaTypeVisible: iconMediaType.visible
    property string mediaType: "image" // image, video, unknown

    signal clicked()

    id: previewElement

    Image {
        id: imageElement
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        opacity: status === Image.Ready ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    // Media type icon
    FPOverlayFontAwesome {
        id: iconMediaType
        iconColor: "white"
        iconUtf8: {
            switch (previewElement.mediaType) {
            case "image":
                return "\uf03e"
            case "video":
                return "\uf008"
            default:
                return ""
            }
        }
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: Style.defaultMargin
        height: 16
        width: height
        visible: !!iconUtf8
    }

    // Warning icon
    FPOverlayFontAwesome {
        iconUtf8: "\uf071"
        iconColor: Style.colorWarning
        anchors.centerIn: parent
        width: 0.1*parent.width
        height: width
        visible: iconMediaType.status === Image.Error
    }

    // Loading icon
    Column {
        width: 0.5*parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.centerIn: parent
        visible: imageElement.status === Image.Loading

        FPOverlayFontAwesome {
            id: preview
            iconUtf8: "\uf110"
            iconColor: Style.colorText
            width: 0.1*parent.width
            height: width
            RotationAnimator on rotation {
                from: 0
                to: 360
                duration: 2000
                loops: Animation.Infinite
                running: preview.visible
            }
        }

        ProgressBar {
            width: parent.width
            from: 0
            to: 100
            value: imageElement.progress*100
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: previewElement.clicked()
    }
}
