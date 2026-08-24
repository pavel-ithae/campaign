cmake_minimum_required(VERSION 3.12.0)
project(campaign LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CAMPAIGN_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE CAMPAIGN_LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(campaign)

set_target_properties(campaign PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

target_sources(campaign
    PRIVATE
        ${CAMPAIGN_LIB_SOURCES}

    PUBLIC
        FILE_SET libcampaignHeaders
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            ${CAMPAIGN_LIB_HEADERS}
)

if(BUILD_TESTING)
    enable_testing()

    target_compile_definitions(campaign PUBLIC CAMPAIGN_LIBRARY_TESTING=1)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()