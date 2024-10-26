// Client.cpp : Defines the entry point for the application.
//

#include <boost/asio.hpp>
#include <iostream>
#include <thread>


const int HEADER = 64; // Bytes
const int PORT = 5050;
const std::string DISCONNECT_MESSAGE = "!DISCONNECT";

#include "ClientClass.h"

using namespace std;

int main(){
    std::vector<Point> points = {};
    try {
        boost::asio::io_context io_context;

        std::string server_ip = boost::asio::ip::host_name();

        ClientClass client(io_context, server_ip);

        // Listen for messages from the server in a separate thread
        std::thread listener_thread([&client, &points](){
            client.listen(points);
        });

        // Join the listener thread to the main thread
        listener_thread.join();

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
