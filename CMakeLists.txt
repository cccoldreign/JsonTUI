cmake_minimum_required(VERSION 3.18)

project(TUI)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Настройка флагов для code coverage
# if(ENABLE_COVERAGE)
#     if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
#         set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -g -O0")
#         set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
#         #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
#     endif()
# endif()
#cmake -DCMAKE_CXX_FLAGS="--coverage -g -O0" ..

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.3
)

FetchContent_GetProperties(json)
# json_POPULATED
# json_SOURCE_DIR
# json_BUILD_DIR

if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

set(FTXUI_BUILD_TESTS ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(ftxui)

# ftxui_POPULATED
# ftxui_SOURCE_DIR
# ftxui_BUILD_DIR

if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

# enable_testing()

add_executable(TUI 
    src/main.cpp
    src/Table.cpp
    src/Vert.cpp
    src/Navy.cpp
    # Test/TestFileCheck.cpp
    # Test/TestScreen.cpp
)

include_directories(include)

target_link_libraries(TUI
  PRIVATE gtest_main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
)

# add_test(NAME TestFileCheck COMMAND test)
# add_test(NAME TestScreen COMMAND test)

# Example.json
# {
#     "Globus": {
#         "hohm": {
#             "ffsa": 1
#         },
#         "ploter": {
#             "Bebeka": "DDddddddddddD",
#             "HOmpakt": "Sta",
#             "ID": 123,
#             "SortdsdAs": "SGML"
#         },
#         "zzz": {
#             "zzz": "dsad"
#         }
#     }
# }