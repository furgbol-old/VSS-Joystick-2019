// Copyright 2019 FURGBot

#ifndef MANUALCONTROL_H
#define MANUALCONTROL_H
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdio>
#include <math.h>

#include <opencv/cv.h>

#include "joystick.hh"

#include "serial_message.h"
#include "serial_sender.h"
#include "parameters.h"

using namespace std;
using namespace cv;
using namespace furgbol::joystick;
using namespace furgbol::parameters;

enum Axis {
    AXIS_X, AXIS_Y
};

enum Direction {
    NEGATIVE = 1, POSITIVE = 3
};

/*!
 * \brief A classe ManualControl administra uma thread para fazer leitura do joystick, montagem do pacote serial
 * e adição na lista para envio da thread SerialCommunicator
 */
class ManualControl {
private:
    bool running_; //!<Flag de controle de execução da Thread
    thread td_; //!<Thread da classe
    mutex mu_; //!<Mutex para escrita na flag running

    int device_n_; //!<Número do joystick utilizado pela thread
    int robot_id_; //!<Número do robô que o joystick vai controlar
    Joystick *joystick_; //!<Objeto da classe Joystick para fazer leitura do arquivo em que o joystick escreve seus dados
    JoystickEvent event_; //!<Objeto da classe JoystickEvent para verificar se houve algum evento no joystick que deve ser processado
    vector<short> axis_; //!<Vetor que guarda o valor dos analógicos do joystick

    SerialMessage message_; //!<Mensagem que será envidada
    SerialSender *serial_; //!<Ponteiro para a thread de comunicação serial

    int max_velocity_;
    int max_axis_;
    int baud_rate_;
    int min_axis_;

    /*!
     * \brief calculateWheelsVelocity calcula a velocity_wheels
     */
    void calculateWheelsVelocity();
    /*!
     * \brief readEventAxis processa um evento que seja disparado por um analógico, preenchendo o vetor axis
     */
    void readEventAxis();

    /*! 
     * \brief run é o loop principal a thread, passado por parâmetro para td
     */
    void run();

public:
    ManualControl();
    ManualControl(int instance, Parameters param, SerialSender *serial);

    ~ManualControl();

    /*!
     * \brief start modifica a flag running para true e reconstroi a thread td para reiniciar a thread
     */
    void start();
    /*!
     * \brief stop trava com o mutex, modificia a flag running para false e da join na thread td
     */
    void stop();
};

#endif // MANUALCONTROL_H
