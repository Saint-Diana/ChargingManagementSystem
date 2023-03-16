cmake_minimum_required(VERSION 3.22)
project(ChargingManagementSystem)

set(CMAKE_CXX_STANDARD 14)

add_executable(ChargingManagementSystem main.cpp Card.cpp Card.h CardManagement.cpp CardManagement.h Constant.h Machine.cpp Machine.h Admin.cpp Admin.h Record.cpp Record.h)
