// Copyright 2019 FURGBot

#include "manual_control.h"

ManualControl::ManualControl(): device_n(-1), running(false), serial(), robot_id_(-1), max_velocity_(127) {
    axis = vector<short>(2, 0);
}

ManualControl::ManualControl(int _device_n, SerialSender *_serial, uint8_t max_velocity, int robot_id) : device_n(_device_n), running(false), serial(_serial), robot_id_(robot_id), max_velocity_(max_velocity) {
    joystick = new Joystick(_device_n);

    axis = vector<short>(2, 0);
}

ManualControl::~ManualControl() {
    this->stop();
}

void ManualControl::start() {
    running = true;
    td = thread(&ManualControl::run, this);
}

void ManualControl::stop() {
    {
        lock_guard<mutex> lock(mu);
        running = false;
    }
    td.join();
}

void ManualControl::run() {
    bool axis_send = false;
    bool activated = false;
    bool started = false;
    
    int control = 0;

    if (!joystick->isFound()) {
        cout << "Falha ao abrir o controle." << endl;
    }

    do {
        if(joystick->sample(&event)) {
            if(event.isButton()) {
                if (event.number == 5) {
                    if (started) {
                        if (!activated) activated = true;
                        else if (activated) activated = false;
                    } else started = true;
                }
            }
            if(event.isAxis()) readEventAxis();
        }

        if (activated && (axis[AXIS_Y] > 10000 || axis[AXIS_X] > 10000)) {
            control++;
            if (control >= 100) {
                calculateWheelsVelocity();
                serial->send(message);
                control = 0;
            }
        }
    } while (running);

    message.clear();
}

void ManualControl::readEventAxis() {
    if (event.number < axis.size()) axis[event.number] = event.value;
}

void ManualControl::calculateWheelsVelocity() {
    float linear_velocity, angular_velocity;
    uint8_t linear_direction, angular_direction;

    linear_velocity = (abs(axis[AXIS_Y]) * max_velocity_) / 32767;
    angular_velocity = (abs(axis[AXIS_X]) * max_velocity_) / 32767;

    if (axis[AXIS_Y] < 0) linear_direction = POSITIVE;
    else linear_direction = NEGATIVE;

    if (axis[AXIS_X] > 0) angular_direction = POSITIVE;
    else angular_direction = NEGATIVE;

    message.setRobotId(robot_id_);
    message.setVel((int)linear_velocity, (int)angular_velocity);
    message.setDir(linear_direction, angular_direction);
}