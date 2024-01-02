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
 * Date:    2023.24.13
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import FlashbackPrism

Item {
    FPLoginRequest {
        id: loginRequest
        onLoginSucceeded: (token, downloadToken, previewToken) => {
            settingsNotifier.uname = unameInput.text
            settingsNotifier.pwd = pwdInput.text
            settingsNotifier.token = token
            settingsNotifier.downloadToken = downloadToken
            settingsNotifier.previewToken = previewToken
            mainStackView.push(albumsTodayComponent)
        }
    }

    Column {
        anchors.centerIn: parent
        width: Math.min(parent.width - 2*Style.defaultMargin, 400)
        spacing: Style.defaultMargin

        Label {
            width: parent.width
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            text: Qt.application.name
            font.pointSize: 28
            horizontalAlignment: Text.AlignHCenter
        }

        Item {
            width: parent.width
            height: Style.defaultMargin
        }

        Image {
            source: "qrc:/qt/qml/FlashbackPrism/assets/icon.svg"
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            height: 140
        }

        Item {
            width: parent.width
            height: Style.defaultMargin
        }

        Label {
            width: parent.width
            text: qsTr("PhotoPrism server URL:")
        }

        TextField {
            id: urlInput
            width: parent.width
            placeholderText: "https://my.photoprism.server"
            text: settingsNotifier.photoprismUrl
        }

        Label {
            width: parent.width
            text: qsTr("Username:")
        }

        TextField {
            id: unameInput
            width: parent.width
            text: settingsNotifier.uname
        }

        Label {
            width: parent.width
            text: qsTr("Password:")
        }

        TextField {
            id: pwdInput
            width: parent.width
            text: settingsNotifier.pwd
            echoMode: TextInput.Password
        }

        Item {
            width: parent.width
            height: Style.defaultMargin
        }

        Button {
            text: qsTr("Login")
            anchors.horizontalCenter: parent.horizontalCenter
            onPressed: {
                settingsNotifier.photoprismUrl = urlInput.text
                loginRequest.login(urlInput.text, unameInput.text, pwdInput.text)
            }
        }
    }

    // Overlay
    FPPopupMessage {
        title: qsTr("Logging in. Please wait...")
        visible: loginRequest.working

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
