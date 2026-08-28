cmake_minimum_required(VERSION 3.12.0)
project(campaign LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CAMPAIGN_LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE CAMPAIGN_LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

if(BUILD_CAMPAIGN_API)
    file(GLOB CAMPAIGN_LIB_API_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/api/*.cpp)
    file(GLOB CAMPAIGN_LIB_API_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/api/*.h)

    list(APPEND CAMPAIGN_LIB_SOURCES ${CAMPAIGN_LIB_API_SOURCES})
    list(APPEND CAMPAIGN_LIB_HEADERS ${CAMPAIGN_LIB_API_HEADERS})
endif()

if(NOT BUILD_CAMPAIGN_API)
    add_library(campaign)
else()
    add_library(campaign SHARED)

    target_compile_definitions(campaign PUBLIC CAMPAIGN_API_BUILD=1)
endif()

set_target_properties(campaign PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
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

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/library)

    if (BUILD_CAMPAIGN_API)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/api)

        find_package(Python3 REQUIRED)

        add_custom_command(TARGET campaign POST_BUILD
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copyLibCSharp.py
            COMMENT "Copying library to CSharp test project...")

        add_custom_target(campaign_csharp_tests
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/runCSharpTest.py
            COMMENT "Running CSharp tests...")
    endif()
endif()