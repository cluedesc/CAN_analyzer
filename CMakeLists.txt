cmake_minimum_required(VERSION 3.16)

project(CAN_analyzer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# imgui
set(IMGUI_DIR external/imgui)

file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
)

file(GLOB IMGUI_BACKEND_SOURCES
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
# imgui-end

# tinyfiledialogs
set(TFD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyfiledialogs)

if(NOT EXISTS "${TFD_DIR}/tinyfiledialogs.c")
    message(FATAL_ERROR "Cannot find tinyfiledialogs.c in ${TFD_DIR}")
endif()

set(TFD_SOURCES ${TFD_DIR}/tinyfiledialogs.c)

set_source_files_properties(${TFD_SOURCES} PROPERTIES LANGUAGE C)
# tinyfiledialogs-end

# nlohmann_json
set(JSON_DIR external/nlohmann_json/single_include)
# nlohmann_json-end

file(GLOB_RECURSE ALL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)

add_executable(
    CAN_analyzer 
    ${ALL_SOURCES}
    ${IMGUI_SOURCES}
    ${IMGUI_BACKEND_SOURCES}
    ${TFD_SOURCES}
)

target_include_directories(
    CAN_analyzer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/app"
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
    "${JSON_DIR}"
    "${TFD_DIR}"
)

# link libraries
find_package(PostgreSQL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(libpqxx REQUIRED)

target_link_libraries(
    CAN_analyzer PRIVATE
    OpenGL::GL
    PostgreSQL::PostgreSQL
    libpqxx::pqxx
    glfw
)
# link libraries-end