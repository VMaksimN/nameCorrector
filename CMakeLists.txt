cmake_minimum_required(VERSION 3.5)

project(name_corrector LANGUAGES CXX)

include_directories(inc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

  add_executable(name_corrector
    src/main.cpp
    src/mainwindow.cpp
    src/listelement.cpp
    src/listelement_gui.cpp
    src/listwindow.cpp
    src/newelementwindow.cpp
    inc/mainwindow.h
    inc/listelement_gui.h
    inc/listelement.h
    inc/listwindow.h
    inc/newelementwindow.h
  )

target_link_libraries(name_corrector PRIVATE Qt5::Widgets)
