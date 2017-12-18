#macros.cmake 2017-12-11

# checkOS()
# checks, whether WIN32, LINUX or APPLE is in use
MACRO( checkOS )
if(WIN32)
    message(STATUS "win32")
elseif(APPLE)
    message(STATUS "mac")
else(WIN32)
    message(STATUS "linux")
endif(WIN32)
ENDMACRO( checkOS )


# setEnviroment()
# sets the project specified enviroment
MACRO( setEnviroment )
	SET ( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/.. CACHE PATH "Path to install the distribution" FORCE)
	SET ( PROJECT_LIB ${CMAKE_INSTALL_PREFIX}/install/lib CACHE PATH "Path to install the project library")
	SET ( PROJECT_BIN ${CMAKE_INSTALL_PREFIX}/install/bin CACHE PATH "Path to install the module executables")	
	SET ( PROJECT_INC ${CMAKE_INSTALL_PREFIX}/install/include CACHE PATH "Path to install the project headers needed by the lib")
ENDMACRO( setEnviroment )

# prepareIncludesAndLibraries()
# sets the specific includes and libraries needed by this project
MACRO( prepareIncludesAndLibraries )
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_AUTORCC ON)

    SET(PROJECT_LIBS ${PROJECT_NAME}.lib CACHE FILE "The Project library")
	INCLUDE("./SourceFiles.cmake")
	ADD_LIBRARY(${PROJECT_LIBS} STATIC  ${ProjectSources})
	
	SET(PROJECT_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include "Path to project includes")
	
	if(WIN32)
        SET(OS_SUFFIX win32)
    elseif(APPLE)
        SET(OS_SUFFIX mac)
    else(WIN32)
        SET(OS_SUFFIX linux)
    endif(WIN32)
    SET(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS_SUFFIX} "Path to project libs")
	
	SET(CPP_REDIS_LIBS "cpp_redis" CACHE PATH "cpp_redis libs to link to")
	SET(TACOPIE_LIBS "tacopie" CACHE PATH "tacopie libs to link to")
	SET(BSON_LIBS "bson-1.0-static" CACHE PATH "BSON libs to link to")
	SET(MONGOC_LIBS "mongoc-1.0-static" CACHE PATH "mongo c libs to link to")

	SET(QT_DIR ${QT_DIR} "Path to Qt")
	
	INCLUDE_DIRECTORIES(
		${PROJECT_INC_DIR}
	)
	LINK_DIRECTORIES(
		${PROJECT_LIB_DIR}
	)
	
    find_package(Qt5 5.10.0 COMPONENTS Core Quick Qml REQUIRED)

	TARGET_LINK_LIBRARIES( 
		${PROJECT_LIBS}
		${CPP_REDIS_LIBS}
		${TACOPIE_LIBS}
		Qt5::Core
		Qt5::Quick
		Qt5::Qml
	)
ENDMACRO( prepareIncludesAndLibraries )

