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
import QtMultimedia
import FlashbackPrism
import "qrc:/lqtutils/fontawesome" as FA

Item {
    property alias photoModel:
        swipeViewRepeater.model
    property alias currentIndex:
        swipeView.currentIndex

    /* private */ property Item currentItem:
        swipeView.currentItem
    /* private */ property FPQueryResultItem currentResultItem:
        currentItem?.photoItem ?? null
    /* private */ property FPPhotoViewStore currentPhotoStore:
        currentItem?.photoStore ?? null
    /* private */ property bool topBarVisible:
        false

    id: photoView
    StackView.onActivated: {
        mainWindow.statusBarColor = topBar.color
        mainWindow.navBarColor = topBar.color
    }

    FPTopBar {
        id: topBar
        height: topBarVisible ? implicitHeight : 0
        clip: true
        z: 1

        Behavior on height { NumberAnimation { duration: 200; easing.type: Easing.OutQuint } }

        // Copy button
        FPTopBarButton {
            text: "\uf0c5"
            onClicked: currentPhotoStore?.copyToClipboard()
            visible: !lqtQmlUtils.isMobile() && (currentResultItem?.isImage() ?? false)
            ToolTip.text: qsTr("Copy")
        }

        // Share button
        FPTopBarButton {
            text: "\uf1e0"
            onClicked: currentPhotoStore?.share()
            visible: lqtQmlUtils.isMobile() && (currentResultItem?.isImage() ?? false)
            ToolTip.text: qsTr("Share")
        }

        // Open button
        FPTopBarButton {
            text: "\uf08e"
            onClicked: currentPhotoStore?.open()
            visible: (currentResultItem?.isImage() ?? false) && !lqtQmlUtils.isMobile()
            ToolTip.text: qsTr("Open with system app")
        }

        // Download
        FPTopBarButton {
            text: "\uf0ed"
            onClicked: currentPhotoStore?.download(function(filePath) {
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
            visible: (currentResultItem?.isImage() ?? false) && !lqtQmlUtils.isMobile()
            ToolTip.text: qsTr("Download")
        }

        FPTopBarButton {
            text: "\uf2f9"
            ToolTip.text: qsTr("Rotate 90° clockwise")
            onClicked: {
                if (currentItem)
                    currentItem.imageElementRotation = (currentItem.imageElementRotation + 90)%360
            }
        }

        FPTopBarButton {
            text: "\uf1f8"
            iconColor: Style.colorWarning
            onClicked: requestArchive.request()
            ToolTip.text: qsTr("Archive photo")
        }
    }

    SwipeView {
        id: swipeView
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: parent.top
        }
        Repeater {
            id: swipeViewRepeater
            Loader {
                active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                sourceComponent: Item {
                    property FPQueryResultItem photoItem: modelData
                    property FPPhotoViewStore photoStore: photoViewStore
                    property real imageElementRotation: 0

                    id: photoDelegate

                    // This portion of code is a workaround to a possible Qt bug: the first item
                    // is not set in swipeView.currentItem, it remains null until the item is changed.
                    Component.onCompleted: photoDelegate.refreshCurrentItem()
                    Connections {
                        target: swipeView
                        function onCurrentIndexChanged() {
                            photoDelegate.refreshCurrentItem()
                        }
                    }

                    function refreshCurrentItem() {
                        if (index === swipeView.currentIndex)
                            photoView.currentItem = photoDelegate
                    }

                    FPPhotoViewStore {
                        id: photoViewStore
                        item: photoItem
                        provider: photoProvider
                    }
                    Loader {
                        id: imageLoader
                        width: photoDelegate.imageElementRotation === 0 || photoDelegate.imageElementRotation === 180 ? parent.width : parent.height
                        height: photoDelegate.imageElementRotation === 0 || photoDelegate.imageElementRotation === 180 ? parent.height : parent.width
                        rotation: photoDelegate.imageElementRotation
                        anchors.centerIn: parent
                        active: photoItem?.isImage() ?? false
                        sourceComponent: ImageView {}
                    }
                    Loader {
                        id: videoLoader
                        width: photoDelegate.imageElementRotation === 0 || photoDelegate.imageElementRotation === 180 ? parent.width : parent.height
                        height: photoDelegate.imageElementRotation === 0 || photoDelegate.imageElementRotation === 180 ? parent.height : parent.width
                        rotation: photoDelegate.imageElementRotation
                        anchors.centerIn: parent
                        active: photoItem?.isVideo() ?? false
                        sourceComponent: VideoView {}
                    }

                    FPPhotoOverlayText {
                        text: qmlUtils.formatDateTimeForPhoto(photoItem.TakenAt)
                        anchors.bottom: parent.bottom
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

    FPRequestPhotoArchive {
        id: requestArchive
        url: settingsNotifier.photoprismUrl
        token: settingsNotifier.token
        photos: currentResultItem ? [
            currentResultItem.UID
        ] : []
        onErrorOccurred: {
            okDialog.title = qsTr("Failure")
            okDialog.text = qsTr("An error occured trying to archive the image")
            okDialog.open()
        }
        onSucceeded: {
            deletedDialog.title = qsTr("Success")
            deletedDialog.text = qsTr("The photo was successfully removed")
            deletedDialog.open()
        }
    }

    FPPopupMessage {
        title: qsTr("Waiting for server response...")
        visible: requestArchive.working

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    component ImageView: Item {
        FPZoomableImage {
            id: imageElement
            anchors.fill: parent
            source: "image://photo/" + photoItem.Hash
            fillMode: Image.PreserveAspectFit
            autoTransform: true
            opacity: imageStatus === Image.Ready ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: 300 } }
        }

        // Error message
        ImageStatusView {
            sourceMediaElement: imageElement
            opacity: imageElement.imageStatus === Image.Error ? 1 : 0
            iconUtf8: "\uf06a"
            text: qsTr("Failed to download image from the server")
            animate: false
            Behavior on opacity { NumberAnimation { duration: 300 } }
        }

        // Waiting
        ImageStatusView {
            iconUtf8: "\uf251"
            sourceMediaElement: imageElement
            opacity: imageElement.imageStatus === Image.Loading ? 1 : 0
            text: qsTr("Loading image. Please wait...")
            progressVisible: true
            progress: currentPhotoStore?.progress ?? 0
            animate: true
            Behavior on opacity { NumberAnimation { duration: 300 } }
        }
    }

    component VideoView: Item {
        Video {
            property bool shouldBePlaying: (photoItem?.isVideo() ?? false)
                                           && swipeView.currentIndex === index
                                           && Qt.application.state === Qt.ApplicationActive
                                           && error === MediaPlayer.NoError
            id: videoElement
            anchors.fill: parent
            source: qmlUtils.photoUrl(modelData)
            fillMode: VideoOutput.PreserveAspectFit
            opacity: error === MediaPlayer.NoError ? 1 : 0
            onShouldBePlayingChanged: {
                if (shouldBePlaying)
                    play()
                else
                    stop()
            }
            Behavior on opacity { NumberAnimation { duration: 300 } }
        }

        // Error message
        ImageStatusView {
            opacity: videoElement.error !== MediaPlayer.NoError ? 1 : 0
            sourceMediaElement: videoElement
            iconUtf8: "\uf06a"
            text: qsTr("Playback failed. Multimedia backed returned the following error:\n%1")
                .arg(videoElement.errorString)
            Behavior on opacity { NumberAnimation { duration: 300 } }
        }
    }

    component ImageStatusView: Column {
        property Item sourceMediaElement: null
        property alias iconUtf8: iconElement.iconUtf8
        property alias text: messageElement.text
        property alias progress: progressBar.value
        property alias progressVisible: progressBar.visible
        property bool animate: false

        id: imageStatusView
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
            width: sourceMediaElement.width/8
            height: width
            Behavior on rotation {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            Timer {
                interval: 2000
                running: imageStatusView.visible && imageStatusView.animate
                repeat: true
                onTriggered: iconElement.rotation += 180
            }
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
        Item {
            width: parent.width
            height: Style.defaultMargin
        }
        ProgressBar {
            id: progressBar
            width: messageElement.contentWidth
            from: 0
            to: 1
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    // File saved
    FPPopupOk {
        id: okDialog
    }

    FPPopupOk {
        id: deletedDialog
        onClosed: {
            mainStackView.pop(mainStackView.find(function(item, index) {
                if (item.objectName === "yearListView") {
                    item.pendingRefreshRequest = true
                    return true
                }
                return false
            }))
        }
    }

    Timer {
        id: hideTopBarTimer
        running: false
        repeat: false
        interval: 4000
        onTriggered: topBarVisible = false
    }
}
