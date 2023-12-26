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
 * Date:    2023.12.25
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import FlashbackPrism

Item {
    Component.onCompleted: photoMonitor.start()

    FPPhotoMonitor {
        id: photoMonitor
    }

    GridView {
        anchors.fill: parent
        model: photoMonitor.flashbackYears
        delegate: Text {
            color: "white"
            text: "" + modelData.year
        }
    }

    // Waiting layer
    FPPopupMessage {
        title: qsTr("Preparing data. Please wait...")
        visible: photoMonitor.working

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
