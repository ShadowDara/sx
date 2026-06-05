cmake_minimum_required(VERSION 3.13)
project(sfplib)

add_library(sfplib)
target_include_directories(sfplib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(sfplib PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp"
)
set_property(TARGET sfplib PROPERTY CXX_STANDARD 20)
