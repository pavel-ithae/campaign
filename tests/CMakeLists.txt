file(GLOB CAMPAIGN_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(campaign_tests)

set_target_properties(campaign_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

target_sources(campaign_tests
    PRIVATE
        ${CAMPAIGN_TEST_SOURCES}
)

target_link_libraries(campaign_tests
    PRIVATE
        campaign
)

include(CTest)
