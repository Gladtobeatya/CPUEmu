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
		if(cpu.isValid(a)){
			cpu.regs[a] += stoi(b);
		}
	};
	ops["SUB"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValid(a)) {
			cpu.regs[a] -= stoi(b);
		}
	};
	ops["JMP"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		cpu.jump(stoi(a));
	};
	ops["PRINT"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.isValid(a)) {
			std::cout << cpu.regs[a] << std::endl;
		}
	};
	ops["MUL"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValid(a)) {
			cpu.regs[a] *= stoi(b);
		}
	};
	ops["DIV"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValid(a)) {
			if (b == "0") {
				std::cerr << "Div by 0.. Stopping CPU" << std::endl;
				cpu.bIsRunning = false;
				return;
			}
			cpu.regs[a] /= stoi(b);
		}
	};
	ops["INC"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.isValid(a)) {
			cpu.regs[a]++;
		}
	};
	ops["DEC"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.isValid(a)) {
			cpu.regs[a]--;
		}
	};
	// Compare two registers and set zeroFlag if they are equal
	ops["CMP"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a, b;
		ss >> a >> b;
		if (cpu.isValid(a) && cpu.isValid(b)) {
			cpu.zeroFlag = (cpu.regs[a] == cpu.regs[b]);
		}
	};
	// Jump if zeroFlag is false
	ops["JZ"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (cpu.zeroFlag) {
			cpu.jump(stoi(a));
		}
	};
	// Jump if zeroFlag is not true
	ops["JNZ"] = [](CPU& cpu, std::stringstream& ss) {
		std::string a;
		ss >> a;
		if (!cpu.zeroFlag) {
			cpu.jump(stoi(a));
		}
	};


	bIsRunning = true;
}

void CPU::jump(int target)
{
	// PC is 0-indexed internally, but instructions are 1-indexed for the user
	pc = target - 1;
	if (pc < 1 || pc >= program.size()) {
		std::cerr << "Jump out of bounds: " << pc + 1 << " stopping CPU" << std::endl;
		bIsRunning = false;
	}
}

bool CPU::isValid(const std::string& reg)
{
	if (!regs.contains(reg)) {
		std::cerr << "Invalid register: " << reg << " Stopping CPU" << std::endl;
		bIsRunning = false;
	}
	return true;
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
