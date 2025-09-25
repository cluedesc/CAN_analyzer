cmake_minimum_required(VERSION 3.16)

project(CAN_analyzer VERSION 1.0.0 LANGUAGES CXX)

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
set(TFD_DIR external/tinyfiledialogs)
set(TFD_SOURCES ${TFD_DIR}/tinyfiledialogs.c)
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
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
    "${TFD_DIR}"
    "${JSON_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/app"
)

# link libraries
find_package(PostgreSQL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(libpqxx REQUIRED)

target_link_libraries(
    CAN_analyzer PRIVATE
    pqxx
    glfw
    OpenGL::GL
    PostgreSQL::PostgreSQL
)
# link libraries-end