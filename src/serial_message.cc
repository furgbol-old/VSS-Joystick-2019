// Copyright 2019 FURGBot

#include "serial_message.h"

#include <string>

namespace furgbol {
namespace joystick {

SerialMessage::SerialMessage() {
    clear();
}

SerialMessage::SerialMessage(int8_t robot_id, int8_t *vel) {
    robot_id_ = robot_id;
    vel_[VEL_R] = vel[VEL_R];
    vel_[VEL_L] = vel[VEL_L];
}

SerialMessage::~SerialMessage() {}

void SerialMessage::serialize(std::vector<unsigned char> &buffer) {
    buffer[ROBOT_ID] = robot_id_+128;
    buffer[VEL_RIGHT] = vel_[VEL_R];
    buffer[VEL_LEFT] = vel_[VEL_L];

    std::cout<<"=========================PACOTE====================================\n"<<std::endl;
    printf("%u\n", buffer[ROBOT_ID]);
    printf("%u\n", buffer[VEL_RIGHT]);
    printf("%u\n", buffer[VEL_LEFT]);

}

void SerialMessage::clear() {
    robot_id_ = 0;
    vel_[VEL_R] = 0;
    vel_[VEL_L] = 0;
}

std::ostream &operator << (std::ostream &stream, furgbol::joystick::SerialMessage const &message) {
    stream << "TeamProtocolMessage{ " << std::endl;
    stream << "\tid: " << static_cast<int>(message.robot_id_) << std::endl;
    stream << "\tvelocity: [ ";
    stream << message.vel_[VEL_R] << ", ";
    stream << message.vel_[VEL_L];
    stream << "]" << std::endl;
    stream << "};";
    return stream;
}

void SerialMessage::setRobotId(int8_t robot_id) {
    robot_id_ = robot_id;
}

void SerialMessage::setVel(int right, int left) {
    vel_[VEL_L] = (int8_t)left;
    vel_[VEL_R] = (int8_t)right;
}

int8_t SerialMessage::getRobotId() {
    return robot_id_;
}

int8_t *SerialMessage::getVel() {
    return vel_;
}

} // namespace joystick
} // namespace furgbol