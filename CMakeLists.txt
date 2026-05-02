cmake_minimum_required(VERSION 4.0.0)
project(cineris VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.c)

list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/dependencies/include/glad/glad.c)

add_executable(cineris ${SOURCES})

target_include_directories(cineris PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/dependencies/include
)

target_link_directories(cineris PRIVATE
  ${CMAKE_SOURCE_DIR}/dependencies/lib
)

target_link_libraries(cineris PRIVATE glfw3)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)
  target_link_libraries(cineris PRIVATE opengl32)
elseif(APPLE)
  find_library(COCOA_LIB Cocoa)
  find_library(IOKIT_LIB IOKit)
  find_library(COREVIDEO_LIB CoreVideo)
  find_library(OPENGL_LIB OpenGL)
  target_link_libraries(cineris PRIVATE 
    ${COCOA_LIB} 
    ${IOKIT_LIB} 
    ${COREVIDEO_LIB} 
    ${OPENGL_LIB}
  )
else() # Linux
  target_link_libraries(cineris PRIVATE GL X11 pthread Xrandr Xi dl)
endif()

add_custom_command(TARGET cineris POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/shaders"
          "${CMAKE_BINARY_DIR}/shaders"
  COMMENT "Copying shaders to build folder."
)