cmake_minimum_required(VERSION 2.8.3)
cmake_policy(VERSION 2.8.3)
project(drift)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif()

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


include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

find_package( Boost COMPONENTS thread program_options signals system timer log log_setup )

include_directories( ${Boost_INCLUDE_DIR} ${CERCS_PREFIX}/include )
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
