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
}

SerialMessage::~SerialMessage() {}

void SerialMessage::serialize(std::vector<unsigned char> &buffer) {
    buffer[ROBOT_ID] = robot_id_+128;
    buffer[VEL_R1] = vel_[VEL_R1];
    buffer[VEL_R2] = vel_[VEL_R2];
    buffer[VEL_R3] = vel_[VEL_R3];
    buffer[VEL_R4] = vel_[VEL_R4];
    buffer[VEL_L1] = vel_[VEL_L1];
    buffer[VEL_L2] = vel_[VEL_L2];
    buffer[VEL_L3] = vel_[VEL_L3];
    buffer[VEL_L4] = vel_[VEL_L4];

    std::cout<<"=========================PACOTE====================================\n"<<std::endl;
    printf("%u\n", buffer[0]);
    printf("%u\n", buffer[1]);
    printf("%u\n", buffer[2]);

}

void SerialMessage::clear() {
    robot_id_ = 0;
    vel_[0] = 0;
    vel_[1] = 0;
    vel_[2] = 0;
    vel_[3] = 0;
    vel_[4] = 0;
    vel_[5] = 0;
    vel_[6] = 0;
    vel_[7] = 0;
}

std::ostream &operator << (std::ostream &stream, furgbol::joystick::SerialMessage const &message)
{
    stream << "TeamProcolMessage{ " << std::endl;
    stream << "\tid: " << static_cast<int>(message.robot_id_) << std::endl;
    stream << "\tvelocity: [ ";
    stream << message.vel_[0] << ", ";
    stream << message.vel_[1];
    stream << "]" << std::endl;
    stream << "};";
    return stream;
}

void SerialMessage::setRobotId(uint8_t robot_id) {
    robot_id_ = robot_id;
}

void SerialMessage::setVel(float right, float left) {
}

uint8_t SerialMessage::getRobotId() {
    return robot_id_;
}

uint8_t *SerialMessage::getVel() {
    return vel_;
}

} // namespace joystick
} // namespace furgbol