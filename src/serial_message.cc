// Copyright 2019 FURGBot

#include "serial_message.h"

#include <string>

namespace furgbol {
namespace joystick {

SerialMessage::SerialMessage() {
    clear();
}

SerialMessage::SerialMessage(uint8_t robot_id, uint8_t *vel, uint8_t *dir) {
    robot_id_ = robot_id;
    vel_[LINEAR_VEL] = vel[LINEAR_VEL];
    vel_[ANGULAR_VEL] = vel[ANGULAR_VEL];
    dir_[LINEAR_DIR] = dir[LINEAR_DIR];
    dir_[ANGULAR_DIR] = dir[ANGULAR_DIR];
}

SerialMessage::~SerialMessage() {}

void SerialMessage::serialize(std::vector<uint8_t> &buffer) {
    buffer[ROBOT_ID] = robot_id_+128;
    buffer[LINEAR_VEL] = vel_[LINEAR];
    buffer[ANGULAR_VEL] = vel_[ANGULAR];
    buffer[LINEAR_DIR] = dir_[LINEAR];
    buffer[ANGULAR_DIR] = dir_[ANGULAR];
}

void SerialMessage::clear() {
    robot_id_ = 0;
    vel_[LINEAR] = 0;
    vel_[ANGULAR] = 0;
    dir_[LINEAR] = 0;
    dir_[ANGULAR] = 0;
}

void SerialMessage::setRobotId(uint8_t robot_id) {
    robot_id_ = robot_id;
}

void SerialMessage::setVel(int linear, int angular) {
    vel_[LINEAR] = (uint8_t)linear;
    vel_[ANGULAR] = (uint8_t)angular;
}

void SerialMessage::setDir(uint8_t linear, uint8_t angular) {
    dir_[LINEAR] = linear;
    dir_[ANGULAR] = angular;
}

uint8_t SerialMessage::getRobotId() {
    return robot_id_;
}

uint8_t *SerialMessage::getVel() {
    return vel_;
}

uint8_t *SerialMessage::getDir() {
    return dir_;
}

std::ostream &operator << (std::ostream &stream, furgbol::joystick::SerialMessage const &message) {
    stream << "--->TeamProcolMessage{ " << std::endl;
    stream << "\tRobot ID: " << static_cast<int>(message.robot_id_) << std::endl;
    stream << "\tLinear Velocity: " << static_cast<int>(message.vel_[LINEAR]) << std::endl;
    stream << "\tAngular Velocity: " << static_cast<int>(message.vel_[ANGULAR]) << std::endl;
    stream << "\tLinear Direction: " << static_cast<int>(message.dir_[LINEAR]) << std::endl;
    stream << "\tAngular Direction: " << static_cast<int>(message.dir_[ANGULAR]) << std::endl;
    stream << "};";
    return stream;
}

} // namespace joystick
} // namespace furgbol