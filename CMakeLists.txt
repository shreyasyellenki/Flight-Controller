cmake_minimum_required(VERSION 3.10)
project(flight_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(flight_controller
    src/main.cpp
    src/Sensor.cpp
    src/Control.cpp
    src/Telemetry.cpp
)