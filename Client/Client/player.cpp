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

void Player::displayPlayer(int startingScore){
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
			lastlegopp->clear();
		}
	}

	int total = std::reduce(history.begin(), history.end(), 0,
		[](int sum, const std::array<int, 3>& a) {
			return sum + std::reduce(a.begin(), a.end(), 0);
		});

	ImGui::Text("Score: %d", startingScore - total);
	ImGui::Text("Average: %f", total / (float)history.size());
	ImGui::Text("Legs Won: %d", legsWon);
	ImGui::Text("Sets Won: %d", setsWon);

	//------------dart board visualization
	double dbheight = 400;
	ImVec2 boardSize = ImVec2(dbheight, dbheight);
	ImVec2 dartSize = ImVec2(20, 20);
	ImVec2 basePos = ImVec2(ImGui::GetCursorPosX() + (dbheight*(195.0/400.0)), ImGui::GetCursorPosY() + (dbheight * (195.0 / 400.0)));
	HelloImGui::ImageFromAsset("..\\assets\\board.png", boardSize);

	ImVec2 initialPos = ImGui::GetCursorPos();
	if (lastleg.size() > 0) {
		for (int i = 0; i < lastleg.size(); i++) {
			ImVec2 dartPos = ImVec2(basePos.x + (lastleg[i].first * (dbheight * (155.0 / 400.0))), basePos.y - (lastleg[i].second * (dbheight * (155.0 / 400.0))));
			ImGui::SetCursorPos(dartPos);
			HelloImGui::ImageFromAsset("..\\assets\\dart.jpg", dartSize);
		}
	}
	ImGui::SetCursorPos(initialPos);

	ImVec2 dart1Pos = ImVec2(basePos.x + (1 * (dbheight * (155.0 / 400.0))), basePos.y - (1 * (dbheight * (155.0 / 400.0)))); //need to adjust by coordinates of previous throw * -200 pixels
	ImVec2 dart2Pos = ImVec2(basePos.x + (1 * (dbheight * (155.0 / 400.0))), basePos.y - (0 * (dbheight * (155.0 / 400.0))));
	ImVec2 dart3Pos = ImVec2(basePos.x + (0 * (dbheight * (155.0 / 400.0))), basePos.y - (1 * (dbheight * (155.0 / 400.0))));
	/*
	ImVec2 initialPos = ImGui::GetCursorPos();

	
	ImGui::SetCursorPos(dart1Pos);
	HelloImGui::ImageFromAsset("..\\assets\\dart.jpg", dartSize);
	ImGui::SetCursorPos(dart2Pos);
	HelloImGui::ImageFromAsset("..\\assets\\dart.jpg", dartSize);
	ImGui::SetCursorPos(dart3Pos);
	HelloImGui::ImageFromAsset("..\\assets\\dart.jpg", dartSize);

	ImGui::SetCursorPos(initialPos);
	*/

	//--------------------------------------

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

bool Player::addPoint(int score, int goal){
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

	if (goal == std::reduce(history.begin(), history.end(), 0,
		[](int sum, const std::array<int, 3>&a) {
			return sum + std::reduce(a.begin(), a.end(), 0);
		}))
	{
		this->legsWon++;
		if (legsWon == 3) {
			setsWon++;
			legsWon = 0;
			return true;
		}
	}
	return false;
}

void Player::clearHistory() {
	history = { {0, 0, 0} };
}