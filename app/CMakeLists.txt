add_executable(nong_bong
        main.cpp
        ${CMAKE_SOURCE_DIR}/gui/resources.qrc
)

target_link_libraries(nong_bong PRIVATE
        Qt6::Widgets
        gui_lib
        core_lib
)

add_custom_command(TARGET nong_bong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/core/resources/database.sqlite
        $<TARGET_FILE_DIR:nong_bong>/database.sqlite
)