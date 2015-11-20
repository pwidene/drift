cmake_minimum_required(VERSION 2.8.3)
cmake_policy(VERSION 2.8.3)
project(drift)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif()

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.8 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
set( CMAKE_SKIP_BUILD_RPATH FALSE )
set( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )
set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )

if( NOT CERCS_PREFIX )
  MESSAGE( STATUS "CERCS_PREFIX not set, using CMAKE_INSTALL_PREFIX (${CMAKE_INSTALL_PREFIX})" )
  set( CERCS_PREFIX ${CMAKE_INSTALL_PREFIX} )
endif()


include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckTypeSize)
include(CheckStructHasMember)
include(CreateLibtoolFile)
include(FindCERCSProject)

add_definitions(-DBOOST_ALL_DYN_LINK)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

find_package( Boost COMPONENTS thread program_options system log log_setup chrono )

if( NOT REDOX_PREFIX )
  MESSAGE( FATAL_ERROR "REDOX_PREFIX not set!! Redox is required in order to build Drift. Stopping.")
endif()

if( NOT LIBJSON_PREFIX )
  MESSAGE( FATAL_ERROR "LIBJSON_PREFIX not set. LibJSON is required in order to build Drift. Stopping.")
endif()

if( OTHER_PREFIX )
  set (OTHER_PREFIX /usr/local)
endif()

FOREACH(XPATH REDOX LIBJSON OTHER)
  set (${XPATH}_INCLUDES ${${XPATH}_PREFIX}/include)
  include_directories( ${XPATH}_INCLUDES )
  set (${XPATH}_LIBPATH ${${XPATH}_PREFIX/lib)
  link_directories( ${XPATH}_LIBPATH )
endforeach()

include_directories(${Boost_INCLUDE_DIR} ${CERCS_PREFIX}/include )
link_directories( ${CERCS_PREFIX}/lib )
set (CERCS_LIB_LIST evpath ffs dill atl cercs_env)

add_subdirectory( src )
add_subdirectory( doc )
add_subdirectory( tests )

# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "CERCS_PREFIX = ${CERCS_PREFIX}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}  (options are: Debug,Release,RelWithDebInfo,MinSizeRel)")
MESSAGE( STATUS "Change a value with: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "    to use installed CERCS libraries specify -DCERCS_USE_INSTALLED=1" )
MESSAGE( STATUS "Boost libraries: ${Boost_LIBRARIES}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
