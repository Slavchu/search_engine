cmake_minimum_required(VERSION 3.24)
project(search_engine)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

enable_testing()

include(CTest)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output_build/)

add_library(json_converter ${CMAKE_SOURCE_DIR}/src/classes/json_converter/json_converter.cpp)
add_library(inverted_index  ${CMAKE_SOURCE_DIR}/src/classes/inverted_index/inverted_index.cpp)
add_library(search_server  ${CMAKE_SOURCE_DIR}/src/classes/search_server/search_server.cpp)

add_executable(search_engine src/classes/search_server/search_server.cpp src/classes/inverted_index/inverted_index.cpp src/classes/json_converter/json_converter.cpp src/main.cpp)


target_include_directories(search_server  PUBLIC ${CMAKE_SOURCE_DIR}/src/classes/search_server)
target_include_directories(inverted_index PUBLIC ${CMAKE_SOURCE_DIR}/src/classes/inverted_index)
target_include_directories(json_converter PUBLIC ${CMAKE_SOURCE_DIR}/src/classes/json_converter)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(GoogleTest)




target_link_libraries(json_converter PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(search_engine PRIVATE json_converter)
target_link_libraries(search_engine PRIVATE inverted_index)
target_link_libraries(search_engine PRIVATE search_server)
target_link_libraries(search_server PRIVATE inverted_index)

target_link_libraries(search_engine PRIVATE gtest_main)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

gtest_discover_tests(search_engine)

include(CPack)


