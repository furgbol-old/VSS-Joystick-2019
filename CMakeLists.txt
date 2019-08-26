cmake_minimum_required(VERSION 3.10)
project(furgbol_joystick_vss)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenCV REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/lib/joystick-master)

include_directories("include" "lib/joystick-master" ${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_library(furgbol_joy SHARED "src/serial_sender.cc" "src/serial_message.cc" "src/manual_control.cc" "src/parameters.cc" "include/json.hpp")

target_link_libraries(furgbol_joy ${Boost_SYSTEM_LIBRARY})

add_executable(joystick_vss "src/main.cpp")

target_link_libraries(joystick_vss furgbol_joy ${Boost_LIBRARIES} ${OpenCV_LIBS} libjoystick.a)
