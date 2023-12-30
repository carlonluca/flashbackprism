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
 * Date:    2023.12.26
 */

import QtQuick
import FlashbackPrism

Item {
    property var model: []

    FPTopBar {
        id: topBar
    }

    GridView {
        id: gridView
        clip: true
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: topBar.bottom
        }
        model: parent.model
        cellWidth: width/3
        cellHeight: cellWidth
        delegate: Image {
            width: gridView.cellWidth
            height: width
            source: qmlUtils.thumbnailUrl(modelData, 1)
            FPPhotoOverlayText {
                text: qmlUtils.formatDateForPhoto(modelData.TakenAt)
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: Style.defaultMargin
            }
            MouseArea {
                anchors.fill: parent
                onClicked: mainStackView.push(photoViewComponent, {
                    "photoItem": modelData
                })
            }
        }
    }

    FPPhotosRequest {
        id: photoRequest
    }
}
