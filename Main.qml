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
import QtQuick.Controls.Material
import "qml/views"

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    title: "FlashbackPrism"
    visibility: Qt.platform.os === "android" ? Window.FullScreen : Window.Windowed

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: settingsNotifier.token ? albumsTodayComponent : loginComponent
    }

    Component { id: loginComponent; FPLogin {} }
    Component { id: albumsTodayComponent; FPYearListView {} }
    Component { id: dayViewComponent; FPYearView {} }
    Component { id: photoViewComponent; FPPhotoView {} }
}
