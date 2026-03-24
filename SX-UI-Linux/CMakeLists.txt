cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

add_executable(myapp src/main.cpp)

target_include_directories(myapp PRIVATE ${GTK_INCLUDE_DIRS})
target_link_libraries(myapp ${GTK_LIBRARIES})
target_compile_options(myapp PRIVATE ${GTK_CFLAGS_OTHER})
