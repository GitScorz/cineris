cmake_minimum_required(VERSION 4.0.0)
project(the_ashmoor_case VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_executable(the_ashmoor_case 
  src/main.cpp
  src/glad.c
)

target_include_directories(the_ashmoor_case PRIVATE dependencies/include)

target_link_directories(the_ashmoor_case PRIVATE dependencies/lib)
target_link_libraries(the_ashmoor_case PRIVATE glfw3)

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