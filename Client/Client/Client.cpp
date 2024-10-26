// Client.cpp : Defines the entry point for the application.
//

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <cmath>

#define pi 3.14159265

const int HEADER = 64; // Bytes
const int PORT = 5050;
const std::string DISCONNECT_MESSAGE = "!DISCONNECT";

#include "ClientClass.h"

using namespace std;

double distFromBull(double x, double y) {
    return sqrt((std::pow(x, 2)) + std::pow(y, 2));
}

int getArea(double dist) {
    if (dist > 0 && dist <= 0.037353) {
        return 1;
    }
    if (dist > 0.037353 && dist <= 0.094118) {
        return 2;
    }
    if (dist > 0.582353 && dist <= 0.629412) {
        return 3;
    }
    if (dist > 0.952941 && dist <= 1) {
        return 4;
    }
    return 0;
}

int getQuadrant(double x, double y) {
    if (x >= 0 && y > 0) {
        return 0;
    }
    if (x > 0 && y <= 0) {
        return 1;
    }
    if (x <= 0 && y < 0) {
        return 2;
    }
    if (x < 0 && y >= 0) {
        return 3;
    }
    return 0;
}

int getNumber(double x, double y, int q, double dist) {
    double angle = 90;
    switch (q)
    {
    case 0: //top right
        if (x != 0) {
            angle = acos(x / dist) * (180 / pi);
        }
        if (angle >= 0 && angle < 9) {
            return 6;
        }
        else if (angle >= 9 && angle < 27) {
            return 13;
        }
        else if (angle >= 27 && angle < 45) {
            return 4;
        }
        else if (angle >= 45 && angle < 63) {
            return 18;
        }
        else if (angle >= 63 && angle < 81) {
            return 1;
        }
        else if (angle >= 81 && angle <= 90) {
            return 20;
        }
        break;
    case 1: //bottom right
        if (y != 0) {
            angle = acos(-y / dist) * (180 / pi);
        }
        if (angle >= 0 && angle < 9) {
            return 3;
        }
        else if (angle >= 9 && angle < 27) {
            return 17;
        }
        else if (angle >= 27 && angle < 45) {
            return 2;
        }
        else if (angle >= 45 && angle < 63) {
            return 15;
        }
        else if (angle >= 63 && angle < 81) {
            return 10;
        }
        else if (angle >= 81 && angle <= 90) {
            return 6;
        }
        break;
    case 2: //bottom left
        if (x != 0) {
            angle = acos((-x) / dist) * (180 / pi);
        }
        if (angle >= 0 && angle < 9) {
            return 11;
        }
        else if (angle >= 9 && angle < 27) {
            return 8;
        }
        else if (angle >= 27 && angle < 45) {
            return 16;
        }
        else if (angle >= 45 && angle < 63) {
            return 7;
        }
        else if (angle >= 63 && angle < 81) {
            return 19;
        }
        else if (angle >= 81 && angle <= 90) {
            return 3;
        }
        break;
    case 3: //top left
        if (y != 0) {
            angle = acos((y) / dist) * (180 / pi);
        }
        if (angle >= 0 && angle < 9) {
            return 20;
        }
        else if (angle >= 9 && angle < 27) {
            return 5;
        }
        else if (angle >= 27 && angle < 45) {
            return 12;
        }
        else if (angle >= 45 && angle < 63) {
            return 9;
        }
        else if (angle >= 63 && angle < 81) {
            return 14;
        }
        else if (angle >= 81 && angle <= 90) {
            return 11;
        }
        break;
    default:
        return 0;
        break;
    }
    return 0;
}

int getScore(double x, double y) {
    int number; //stores number hit
    double dist; //stores distance from bullseye
    int area; //stores area the dart hit 0-normal 1-redbull 2-greenbull 3-triple 4-double
    int quadrant; //stores which quadrant of board was hit 0-topright 1-bottomright 2-bottomleft 3-topleft
    if (x == 0 && y == 0) { //dart is in bullseye (this stops dividing by 0)
        return 50;
    }
    dist = distFromBull(x, y);
    if (dist > 1) { //dart is outside of board
        return 0;
    }
    area = getArea(dist);
    quadrant = getQuadrant(x, y);
    number = getNumber(x, y, quadrant, dist);
    switch (area)
    {
    case 0: //normal
        return number;
    case 1: //red bullseye
        return 50;
    case 2: //green bullseye
        return 25;
    case 3: //triple
        return 3 * number;
    case 4: //double
        return 2 * number;
    default:
        break;
    }
    return 0;
}

//----------------------------Just Scoring Above-----------------------------------------------------

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
