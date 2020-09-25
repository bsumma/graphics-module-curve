cmake_minimum_required(VERSION 2.8)
PROJECT(DE_CASTLEJAU)
SET(CMAKE_BUILD_TYPE "Release")

if (!MSVC)
SET(CMAKE_CXX_FLAGS "-Wno-deprecated")
endif()

#Compile and Link GLFW
ADD_SUBDIRECTORY(glfw-3.2)
link_libraries(glfw)
include_directories(${glfw_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/glfw-3.2/deps")

add_library(glad "${CMAKE_SOURCE_DIR}/glfw-3.2/deps/glad/glad.h"
         		 "${CMAKE_SOURCE_DIR}/glfw-3.2/deps/glad.c")
				 
link_libraries(glad)

SET(MY_SHADER_PATH ${CMAKE_SOURCE_DIR}/shaders/)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/shaders/shader.h.in ${CMAKE_SOURCE_DIR}/shaders/shader.h)


include_directories( ${CMAKE_SOURCE_DIR}/source
							${CMAKE_SOURCE_DIR}/source/common
							${CMAKE_SOURCE_DIR}/shaders)
add_executable(de_casteljau WIN32 MACOSX_BUNDLE 
	source/de_casteljau.cpp 
	shaders/fshader.glsl
	shaders/vshader.glsl
	source/Curve.cpp
	source/Curve.h
	source/common/Angel.h
	source/common/mat.h
    source/common/vec.h)

#Windows cleanup
if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(de_casteljau PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

#Apple cleanup
if (APPLE)
    set_target_properties(de_casteljau PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "simple")
	
    set_target_properties(de_casteljau PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION_FULL}
    					  MACOSX_BUNDLE_ICON_FILE glfw.icns)
endif()
