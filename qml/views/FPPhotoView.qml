/*
 * This file is part of FlashbackPrism.
 *
 * Copyright (c) 2023 Luca Carlon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Author:  Luca Carlon
 * Company: -
 * Date:    2023.12.27
 */

import QtQuick
import QtQuick.Controls
import FlashbackPrism
import "qrc:/lqtutils/fontawesome" as FA

Item {
    property var photoModel: []
    /* private */ property alias currentIndex:
        swipeView.currentIndex
    /* private */ property bool topBarVisible:
        false

    id: photoView

    FPTopBar {
        id: topBar
        height: topBarVisible ? implicitHeight : 0
        clip: true

        Behavior on height { NumberAnimation { duration: 200; easing.type: Easing.OutQuint } }

        // Copy button
        FPTopBarButton {
            text: "\uf0c5"
            onClicked: swipeView.currentItem?.item?.photoStore?.copyToClipboard()
            visible: !lqtQmlUtils.isMobile()
            ToolTip.text: qsTr("Copy")
        }

        // Share button
        FPTopBarButton {
            text: "\uf1e0"
            onClicked: swipeView.currentItem?.item?.photoStore?.share()
            visible: lqtQmlUtils.isMobile()
            ToolTip.text: qsTr("Share")
        }

        // Open button
        FPTopBarButton {
            text: "\uf08e"
            onClicked: swipeView.currentItem?.item?.photoStore?.open()
            ToolTip.text: qsTr("Open with system app")
        }

        // Download
        FPTopBarButton {
            text: "\uf0ed"
            onClicked: swipeView.currentItem?.item?.photoStore?.download(function(filePath) {
                if (filePath) {
                    okDialog.title = qsTr("Photo downloaded")
                    okDialog.text = qsTr("Photo downloaded to:") + " " + filePath
                    okDialog.open()
                    return
                }

                okDialog.title = qsTr("Failure")
                okDialog.text = qsTr("Failed to download photo")
                okDialog.open()
            })
            ToolTip.text: qsTr("Download")
        }

        FPTopBarButton {
            text: "\uf2f9"
            ToolTip.text: qsTr("Rotate 90° clockwise")
            onClicked: {
                const item = swipeView.currentItem?.item
                if (item)
                    item.imageElementRotation = (item.imageElementRotation + 90)%360
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: topBar.bottom
        }
        currentIndex: photoView.currentIndex
        Repeater {
            model: photoModel
            Loader {
                active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                sourceComponent: Item {
                    property FPQueryResultItem photoItem: modelData
                    property FPPhotoViewStore photoStore: photoViewStore
                    property real imageElementRotation: 0

                    id: photoDelegate

                    FPPhotoViewStore {
                        id: photoViewStore
                        item: photoItem
                        provider: photoProvider
                    }
                    FPZoomableImage {
                        id: imageElement
                        width: photoDelegate.imageElementRotation === 0 || photoDelegate.imageElementRotation === 180 ? parent.width : parent.height
                        height: photoDelegate.imageElementRotation === 0 || photoDelegate.imageElementRotation === 180 ? parent.height : parent.width
                        rotation: photoDelegate.imageElementRotation
                        anchors.centerIn: parent
                        source: "image://photo/" + photoItem.Hash
                        fillMode: Image.PreserveAspectFit
                        autoTransform: true
                    }
                    // Error message
                    ImageStatusView {
                        sourceImageElement: imageElement
                        visible: imageElement.imageStatus === Image.Error
                        iconUtf8: "\uf06a"
                        text: qsTr("Failed to download image from the server")
                    }

                    // Waiting
                    ImageStatusView {
                        iconUtf8: "\uf251"
                        sourceImageElement: imageElement
                        visible: imageElement.imageStatus === Image.Loading
                        text: qsTr("Loading image. Please wait...")
                    }

                    FPPhotoOverlayText {
                        text: qmlUtils.formatDateTimeForPhoto(photoItem.TakenAt)
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: Style.defaultMargin
                    }
                    MouseArea {
                        propagateComposedEvents: true
                        anchors.fill: parent
                        onClicked: {
                            topBarVisible = !topBarVisible
                            if (topBarVisible)
                                hideTopBarTimer.start()
                        }
                    }
                }
            }
        }
    }

    component ImageStatusView: Column {
        property var sourceImageElement: null
        property alias iconUtf8: iconElement.iconUtf8
        property alias text: messageElement.text

        spacing: Style.defaultMargin
        width: parent.width
        anchors {
            centerIn: parent
            margins: Style.defaultMargin
        }

        FA.LQTFontAwesomeFreeSolid {
            id: iconElement
            anchors.horizontalCenter: parent.horizontalCenter
            iconColor: Style.colorWarning
            width: sourceImageElement.width/8
            height: width
        }
        Item {
            width: parent.width
            height: Style.defaultMargin
        }
        FPText {
            id: messageElement
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

        }
    }

    // File saved
    FPPopupOk {
        id: okDialog
    }

    Timer {
        id: hideTopBarTimer
        running: false
        repeat: false
        interval: 4000
        onTriggered: topBarVisible = false
    }
}
