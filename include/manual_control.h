// Copyright 2019 FURGBot

#ifndef MANUALCONTROL_H
#define MANUALCONTROL_H
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdio>

#include <opencv/cv.h>

#include "joystick.hh"

#include "serial_message.h"
#include "serial_sender.h"

#define MIN_AXIS 10000
#define MAX_AXIS 32767
#define KICK_TIMES 10

using namespace std;
using namespace cv;
using namespace furgbol::joystick;

enum Direction {
    CLOCKWISE, COUNTERCLOCKWISE
};

enum Axis {
    AXIS_X, AXIS_Y
};

/*!
 * \brief A classe ManualControl administra uma thread para fazer leitura do joystick, montagem do pacote serial
 * e adição na lista para envio da thread SerialCommunicator
 */
class ManualControl {
private:
    bool running; //!<Flag de controle de execução da Thread
    thread td; //!<Thread da classe
    mutex mu; //!<Mutex para escrita na flag running

    int device_n; //!<Número do joystick utilizado pela thread
    int robot_id_; //!<Número do robô que o joystick vai controlar
    Joystick *joystick; //!<Objeto da classe Joystick para fazer leitura do arquivo em que o joystick escreve seus dados
    JoystickEvent event; //!<Objeto da classe JoystickEvent para verificar se houve algum evento no joystick que deve ser processado
    vector<short> axis; //!<Vetor que guarda o valor dos analógicos do joystick

    float left_wheel_velocity; //!<Velocidade da roda esquerda do robô
    float right_wheel_velocity; //!<Velocidade da roda direita do robô

    float factor_;

    int pivot_speed; //!<Velocidade do pivô
    float pivot_scale; //!<Escala de equilíbrio entre o pivô e o drive
    float pivot_y_limit; //!<Limite do pivô

    bool active;

    SerialMessage message; //!<Mensagem que será envidada
    SerialSender *serial; //!<Ponteiro para a thread de comunicação serial

    /*!
     * \brief calculateWheelsVelocity utiliza as matrizes velocity, M e R para calcular a velocity_wheels
     */
    void calculateWheelsVelocity();
    /*!
     * \brief readEventButton processa um evento que seja disparado por um botão, verificando que tipo de ação se deve tomar
     * \return booleano indicando se há ou não a necessidade de enviar um dado para o robô
     */
    bool readEventButton();
    /*!
     * \brief readEventAxis processa um evento que seja disparado por um analógico, preenchendo o vetor axis
     */
    void readEventAxis();
    /*!
     * \brief verifyVelocityAxis verifica se os valores que estão no vetor de axis são suficientes para calcular a velocidade
     * \return booleano indicando se o dado é ou não relevante
     */
    bool verifyVelocityAxis();

    /*! 
     * \brief run é o loop principal a thread, passado por parâmetro para td
     */
    void run();

public:
    ManualControl();
    ManualControl(int _device_n, SerialSender *_serial, float factor, int robot_id);

    ~ManualControl();

    /*!
     * \brief start modifica a flag running para true e reconstroi a thread td para reiniciar a thread
     */
    void start();
    /*!
     * \brief stop trava com o mutex, modificia a flag running para false e da join na thread td
     */
    void stop();

    void setId();
};

#endif // MANUALCONTROL_H
