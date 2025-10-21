qt_add_library(gui_lib
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(gui_lib PRIVATE Qt6::Widgets)

target_include_directories(gui_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
