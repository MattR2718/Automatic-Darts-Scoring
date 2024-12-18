#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <nlohmann/json.hpp>
#include <boost/asio.hpp>

const int HEADER = 64; // Bytes
const int PORT = 5050;
const std::string DISCONNECT_MESSAGE = "!DISCONNECT";

char server_ip[16]{ 0 };

using boost::asio::ip::tcp;
using json = nlohmann::json;

struct Point {
    double x;
    double y;
};

class ClientClass {
public:
    ClientClass(boost::asio::io_context& io_context, const std::string& server_ip)
        : socket_(io_context) {
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(server_ip, std::to_string(PORT));

        // Connect to the server
        boost::asio::connect(socket_, endpoints);
    }

    void listen(std::vector<Point>& points) {
        while (true) {
            try {
                // Receive response from server
                char length_buffer[HEADER];
                size_t length = socket_.read_some(boost::asio::buffer(length_buffer));

                std::string length_str(length_buffer, length);
                length_str.erase(std::remove(length_str.begin(), length_str.end(), ' '), length_str.end());
                std::size_t msg_length = std::stoi(length_str);

                std::cout << "MESSAGE LENGTH: " << msg_length << '\n';

                // receive the actual serialized data
                std::vector<char> buffer(msg_length);
                size_t received_length = boost::asio::read(socket_, boost::asio::buffer(buffer));

				std::cout << "RECEIVED LENGTH: " << received_length << '\n';

                // Store the received object in a vector of tuples
                last_received_object_.clear();
                for (size_t i = 0; i < received_length; ++i) {
                    last_received_object_.push_back(buffer[i]);
                }

                // Converts recieved data into string
                std::string received_data(last_received_object_.begin(), last_received_object_.end());
                //std::cout << "Received an object from the server: " << received_data << std::endl;

                // Parse the received JSON data into a vector of Points
                points = parse_data(received_data);

                // Print the points
                for (const auto& point : points) {
                    std::cout << "Point: (" << point.x << ", " << point.y << ")\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error receiving data: " << e.what() << std::endl;
                break;
            }
        }
    }

private:
    tcp::socket socket_;
    std::vector<char> last_received_object_;

    std::vector<Point> parse_data(const std::string& data) {
        std::vector<Point> points;

        std::cout << "DATA: \n";
        std::cout << data << '\n';
        std::cout << "END DATA\n";

        // Parse JSON data
        auto json_data = json::parse(data);
        std::cout << json_data << '\n';
        for (const auto& item : json_data) {
            Point point;
            point.x = item["x"];
            point.y = item["y"];
            points.push_back(point);
        }

        return points;
    }
};


void getDarts(std::string server_ip, std::vector<Point>& pts) {
    //std::vector<Point> points = {};
    try {
        boost::asio::io_context io_context;

        //std::string server_ip = boost::asio::ip::host_name();

        ClientClass client(io_context, server_ip);

        // Listen for messages from the server in a separate thread
        std::thread listener_thread([&client, &pts]() {
            client.listen(pts);
            });

        // Join the listener thread to the main thread
        //listener_thread.join();

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
