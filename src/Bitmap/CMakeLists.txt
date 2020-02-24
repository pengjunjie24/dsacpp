
aux_source_directory(. bitmap_src)
add_library(bitmap ${bitmap_src})

add_subdirectory(tests)