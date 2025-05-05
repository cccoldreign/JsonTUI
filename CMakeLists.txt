cmake_minimum_required(VERSION 3.18)

project(TUI)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

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

FetchContent_GetProperties(ftxui)

# ftxui_POPULATED
# ftxui_SOURCE_DIR
# ftxui_BUILD_DIR

if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(TUI 
    src/main.cpp
    src/Table.cpp
    src/Vert.cpp
    src/Navy.cpp
)
#add_compile_options(GUI PRIVATE --table /Users/coldreign/code/Study/c++/JsonUI/Example/Example4.json)
include_directories(include)

# target_include_directories(GUI PRIVATE src)

target_link_libraries(TUI
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
)

# target_link_libraries(GUI ${json_SOURCE_DIR}/include/nlohman)