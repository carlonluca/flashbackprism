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
import QtQuick.Effects
import QtQuick.Layouts
import FlashbackPrism

Item {
    property bool pendingRefreshRequest: false

    id: yearListView
    objectName: "yearListView"
    StackView.onActivated: {
        mainWindow.statusBarColor = topBar.color
        mainWindow.navBarColor = Material.background
    }

    Component.onCompleted: {
        photoMonitor.start()
        photoMonitor.refreshModel()
    }

    FPTopBar {
        id: topBar
        backVisible: false
        pageTitle: Qt.application.name

        FPTopBarButton {
            id: about
            onClicked: aboutDialog.open()
            text: "\u003f"
        }

        FPTopBarButton {
            id: menu
            onClicked: contextMenu.popup()
            text: "\uf0c9"

            Menu {
                id: contextMenu
                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: photoMonitor.refreshModel()
                }
                MenuItem {
                    text: qsTr("Logout")
                    onClicked: logout()
                }
            }
        }
    }

    Connections {
        target: yearListView.StackView
        function onStatusChanged() {
            switch (yearListView.StackView.status) {
            case StackView.Inactive:
            case StackView.Deactivating:
            case StackView.Activating:
                photoMonitor.stop()
                break
            case StackView.Active:
                photoMonitor.start()
                if (pendingRefreshRequest) {
                    pendingRefreshRequest = false
                    photoMonitor.refreshModel()
                }
                break
            }
        }
    }

    Connections {
        target: photoMonitor
        function onErrorOccurred(error) {
            if (error === NetworkError.AuthenticationRequiredError)
                unauthorizedDialog.open()
        }
    }

    FPGridView {
        id: gridView
        anchors {
            top: topBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        model: photoMonitor.flashbackYears
        visible: count > 0
        cellWidth: width/3
        cellHeight: cellWidth
        clip: true
        delegate: FPPreviewElement {
            width: gridView.width/3
            height: width
            source: qmlUtils.thumbnailUrl(modelData.items[0], 1)
            mediaType: ""
            onClicked: mainStackView.push(dayViewComponent, { "model": modelData.items })
            FPPhotoOverlayText {
                text: modelData.year
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: Style.defaultMargin
            }
        }
    }

    FPText {
        id: emptyGridLabel
        anchors.fill: gridView
        visible: gridView.count <= 0
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: qsTr("No flashbacks for today!")
    }

    // Relogin
    FPPopupOk {
        id: unauthorizedDialog
        title: qsTr("Error occurred")
        text: qsTr("Token is expired or missing. Please login again to renew your authentication.")
        onAccepted: logout()
    }

    // Waiting layer
    FPPopupMessage {
        title: qsTr("Preparing data. Please wait...")
        visible: photoMonitor.working

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    // About dialog
    FPPopupBase {
        id: aboutDialog
        title: qsTr("About")
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        standardButtons: Dialog.Ok

        RowLayout {
            width: parent.width
            spacing: 30
            Image {
                Layout.preferredWidth: 92
                Layout.preferredHeight: 192
                source: "qrc:/qt/qml/FlashbackPrism/assets/icon.svg"
                fillMode: Image.PreserveAspectFit
            }
            Column {
                Layout.fillWidth: true
                spacing: Style.defaultMargin
                KeyValueLabel {
                    text: buildLine(qsTr("Author"), "Luca Carlon")
                }
                KeyValueLabel {
                    text: buildLine(qsTr("Application"), Qt.application.version)
                }
                KeyValueLabel {
                    text: buildLine(qsTr("SSL supported"), (qmlUtils.sslSupported() ? qsTr("yes") : qsTr("no")))
                }
                KeyValueLabel {
                    text: buildLine(qsTr("SSL build"), (qmlUtils.sslSupported() ? qmlUtils.sslBuildVersion() : "-"))
                }
                KeyValueLabel {
                    text: buildLine(qsTr("SSL runtime"), (qmlUtils.sslSupported() ? qmlUtils.sslRuntimeVersion() : "-"))
                }
                KeyValueLabel {
                    text: buildLine(qsTr("Qt"), qmlUtils.qtVersion())
                }
            }
        }

        component KeyValueLabel: FPText {
            width: parent.width
            elide: Text.ElideRight

            function buildLine(key, value) {
                return "<b>" + key + "</b>" + ": " + value
            }
        }
    }

    function logout() {
        mainStackView.clear()
        mainStackView.push(loginComponent)
        settingsNotifier.token = ""
    }
}
