// Copyright 2019 FURGBot

#include "parameters.h"

#include <fstream>

namespace furgbol {
namespace parameters {

Parameters::Parameters() {
    read_parameters();

    std::cout << "****Parameters Setted****" << std::endl;
    std::cout << "-> Networking:" << std::endl;
    std::cout << "Serial Port: " << serial_port << std::endl;
    std::cout << "Frequency: " << frequency << std::endl;
    std::cout << "-> Limiting:" << std::endl;
    std::cout << "Min Axis: " << min_axis << std::endl;
    std::cout << "Max Axis: " << max_axis << std::endl;
    std::cout << "Max Velocity: " << max_velocity << std::endl;
    std::cout << "-> Robot 1: " << std::endl;
    std::cout << "Device Number: " << device_one << std::endl;
    std::cout << "Robot ID: " << id_robot_one << std::endl;
    std::cout << "-> Robot 2: " << std::endl;
    std::cout << "Device Number: " << device_two << std::endl;
    std::cout << "Robot ID: " << id_robot_two << std::endl;
    std::cout << std::endl;
}

Parameters::~Parameters() {}

void Parameters::read_parameters() {
    std::ifstream i("config/params.json");
    nlohmann::json j;
    i >> j;

    serial_port = j["networking"]["serial_port"];
    frequency = j["networking"]["frequency"];
    min_axis = j["limiting"]["min_axis"];
    max_axis = j["limiting"]["max_axis"];
    max_velocity = j["limiting"]["max_velocity"];
    device_one = j["instances"]["instance_1"]["device"];
    device_two = j["instances"]["instance_2"]["device"];
    id_robot_one = j["instances"]["instance_1"]["robot_id"];
    id_robot_two = j["instances"]["instance_2"]["robot_id"];
}

}
}