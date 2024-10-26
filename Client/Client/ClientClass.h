#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <nlohmann/json.hpp>

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

                // receive the actual serialized data
                std::vector<char> buffer(msg_length);
                size_t received_length = boost::asio::read(socket_, boost::asio::buffer(buffer));

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

        // Parse JSON data
        auto json_data = json::parse(data);
        for (const auto& item : json_data) {
            Point point;
            point.x = item["x"];
            point.y = item["y"];
            points.push_back(point);
        }

        return points;
    }
};
