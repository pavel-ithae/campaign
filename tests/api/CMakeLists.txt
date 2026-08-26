file(GLOB CAMPAIGN_API_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_executable(campaign_api_tests)

set_target_properties(campaign_api_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

target_sources(campaign_api_tests
    PRIVATE
        ${CAMPAIGN_API_TEST_SOURCES}
)

target_link_libraries(campaign_api_tests
    PRIVATE
        campaign
)