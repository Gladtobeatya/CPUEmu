#include "CPU.h"

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
	while (pc < pSize) {
		std::string line = program[pc++];
		std::stringstream ss(line);
		std::string op, a, b;
		ss >> op;
		if (op == "SET") {
			ss >> a >> b;
			regs[a] = stoi(b);
		}
		else if (op == "ADD") {
			ss >> a >> b;
			regs[a] += stoi(b);
		}
		else if (op == "JMP") {
			ss >> a;
			pc = stoi(a) - 1;
			if(pc < 1 || pc >= pSize) {
				std::cerr << "Jump out of bounds: " << pc + 1<< std::endl;
				break;
			}
		}
		else if (op == "PRINT") {
			ss >> a;
			std::cout << regs[a] << std::endl;
		}

	}
}
