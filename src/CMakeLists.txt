cmake_minimum_required(VERSION 3.8)
project(02)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        ipk-mtrip
        main.cpp
        probe.cpp
        reflect.cpp
        meter.cpp
        reflect.h meter.h connection.cpp connection.h argumentparser.cpp argumentparser.h)