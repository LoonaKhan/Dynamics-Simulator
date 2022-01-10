cmake_minimum_required(VERSION 3.20)
project(Dynamics_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Dynamics_Simulator
        main.cpp Collider.cpp Collider.h)

target_link_libraries( Dynamics_Simulator sfml-graphics sfml-window sfml-system)