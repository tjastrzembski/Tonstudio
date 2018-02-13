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
  message(STATUS "checkIncludeDir:")
 get_property(dirs TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
    foreach(dir ${dirs} )
        message(STATUS "dir ='${dir}'")
    endforeach()
ENDFUNCTION( checkIncludeDir )

FUNCTION( checkLinkDir )
  message(STATUS "checkLinkDir:")
 get_property(dirs TARGET  ${PROJECT_NAME} PROPERTY LINK_LIBRARIES)
    foreach(dir ${dirs} )
        message(STATUS "dir ='${dir}'")
    endforeach()
ENDFUNCTION( checkLinkDir )

# setEnviroment()
# sets the project specified enviroment
MACRO( setEnviroment )
        SET ( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "Path to install the distribution" FORCE)
        SET ( PROJECT_LIB "${CMAKE_INSTALL_PREFIX}/install/lib" CACHE PATH "Path to install the project library")
        SET ( PROJECT_BIN "${CMAKE_INSTALL_PREFIX}/install/bin" CACHE PATH "Path to install the module executables")
        SET ( PROJECT_INC "${CMAKE_INSTALL_PREFIX}/install/include" CACHE PATH "Path to install the project headers needed by the lib")
ENDMACRO( setEnviroment )

# gatherProjectFiles()
#gather all project files
MACRO( gatherProjectFiles )
SET( RELATIVE_DIR "." )
FILE( GLOB DIR_FILES
    "${RELATIVE_DIR}/*.h"
    "${RELATIVE_DIR}/*.cpp"
    "${RELATIVE_DIR}/*.qrc"
)
foreach( FILE ${DIR_FILES} )
        list( APPEND ProjectSources "${FILE}" )
endforeach()

ENDMACRO( gatherProjectFiles )


# prepareIncludesAndLibraries()
# sets the specific includes and libraries needed by this project
MACRO( prepareIncludesAndLibraries )

if(WIN32)
    find_package(Boost 1.64.0 REQUIRED)
    TARGET_INCLUDE_DIRECTORIES(
        ${PROJECT_NAME}
        PUBLIC ${Boost_INCLUDE_DIRS}
        )
    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        PUBLIC ${Boost_LIBRARIES}
        )
