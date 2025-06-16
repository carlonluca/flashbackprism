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
import QtQuick.Controls
import FlashbackPrism
import "qrc:/lqtutils/fontawesome" as FA

Item {
    property var model: []

    id: yearView
    StackView.onActivated: {
        mainWindow.statusBarColor = topBar.color
        mainWindow.navBarColor = Material.background
    }

    FPTopBar {
        id: topBar
        pageTitle: {
            try { return "Year: " + model[0].TakenAt.getFullYear() }
            catch (e) {}
        }
    }

    FPGridView {
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
            id: gridViewImage
            width: gridView.cellWidth
            height: width
            source: qmlUtils.thumbnailUrl(modelData, 1)
            FPOverlayFontAwesome {
                iconColor: "white"
                iconUtf8: {
                    if (modelData.isImage())
                        return "\uf03e"
                    if (modelData.isVideo())
                        return "\uf008"
                    console.warn("Unknown media type:", modelData.Type)
                    return "\u003f"
                }
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: Style.defaultMargin
                height: 16
                width: height
            }
            FPOverlayFontAwesome {
                iconUtf8: "\uf071"
                iconColor: Style.colorWarning
                anchors.centerIn: parent
                width: 0.1*parent.width
                height: width
                visible: gridViewImage.status === Image.Error
            }
            FPOverlayFontAwesome {
                iconUtf8: "\uf110"
                iconColor: Style.colorText
                anchors.centerIn: parent
                width: 0.1*parent.width
                height: width
                visible: gridViewImage.status === Image.Loading
                RotationAnimator on rotation {
                    from: 0
                    to: 360
                    duration: 2000
                    loops: Animation.Infinite
                    running: parent.visible
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: mainStackView.push(photoViewComponent, {
                    "photoModel": yearView.model,
                    "currentIndex": index
                })
            }
        }
    }
}
