#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <string>
#include <mutex>

class DoubleClient {
public:
    DoubleClient(const std::string& server_ip, const std::string& port);
    ~DoubleClient();

    void setServerIP(const std::string& server_ip);
    void setPort(const std::string& port);

    // Start the client thread
    void start();

    // Stop the client thread
    void stop();

    // Get the latest received values
    bool getLatestValues(double& value1, double& value2);

    // Check if new data has arrived
    bool hasNewData() const { return data_arrived.load(); }

    // Check if client is connected
    bool isConnected() const { return connected.load(); }

private:
    void clientThread();

    boost::asio::io_context io_context;
    std::string server_ip;
    std::string port;
    std::thread client_thread;
    std::atomic<bool> running{ false };
    std::atomic<bool> connected{ false };
    std::atomic<bool> data_arrived{ false };

    std::mutex data_mutex;
    double latest_value1{ 0.0 };
    double latest_value2{ 0.0 };
};












// main.cpp
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>\n";
        std::cerr << "Example: " << argv[0] << " 192.168.1.100 12345\n";
        return 1;
    }

    DoubleClient client(argv[1], argv[2]);
    client.start();

    // Example: periodically check for new values
    double value1, value2;
    try {
        while (true) {
            if (client.getLatestValues(value1, value2)) {
                std::cout << "Main thread got values: "
                    << value1 << ", " << value2 << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Main thread error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}