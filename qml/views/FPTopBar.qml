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

Rectangle {
    default property alias data: actionRow.data
    property bool backVisible: true
    property bool appNameVisible: false

    color: "black"
    implicitHeight: backButton.height + 2*Style.defaultMargin
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: Style.defaultMargin
        spacing: Style.defaultMargin
        FPTopBarButton {
            id: backButton
            text: "\uf060"
            font.family: fontAwesomeFreeSolid.family
            font.styleName: fontAwesomeFreeSolid.styleName
            font.weight: fontAwesomeFreeSolid.weight
            anchors.verticalCenter: parent.verticalCenter
            onClicked: mainStackView.pop()
            visible: backVisible
            ToolTip.text: qsTr("Copy")
        }
        FPText {
            id: appName
            text: Qt.application.name
            font.pixelSize: backButton.height*0.4
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            visible: appNameVisible
        }
    }

    Row {
        id: actionRow
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: Style.defaultMargin
        spacing: Style.defaultMargin
    }
}
