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

Item {
    readonly property alias color: background.color

    default property alias data: actionRow.data
    property bool backVisible: true
    property string pageTitle: ""

    implicitHeight: Math.max(backButton.height, actionRow.height) + 2*Style.defaultMargin
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }

    Rectangle {
        id: background
        opacity: 0.3
        color: "black"
        anchors.fill: parent
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
            id: pageTitleElement
            text: pageTitle
            font.pixelSize: backButton.height*0.4
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            visible: !!pageTitle
        }
    }

    Grid {
        id: actionRow
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: Style.defaultMargin
        spacing: Style.defaultMargin
        columns: 3
    }
}
