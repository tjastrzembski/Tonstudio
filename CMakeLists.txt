# 2017-12-11	CMakeList.txt

cmake_minimum_required(VERSION 3.9)

include("./macros.cmake")
message( STATUS "macros loaded." )

#sets the source folder into src

#specifies your project ( and your file )
set(PROJECT_NAME Tonstudio )
project( ${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/mongo-cxx-driver/lib/cmake")


setEnviroment()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Platform dependent definitions
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")



