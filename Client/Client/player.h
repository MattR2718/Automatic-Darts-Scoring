#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <array>

#include <imgui.h>

class Player {
public:
	Player(std::string n = "Player");

	void displayPlayer();

	void addLeg(std::array<int, 3> leg);

private:
	std::string name = "";
	int score = 0;
	std::vector<std::array<int, 3>> history = { {4, 5, 6}, {0, 4, 6}, {8, 98, 180} };

	std::array<int, 3> manualLeg{ 0, 0, 0 };

	inline static int uid = 1;
	int pid = -1;

};