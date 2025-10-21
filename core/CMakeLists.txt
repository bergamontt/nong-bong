add_executable(nong_bong
    main.cpp
)

target_link_libraries(nong_bong PRIVATE
    Qt6::Widgets
    gui_lib
)
