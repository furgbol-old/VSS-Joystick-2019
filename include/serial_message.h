// Copyright 2019 FURGBot

#ifndef JOYSTICK_SERIAL_MESSAGE_H
#define JOYSTICK_SERIAL_MESSAGE_H

#include <iostream>
#include <stdint.h>
#include <vector>

namespace furgbol {
namespace joystick {

enum SerialMessageHeaderFlags {
    ROBOT_ID, VEL_RIGHT, VEL_LEFT
};

enum VelocityVectorFlags {
    VEL_R, VEL_L
};

class SerialMessage {
    private:
        uint8_t robot_id_;
        uint8_t vel_[8];

    public:
        SerialMessage();
        SerialMessage(uint8_t robot_id, uint8_t *vel);
        ~SerialMessage();

        void serialize(std::vector<unsigned char> &buffer);

        void clear();

        friend std::ostream &operator <<(std::ostream &, SerialMessage const &);

        void setRobotId(uint8_t id);
        void setVel(float right, float left);

        uint8_t getRobotId();
        uint8_t *getVel();
};

}  // namespace joystick
}  // namespace furgbol

#endif // JOYSTICK_SERIAL_MESSAGE_H