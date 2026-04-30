#include "CPU.h"

void CPU::init()
{
	// Initialize registers R0 to R9 to 0
	for (int i = 0; i < 10; i++) {
		regs["R" + std::to_string(i)] = 0;
	}

	ops["SET"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			cpu.regs[a] = cpu.tryGetValue(b);
		}
		else cpu.fail();
	};
	ops["ADD"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			cpu.regs[a] += cpu.tryGetValue(b);
		}
		else cpu.fail();
	};
	ops["SUB"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			cpu.regs[a] -= cpu.tryGetValue(b);
		}
		else cpu.fail();
	};
	ops["JMP"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (CPU::isValidImm(a)) {
			cpu.jump(stoi(a));
		}
		else cpu.fail();
	};
	ops["PRINT"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		std::cout << cpu.tryGetValue(a) << std::endl;
	};
	ops["MUL"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			cpu.regs[a] *= cpu.tryGetValue(b);
		}
		else cpu.fail();
	};
	ops["DIV"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			int val = cpu.tryGetValue(b);
			if (val == 0) {
				std::cerr << "Div by 0.. Stopping CPU" << std::endl;
				cpu.bIsRunning = false;
				return;
			}
			cpu.regs[a] /= val;
		}
		else cpu.fail();
	};
	ops["INC"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.isValidReg(a)) {
			cpu.regs[a]++;
		}
		else cpu.fail();
	};
	ops["DEC"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.isValidReg(a)) {
			cpu.regs[a]--;
		}
		else cpu.fail();
	};
	// Set zeroFlag to true if equal
	ops["CMP"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			cpu.zeroFlag = (cpu.regs[a] == cpu.tryGetValue(b));
		}
		else cpu.fail();
	};
	// Jump if zeroFlag is true
	ops["JZ"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.zeroFlag && CPU::isValidImm(a)){
			cpu.jump(stoi(a));
		}
		else cpu.fail();
	};
	// Jump if zeroFlag is not true
	ops["JNZ"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (!cpu.zeroFlag && CPU::isValidImm(a)) {
			cpu.jump(stoi(a));
		}
		else cpu.fail();
	};
	ops["MOV"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValidReg(a)) {
			cpu.regs[a] = cpu.tryGetValue(b);
		}
		else cpu.fail();
	};
	ops["HALT"] = [](CPU& cpu, std::stringstream& ss) {
		cpu.bIsRunning = false;
	};
}

void CPU::jump(int target)
{
	// PC is 0-indexed internally
	pc = target;
	if (pc < 0 || pc >= program.size()) {
		std::cerr << "Jump out of bounds: " << pc << " stopping CPU" << std::endl;
		bIsRunning = false;
	}
}

bool CPU::isValidReg(const std::string& reg)
{
	return regs.contains(reg);
}

bool CPU::isValidImm(const std::string& imm)
{
	for(auto c : imm) {
		if (!isdigit(c)) return false;
	}
	return true;
}

void CPU::fail()
{
	std::cout << "CPU error at : " << pc - 1 << std::endl;
	bIsRunning = false;
}

int CPU::tryGetValue(const std::string& s)
{
	if (isValidReg(s)) {
		return regs[s];
	}
	//Lidl test to check if it's a number, assuming no negative numbers for simplicity
	if (CPU::isValidImm(s)) {
		return std::stoi(s);
	}
	fail();
	return 0;
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
		if (ops.contains(op)) {
			ops.at(op)(*this, ss);
		}
		else {
			std::cerr << "Unknown opcode: " << op << " Stopping CPU" << std::endl;
			bIsRunning = false;
		}

	}
}
