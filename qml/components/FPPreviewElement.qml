import QtQuick
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
                console.warn("Unknown media type:", modelData.Type)
                return "\u003f"
            }
        }
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: Style.defaultMargin
        height: 16
        width: height
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
    FPOverlayFontAwesome {
        id: preview
        iconUtf8: "\uf110"
        iconColor: Style.colorText
        anchors.centerIn: parent
        width: 0.1*parent.width
        height: width
        visible: imageElement.status === Image.Loading
        RotationAnimator on rotation {
            from: 0
            to: 360
            duration: 2000
            loops: Animation.Infinite
            running: preview.visible
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: previewElement.clicked()
    }
}
