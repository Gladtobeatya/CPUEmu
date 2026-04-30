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
	bool bIsRunning = true;
	std::unordered_map<std::string, int> regs;
	std::vector<std::string> program;

	// Define a type for instruction functions (alias)
	using Instr = std::function<void(CPU&, std::stringstream&)>;
	std::unordered_map<std::string, Instr> ops;

	// CPU flag set by CMP: true if last comparison result was equal
	bool zeroFlag = false;

	void init();
	void jump(int target);
	bool isValidReg(const std::string& reg);
	static bool isValidImm(const std::string& imm);
	void fail();
	int tryGetValue(const std::string& s);

public:

	CPU() { init(); }
	void load(const std::string& filename);
	void run();


};

