// Copyright 2019 FURGBot

#include "manual_control.h"

ManualControl::ManualControl(): device_n(-1), running(false), serial(), robot_id_(-1) {
    axis = vector<short>(2, 0);
}

ManualControl::ManualControl(int _device_n, SerialSender *_serial, int robot_id) : device_n(_device_n), running(false), serial(_serial), robot_id_(robot_id) {
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

    if (!joystick->isFound()) {
        cout << "Falha ao abrir o controle." << endl;
    }

    while (running) {
        if(joystick->sample(&event)) {
            if(event.isButton()) {
                if (event.number == 5) {
                    activated = true;
                    cout << "Activated!" << endl;
                } else activated = false;
            }

            if(event.isAxis()) readEventAxis();
        }

        if (activated && (axis[AXIS_Y] > 10000 || axis[AXIS_X] > 10000)) {
            calculateWheelsVelocity();
            serial->send(message);
        }
    }

    message.clear();
}

void ManualControl::readEventAxis() {
    if (event.number < axis.size()) axis[event.number] = event.value;
}

void ManualControl::calculateWheelsVelocity() {
    float left_velocity, right_velocity;
    int left_direction, right_direction;

    if (axis[AXIS_Y] < 0) {
        left_direction = 2;
        right_direction = 2;
    } else {
        left_direction = 0;
        right_direction = 0;
    }

    float axis_x = abs(axis[AXIS_X]) * 127 / 32707;
    float axis_y = abs(axis[AXIS_Y]) * 127 / 32707;

    if (axis[AXIS_X] > 0) {
        right_velocity = axis_y;
        left_velocity = sqrt((axis_x * axis_x) + (axis_y * axis_y));
    } else {
        right_velocity = sqrt((axis_x * axis_x) + (axis_y * axis_y));
        left_velocity = axis_y;
    }

    if (right_velocity > 127) right_velocity = 127;
    if (left_velocity > 127) left_velocity = 127;

    message.setRobotId(robot_id_);
    message.setVel((int)right_velocity, (int)left_velocity);
    message.setDir(right_direction, left_direction);
}