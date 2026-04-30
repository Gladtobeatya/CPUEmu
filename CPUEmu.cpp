// CPUEmu.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "CPU.h"

using namespace std;

int main()
{


    CPU cpu;
	cpu.load("input.txt");
	cpu.run();
	
	return 0;	
}
