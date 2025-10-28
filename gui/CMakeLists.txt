add_library(gui_lib
	BankCardWidget.h
	BankCardWidget.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
)

set_target_properties(gui_lib PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

target_link_libraries(gui_lib PRIVATE
	Qt6::Widgets
        soci_core
)

target_include_directories(gui_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
