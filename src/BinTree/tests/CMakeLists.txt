
aux_source_directory(. bintree_test)
add_executable(MainBinTreeTest ${bintree_test})
target_link_libraries(MainBinTreeTest print bitmap)