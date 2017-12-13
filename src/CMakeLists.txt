# 2017-12-11	CMakeList.txt

cmake_minimum_required(VERSION 3.0.2)

include("./macros.cmake")
message( STATUS "macros loaded." )

#enable c++11
#toggleCpp11Compiler()

#specifies your project ( and your file )
set(PROJECT_NAME "Tonstudio")
project( ${PROJECT_NAME} )
setEnviroment()
prepareIncludesAndLibraries()

# Platform dependent definitions
add_executable( ${PROJECT_NAME}.exe ./main.cpp "qml.qrc")

if(WIN32)
    target_link_libraries( ${PROJECT_NAME}.exe
        ${PROJECT_LIBS})
else()
find_package( Threads REQUIRED)
target_link_libraries( ${PROJECT_NAME}.exe
    ${PROJECT_LIBS}
    Threads::Threads)
endif(WIN32)

setInstallProperties()

#toggleGoogleTest()
#toggleDoxyGen()
