cmake_minimum_required(VERSION 3.16)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
project(FlashbackPrism VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

include(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqtutils/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer/CMakeLists.txt)
set(QTAES_ENABLE_INSTALL OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/qtaes)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer)

qt_standard_project_setup(REQUIRES 6.6)

qt_add_executable(appFlashbackPrism
    main.cpp
    fpphotomonitor.h fpphotomonitor.cpp
    rest/fploginrequest.h rest/fploginrequest.cpp
    rest/fpphotosrequest.h rest/fpphotosrequest.cpp
    rest/fprequest.h rest/fprequest.cpp
    rest/fpconfigrequest.h rest/fpconfigrequest.cpp
    data/fppersistentsetup.h data/fppersistentsetup.cpp
    data/fplogindata.h data/fplogindata.cpp
    data/fpqueryresultitem.h data/fpqueryresultitem.cpp
    data/fpconfigdata.h data/fpconfigdata.cpp
)

set_source_files_properties(qml/Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
qt_add_qml_module(appFlashbackPrism
    URI FlashbackPrism
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES qml/views/FPLogin.qml
    QML_FILES qml/Style.qml
    QML_FILES qml/components/FPPopupMessage.qml
    QML_FILES qml/views/FPAlbumsToday.qml
)

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
    lqtutils
    lqobjectserializer
)

include(GNUInstallDirs)
install(TARGETS appFlashbackPrism
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
