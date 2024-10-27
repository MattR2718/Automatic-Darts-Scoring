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
	if (turn) {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	}
	ImGui::Text("Player: %s", name.c_str());
	if (turn) {
		ImGui::PopStyleColor();
	}
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	const ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 7);

	if (ImGui::BeginTable(std::string("##" + name + std::to_string(pid)).c_str(), (int)history.size(), flags, outer_size)) {

		//ImGui::SetColumnWidth(-1, 100.0);

		for (int i = 0; i < history.size(); i++) {
			ImGui::TableSetupColumn(std::string("Turn " + std::to_string(i + 1)).c_str(), ImGuiTableColumnFlags_WidthFixed, 400.0);
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

		if (this->scrollTable) {
			float maxScrollX = ImGui::GetScrollMaxX();
			ImGui::SetScrollX(maxScrollX + 400);
			this->scrollTable = false;
		}
		ImGui::EndTable();

	}

	ImGui::InputInt3(std::string("##" + std::to_string(this->pid)).c_str(), manualLeg.data());
	ImGui::SameLine();
	if (ImGui::Button(std::string("Add Throw##" + std::to_string(this->pid)).c_str())) {
		if (turn) {
			this->addLeg(manualLeg);
			manualLeg = { 0, 0, 0 };
			this->scrollTable = true;
			turn = !turn;
			*oppositePlayer = !*oppositePlayer;
		}
	}

	int total = std::reduce(history.begin(), history.end(), 0,
		[](int sum, const std::array<int, 3>& a) {
			return sum + std::reduce(a.begin(), a.end(), 0);
		});

	ImGui::Text("Score: %d", total);
	ImGui::Text("Average: %f", total / (float)history.size());




	ImGui::Separator();
}

void Player::addLeg(std::array<int, 3> leg){
	history.push_back(leg);
}

void Player::setTurn(bool t){
	turn = t;
}

bool Player::getTurn(){
	return this->turn;
}

bool* Player::getTurnPtr(){
	return &turn;
}

void Player::setOpTurnPtr(bool* t){
	oppositePlayer = t;
}

void Player::addPoint(int score){
	int set = -1;
	for (int i = 0; i < manualLeg.size(); i++) {
		if (manualLeg[i] == 0) {
			manualLeg[i] = score;
			set = i;
			break;
		}
	}
	if (set == 2) {
		this->addLeg(manualLeg);
		manualLeg = { 0, 0, 0 };
		this->scrollTable = true;
		turn = !turn;
		*oppositePlayer = !*oppositePlayer;
	}
	else if (set == -1) {
		std::cerr << "Point not set, zero scored\n";
	}
	
}
