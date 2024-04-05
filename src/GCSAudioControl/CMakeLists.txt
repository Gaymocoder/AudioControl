cmake_minimum_required(VERSION 3.28)
project(GCSAudioControl_src)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(Resources ${CMAKE_BINARY_DIR}/../resources/resources.rc)
set(AudioDataCalls_LIB AudioDataCalls.cpp)
set(AudioActions_LIB AudioActions.cpp)
set(Extras_LIB Extras.cpp)
set(MAIN_EXE main.cpp)

add_library(GCS.AudioDataCalls SHARED ${AudioDataCalls_LIB})
add_library(GCS.AudioActions SHARED ${AudioActions_LIB})
add_library(GCS.Extras SHARED ${Extras_LIB})
add_executable(GCSAudioControl ${MAIN_EXE} ${Resources})

target_link_libraries(GCS.AudioDataCalls GCS.Extras)
target_link_libraries(GCS.AudioActions GCS.AudioDataCalls GCS.Extras)
target_link_libraries(GCSAudioControl GCS.AudioDataCalls GCS.AudioActions GCS.Extras)