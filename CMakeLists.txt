cmake_minimum_required(VERSION 3.1...3.27)
project(
  ReEntrantLockProject
  VERSION 1.0
  LANGUAGES CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

include(CTest)

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Werror)
endif()

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(app)
add_subdirectory(libs)
add_subdirectory(tests)