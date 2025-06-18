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
 * Date:    2023.01.03
 */

import QtQuick

Item {
    property alias source: photoSurfaceImage.source
    property alias fillMode: photoSurfaceImage.fillMode
    property alias autoTransform: photoSurfaceImage.autoTransform
    property alias imageRotation: photoSurfaceImage.rotation
    property int imageStatus: photoSurfaceImage.status

    id: photoSurface
    visible: source !== ""
    onVisibleChanged: photoFlick.reset()

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Flickable {
        id: photoFlick
        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds
        interactive: true
        clip: true
        rebound: Transition { NumberAnimation { duration: 0 } }

        PinchArea {
            property real initWidth
            property real initHeight

            width: Math.max(photoFlick.contentWidth, photoFlick.width)
            height: Math.max(photoFlick.contentHeight, photoFlick.height)
            pinch.dragAxis: Pinch.XAndYAxis

            onPinchStarted: {
                initWidth = photoFlick.contentWidth
                initHeight = photoFlick.contentHeight
            }

            onPinchUpdated: (pinch) => {
                photoFlick.contentX += pinch.previousCenter.x - pinch.center.x
                photoFlick.contentY += pinch.previousCenter.y - pinch.center.y

                const fitSize = photoSurfaceImage.fitSize()
                const maxW = fitSize.width*4
                const minW = fitSize.width
                const w = Math.min(Math.max(initWidth*pinch.scale, minW), maxW)
                const h = w*fitSize.height/fitSize.width

                photoFlick.resizeContent(w, h, pinch.center)
                photoFlick.returnToBounds()
            }

            onPinchFinished: photoFlick.returnToBounds()

            Image {
                property real r1: photoFlick.width/photoFlick.height
                property real r2: !sourceSize.height ? 0 : sourceSize.width/sourceSize.height

                id: photoSurfaceImage
                width: photoFlick.contentWidth
                height: photoFlick.contentHeight
                anchors.centerIn: parent

                onProgressChanged: console.log("Progress: " + progress)

                onR1Changed: refreshSize()
                onR2Changed: refreshSize()

                function refreshSize() {
                    const s = fitSize()
                    photoFlick.contentWidth = s.width
                    photoFlick.contentHeight = s.height
                }

                function fitSize() {
                    const w = r2 >= r1 ? photoFlick.width : photoFlick.height*r2
                    const h = r2 >= r1 ? photoFlick.width/r2 : photoFlick.height
                    return Qt.size(w, h)
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: photoFlick.reset()
                }
            }
        }

        function reset() {
            contentWidth = parent.width
            contentHeight = parent.height
            returnToBounds()
        }

        function scale(fac) {
            const maxW = photoSurfaceImage.sourceSize.width*4
            const minW = Math.min(photoSurface.width, photoSurfaceImage.sourceSize.width)
            const maxH = photoSurfaceImage.sourceSize.height*4
            const minH = Math.min(photoSurface.height, photoSurfaceImage.sourceSize.height)
            const w = Math.min(Math.max(contentWidth*fac, minW), maxW)
            const h = Math.min(Math.max(contentHeight*fac, minH), maxH)

            if (w <= photoSurface.width && h <= photoSurface.height)
                photoFlick.reset()
            else {
                photoFlick.resizeContent(w, h, Qt.point(photoFlick.width/2, photoFlick.height/2))
                photoFlick.returnToBounds()
            }
        }
    }
}
