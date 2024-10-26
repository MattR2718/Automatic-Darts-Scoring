// Client.cpp : Defines the entry point for the application.
//

#include "Client.h"
#include <fmt/core.h>
#include <boost/asio.hpp>
#include <iostream>
#include <string>

const int HEADER = 64; // Bytes for the length of the message
const int PORT = 5050;
const std::string DISCONNECT_MESSAGE = "!DISCONNECT";
const std::string FORMAT = "utf-8";

#include "ClientClass.h"

using boost::asio::ip::tcp;

using namespace std;

int main()
{
    try {
        boost::asio::io_context io_context;

        // Set server IP
        std::string server_ip = boost::asio::ip::host_name();  // Gets localhost

        ClientClass client(io_context, server_ip);

        // Send messages to server
        client.send("Hello World!");
        client.send("HI YIPPIE!");
        client.send(DISCONNECT_MESSAGE);

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}