cmake_minimum_required(VERSION 3.6)
project(yuyv2rgb)

include_directories(/usr/local/ffmpeg/include /usr/include/vigra)
link_directories(/usr/local/ffmpeg/lib  /usr/lib)
link_libraries(avformat avcodec avutil avdevice swresample swscale vigraimpex)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES  main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/ye/Desktop/yuyv2rgb/")
add_executable(yuyv2rgb ${SOURCE_FILES})