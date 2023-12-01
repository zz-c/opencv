cmake_minimum_required(VERSION 3.12)
project(opencv_find_package)

# 查找OpenCV包
find_package(OpenCV REQUIRED)

# 如果找到OpenCV，输出信息并设置包含目录
if(OpenCV_FOUND)
    message(STATUS "OpenCV found.")
    include_directories(${OpenCV_INCLUDE_DIRS})
    
    # 添加你的可执行文件或库
    add_executable(opencv_find_package src/main.cpp)
    
    # 链接OpenCV库
    target_link_libraries(opencv_find_package ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV.")
endif()
