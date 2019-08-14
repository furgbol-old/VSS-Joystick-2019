// Copyright 2019 FURGBot

#include "manual_control.h"

ManualControl::ManualControl(): device_n(-1), running(false), serial(), pivot_y_limit(32.0), factor_(100.0), robot_id_(-1) {
    axis = vector<short>(2, 0);
}

ManualControl::ManualControl(int _device_n, SerialSender *_serial, float factor, int robot_id) : device_n(_device_n), running(false), serial(_serial), factor_(factor), robot_id_(robot_id) {
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
    bool button_send = false;
    bool axis_send = false;

    if (!joystick->isFound()) {
        cout << "Falha ao abrir o controle." << endl;
    }

    while (running) {
        if(joystick->sample(&event)) {
            if(event.isButton()) button_send = readEventButton();
            else button_send = false;

            if(event.isAxis()) readEventAxis();
        } else button_send = false;

        axis_send = verifyVelocityAxis();

        if (axis_send && button_send) {
            cout << "Mensagem: " << endl;
            cout << message << endl;
            setId();
            calculateWheelsVelocity();
            serial->send(message);
        }
    }

    message.clear();
}


//Setters
void ManualControl::setId() {
    message.setRobotId(robot_id_);
}

void ManualControl::readEventAxis() {
    if (event.number<axis.size()) axis[event.number] = event.value;
}

bool ManualControl::verifyVelocityAxis() {
    for (int i = 0; i < 2; i++)
        if(abs(axis[i]) >= MIN_AXIS) return true;
    return false;
}

bool ManualControl::readEventButton() {
    if (event.number == 5) return event.value;
    return false;
}

void ManualControl::calculateWheelsVelocity() {
    int right_velocity, left_velocity;
    float axis_y, axis_x;

    axis_y = 63.0 * axis[AXIS_Y] / 32767;
    axis_x = 63.0 * axis[AXIS_X] / 32767;

    if (axis_y >= 0) {
        if (axis_x >= 0) {
            left_velocity = 63.0;
            right_velocity = 63.0 - axis_x;
        } else {
            left_velocity = 63.0 + axis_x;
            right_velocity = 63.0;
        }
    } else {
        if (axis_y >= 0) {
            right_velocity = 63.0;
            left_velocity = 63.0 - axis_x;
        } else {
            right_velocity = 63.0 + axis_x;
            left_velocity = 63.0;
        }
    }

    left_velocity = left_velocity * axis_y / 64.0;
    right_velocity = right_velocity * axis_y / 64.0;

    pivot_speed = axis_x;
    if (abs(axis_y) > pivot_y_limit) pivot_scale = 0.0;
    else pivot_scale = 1.0 - abs(axis_y) / pivot_y_limit;

    left_wheel_velocity = (1.0 - pivot_scale) * left_velocity + pivot_scale * (pivot_speed);
    right_wheel_velocity = (1.0 - pivot_scale) * right_velocity + pivot_scale * (-pivot_speed);

    message.setVel((int)right_wheel_velocity, (int)left_wheel_velocity);
}