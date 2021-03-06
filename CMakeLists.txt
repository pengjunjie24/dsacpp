cmake_minimum_required(VERSION 2.8.12)

project(pallette C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

#设置C++编译选项
set(CXX_FLAGS
    -g
    -ffunction-sections
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wno-unused-parameter
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -march=native
    -rdynamic
)

#设置编译器
set(CMAKE_CXX_COMPILER "g++")
add_compile_options(-std=c++11)
#设置Debug模式选项
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g2 -ggdb")
#设置Release模式选项
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -finline-limit=1000 -DNDEBUG")
#可执行文件存放路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#库文件存放路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_INSTALL_PREFIX /opt/dsacpp)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

add_subdirectory(Stack)
add_subdirectory(Bitmap)
add_subdirectory(BinTree)
add_subdirectory(AVL)
add_subdirectory(Splay)
add_subdirectory(Fibonacci)
add_subdirectory(BTree)
add_subdirectory(RedBlack)
add_subdirectory(HashTable)