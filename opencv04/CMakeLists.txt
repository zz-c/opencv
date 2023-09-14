project("opencv04" VERSION 1.0.0)

#MESSAGE(STATUS "${PROJECT_NAME} 111 ${CMAKE_INSTALL_PREFIX} ${CMAKE_BINARY_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}")

set(OPENCV_DLL_DIR "E:/clib/opencv-4.8.0/x64/vc16/bin")
# 1 拷贝文件夹里的文件 set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
file(COPY ${OPENCV_DLL_DIR}/ DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
# 2 拷贝bin文件
# file(COPY ${OPENCV_DLL_DIR} DESTINATION ${EXECUTABLE_OUTPUT_PATH})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories("E:/clib/opencv-4.8.0/include")

link_directories("E:/clib/opencv-4.8.0/x64/vc16/lib")
add_executable(${PROJECT_NAME} "src/cascader_06.cpp")

target_link_libraries(${PROJECT_NAME} opencv_world480d)

