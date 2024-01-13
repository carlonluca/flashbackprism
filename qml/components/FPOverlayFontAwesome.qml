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
 * Date:    2023.01.13
 */

import QtQuick
import QtQuick.Effects
import FlashbackPrism
import "qrc:/lqtutils/fontawesome" as FA

Item {
    property alias iconUtf8: iconElement.iconUtf8
    property alias iconColor: iconElement.iconColor

    FA.LQTFontAwesomeFreeSolid {
        id: iconElement
        anchors.fill: parent
        iconColor: "white"
        visible: false
    }
    MultiEffect {
        source: iconElement
        anchors.fill: iconElement
        shadowColor: "black"
        shadowEnabled: true
        shadowScale: 1
    }
}
