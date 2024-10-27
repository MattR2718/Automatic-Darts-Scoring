#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include <hello_imgui/hello_imgui.h>
#include <imgui.h>

class Player {
public:
	Player(std::string n = "Player");

	void displayPlayer(int startingScore);

	void addLeg(std::array<int, 3> leg);

	void setTurn(bool t);

	bool getTurn();

	bool* getTurnPtr();

	void setOpTurnPtr(bool* t);

	bool addPoint(int score, int goal);

	void clearHistory();

	int legsWon = 0;

	std::vector<std::pair<double, double>> lastleg;
	std::vector<std::pair<double, double>> *lastlegopp;

private:
	std::string name = "";
	int score = 0;
	std::vector<std::array<int, 3>> history = { {0, 0, 0} };

	std::array<int, 3> manualLeg{ 0, 0, 0 };

	inline static int uid = 1;
	int pid = -1;

	bool scrollTable = true;

	bool turn = false;

	bool* oppositePlayer = nullptr;

	int setsWon = 0;

};