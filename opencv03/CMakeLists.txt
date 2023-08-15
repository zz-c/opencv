project ("opencv03" VERSION 1.0.0)

set(OPENCV_DLL_DIR "E:/clib/opencv-4.8/x64/vc17/bin")
# 1 拷贝文件夹里的文件 set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
file(COPY ${OPENCV_DLL_DIR}/ DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories("E:/clib/opencv-4.8/include")

link_directories("E:/clib/opencv-4.8/x64/vc17/lib")

add_executable(${PROJECT_NAME} "src/case1.cpp")
target_link_libraries(${PROJECT_NAME} opencv_world480d)




