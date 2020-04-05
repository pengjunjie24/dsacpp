
aux_source_directory(. splay_test)
add_executable(SplayTest ${splay_test})
target_link_libraries(SplayTest print bitmap)