cmake_minimum_required(VERSION 3.17)
project(luajit_sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ldl")

add_executable(luajit_sandbox main.cpp)

target_link_libraries(luajit_sandbox PUBLIC ${PROJECT_SOURCE_DIR}/libs/luajit/libluajit.a)
target_include_directories(luajit_sandbox PUBLIC ${PROJECT_SOURCE_DIR}/libs/luajit/includes)


