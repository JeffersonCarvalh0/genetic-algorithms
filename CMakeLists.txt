cmake_minimum_required(VERSION 3.7.2)
set(CMAKE_CXX_STANDARD 14)
project(KSP)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

add_executable(main ${SOURCE_FILES})
target_include_directories(main PUBLIC ${HEADERS})
