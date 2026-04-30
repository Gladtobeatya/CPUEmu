#include "CPU.h"

void CPU::init()
{
	ops["SET"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		cpu.regs[a] = stoi(b);
	};
	ops["ADD"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		cpu.regs[a] += stoi(b);
	};
	ops["JMP"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		cpu.pc = stoi(a) - 1;
		if(cpu.pc < 1 || cpu.pc >= cpu.program.size()) {
			std::cerr << "Jump out of bounds: " << cpu.pc + 1 << " stopping CPU" << std::endl;
			cpu.bIsRunning = false;
		}
	};
	ops["PRINT"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		std::cout << cpu.regs[a] << std::endl;
	};
	bIsRunning = true;
}

void CPU::load(const std::string& filename)
{
	std::ifstream file(filename);
	if(file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			program.push_back(line);
		}
	}
}

void CPU::run()
{
	size_t pSize = program.size();
	std::cerr << "Program loaded with " << pSize << " instructions." << std::endl;
	while (bIsRunning && pc < pSize) {
		std::string line = program[pc++];
		std::stringstream ss(line);
		std::string op;
		ss >> op;
		ops[op](*this, ss);

	}
}
