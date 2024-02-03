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
 * Date:    2024.01.01
 */

import QtQuick
import QtQuick.Controls
import FlashbackPrism

Button {
    property alias iconColor: textElement.color

    id: control
    ToolTip.visible: hovered && !!ToolTip.text
    ToolTip.delay: Style.defaultToolTipDelay

    contentItem: Text {
        id: textElement
        font.family: fontAwesomeFreeSolid.family
        font.styleName: fontAwesomeFreeSolid.styleName
        font.weight: fontAwesomeFreeSolid.weight
        text: control.text
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
