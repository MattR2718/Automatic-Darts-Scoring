// Client.cpp : Defines the entry point for the application.
//

#include "Client.h"

HelloImGui::ScreenSize get_screen_size() {

    glfwInit();

    // Get primary monitor
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    int monitorWidth = mode->width;
    int monitorHeight = mode->height;

    const HelloImGui::ScreenSize window_size = { monitorWidth * 0.5, monitorHeight * 0.5 };

	return window_size;
}


int main(int argc, char** argv){

    std::vector<Player> players;
    players.emplace_back(Player());
    players.emplace_back(Player());

    players[0].setTurn(true);
    players[1].setTurn(false);

    players[0].setOpTurnPtr(players[1].getTurnPtr());
    players[1].setOpTurnPtr(players[0].getTurnPtr());

    std::vector<Point> points;


    auto guiFunction = [&players, &points]() {

        ImGui::ShowDemoWindow();

		ImGui::InputText("Server IP", server_ip, 16);
        ImGui::SameLine();
        if (ImGui::Button("Connect")) {
			boost::asio::io_context io_context;
			std::string sip = std::string(server_ip);
			ClientClass client(io_context, sip);
			
			client.listen(points);
        }

		if (points.size() > 0) {
            for (Player& p : players) {
                p.addPoint(points[0].x, points[0].y);
				points.erase(points.begin());
            }
		}

        for (Player& p : players) {
			p.displayPlayer();
        }
        if (ImGui::Button("Bye!"))                 // Display a button
            // and immediately handle its action if it is clicked!
            HelloImGui::GetRunnerParams()->appShallExit = true;
        };
    HelloImGui::Run(guiFunction, "Hello, globe", true);
    return 0;
}

