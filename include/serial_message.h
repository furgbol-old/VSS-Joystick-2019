// Copyright 2019 FURGBot

#ifndef JOYSTICK_SERIAL_MESSAGE_H
#define JOYSTICK_SERIAL_MESSAGE_H

#include <iostream>
#include <stdint.h>
#include <vector>

namespace furgbol {
namespace joystick {

enum SerialMessageHeaderFlags {
    ROBOT_ID = 0, VEL_X = 1, VEL_Y = 2, VEL_THETA = 3, DIRECTION = 5
};

class SerialMessage {
    private:
        uint8_t robot_id_;
        uint8_t vel_[3];
        uint8_t dir_[3]; 

    public:
        SerialMessage();
        SerialMessage(uint8_t robot_id, uint8_t *vel);
        ~SerialMessage();

        void serialize(std::vector<unsigned char> &buffer);

        void clear();

        friend std::ostream &operator <<(std::ostream &, SerialMessage const &);

        void setRobotId(uint8_t id);
        void setVel(uint8_t* vel);
        void setDir(uint8_t* dir);

        uint8_t getRobotId();
        uint8_t *getVel();
        uint8_t *getDir();
};

}  // namespace joystick
}  // namespace furgbol

#endif // JOYSTICK_SERIAL_MESSAGE_H