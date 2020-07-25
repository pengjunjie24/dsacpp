
aux_source_directory(. hash_test)
add_executable(HashTest ${hash_test})
target_link_libraries(HashTest bitmap)