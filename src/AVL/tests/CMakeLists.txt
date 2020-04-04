
aux_source_directory(. avl_test)
add_executable(AVLTest ${avl_test})
target_link_libraries(AVLTest print bitmap)