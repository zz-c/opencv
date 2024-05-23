project("opencvtest" VERSION 1.0.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories("E:/clib/opencv-4.8.0/include")

link_directories("E:/clib/opencv-4.8.0/x64/vc16/lib")
add_executable(${PROJECT_NAME} "src/main-004.cpp")
target_link_libraries(${PROJECT_NAME} opencv_world480d)