# setInstallProperties()
# sets the install path and changes the name of lib and exec
MACRO( setInstallProperties )
	SET_TARGET_PROPERTIES(${PROJECT_NAME}.exe PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
	SET_TARGET_PROPERTIES(${PROJECT_LIBS} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

	INSTALL(TARGETS ${PROJECT_LIBS} 
		ARCHIVE DESTINATION ${PROJECT_LIB}
		LIBRARY DESTINATION ${PROJECT_LIB}
		)

	INSTALL(TARGETS ${PROJECT_NAME}.exe RUNTIME DESTINATION ${PROJECT_BIN})

	get_target_property( _SOURCE_FILES ${PROJECT_LIBS} SOURCES )
	set( _HEADER_FILES )
	foreach( _FILE ${_SOURCE_FILES} )
		get_filename_component( _EXTENSION_TMP ${_FILE} EXT )
		string( TOLOWER ${_EXTENSION_TMP} _EXTENSION )
		if( "${_EXTENSION}" STREQUAL ".h" )
			get_filename_component( _PATH ${_FILE} PATH )
			string( REPLACE "src" "" _PATH "${_PATH}" )
			string( REPLACE "Src" "" _PATH "${_PATH}" )
			string( REPLACE "source" "" _PATH "${_PATH}" )
			string( REPLACE "Source" "" _PATH "${_PATH}" )
			string( REPLACE "include" "" _PATH "${_PATH}" )
			string( REPLACE "Include" "" _PATH "${_PATH}" )
			install( FILES 	${_FILE} DESTINATION "${PROJECT_INC}/${_PATH}")
		endif( "${_EXTENSION}" STREQUAL ".h" )
	endforeach( _FILE _SOURCE_FILES )

ENDMACRO( setInstallProperties )

#toggleCpp11Compiler()
#sets the compiling settings to c++11
MACRO( toggleCpp11Compiler )
	SET(GCChome /tools/modulesystem/tools/gcc/gcc-4.8.2/install/sled11.x86_64.gcc.release)
	#for setting the c++11 flag we need a better compiler
	#NOTE: if make sure you loaded the gcc-4.8.2 module before compiling
	SET(CMAKE_C_COMPILER "${GCChome}/bin/gcc-4.8.2")
	SET(CMAKE_CXX_COMPILER "${GCChome}/bin/g++-4.8.2")
	#NOTE: if it's not working, you probably have to generate it twice
	SET(CMAKE_CXX_COMPILER_ARG1 "--std=c++11")
	MESSAGE( STATUS "c++11 enabled." )
endmacro( toggleCpp11Compiler )

#toggleGoogleTest()
MACRO ( toggleGoogleTest )
	SET(GTEST_INCDIR "$ENV{GTEST_ROOT}/include" CACHE PATH "Path to gtest headers")
	SET(GTEST_LIBDIR "$ENV{GTEST_ROOT}/lib" CACHE PATH "Path to gtest libs")
	SET(GTEST_LIBS "gtest;gtest_main")

	SET(BUILD_TESTS ON CACHE BOOL "Build tests")
	IF(BUILD_TESTS)
		SET(BUILD_TEST_AUTO_EXEC OFF CACHE BOOL "Run tests automatically after building.")
		SET(BUILD_TEST_CPP_CHECK OFF CACHE BOOL "Run cpp check tests.")
		ADD_SUBDIRECTORY( "Test")
	ENDIF(BUILD_TESTS)
ENDMACRO( toggleGoogleTest )

#toggleDoxyGen()
MACRO ( toggleDoxyGen )
	SET(BUILD_DOXYGEN_DOCU ON CACHE BOOL "Build API doc")
	IF(BUILD_DOXYGEN_DOCU)
		ADD_SUBDIRECTORY( "../docs" "../docs" )
	ENDIF(BUILD_DOXYGEN_DOCU)
ENDMACRO( toggleDoxyGen )

#experimental, does not have any functionality in this context
function(WinDeployQt)
        cmake_parse_arguments(_deploy
                "COMPILER_RUNTIME;FORCE"
                "TARGET"
                "INCLUDE_MODULES;EXCLUDE_MODULES"
                ${ARGN}
                )

        if(NOT _deploy_TARGET)
                message(FATAL_ERROR "A TARGET must be specified")
        endif()
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                list(APPEND _ARGS --debug)
        elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
                list(APPEND _ARGS --release-with-debug-info)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
                list(APPEND _ARGS --release)
        endif()
        if(_deploy_COMPILER_RUNTIME)
                list(APPEND _ARGS --compiler-runtime)
        endif()
        if(_deploy_FORCE)
                list(APPEND _ARGS --force)
        endif()

        foreach(mod ${_deploy_INCLUDE_MODULES})
                string(TOLOWER ${mod} mod)
                string(REPLACE "qt5::" "" mod ${mod})
                list(APPEND _ARGS "--${mod}")
        endforeach()
        foreach(mod ${_deploy_EXCLUDE_MODULES})
                string(TOLOWER ${mod} mod)
                string(REPLACE "qt5::" "" mod ${mod})
                list(APPEND _ARGS "--no-${mod}")
        endforeach()

        find_program(_deploy_PROGRAM windeployqt
                PATHS $ENV{QTDIR}/bin/)
        if(_deploy_PROGRAM)
                message(STATUS "Found ${_deploy_PROGRAM}")
        else()
                message(FATAL_ERROR "Unable to find windeployqt")
        endif()

        if(COMPILER_RUNTIME AND NOT $ENV{VVVV})
                message(STATUS "not set, the VC++ redistributable installer will NOT be bundled")
        endif()

endfunction()
