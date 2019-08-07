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

enum Direction{
    CLOCKWISE, COUNTERCLOCKWISE
};

/*!
 * \brief A classe ManualControl administra uma thread para fazer leitura do joystick, montagem do pacote serial
 * e adição na lista para envio da thread SerialCommunicator
 */
class ManualControl
{
private:
    bool running; //!<Flag de controle de execução da Thread
    thread td; //!<Thread da classe
    mutex mu; //!<Mutex para escrita na flag running

    int device_n; //!<Número do joystick utilizado pela thread
    Joystick *joystick; //!<Objeto da classe Joystick para fazer leitura do arquivo em que o joystick escreve seus dados
    JoystickEvent event; //!<Objeto da classe JoystickEvent para verificar se houve algum evento no joystick que deve ser processado
    vector<short> axis; //!<Vetor que guarda o valor dos analógicos do joystick

    float max_velocity; //!<Velocidade linear máxima que o robô pode assumir (0 - 1,5)
    float max_ang_velocity; //!<Velocidade angular máxima que o robô pode assumir (0 - 10)
    int dribbler_velocity; //!<Velocidade do dribbler em valor de pwm (0 - 127)
    int kick_power; //!<Força do chute em pwm (0 - 127)
    int pass_power; //!<Força do passe em pwm (0 - 127)
    Mat_<float> velocity; //!<Matriz (3,1) de velocidade do robô [Vx, Vy, Vo]
    Mat_<float> velocity_robot; //!<Matriz (3,1) de velocidade do robô [Vx, Vy, Vo]
    //Mat_<float> M; //!<Matriz (4,3) de modelo cinemático do robô para distribuir a velocidade para as quatro rodas
    Mat_<float> R; //!<Matriz (3,3) de rotação utilizada para acertar as coordenadas do robô com as do joystick (y+ como frente do robô)
    float robot_angle; //!<Angulo necessário em radianos para utilizar na matriz R (Pi/2)
    //Mat_<float> velocity_wheels; //!<Matriz (4,1) de velocidade das rodas do robô
    bool rotating; //!<Flag para indicar se o botão de rotação está pressionado ou não
    bool dribbling; //!<Flag para indicar se o botão de dribbler está pressionado ou não
    int kicking; //!<Variável para garantir que o robô vai tentar chutar KICK_TIMES para facilitar o chute na hora de controlar
    float bonus_velocity; //!<Variável para dar um boost de velocidade quando clicar no botão

    SerialMessage message; //!<Mensagem que será envidada
    SerialSender *serial; //!<Ponteiro para a thread de comunicação serial

    /*!
     * \brief initKinematicModel inicia a matriz M criando o modelo cinemático do robô
     */
    void initKinematicModel();
    /*!
     * \brief calculateVelocity calcula a velocidade linear do robô com base no valor oferecido pelo vetor axis
     */
    void calculateVelocity();
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
    ManualControl(int _device_n, SerialSender *_serial);

    ~ManualControl();

    /*!
     * \brief start modifica a flag running para true e reconstroi a thread td para reiniciar a thread
     */
    void start();
    /*!
     * \brief stop trava com o mutex, modificia a flag running para false e da join na thread td
     */
    void stop();

    void setId(int _id);
    void setMaxVelocity(float _velocity);
    void setMaxAngularVelocity(float _velocity);
    void setDribblerVelocity(int _velocity);
    void setKickPower(int _power);
    void setPassPower(int _power);
};

#endif // MANUALCONTROL_H
