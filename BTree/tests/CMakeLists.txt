
aux_source_directory(. btree_test)
add_executable(BTreeTest ${btree_test})
target_link_libraries(BTreeTest bitmap)