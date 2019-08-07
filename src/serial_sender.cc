// Copyright 2019 FURGBot

#include "serial_sender.h"

#include <string>

namespace furgbol {
namespace joystick {

SerialSender::SerialSender(std::string serial_port_name) : io_service_(), serial_port_(io_service_), buffer_(buf_.data()), buffer_to_send_(8, 0) {
    serial_port_.open(serial_port_name);

    try {
        serial_port_.set_option(boost::asio::serial_port_base::baud_rate(115200));
        serial_port_.set_option(boost::asio::serial_port_base::character_size(8));
    } catch (boost::system::system_error error) {
        std::cout << "Comunicator Error: " << error.what() << std::endl;
    }
}

SerialSender::~SerialSender() {
    serial_port_.close();
}

void SerialSender::send(SerialMessage message_to_send) {
    message_to_send.serialize(buffer_to_send_);
    serial_port_.write_some(boost::asio::buffer(buffer_to_send_, buffer_to_send_.size()));
}

} // namespace joystick
} // namespace furgbol