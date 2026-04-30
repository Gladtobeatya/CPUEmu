#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

class CPU
{
private:
	int pc = 0;
	std::unordered_map<std::string, int> regs;
	std::vector<std::string> program;

public:

	void load(const std::string& filename);
	void run();


};

