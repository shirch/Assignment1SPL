cmake_minimum_required(VERSION 3.8)
project(ass1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Commands.cpp
        include/Commands.h
        include/Environment.h
        include/Files.h
        src/Files.cpp
        src/FileSystem.cpp
        include/FileSystem.h
        src/Main.cpp
        include/Main.h
        src/stringFunction.cpp
        include/stringFunction.h
        src/Environment.cpp
        include/GlobalVariables.h)


add_executable(ass1 ${SOURCE_FILES})

