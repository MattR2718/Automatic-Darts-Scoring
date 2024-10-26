#pragma once
#include <boost/asio.hpp>
#include <iostream>


using boost::asio::ip::tcp;

class ClientClass {
public:
    ClientClass(boost::asio::io_context& io_context, const std::string& server_ip)
        : socket_(io_context) {
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(server_ip, std::to_string(PORT));

        // Connect to the server
        boost::asio::connect(socket_, endpoints);
    }

    void send(const std::string& msg) {
        // Encode the message and calculate its length
        std::string message = msg;
        std::size_t msg_length = message.size();

        // Prepare the header with padding
        std::string send_length = std::to_string(msg_length);
        send_length.insert(send_length.end(), HEADER - send_length.size(), ' '); // Padding

        // Send header and message
        boost::asio::write(socket_, boost::asio::buffer(send_length));
        boost::asio::write(socket_, boost::asio::buffer(message));

        // Receive response from server
        char response[2048];
        size_t length = socket_.read_some(boost::asio::buffer(response));
        std::cout << "Server: " << std::string(response, length) << std::endl;
    }

private:
    tcp::socket socket_;
};