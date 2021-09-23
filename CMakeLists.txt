cmake_minimum_required(VERSION 3.3)

project(JumpMaestro LANGUAGES CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC ${ROOT}/src)
set(INCLUDE ${ROOT}/include)
set(BIN ${ROOT}/bin)
set(ASSETS ${ROOT}/assets)
set(DEPS ${ROOT}/deps)

# deps
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${DEPS}/JSON)

# build
set(EXECUTABLE_OUTPUT_PATH ${BIN})

file(GLOB_RECURSE sources "${INCLUDE}/*.h" "${SRC}/*.cpp")
add_executable(JumpMaestro ${sources})
target_include_directories(JumpMaestro PRIVATE ${INCLUDE})
target_link_libraries(JumpMaestro PRIVATE SDL2 SDL2_ttf nlohmann_json::nlohmann_json)