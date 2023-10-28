cmake_minimum_required(VERSION 3.24)
project(test1)

set(CMAKE_CXX_STANDARD 11)
set(LINK_DIR ./lib)
link_directories(${LINK_DIR})

add_executable(test1 main.cpp entity.cpp ssp_pair.cpp big.cpp ecn.cpp miracl.c zzn.cpp zzn2.cpp
        sm3_miracl_wrapper.cpp
        sm3_reference.cpp
        EccTest.c
        sha256.cpp
        sha256.h
)
