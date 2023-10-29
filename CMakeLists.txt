cmake_minimum_required(VERSION 3.14)

project(LinRegExample LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Charts)

qt_standard_project_setup(REQUIRES 6.3)

qt_add_executable(${PROJECT_NAME}
  chartview_t.h chartview_t.cpp
  mainwindow.cpp mainwindow.h
  main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
