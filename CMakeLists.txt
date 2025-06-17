# This file is part of FlashbackPrism.
#
# Copyright (c) 2023 Luca Carlon
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Author:  Luca Carlon
# Company: -
# Date:    2023.12.24

cmake_minimum_required(VERSION 3.16)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
project(FlashbackPrism VERSION 1.3.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick StateMachine Svg QuickControls2 Multimedia)
find_package(Qt6 OPTIONAL_COMPONENTS DBus)

set(ENABLE_FONT_AWESOME ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqtutils)
include(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer/CMakeLists.txt)
set(QTAES_ENABLE_INSTALL OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/qtaes)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqtutils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer)
include(${CMAKE_CURRENT_SOURCE_DIR}/deps/android_openssl/android_openssl.cmake)

add_compile_definitions(APP_VERSION="${CMAKE_PROJECT_VERSION}")

qt_standard_project_setup(REQUIRES 6.6)

qt_add_executable(appFlashbackPrism
    main.cpp
    fpphotomonitor.h fpphotomonitor.cpp
    fpqmlutils.h fpqmlutils.cpp
    fpnotificationprocessor.h fpnotificationprocessor.cpp
    rest/fploginrequest.h rest/fploginrequest.cpp
    rest/fpphotosrequest.h rest/fpphotosrequest.cpp
    rest/fprequest.h rest/fprequest.cpp
    rest/fpconfigrequest.h rest/fpconfigrequest.cpp
    rest/fpflashbackyearsrequest.h rest/fpflashbackyearsrequest.cpp
    rest/fprequestphotoaction.h rest/fprequestphotoaction.cpp
    data/fppersistentsetup.h data/fppersistentsetup.cpp
    data/fplogindata.h data/fplogindata.cpp
    data/fpqueryresultitem.h data/fpqueryresultitem.cpp
    data/fpconfigdata.h data/fpconfigdata.cpp
    ui/fpyearlistbackend.h ui/fpyearlistbackend.cpp
    ui/fpphotoprovider.h ui/fpphotoprovider.cpp
)

set_source_files_properties(qml/Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
qt_add_qml_module(appFlashbackPrism
    URI FlashbackPrism
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES qml/views/FPLogin.qml
    QML_FILES qml/Style.qml
    QML_FILES qml/components/FPPopupMessage.qml
    QML_FILES qml/components/FPPopupOk.qml
    QML_FILES qml/views/FPYearListView.qml
    QML_FILES qml/views/FPYearView.qml
    QML_FILES qml/views/FPTopBar.qml
    QML_FILES qml/components/FPPhotoOverlayText.qml
    QML_FILES qml/views/FPPhotoView.qml
    QML_FILES qml/views/FPZoomableImage.qml
    QML_FILES qml/components/FPText.qml
    QML_FILES qml/components/FPTopBarButton.qml
    QML_FILES qml/components/FPPopupBase.qml
    QML_FILES qml/components/FPPreviewElement.qml
    QML_FILES qml/components/FPOverlayFontAwesome.qml
    QML_FILES qml/components/FPGridView.qml
    RESOURCES
        assets/icon_96.png
        assets/icon.svg
        assets/icon_notification.png
        assets/logo.svg

)

# Android
set_property(TARGET appFlashbackPrism PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/android")
if (ANDROID)
    add_android_openssl_libraries(appFlashbackPrism)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appFlashbackPrism PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFlashbackPrism
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appFlashbackPrism PRIVATE
    Qt6::Quick
    Qt6::StateMachine
    Qt6::CorePrivate
    Qt6::Svg
    Qt6::QuickControls2
    Qt6::Multimedia
    lqtutilsplugin
    lqobjectserializer
)
if (Qt6DBus_FOUND)
    target_link_libraries(appFlashbackPrism PRIVATE Qt6::DBus)
endif()

include(GNUInstallDirs)
install(TARGETS appFlashbackPrism
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
