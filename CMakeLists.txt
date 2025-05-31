cmake_minimum_required(VERSION 3.16)

project(Qt0 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 👇 Добавим определение версии Qt явно
set(QT_VERSION_MAJOR 6)  # или 5, если работаешь с Qt5

# 🔍 Добавим нужные модули
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network WebSockets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)


# 🧩 Добавим исходные файлы
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    loginwindow.h
    loginwindow.cpp
    gamewindow.cpp
    gamewindow.h
)

# 🎯 Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Qt0
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        loginwindow.h loginwindow.cpp
        dialog.ui
        loginwindows.ui
        gamewindow.h gamewindow.cpp
        chesspieceitem.h chesspieceitem.cpp
        chess_validator.py
        chesspieceitem.h chesspieceitem.cpp
        boardsquare.h boardsquare.cpp
        modeselectionwindow.h modeselectionwindow.cpp
        chess_bot.py
    )
else()
    if(ANDROID)
        add_library(Qt0 SHARED ${PROJECT_SOURCES})
    else()
        add_executable(Qt0 ${PROJECT_SOURCES})
    endif()
endif()

# 🔗 Линковка необходимых модулей
target_link_libraries(Qt0 PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Svg

)

# ⚙️ Установка свойств
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Qt0)
endif()

set_target_properties(Qt0 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Qt0
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ✅ Финализация Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Qt0)
endif()
