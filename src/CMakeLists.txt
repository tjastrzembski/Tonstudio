# 2018-02-13	CMakeList.txt

cmake_minimum_required(VERSION 3.9)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_CURRENT_BINARY_DIR "./..")

add_executable( ${PROJECT_NAME} ./main.cpp ./qml.qrc )
prepareIncludesAndLibraries()
setInstallProperties()



		
		
