cmake_minimum_required(VERSION 3.16)

project(FlashbackPrism VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

include(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqtutils/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/deps/lqobjectserializer/LQObjectSerializer/CMakeLists.txt)

qt_standard_project_setup(REQUIRES 6.6)

qt_add_executable(appFlashbackPrism
    main.cpp
)

qt_add_qml_module(appFlashbackPrism
    URI FlashbackPrism
    VERSION 1.0
    QML_FILES Main.qml
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
