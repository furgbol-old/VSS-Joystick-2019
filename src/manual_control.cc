// Copyright 2019 FURGBot

#include "manual_control.h"

ManualControl::ManualControl(): device_n_(-1), running_(false), serial_(), robot_id_(-1), max_velocity_(-1) {
    axis_ = vector<short>(2, 0);
}

ManualControl::ManualControl(int instance, Parameters param, SerialSender *serial) : running_(false), 
                            serial_(serial), max_velocity_(param.max_velocity), baud_rate_(param.baud_rate),
                            max_axis_(param.max_axis), min_axis_(param.min_axis) {
    switch (instance) {
        case 1:
            device_n_ = param.device_one;
            robot_id_ = param.id_robot_one;
        case 2:
            device_n_ = param.device_two;
            robot_id_ = param.id_robot_two;
    }

    joystick_ = new Joystick(device_n_);

    axis_ = vector<short>(2, 0);
}

ManualControl::~ManualControl() {
    this->stop();
}

void ManualControl::start() {
    running_ = true;
    td_ = thread(&ManualControl::run, this);
}

void ManualControl::stop() {
    {
        lock_guard<mutex> lock(mu_);
        running_ = false;
    }
    td_.join();
}

void ManualControl::run() {
    bool axis_send = false;
    bool activated = false;
    bool started = false;
    
    int control = 0;

    if (!joystick_->isFound()) {
        cout << "Falha ao abrir o controle." << endl;
    }

    do {
        if(joystick_->sample(&event_)) {
            if(event_.isButton()) {
                if (event_.number == 5 /*5 é o valor do RB*/) {
                    if (started) {
                        if (!activated) activated = true;
                        else if (activated) activated = false;
                    } else started = true;
                }
            }
            if(event_.isAxis()) readEventAxis();
        }

        if (abs(axis_[AXIS_X]) < min_axis_) axis_[AXIS_X] = 0;
        if (abs(axis_[AXIS_Y]) < min_axis_) axis_[AXIS_Y] = 0;

        if (activated && (axis_[AXIS_Y] > 0 || axis_[AXIS_X] > 0)) {
            control++;
            if (control >= baud_rate_) {
                calculateWheelsVelocity();
                serial_->send(message_);
                control = 0;
            }
        }
    } while (running_);

    message_.clear();
}

void ManualControl::readEventAxis() {
    if (event_.number < axis_.size()) axis_[event_.number] = event_.value;
}

void ManualControl::calculateWheelsVelocity() {
    float linear_velocity, angular_velocity;
    uint8_t linear_direction, angular_direction;

    linear_velocity = (abs(axis_[AXIS_Y]) * max_velocity_) / max_axis_;
    angular_velocity = (abs(axis_[AXIS_X]) * max_velocity_) / max_axis_;

    if (axis_[AXIS_Y] < 0) linear_direction = POSITIVE;
    else linear_direction = NEGATIVE;

    if (axis_[AXIS_X] > 0) angular_direction = POSITIVE;
    else angular_direction = NEGATIVE;

    message_.setRobotId(robot_id_);
    message_.setVel((int)linear_velocity, (int)angular_velocity);
    message_.setDir(linear_direction, angular_direction);
}