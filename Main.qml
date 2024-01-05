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
    readonly property rect visibleArea: {
        Screen.orientation
        const visibleArea_ = lqtQmlUtils.visibleDisplayFrame()
        if  (visibleArea_.width === 0)
            return Qt.rect(0, 0, width, height)
        return visibleArea_
    }

    width: 1280
    height: 720
    visible: true
    title: "FlashbackPrism"
    //flags: lqtQmlUtils.isMobile() ? Qt.MaximizeUsingFullscreenGeometryHint : Qt.Window

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Component.onCompleted: {
        lqtQmlUtils.setBarColorLight(false, true)
        lqtQmlUtils.setStatusBarColor(Qt.rgba(0, 0, 0, 0))
        lqtQmlUtils.setNavBarColor(Qt.rgba(0, 0, 0, 0))
    }

    StackView {
        id: mainStackView
        anchors.fill: parent
        anchors.topMargin: {
            Screen.orientation
            return Math.max(lqtQmlUtils.safeAreaTopInset(), visibleArea.y)
        }
        anchors.leftMargin: {
            Screen.orientation
            return Math.max(lqtQmlUtils.safeAreaLeftInset(), visibleArea.x)
        }
        anchors.rightMargin: {
            Screen.orientation
            return Math.max(lqtQmlUtils.safeAreaRightInset(),
                            parent.width - visibleArea.x - visibleArea.width)
        }
        anchors.bottomMargin: {
            Screen.orientation
            return Math.max(lqtQmlUtils.safeAreaBottomInset(),
                            parent.height - visibleArea.y - visibleArea.height)
        }
        initialItem: settingsNotifier.token ? albumsTodayComponent : loginComponent

        Component.onCompleted: forceActiveFocus()

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape)
                handleBackKey(event)
        }

        function handleBackKey(event) {
            if (mainStackView.depth === 1) {
                Qt.quit()
                return
            }
            mainStackView.pop()
            event.accepted = true
        }
    }

    Component { id: loginComponent; FPLogin {} }
    Component { id: albumsTodayComponent; FPYearListView {} }
    Component { id: dayViewComponent; FPYearView {} }
    Component { id: photoViewComponent; FPPhotoView {} }
}
