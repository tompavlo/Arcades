cmake_minimum_required(VERSION 3.5)

project(Arcades VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia Sql)

set(PROJECT_SOURCES
    main.cpp
    mainmenu.cpp
    mainmenu.h
    mainmenu.ui
    resource.qrc
    clickablelabel.h
    clickablelabel.cpp
    firstinitialisation.h
    firstinitialisation.cpp
    firstinitialisation.ui
    usersmanipulation.h
    usersmanipulation.cpp
    usersmanipulation.ui

)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/icons/icon.rc")
    qt_add_executable(Arcades
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
        tictactoe.h tictactoe.cpp tictactoe.ui
        stylehelper.h stylehelper.cpp
    )
else()
    if(ANDROID)
        add_library(Arcades SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Arcades
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Arcades PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Arcades)
endif()
set_target_properties(Arcades PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Arcades
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Arcades)
endif()
