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
import QtQuick.Effects
import FlashbackPrism

Item {
    property alias text: yearElement.text

    width: yearElement.width
    height: yearElement.height

    Text {
        id: yearElement
        anchors.top: parent.top
        anchors.right: parent.right
        color: "white"
        font.bold: true
        font.pointSize: 16
        visible: false
    }
    MultiEffect {
        source: yearElement
        anchors.fill: yearElement
        shadowColor: "black"
        shadowEnabled: true
        shadowScale: 1
    }
}
