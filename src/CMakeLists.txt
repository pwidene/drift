include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

find_package( Boost COMPONENTS program_options signals system timer )

include_directories( ${Boost_INCLUDE_DIR} ${CERCS_PREFIX}/include )
link_directories( ${CERCS_PREFIX}/lib )

# FIND_CERCS_PROJECT (cercs_env LIBRARY cercs_env INCLUDES cercs_env.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (atl LIBRARY atl INCLUDES atl.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (gen_thread LIBRARY gen_thread INCLUDES gen_thread.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (ffs LIBRARY ffs INCLUDES ffs.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (dill LIBRARY dill REQUIRED ${CERCS_FIND_OPTIONS})

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

set (LIBDRIFT_SRC_LIST trie.cc)
set (CERCS_LIB_LIST evpath ffs dill gen_thread atl cercs_env)

add_library(drift SHARED ${LIBDRIFT_SRC_LIST})

add_executable(driftd driftd.cc)
target_link_libraries(driftd drift ${CERCS_LIB_LIST} ${Boost_LIBRARIES})
