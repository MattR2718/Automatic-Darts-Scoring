#include "mt_client.h"
#include <iostream>

using boost::asio::ip::tcp;

DoubleClient::DoubleClient(const std::string& server_ip, const std::string& port)
    : server_ip(server_ip), port(port) {
}

DoubleClient::~DoubleClient() {
    stop();
}

void DoubleClient::setServerIP(const std::string& server_ip){
	this->server_ip = server_ip;
}

void DoubleClient::setPort(const std::string& port){
	this->port = port;
}

void DoubleClient::start() {
    if (!running) {
        running = true;
        client_thread = std::thread(&DoubleClient::clientThread, this);
    }
}

void DoubleClient::stop() {
    running = false;
    if (client_thread.joinable()) {
        client_thread.join();
    }
    connected = false;
}

bool DoubleClient::getLatestValues(double& value1, double& value2) {
    if (!data_arrived) {
        return false;
    }

    std::lock_guard<std::mutex> lock(data_mutex);
    value1 = latest_value1;
    value2 = latest_value2;
    data_arrived = false;
    return true;
}

void DoubleClient::clientThread() {
    while (running) {
        try {
            // Resolve the endpoint
            tcp::resolver resolver(io_context);
            tcp::resolver::results_type endpoints =
                resolver.resolve(server_ip, port);

            // Create and connect the socket
            tcp::socket socket(io_context);
            std::cout << "Connecting to " << server_ip << ":" << port
                << "..." << std::endl;

            boost::asio::connect(socket, endpoints);
            connected = true;
            std::cout << "Connected successfully!" << std::endl;

            while (running && connected) {
                std::array<double, 2> received_doubles;

                boost::asio::read(socket,
                    boost::asio::buffer(&received_doubles, sizeof(double) * 2));

                // Update the latest values
                {
                    std::lock_guard<std::mutex> lock(data_mutex);
                    latest_value1 = received_doubles[0];
                    latest_value2 = received_doubles[1];
                    data_arrived = true;
                }

                std::cout << "Received values: " << received_doubles[0]
                    << ", " << received_doubles[1] << std::endl;
            }

        }
        catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            connected = false;

            // Wait before trying to reconnect
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}