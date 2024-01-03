cmake_minimum_required(VERSION 3.16)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
project(FlashbackPrism VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick StateMachine Svg QuickControls2)
find_package(Qt6 OPTIONAL_COMPONENTS DBus)

set(ENABLE_FONT_AWESOME ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqtutils)
include(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer/CMakeLists.txt)
set(QTAES_ENABLE_INSTALL OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/qtaes)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqtutils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer)

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
    QML_FILES qml/components/FPText.qml
    QML_FILES qml/components/FPTopBarButton.qml
    QML_FILES qml/components/FPPopupBase.qml
    RESOURCES
        assets/icon_96.png
        assets/icon.svg
)

# Android
set_property(TARGET appFlashbackPrism PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/android")

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
