#macros.cmake 2017-12-11

# checkOS()
# checks, whether WIN32, LINUX or APPLE is in use
FUNCTION( checkOS OS)
if(WIN32)
	SET(${OS} win32 PARENT_SCOPE)
elseif(APPLE)
	SET(${OS} mac PARENT_SCOPE)
else(WIN32)
	SET(${OS} linux PARENT_SCOPE)
endif(WIN32)
ENDFUNCTION( checkOS )

FUNCTION( checkIncludeDir )
 get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(dir ${dirs} )
        message(STATUS "dir ='${dir}'")
    endforeach()
ENDFUNCTION( checkIncludeDir )

FUNCTION( checkLinkDir )
 get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
    foreach(dir ${dirs} )
        message(STATUS "dir ='${dir}'")
    endforeach()
ENDFUNCTION( checkLinkDir )

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

	#check which includes and libs have to be used
    checkOS(OS_SUFFIX)
	
	#includes 
    SET(PROJECT_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include/${OS_SUFFIX}) #"Path to project includes")
	SET(REDIS_API_INC "${PROJECT_INC_DIR}/redis API" ) 						#"Path to project includes")
	SET(BSON_INC "${PROJECT_INC_DIR}/mongoDB API/libbson-1.0" ) 			#"Path to project includes")
	SET(MONGOC_INC "${PROJECT_INC_DIR}/mongoDB API/libmongoc-1.0") 			#"Path to project includes")
	SET(PORTAUDIO_INC "${PROJECT_INC_DIR}/portaudio")
        SET(CASSANDRA_INC "${PROJECT_INC_DIR}/cassandra")
        SET(LIBUV_INC "${PROJECT_INC_DIR}/libuv")
        SET(OPENSSL_INC "${PROJECT_INC_DIR}/openssl")


        #libs
        SET(PROJECT_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib/${OS_SUFFIX}" )	#"Path to project libs"
        if(WIN32)
            SET(CPP_REDIS_LIBS "cpp_redis.lib" CACHE FILEPATH  "cpp_redis libs to link to")
            SET(TACOPIE_LIBS "tacopie.lib" CACHE FILEPATH  "tacopie libs to link to")
            SET(BSON_LIBS "bson-1.0.lib" CACHE FILEPATH  "BSON libs to link to")
            SET(MONGOC_LIBS "mongoc-1.0.lib" CACHE FILEPATH  "mongo c libs to link to")
            SET(PORTAUDIO_LIBS "portaudio_x64.lib" CACHE FILEPATH "portaudio libs to link to")
            SET(CASSANDRA_LIBS "cassandra.lib" CACHE FILEPATH "cassandra libs to link to")
            SET(LIBUV_LIBS "libuv.lib" CACHE FILEPATH "libuv libs to link to")
            SET(LIBEAY_LIBS "libeay32.lib" CACHE FILEPATH "openSSL libs to link to")

            SET(SSLEAY_LIBS "ssleay32.lib" CACHE FILEPATH "openSSL2 libs to link to")
        elseif(APPLE)
            SET(CPP_REDIS_LIBS "libcpp_redis.a" CACHE FILEPATH  "cpp_redis libs to link to")
            SET(TACOPIE_LIBS "libtacopie.a" CACHE FILEPATH  "tacopie libs to link to")
            SET(BSON_LIBS "libbson-1.0.dylib" CACHE FILEPATH  "BSON libs to link to")
            SET(MONGOC_LIBS "libmongoc-1.0.dylib" CACHE FILEPATH  "mongo c libs to link to")
            SET(PORTAUDIO_LIBS "libportaudio.dylib" CACHE FILEPATH  "portaudio libs to link to")
        else(WIN32)
            SET(CPP_REDIS_LIBS "libcpp_redis.a" CACHE FILEPATH  "cpp_redis libs to link to")
            SET(TACOPIE_LIBS "libtacopie.a" CACHE FILEPATH  "tacopie libs to link to")
            SET(BSON_LIBS "libbson-1.0.dylib" CACHE FILEPATH  "BSON libs to link to")
            SET(MONGOC_LIBS "libmongoc-1.0.dylib" CACHE FILEPATH  "mongo c libs to link to")
            SET(PORTAUDIO_LIBS "libportaudio.dylib" CACHE FILEPATH  "portaudio libs to link to")
        endif(WIN32)

   # SET(ADDITIONAL_LIBS "-lsasl2;-lssl;-lcrypto" CACHE PATH "additional libs to link")

	SET(QT_DIR ${QT_DIR} )#"Path to Qt")
	
	INCLUDE_DIRECTORIES(
                ${REDIS_API_INC}
		${BSON_INC}
		${MONGOC_INC}
		${PORTAUDIO_INC}
                ${CASSANDRA_INC}
                ${LIBUV_INC}
                ${OPENSSL_INC}
	)
	#checkIncludeDir()
		
	LINK_DIRECTORIES(
		${PROJECT_LIB_DIR}
	)
	#checkLinkDir()
	
    find_package(Qt5 5.10.0 COMPONENTS Core Quick Qml REQUIRED)

	TARGET_LINK_LIBRARIES( 
		${PROJECT_LIBS}
		${CPP_REDIS_LIBS}
		${TACOPIE_LIBS}
		${MONGOC_LIBS}
                ${PORTAUDIO_LIBS}
                ${CASSANDRA_LIBS}
                ${LIBUV_LIBS}
                ${LIBEAY_LIBS}
		#${ADDITIONAL_LIBS}
		Qt5::Core
		Qt5::Quick
		Qt5::Qml
	)
	
ENDMACRO( prepareIncludesAndLibraries )

#experimental
MACRO( WinDeployQt )
        find_program(wDqT windeployqt
                PATHS ${Qt5_DIR}/../../../bin/)

if(wDqT)
		INSTALL(CODE "MESSAGE(\"Found ${wDqT}\")")
		INSTALL(CODE "execute_process(COMMAND ${wDqT}  \"--qmldir\" \"${CMAKE_CURRENT_SOURCE_DIR}\" \"${PROJECT_BIN}\")")
	else()
		INSTALL(CODE "MESSAGE(\"windowsDeployQt not found ${wDqT}.\")")
	endif()
					
ENDMACRO( WinDeployQt )

#experimental
MACRO( CopyDLLs )    
	checkOS(OS_SUFFIX)
	INSTALL(DIRECTORY ${PROJECT_LIB_DIR}/bin DESTINATION ${PROJECT_BIN}/..)	
ENDMACRO( CopyDLLs )


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

	if(WIN32)
		WinDeployQt()
		copyDLLs()
	endif(WIN32)
	
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
