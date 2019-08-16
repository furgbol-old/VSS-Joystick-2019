// Copyright 2019 FURGBot

#ifndef JOYSTICK_SERIAL_SENDER_H
#define JOYSTICK_SERIAL_SENDER_H

#include "serial_message.h"

#include <boost/asio.hpp>

namespace furgbol {
namespace joystick {

class SerialSender {
  private:
    boost::asio::streambuf buf_;
    boost::asio::io_service io_service_;
    boost::asio::streambuf::const_buffers_type buffer_;
    boost::asio::serial_port serial_port_;
    std::vector<unsigned char> buffer_to_send_;

  public:
    SerialSender(std::string serial_port_name);
    ~SerialSender();

    void send(SerialMessage message_to_send);
};

}  // namespace joystick
}  // namespace furgbol

#endif // JOYSTICK_SERIAL_SENDER_H