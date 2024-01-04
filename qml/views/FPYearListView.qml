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
    id: yearListView

    Component.onCompleted: photoMonitor.refreshModel()

    FPTopBar {
        id: topBar
        backVisible: false
        iconVisible: true
        appNameVisible: false

        FPText {
            text: "v" + Qt.application.version
            color: "white"
            anchors.verticalCenter: parent.verticalCenter
        }

        FPTopBarButton {
            id: about
            onClicked: aboutDialog.open()
            anchors.verticalCenter: parent.verticalCenter
            text: "\u003f"
        }

        FPTopBarButton {
            id: menu
            onClicked: contextMenu.popup()
            anchors.verticalCenter: parent.verticalCenter
            text: "\uf0c9"

            Menu {
                id: contextMenu
                MenuItem {
                    text: qsTr("Logout")
                    onClicked: {
                        mainStackView.clear()
                        mainStackView.push(loginComponent)
                        settingsNotifier.uname = ""
                        settingsNotifier.pwd = ""
                        settingsNotifier.token = ""
                    }
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
                break
            }
        }
    }

    GridView {
        id: gridView
        anchors {
            top: topBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        model: photoMonitor.flashbackYears
        cellWidth: width/3
        cellHeight: cellWidth
        clip: true
        delegate: Image {
            width: gridView.width/3
            source: qmlUtils.thumbnailUrl(modelData.items[0], 1)
            fillMode: Image.PreserveAspectFit
            FPPhotoOverlayText {
                text: modelData.year
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: Style.defaultMargin
            }
            MouseArea {
                anchors.fill: parent
                onClicked: mainStackView.push(dayViewComponent, { "model": modelData.items })
            }
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
                    text: buildLine(qsTr("Application version"), Qt.application.version)
                }
                KeyValueLabel {
                    text: buildLine(qsTr("SSL supported"), (qmlUtils.sslSupported() ? qsTr("yes") : qsTr("no")))
                }
                KeyValueLabel {
                    text: buildLine(qsTr("SSL build version"), (qmlUtils.sslSupported() ? qmlUtils.sslBuildVersion() : "-"))
                }
                KeyValueLabel {
                    text: buildLine(qsTr("SSL runtime version"), (qmlUtils.sslSupported() ? qmlUtils.sslRuntimeVersion() : "-"))
                }
                KeyValueLabel {
                    text: buildLine(qsTr("Qt version"), qmlUtils.qtVersion())
                }
            }
        }

        component KeyValueLabel: FPText {
            width: parent.width
            lineHeight: 1.5
            lineHeightMode: Text.ProportionalHeight
            elide: Text.ElideRight

            function buildLine(key, value) {
                return "<b>" + key + "</b>" + ": " + value
            }
        }
    }
}
