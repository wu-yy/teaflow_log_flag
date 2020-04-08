cmake_minimum_required(VERSION 3.13)
project(teaflow_test)

set(CMAKE_CXX_STANDARD 14)

# 设置include文件
INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# lib 文件
SET(ft_sources  log.cpp  tracer.cpp flags.h flags.cpp)
ADD_LIBRARY(ft ${ft_sources})
#TARGET_LINK_LIBRARIES(ft ${OpenCV_LIBS})

add_executable(teaflow_test main.cpp)
TARGET_LINK_LIBRARIES(teaflow_test ft)

##可以有
#install(TARGETS teaflow_test DESTINATION bin)

