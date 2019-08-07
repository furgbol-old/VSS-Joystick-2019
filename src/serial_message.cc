// Copyright 2019 FURGBot

#include "serial_message.h"

#include <string>

namespace furgbol {
namespace joystick {

SerialMessage::SerialMessage() {
    clear();
}

SerialMessage::SerialMessage(uint8_t robot_id, uint8_t *vel) {
    robot_id_ = robot_id;
    vel_[0] = vel[0];
    vel_[1] = vel[1];
    vel_[2] = vel[2];
}

SerialMessage::~SerialMessage() {}

void SerialMessage::serialize(std::vector<unsigned char> &buffer) {
    buffer[ROBOT_ID] = robot_id_+128;
    buffer[VEL_X] = vel_[0];
    buffer[VEL_Y] = vel_[1];
    buffer[VEL_Y] = vel_[2];
    buffer[DIRECTION] = 0;
    buffer[DIRECTION] = dir_[2] << 2 | dir_[1] << 1 | dir_[0];

    std::cout<<"=========================PACOTE====================================\n"<<std::endl;
    printf("%u\n",buffer[0]);
    printf("%u\n",buffer[1]);
    printf("%u\n",buffer[2]);
    printf("%u\n",buffer[3]);
    printf("%u\n",buffer[4]);

}

void SerialMessage::clear() {
    robot_id_ = 0;
    vel_[0] = 0;
    vel_[1] = 0;
    vel_[2] = 0;
}

std::ostream &operator << (std::ostream &stream, furgbol::joystick::SerialMessage const &message)
{
    stream << "TeamProcolMessage{ " << std::endl;
    stream << "\tid: " << static_cast<int>(message.robot_id_) << std::endl;
    stream << "\tvelocity: [ ";
    stream << static_cast<int>(message.vel_[0]) << ", ";
    stream << static_cast<int>(message.vel_[1]) << ", ";
    stream << static_cast<int>(message.vel_[2]);
    stream << "]" << std::endl;
    stream << "\tdirection: [ ";
    stream << static_cast<int>(message.dir_[0]) << ", ";
    stream << static_cast<int>(message.dir_[1]) << ", ";
    stream << static_cast<int>(message.dir_[2]);
    stream << "]" << std::endl;
    stream << "};";
    return stream;
}

void SerialMessage::setRobotId(uint8_t robot_id) {
    robot_id_ = robot_id;
}

void SerialMessage::setVel(uint8_t* vel) {
    vel_[0] = vel[0];
    vel_[1] = vel[1];
    vel_[2] = vel[2];
}

void SerialMessage::setDir(uint8_t* dir) {
    dir_[0] = dir[0];
    dir_[1] = dir[1];
    dir_[2] = dir[2];
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