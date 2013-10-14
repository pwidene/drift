
# FIND_CERCS_PROJECT (cercs_env LIBRARY cercs_env INCLUDES cercs_env.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (atl LIBRARY atl INCLUDES atl.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (gen_thread LIBRARY gen_thread INCLUDES gen_thread.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (ffs LIBRARY ffs INCLUDES ffs.h REQUIRED ${CERCS_FIND_OPTIONS})
# FIND_CERCS_PROJECT (dill LIBRARY dill REQUIRED ${CERCS_FIND_OPTIONS})

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

include_directories( ${MONGO_INCLUDE_DIR} )

set (LIBDRIFT_SRC_LIST trie.cc)
set (LIBDRIFTF_SRC_LIST formats.cc)

add_library(drift SHARED ${LIBDRIFT_SRC_LIST})
add_library(driftf SHARED ${LIBDRIFTF_SRC_LIST})

add_executable(driftd driftd.cc control.cc service.cc)
target_link_libraries(driftd drift driftf ${CERCS_LIB_LIST} ${Boost_LIBRARIES} ${MONGO_LIBRARIES})
set_target_properties( driftd PROPERTIES RPATH_MACOSX TRUE )
install(TARGETS driftd drift driftf 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
