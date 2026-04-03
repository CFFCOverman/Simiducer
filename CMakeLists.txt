cmake_minimum_required(VERSION 3.15)
project(SimiducerWorkspace CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# 1. 外部依赖项 (Vendor)
# ==========================================
include(FetchContent)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.90.4)
FetchContent_MakeAvailable(imgui)

find_package(OpenGL REQUIRED)

# 编译 GLAD
add_library(glad ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glad/include)

# 编译 ImGui
set(IMGUI_DIR ${imgui_SOURCE_DIR})
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(imgui PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glad/include)
target_link_libraries(imgui PRIVATE glfw OpenGL::GL)

# ==========================================
# 2. 核心引擎库 (SimiducerEngine)
# ==========================================
file(GLOB_RECURSE ENGINE_SOURCES "SimiducerEngine/src/*.cpp")
add_library(SimiducerEngine STATIC ${ENGINE_SOURCES})

# 告诉引擎去哪找头文件
target_include_directories(SimiducerEngine PUBLIC ${CMAKE_SOURCE_DIR}/SimiducerEngine/include)
target_include_directories(SimiducerEngine PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glad/include)
target_include_directories(SimiducerEngine PRIVATE ${CMAKE_SOURCE_DIR}/vendor/stb)

target_link_libraries(SimiducerEngine PUBLIC glfw OpenGL::GL glad glm imgui)

# ==========================================
# 3. 沙盒应用 (EarthProject)
# ==========================================
file(GLOB_RECURSE APP_SOURCES "EarthProject/src/*.cpp")
add_executable(EarthProject ${APP_SOURCES})

target_include_directories(EarthProject PRIVATE ${CMAKE_SOURCE_DIR}/SimiducerEngine/include)
target_link_libraries(EarthProject PRIVATE SimiducerEngine)