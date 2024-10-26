// Client.cpp : Defines the entry point for the application.
//

#include "Client.h"


int main(int argc, char** argv){

    auto guiFunction = []() {
        ImGui::Text("Hello, ");                    // Display a simple label
        HelloImGui::ImageFromAsset("Bran.jpg");   // Display a static image
        if (ImGui::Button("Bye!"))                 // Display a button
            // and immediately handle its action if it is clicked!
            HelloImGui::GetRunnerParams()->appShallExit = true;
        };
    HelloImGui::Run(guiFunction, "Hello, globe", true);
    return 0;
}

