// Copyright 2019 FURGBot

#ifndef JOYSTICK_SERIAL_MESSAGE_H
#define JOYSTICK_SERIAL_MESSAGE_H

#include <iostream>
#include <stdint.h>
#include <vector>

namespace furgbol {
namespace joystick {

enum SerialMessageHeaderFlags {
    ROBOT_ID, VEL_RIGHT, VEL_LEFT, DIR_RIGHT, DIR_LEFT
};

enum VelocityVectorFlags {
    VEL_R, VEL_L
};

enum DirectionsVectorFlags {
    DIR_R, DIR_L
};

class SerialMessage {
    private:
        uint8_t robot_id_;
        uint8_t vel_[2];
        uint8_t dir_[2];

    public:
        SerialMessage();
        SerialMessage(uint8_t robot_id, uint8_t *vel, uint8_t *dir);
        ~SerialMessage();

        void serialize(std::vector<unsigned char> &buffer);

        void clear();

        friend std::ostream &operator <<(std::ostream &, SerialMessage const &);

        void setRobotId(uint8_t id);
        void setVel(int right, int left);
        void setDir(uint8_t right, uint8_t left);

        uint8_t getRobotId();
        uint8_t *getVel();
        uint8_t *getDir();
};

}  // namespace joystick
}  // namespace furgbol

#endif // JOYSTICK_SERIAL_MESSAGE_H