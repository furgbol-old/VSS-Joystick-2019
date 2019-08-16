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
    vel_[VEL_R] = vel[VEL_R];
    vel_[VEL_L] = vel[VEL_L];
    dir_[DIR_R] = dir[DIR_R];
    dir_[DIR_L] = dir[DIR_L];
}

SerialMessage::~SerialMessage() {}

void SerialMessage::serialize(std::vector<uint8_t> &buffer) {
    buffer[ROBOT_ID] = robot_id_+128;
    buffer[VEL_RIGHT] = vel_[VEL_R];
    buffer[VEL_LEFT] = vel_[VEL_L];
    buffer[DIR_RIGHT] = dir_[DIR_R];
    buffer[DIR_LEFT] = dir_[DIR_L];

    std::cout<<"=========================PACOTE====================================\n"<<std::endl;
    printf("Robot id: %d\n", buffer[ROBOT_ID]);
    printf("Right velocity: %d\n", buffer[VEL_RIGHT]);
    printf("Left velocity: %d\n", buffer[VEL_LEFT]);
    printf("Right direction: %d\n", buffer[DIR_RIGHT]);
    printf("Left velocity: %d\n", buffer[DIR_LEFT]);
}

void SerialMessage::clear() {
    robot_id_ = 0;
    vel_[VEL_R] = 0;
    vel_[VEL_L] = 0;
    dir_[DIR_R] = 0;
    dir_[DIR_L] = 0;
}

void SerialMessage::setRobotId(uint8_t robot_id) {
    robot_id_ = robot_id;
}

void SerialMessage::setVel(int right, int left) {
    vel_[VEL_L] = (uint8_t)left;
    vel_[VEL_R] = (uint8_t)right;
}

void SerialMessage::setDir(uint8_t right, uint8_t left) {
    dir_[VEL_L] = left;
    dir_[VEL_R] = right;
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

} // namespace joystick
} // namespace furgbol