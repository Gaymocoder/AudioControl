cmake_minimum_required(VERSION 3.28)
project(GCSAudioControl_src)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(MAIN_EXE main.cpp)
add_executable(GCSAudioControl ${MAIN_EXE})