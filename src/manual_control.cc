// Copyright 2019 FURGBot

#include "manual_control.h"

ManualControl::ManualControl(): device_n(-1), running(false), serial(), pivot_y_limit(32.0) {
    axis = vector<short>(2, 0);
}

ManualControl::ManualControl(int _device_n, SerialSender *_serial): device_n(_device_n), running(false), serial(_serial) {
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
    if(!joystick->isFound()){
        cout<<"Falha ao abrir o controle."<<endl;
    }

    while(running){
        if (joystick->sample(&event)) {
            if (event.isAxis()) {
                readEventAxis();
            }
        } else {
            button_send = false;
        }

        axis_send = verifyVelocityAxis();

        if (axis_send) calculateWheelsVelocity();
        else {
            right_wheel_velocity = 0.0;
            left_wheel_velocity = 0.0;
        }

        if(axis_send || button_send) {
            cout << "Mensagem: " << endl;
            cout << message << endl;
            calculateWheelsVelocity();
            serial->send(message);
        }
    }

    message.clear();
}


//Setters
void ManualControl::setId(int _id) {
    message.setRobotId(_id);
}

void ManualControl::readEventAxis() {
    if (event.number<axis.size()) axis[event.number] = event.value;
}

bool ManualControl::verifyVelocityAxis() {
    for(int i = 0; i < 2; i++)
        if(abs(axis[i]) >= MIN_AXIS) return true;
    return false;
}

void ManualControl::calculateWheelsVelocity() {
    int right_velocity, left_velocity;

    if (axis[AXIS_Y] >= 0) {
        //Forward
        left_velocity = (axis[AXIS_X] >= 0) ? 127.0 : (127.0 + axis[AXIS_X]);
        right_velocity = (axis[AXIS_X] >= 0) ? (127.0 - axis[AXIS_X]) : 127.0;
    } else {
        //Reverse
        left_velocity = (axis[AXIS_X] >= 0) ? (127.0 - axis[AXIS_X]) : 127.0;
        right_velocity = (axis[AXIS_X] >= 0) ? 127.0 : (127.0 + axis[AXIS_X]);
    }

    left_velocity = left_velocity * axis[AXIS_Y] / 128.0;
    right_velocity = right_velocity * axis[AXIS_Y] / 128.0;

    pivot_speed = axis[AXIS_X];
    pivot_scale = (abs(axis[AXIS_Y]) > pivot_y_limit) ? 0.0 : (1.0 - abs(axis[AXIS_Y])/pivot_y_limit);

    left_wheel_velocity = (1.0 - pivot_scale) * left_velocity + pivot_scale * (pivot_speed);
    right_wheel_velocity = (1.0 - pivot_scale) * right_velocity + pivot_scale * (-pivot_speed);
}