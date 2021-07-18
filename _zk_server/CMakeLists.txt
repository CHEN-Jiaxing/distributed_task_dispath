cmake_minimum_required(VERSION 3.11)
project(_zk_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(_zk_server
        clust_comm.cpp
        clust_comm.h
        clust_file_test.cpp
        work.cpp)