endif(WIN32)

    find_package(libbsoncxx REQUIRED)
    find_package(libmongocxx REQUIRED)
    find_package(Threads REQUIRED)
    find_package(Qt5 5.10.0 COMPONENTS Core Quick Qml REQUIRED)

	#check which includes and libs have to be used
    checkOS(OS_SUFFIX)

	#includes 
        SET(PROJECT_INC_DIR "${CMAKE_SOURCE_DIR}/include/${OS_SUFFIX}" CACHE PATH "Path to project includes")
        SET(REDIS_API_INC "${PROJECT_INC_DIR}/redis API" CACHE PATH "" )
        SET(PORTAUDIO_INC "${PROJECT_INC_DIR}/portaudio" CACHE PATH "")
        #SET(BSON_INC "${PROJECT_INC_DIR}/mongoDB API/libbson-1.0" CACHE PATH "" )
        #SET(MONGOC_INC "${PROJECT_INC_DIR}/mongoDB API/libmongoc-1.0" CACHE PATH "")
        #SET(MONGOCXX_INC "${PROJECT_INC_DIR}/mongoDB API/mongocxx" CACHE PATH "")

        #libs
        gatherProjectFiles()
        SET(PROJECT_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/${OS_SUFFIX}" CACHE PATH "Path to project libs")
        if(WIN32)
            SET(OPENSSL_INC "${PROJECT_INC_DIR}/openssl" CACHE PATH "")
            SET(CPP_REDIS_LIBS "${PROJECT_LIB_DIR}/cpp_redis.lib" CACHE FILEPATH  "cpp_redis libs to link to")
            SET(TACOPIE_LIBS "${PROJECT_LIB_DIR}/tacopie.lib" CACHE FILEPATH  "tacopie libs to link to")
            SET(PORTAUDIO_LIBS "${PROJECT_LIB_DIR}/portaudio_x64.lib" CACHE FILEPATH "portaudio libs to link to")
            SET(CASSANDRA_LIBS "${PROJECT_LIB_DIR}/cassandra.lib" CACHE FILEPATH "cassandra libs to link to")
            SET(LIBUV_LIBS "${PROJECT_LIB_DIR}/libuv.lib" CACHE FILEPATH "libuv libs to link to")
            SET(LIBEAY_LIBS "${PROJECT_LIB_DIR}/libeay32.lib" CACHE FILEPATH "openSSL libs to link to")
            SET(SSLEAY_LIBS "${PROJECT_LIB_DIR}/ssleay32.lib" CACHE FILEPATH "openSSL2 libs to link to")
            #SET(BSON_LIBS "${PROJECT_LIB_DIR}/bson-1.0.lib" CACHE FILEPATH  "BSON libs to link to")
            #SET(BSONCXX_LIBS "${PROJECT_LIB_DIR}/bsoncxx.lib" CACHE FILEPATH  "BSON libs to link to")
            #SET(MONGOC_LIBS "${PROJECT_LIB_DIR}/mongoc-1.0.lib" CACHE FILEPATH  "mongo c libs to link to")
            #SET(MONGOCXX_LIBS "${PROJECT_LIB_DIR}/mongocxx.lib" CACHE FILEPATH  "mongo c libs to link to")
        elseif(APPLE)
            SET(CASSANDRA_INC "/usr/local/include" CACHE PATH "")
            #SET(LIBUV_INC "/usr/local/include" CACHE PATH "")
            SET(CPP_REDIS_LIBS "${PROJECT_LIB_DIR}/libcpp_redis.a" CACHE FILEPATH  "cpp_redis libs to link to")
            SET(TACOPIE_LIBS "${PROJECT_LIB_DIR}/libtacopie.a" CACHE FILEPATH  "tacopie libs to link to")
            SET(PORTAUDIO_LIBS "${PROJECT_LIB_DIR}/libportaudio.dylib" CACHE FILEPATH  "portaudio libs to link to")
            SET(CASSANDRA_LIBS "/usr/local/lib/libcassandra.dylib" CACHE FILEPATH "cassandra libs to link to")
            SET(LIBUV_LIBS "/usr/local/lib/libuv.dylib" CACHE FILEPATH "libuv libs to link to")
            #TODO: neccessary APPLE STUFF
        else(WIN32)
            #Linux
        endif(WIN32)

        if(NOT Qt_FOUND)
             SET(QT_DIR ${QT_DIR} )#"Path to Qt")
        endif(NOT Qt_FOUND)

        if(NOT libmongocxx_FOUND)
             SET( LIBBSONCXX_INCLUDE_DIRS "${PROJECT_INC_DIR}/mongoDB API/mongocxx" CACHE PATH  "_")
             SET( LIBBSONCXX_LIBRARIES "${PROJECT_LIB_DIR}/bsoncxx.lib" CACHE FILEPATH "_" )
             SET( LIBMONGOCXX_LIBRARIES "${PROJECT_LIB_DIR}/mongocxx.lib" CACHE FILEPATH "_" )
         else(NOT libmongocxx_FOUND)
             if(WIN32)
                SET( LIBBSONCXX_LIBRARIES "C:/mongo-cxx-driver/lib/bsoncxx.lib" CACHE FILEPATH "_" FORCE)
                SET( LIBMONGOCXX_LIBRARIES "C:/mongo-cxx-driver/lib/mongocxx.lib" CACHE FILEPATH "_" FORCE)
            elseif(APPLE)
                SET( LIBBSONCXX_LIBRARIES "/usr/local/lib/libbsoncxx.dylib" CACHE FILEPATH "_" FORCE)
                SET( LIBMONGOCXX_LIBRARIES "/usr/local/lib/libmongocxx.dylib" CACHE FILEPATH "_" FORCE )
            else(WIN32)
                #Linux
            endif(WIN32)
         endif(NOT libmongocxx_FOUND)


        TARGET_INCLUDE_DIRECTORIES(
            ${PROJECT_NAME}
            PRIVATE ${LIBBSONCXX_INCLUDE_DIRS}
            PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
            PUBLIC ${REDIS_API_INC}
            PRIVATE ${PORTAUDIO_INC}
            PRIVATE ${CASSANDRA_INC}
            PRIVATE ${LIBUV_INC}
            PRIVATE ${OPENSSL_INC}
            #${BSON_INC}
            #${MONGOC_INC}
            #${MONGOCXX_INC}
        )
        #checkIncludeDir()

        TARGET_LINK_LIBRARIES(
            ${PROJECT_NAME}
            PRIVATE ${LIBBSONCXX_LIBRARIES}
            PRIVATE ${LIBMONGOCXX_LIBRARIES}
            PUBLIC ${SSLEAY_LIBS} 
            PUBLIC ${CPP_REDIS_LIBS}
            PUBLIC ${TACOPIE_LIBS}
            PUBLIC ${PORTAUDIO_LIBS}
            PUBLIC ${CASSANDRA_LIBS}
            PUBLIC ${LIBUV_LIBS}
            PUBLIC ${LIBEAY_LIBS}
            Qt5::Core
            Qt5::Quick
            Qt5::Qml
            Threads::Threads
            #${BSON_LIBS}
            #${BSONCXX_LIBS}
            #${MONGOC_LIBS}
            #${MONGOCXX_LIBS}
	)

    TARGET_SOURCES(
        ${PROJECT_NAME}
        PRIVATE ${ProjectSources}
        )
    checkLinkDir()

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
#deprecated at this time
# sets the install path and changes the name of lib and exec
MACRO( setInstallProperties )
        SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJECT_BIN})

	if(WIN32)
		WinDeployQt()
		copyDLLs()
	endif(WIN32)

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
