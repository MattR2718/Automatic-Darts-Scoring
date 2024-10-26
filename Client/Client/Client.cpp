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

    auto guiFunction = [&players]() {

        ImGui::ShowDemoWindow();

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

