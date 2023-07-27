project ("opencv03" VERSION 1.0.0)

set(OPENCV_DLL_DIR "E:/clib/opencv-3.4.10/x64/vc15/bin")
# 1 拷贝文件夹里的文件 set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
file(COPY ${OPENCV_DLL_DIR}/ DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories("E:/clib/opencv-3.4.10/include")

link_directories("E:/clib/opencv-3.4.10/x64/vc15/lib")

file(GLOB mainFiles "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
foreach(mainFile ${mainFiles})
	get_filename_component(mainFileName ${mainFile} NAME)
	MESSAGE(STATUS "mainFileName-03 ${mainFile} ${mainFileName}")
	add_executable ("${PROJECT_NAME}-${mainFileName}" ${mainFile})
	target_link_libraries("${PROJECT_NAME}-${mainFileName}" opencv_world3410d)
endforeach()




