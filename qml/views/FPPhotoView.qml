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
    property alias source: imageElement.source

    FPTopBar {
        id: topBar
    }

    Image {
        id: imageElement
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: topBar.bottom
        }
        fillMode: Image.PreserveAspectFit
        onStatusChanged: function(status) {
            switch (status) {
            case Image.Null:
                loadingDialog.close()
                break
            case Image.Ready:
                loadingDialog.close()
                break
            case Image.Loading:
                loadingDialog.open()
                break
            case Image.Error:
                loadingDialog.close()
                break
            }
        }

        Column {
            anchors {
                centerIn: parent
                margins: Style.defaultMargin
            }
            visible: imageElement.status === Image.Error
            spacing: Style.defaultMargin
            FA.LQTFontAwesomeFreeSolid {
                anchors.horizontalCenter: parent.horizontalCenter
                iconUtf8: "\uf06a"
                iconColor: Style.colorWarning
                width: imageElement.width/8
                height: width
            }
            Item {
                width: parent.width
                height: Style.defaultMargin
            }
            FPText {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Failed to download image from the server")
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