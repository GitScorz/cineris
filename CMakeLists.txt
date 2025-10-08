cmake_minimum_required(VERSION 4.0.0)
project(the_ashmoor_case VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.c)
add_executable(the_ashmoor_case ${SOURCES})

target_include_directories(the_ashmoor_case PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/dependencies/include
)

target_link_directories(the_ashmoor_case PRIVATE
  ${CMAKE_SOURCE_DIR}/dependencies/lib
)

target_link_libraries(the_ashmoor_case PRIVATE glfw3)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)
  target_link_libraries(the_ashmoor_case PRIVATE opengl32)
elseif(APPLE)
  find_library(COCOA_LIB Cocoa)
  find_library(IOKIT_LIB IOKit)
  find_library(COREVIDEO_LIB CoreVideo)
  find_library(OPENGL_LIB OpenGL)
  target_link_libraries(the_ashmoor_case PRIVATE 
    ${COCOA_LIB} 
    ${IOKIT_LIB} 
    ${COREVIDEO_LIB} 
    ${OPENGL_LIB}
  )
else() # Linux
  target_link_libraries(the_ashmoor_case PRIVATE GL X11 pthread Xrandr Xi dl)
endif()