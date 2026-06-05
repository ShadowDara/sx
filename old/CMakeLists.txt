cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test
    src/main.cpp
    src/sort.cpp
    src/wordcounter.cpp
)


project(mylib)

add_library(
    mylib
    SHARED
    src/lib.cpp
)

# executable bauen
add_executable(
    app
    wrapper/main.cpp
)

# gegen die .so linken
target_link_libraries(
    app
    mylib
)
