cmake_minimum_required(VERSION 3.5)

project(SimpleExample LANGUAGES CXX)

add_executable(simple main.cpp)

target_link_libraries(simple PUBLIC pulse)