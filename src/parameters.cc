// Copyright 2019 FURGBot

#include "parameters.h"

#include <fstream>

namespace furgbol {
namespace parameters {

Parameters::Parameters() {
    read_parameters();
}

Parameters::~Parameters() {}

void Parameters::read_parameters() {
    std::ifstream i("config/params.json");
    nlohmann::json j;
    i >> j;

    serial_port = j["networking"]["serial_port"];
    baud_rate = j["networking"]["baud_rate"];
    min_axis = j["limiting"]["min_axis"];
    max_axis = j["limiting"]["max_axis"];
    max_velocity = j["limiting"]["max_velocity"];
    device_one = j["instances"]["instance_1"]["device_one"];
    device_two = j["instances"]["instance_1"]["device_two"];
    id_robot_one = j["instances"]["instance_2"]["id_robot_one"];
    id_robot_two = j["instances"]["instance_2"]["id_robot_two"];
}

}
}