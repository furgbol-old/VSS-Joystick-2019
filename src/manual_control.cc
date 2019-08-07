// Copyright 2019 FURGBot

#include "manual_control.h"

ManualControl::ManualControl(): device_n(-1), running(false), serial(), pivot_scale(32.0) {
    axis = vector<short>(2, 0);
    initKinematicModel();
}

ManualControl::ManualControl(int _device_n, SerialSender *_serial): device_n(_device_n), running(false), serial(_serial) {
    joystick = new Joystick(_device_n);

    axis = vector<short>(2, 0);
    initKinematicModel();
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
        if(joystick->sample(&event)){
            if(event.isButton()){
                button_send = readEventButton();
            }
            else button_send = false;

            if(event.isAxis()){
                readEventAxis();
            }
        }
        else{
            button_send = false;
        }

        axis_send = verifyVelocityAxis();

        if (axis_send) calculateWheelsVelocity();
        else {
            right_wheel_velocity = 0.0;
            left_wheel_velocity = 0.0;
        }

        if(axis_send || button_send){
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
    if (nJoyY >= 0) {
  // Forward
  nMotPremixL = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
  nMotPremixR = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
} else {
  // Reverse
  nMotPremixL = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
  nMotPremixR = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
}

nMotPremixL = nMotPremixL * nJoyY/128.0;
nMotPremixR = nMotPremixR * nJoyY/128.0;

nPivSpeed = nJoyX;
fPivScale = (abs(nJoyY)>fPivYLimit)? 0.0 : (1.0 - abs(nJoyY)/fPivYLimit);

nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*( nPivSpeed);
nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);
}