    #This file is part of name_corrector
    #Copyright (C) 2020 Вершинин Максим

    #This program is free software: you can redistribute it and/or modify
    #it under the terms of the GNU General Public License as published by
    #the Free Software Foundation, either version 3 of the License, or
    #(at your option) any later version.

    #This program is distributed in the hope that it will be useful,
    #but WITHOUT ANY WARRANTY; without even the implied warranty of
    #MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    #GNU General Public License for more details.

    #You should have received a copy of the GNU General Public License
    #along with this program.  If not, see <https://www.gnu.org/licenses/>.


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
    src/connectablelist.cpp
    inc/mainwindow.h
    inc/listelement_gui.h
    inc/listelement.h
    inc/listwindow.h
    inc/newelementwindow.h
    inc/connectablelist.h
  )

target_link_libraries(name_corrector PRIVATE Qt5::Widgets)
