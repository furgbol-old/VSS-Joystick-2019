// Copyright 2019 FURGBot

#ifndef JOYSTICK_PARAMETERS_H
#define JOYSTICK_PARAMETERS_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <json.hpp>

namespace furgbol {
namespace parameters {

class Parameters {
    public:
        int baud_rate;
        int min_axis;
        int max_axis;
        int max_velocity;
        int device_one;
        int device_two;
        int id_robot_one;
        int id_robot_two;
        std::string serial_port;

        Parameters();
        ~Parameters();

        void read_parameters();
};

}  // namespace paramaters
}  // namespace furgbol

#endif // JOYSTICK_PARAMETERS_H