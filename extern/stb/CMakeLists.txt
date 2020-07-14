cmake_minimum_required(VERSION 3.0)
project(stb)


add_library(stb include/stb_image.h)
target_include_directories(stb PUBLIC include/)

set_target_properties(stb PROPERTIES LINKER_LANGUAGE C)
