cmake_minimum_required(VERSION 3.9)

project(ELF_Game VERSION 1.0.0 DESCRIPTION "Game for fun")
add_definitions(-std=c++14)

#include_directories(src)
add_executable(ELF_Game 
	src/elf.cpp
	src/source.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/repos/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(ELF_Game sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
