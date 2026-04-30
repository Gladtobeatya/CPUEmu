#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>

class CPU
{
private:
	int pc = 0;
	bool bIsRunning = false;
	std::unordered_map<std::string, int> regs;
	std::vector<std::string> program;

	// Define a type for instruction functions (alias)
	using Instr = std::function<void(CPU&, std::stringstream&)>;
	std::unordered_map<std::string, Instr> ops;

	void init();

public:

	CPU() { init(); }
	void load(const std::string& filename);
	void run();


};

