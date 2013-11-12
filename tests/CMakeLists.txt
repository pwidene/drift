cmake_minimum_required(VERSION 2.8)

set (NO_INST_PROGS ping)

add_executable(ping ping.cc)
target_link_libraries(ping driftf ${CERCS_LIB_LIST} ${Boost_LIBRARIES})

ENABLE_TESTING()

INCLUDE_DIRECTORIES(BEFORE ${drift_BINARY_DIR} ${drift_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

foreach (TEST ${TESTS} )
    ADD_EXECUTABLE(${TEST} ${TEST}.cc)
    TARGET_LINK_LIBRARIES(${TEST} driftf ${CERCS_LIB_LIST} ${Boost_LIBRARIES})
#    ADD_TEST(${TEST} "${TEST}")
endforeach()

#ADD_EXECUTABLE(thin_client thin_client.c)
#TARGET_LINK_LIBRARIES(thin_client evpath ${BASE_LIBS})

# ADD_TEST(evtest_UDP evtest -t udp)
# IF (ENET_FOUND)
#     ADD_TEST(evtest_ENET evtest -t enet)
#     ADD_TEST(bulktest_ENET bulktest -t enet)
# ENDIF (ENET_FOUND)
# ADD_SUBDIRECTORY( testdll )

