// Client.cpp : Defines the entry point for the application.
//

#include "Client.h"

int main(int argc, char** argv){

    std::vector<Player> players;
    players.emplace_back(Player());
    players.emplace_back(Player());

    players[0].setTurn(true);
    players[1].setTurn(false);

    players[0].setOpTurnPtr(players[1].getTurnPtr());
    players[1].setOpTurnPtr(players[0].getTurnPtr());

    std::vector<Point> points;

    DoubleClient client("", "");

	DartMath dm;


    auto guiFunction = [&players, &client, &dm]() {
        ImGui::InputText("Server IP", server_ip, 16);
        ImGui::SameLine();

        static bool shouldOpenModal = false;
        static double modalValue1 = 0.0;
        static double modalValue2 = 0.0;
        static int modalScore = 0;
        static Player* currentPlayer = nullptr;

        // Add static variables to track previous values
        static double previousValue1 = 0.0;
        static double previousValue2 = 0.0;

		static int startingScore = 501;

        if (ImGui::Button("Connect")) {
            std::string sip = std::string(server_ip);
            client.setServerIP(sip);
            client.setPort("5050");
            client.start();
        }
		
        ImGui::SameLine();

        if (ImGui::Button("Reset Leg"))
        {
			for (auto& p : players)
			{
				p.clearHistory();
			}
        }

		if (ImGui::InputInt("Starting Score", &startingScore))
		{
			for (auto& p : players)
			{
                p.clearHistory();
			}
		}

        if (client.isConnected()) {
            double value1, value2;
            if (client.getLatestValues(value1, value2)) {
                // Check if the values are different from the previous values
                bool valuesChanged = (value1 != previousValue1) || (value2 != previousValue2);

                for (auto& p : players) {
                    if (p.getTurn() && !shouldOpenModal && valuesChanged) {
                        modalValue1 = value1;
                        modalValue2 = value2;
                        modalScore = dm.getScore(value1, value2);
                        currentPlayer = &p;
                        shouldOpenModal = true;
                        ImGui::OpenPopup("Prediction");

                        // Update previous values after opening the modal
                        previousValue1 = value1;
                        previousValue2 = value2;
                        break;
                    }
                }
            }
        }

        // Modal window code
        if (ImGui::BeginPopupModal("Prediction", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Modal)) {
            ImGui::Text("Value1: %f", modalValue1);
            ImGui::Text("Value2: %f", modalValue2);
            ImGui::InputInt("Score Prediction", &modalScore);

            if (ImGui::Button("Accept") && currentPlayer != nullptr) {
                if (currentPlayer->addPoint(modalScore, startingScore)) {
                    for (auto& p : players) {
                        p.legsWon = 0;
                    }
                }
                shouldOpenModal = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Skip")) {
                shouldOpenModal = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        for (Player& p : players) {
            p.displayPlayer(startingScore);
        }
        
    };


    HelloImGui::Run(guiFunction, "Darts Scorer", true, true);

    client.stop();

    return 0;
}

//#ifdef _WIN32
//#include <windows.h>
//
//// Windows entry point when using /SUBSYSTEM:WINDOWS
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//    // Call the standard main function
//    return main(__argc, __argv);
//}
//#endif