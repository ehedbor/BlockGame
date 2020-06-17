cmake_minimum_required(VERSION 3.1)

project(BlockGame
    VERSION 1.0
    DESCRIPTION "Minecraft clone with OpenGL"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(BlockGame src/main.cpp)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
target_link_libraries(BlockGame glad glfw)
