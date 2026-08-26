# Try to include local Catch2 (3.0) package.
find_package(Catch2 3 QUIET)

# If not found locally, fetch Catch2 from github.
if(NOT Catch2_FOUND)
    Include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1
    )

    FetchContent_MakeAvailable(Catch2)
endif()

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

# Link Catch2 (with main) library.
target_link_libraries(campaign_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(campaign_tests)