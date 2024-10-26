#include "player.h"

Player::Player(std::string n){
	pid = uid++;
	if (n == "Player") {
		name = n + std::to_string(pid);
	}
	else {
		name = n;
	}
}

void Player::displayPlayer(){
	ImGui::Text("Player: %s", name.c_str());
	
	if (ImGui::BeginTable(std::string("##" + name).c_str(), history.size(), ImGuiTableFlags_NoHostExtendX)) {

		//ImGui::SetColumnWidth(-1, 100.0);

		for (int i = 0; i < history.size(); i++) {
			ImGui::TableSetupColumn(std::string("Turn " + std::to_string(i + 1)).c_str(), ImGuiTableColumnFlags_WidthFixed, 500.0);
		}


		ImGui::TableHeadersRow();

		for (int i = 0; i < 3; i++) {
			ImGui::TableNextRow();
			ImGui::PushID(i);
			for (int j = 0; j < history.size(); j++) {
				ImGui::TableSetColumnIndex(j);
				ImGui::PushID(j);
				switch (i) {
				case 0:
					ImGui::InputInt(std::string("##0" + std::to_string(pid)).c_str(), &history[j][0]);
					break;
				case 1:
					ImGui::InputInt(std::string("##1" + std::to_string(pid)).c_str(), &history[j][1]);
					break;
				case 2:
					ImGui::InputInt(std::string("##2" + std::to_string(pid)).c_str(), &history[j][2]);
					break;
				}
				ImGui::PopID();
			}
			ImGui::PopID();
		}
		ImGui::EndTable();

	}

	ImGui::InputInt3(std::string("Leg##" + std::to_string(this->pid)).c_str(), manualLeg.data());
	ImGui::SameLine();
	if (ImGui::Button(std::string("Add Leg##" + std::to_string(this->pid)).c_str())) {
		this->addLeg(manualLeg);
		manualLeg = { 0, 0, 0 };
	}



	ImGui::Separator();
}

void Player::addLeg(std::array<int, 3> leg){
	history.push_back(leg);
}
