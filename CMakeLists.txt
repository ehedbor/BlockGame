cmake_minimum_required(VERSION 3.1)

project(BlockGame
    VERSION 1.0
    DESCRIPTION "Minecraft clone with OpenGL"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(gen_dir "${PROJECT_BINARY_DIR}/generated")

# add compiler flags

#SET(MINGW_COMPILE_FLAGS "")
#SET(MINGW_LINKER_FLAGS "-lstdc++fs")


# thank god for https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
# what the hell even is cmake??
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

# Generate a .cpp file to hold shaders
find_package(PythonInterp 3.2 REQUIRED)
add_custom_command(
    PRE_BUILD
    OUTPUT "${gen_dir}/ShaderSources.cpp"
    COMMAND "${PYTHON_EXECUTABLE}"
        "${CMAKE_CURRENT_LIST_DIR}/GenerateShaders.py"
        "${CMAKE_CURRENT_LIST_DIR}/shaders/"
        "${gen_dir}"
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/GenerateShaders.py"
        "shaders/vertex.vert" "shaders/fragment.frag"
    COMMENT "Generating shader source file"
)

add_executable(BlockGame
    src/Shader.cpp src/Shader.h src/Window.cpp src/Window.h
    ${gen_dir}/ShaderSources.cpp src/Game.cpp src/Game.h src/Texture.cpp src/Texture.h src/main.cpp src/Input.cpp src/Input.h src/Camera.cpp src/Camera.h src/ResourceManager.cpp src/ResourceManager.h)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/stb)
add_subdirectory(extern/glm)
target_link_libraries(BlockGame glad glfw stb glm)

target_compile_options(BlockGame PRIVATE -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable)
target_link_options(BlockGame PRIVATE -lstdc++fs)
