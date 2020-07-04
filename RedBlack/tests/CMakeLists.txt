
aux_source_directory(. redblack_test)
add_executable(RedBlackTest ${redblack_test})
target_link_libraries(RedBlackTest bitmap)