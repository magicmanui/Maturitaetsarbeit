#include "CPU.h"
#include <chrono>
#include <iostream>

using namespace chrono;

CPU::CPU() {
	this->RAM.assign(65536, 0);
	this->clock_speed = 10'000'000;
	CPU::a, CPU::b, CPU::clusterSelect = 0;
	counter = 0;
	CPU::flags = 0;
}

CPU::CPU(int clock_speed = 10'000'000) {
	this->RAM.assign(65536, 0);
	this->clock_speed = clock_speed;
	CPU::a, CPU::b, CPU::clusterSelect = 0;
	counter = 0;
	CPU::flags = 0;
}

CPU::CPU(vector<uint8_t> Ram) {
	this->RAM = Ram;
	this->clock_speed = 10'000'000;
	CPU::a, CPU::b, CPU::clusterSelect = 0;
	counter = 0;
	CPU::flags = 0;
}

vector<int> CPU::step() {
	uint8_t instruction = RAM[counter];
	int clockTicks;
	auto start = high_resolution_clock::now();	
	vector<int> out = {};
	switch (instruction)
	{
	case 0:
		//NOP
		clockTicks = 2;
		++CPU::counter;
		break;
	case 1:
		//lda b
		clockTicks = 3;
		CPU::b = CPU::a;
		++CPU::counter;
		break;
	case 2:
		//lda io
		clockTicks = 7;
		CPU::a = io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		CPU::counter += 3;
		break;
	case 3:
		// lda [addr]
		clockTicks = 8;
		CPU::a = RAM[RAM[counter + 1] + RAM[counter + 2] << 8];
		CPU::counter += 3;
		break;
	case 4:
		// lda b, [haddr]
		clockTicks = 6;
		CPU::a = RAM[CPU::b + RAM[counter + 1] << 8];
		counter += 2;
		break;
	case 7:
		//lda imm
		clockTicks = 4;
		CPU::a = RAM[counter + 1];
		counter += 2;
		break;
	case 8:
		//ldb a
		clockTicks = 3;
		CPU::b = CPU::a;
		counter += 1;
		break;
	case 10:
		//ldb io
		clockTicks = 7;
		CPU::b = io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		counter += 3;
		break;
	case 11:
		//ldb [addr]
		clockTicks = 8;
		CPU::b = RAM[RAM[counter + 1] + RAM[counter + 2] << 8];
		CPU::counter += 3;
		break;
	case 15:
		//ldb imm
		clockTicks = 4;
		CPU::a = RAM[counter + 1];
		counter += 2;
		break;
	case 16:
		//ldc a
		clockTicks = 3;
		CPU::clusterSelect = CPU::a;
		counter += 1;
		break;
	case 17:
		//ldc b
		clockTicks = 3;
		CPU::clusterSelect = CPU::b;
		counter += 1;
		break;
	case 18:
		//ldc io
		clockTicks = 7;
		CPU::clusterSelect = io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		counter += 3;
		break;
	case 19:
		//ldc [addr]
		clockTicks = 8;
		CPU::clusterSelect = RAM[RAM[counter + 1] + RAM[counter + 2] << 8];
		CPU::counter += 3;
		break;
	case 20:
		//ldc b, [haddr]
		clockTicks = 6;
		CPU::clusterSelect = RAM[CPU::b + RAM[counter + 1] << 8];
		counter += 2;
		break;
	case 23:
		//ldc imm
		clockTicks = 4;
		CPU::clusterSelect = RAM[counter + 1];
		counter += 2;
		break;
	case 24:
		//ldio a
		clockTicks = 7;
		out = {clusterSelect, RAM[counter + 1], RAM[counter + 2], CPU::a}; // little endien
		counter += 3;
		return out;
		break;
	case 25:
		//ldio b
		clockTicks = 7;
		out = { clusterSelect, RAM[counter + 1], RAM[counter + 2], CPU::b };
		counter += 3;
		return out;
		break;
	case 31:
		//ldio imm
		clockTicks = 8;
		out = { clusterSelect, RAM[counter + 2], RAM[counter + 3], RAM[counter + 1] };
		counter += 4;
		return out;
		break;
	case 33:
		//sta b
		clockTicks = 3;
		CPU::b = CPU::a;
		counter += 1;
		break;
	case 34:
		//sta io
		clockTicks = 7;
		out = { clusterSelect, RAM[counter + 1], RAM[counter + 2], CPU::a }; // little endien
		counter += 3;
		return out;
		break;
	case 35:
		//sta [addr]
		clockTicks = 8;
		RAM[RAM[counter + 1] + RAM[counter + 2] * 256] = CPU::a;
		counter += 3;
		break;
	case 36:
		//sta b, [haddr]
		clockTicks = 6;
		RAM[b + RAM[counter + 1] * 256] = CPU::a;
		counter += 2;
		break;
	case 37:
		//sta c
		clockTicks = 3;
		CPU::clusterSelect = CPU::a;
		counter += 1;
		break;
	case 40:
		//stb a
		clockTicks = 3;
		CPU::a = CPU::b;
		counter += 1;
		break;
	case 42:
		//stb io
		clockTicks = 7;
		out = { clusterSelect, RAM[counter + 1], RAM[counter + 2], CPU::b }; // little endien
		counter += 3;
		return out;
		break;
	case 43:
		//stb [addr]
		clockTicks = 8;
		RAM[RAM[counter + 1] + RAM[counter + 2] * 256] = CPU::b;
		counter += 3;
		break;
	case 45:
		//stb c
		clockTicks = 3;
		CPU::clusterSelect = CPU::b;
		counter += 1;
		break;
	case 48:
		//stcounterl a
		clockTicks = 3;
		counter += 1;
		CPU::a = counter & 255;
		break;
	case 49:
		//stcounterh a
		clockTicks = 3;
		counter += 1;
		CPU::a = counter >> 8;
		break;
	case 50:
		//stcounterl b
		clockTicks = 3;
		counter += 1;
		CPU::b = counter & 255;
		break;
	case 51:
		//stcounterh b
		clockTicks = 3;
		counter += 1;
		CPU::b = counter >> 8;
		break;
	case 56:
		//stio a
		clockTicks = 7;
		CPU::a = io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		CPU::counter += 3;
		break;
	case 57:
		//stio b
		clockTicks = 7;
		CPU::b = io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		CPU::counter += 3;
		break;
	case 61:
		//stio c
		clockTicks = 7;
		CPU::clusterSelect = io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		CPU::counter += 3;
		break;
	case 64:
		//add a
		clockTicks = 4;
		CPU::flags = 0;
		if (int(CPU::a) * 2 < 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + CPU::a;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 1;
		break;
	case 65:
		//add b
		clockTicks = 4;
		CPU::flags = 0;
		if (int(CPU::a) + CPU::b < 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + CPU::b;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 1;
		break;
	case 66:
		//add io
		clockTicks = 8;
		CPU::flags = 0;
		if (int(CPU::a + io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]) < 255)) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 67:
		//add [addr]
		clockTicks = 9;
		CPU::flags = 0;
		if (int(CPU::a) + RAM[RAM[counter + 1] + RAM[counter + 2] * 256] < 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + RAM[RAM[counter + 1] + RAM[counter + 2] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 68:
		//add b, [haddr]
		clockTicks = 7;
		CPU::flags = 0;
		if (int(CPU::a) + RAM[CPU::b + RAM[counter + 1] * 256] < 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + RAM[CPU::b + RAM[counter + 1] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 71:
		//add imm
		clockTicks = 5;
		CPU::flags = 0;
		if (int(CPU::a) + RAM[counter + 1] > 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + RAM[counter + 1];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 72:
		//sub a
		clockTicks = 4;
		flags = 3;
		CPU::a = 0;
		counter += 1;
		break;
	case 73:
		//sub b
		clockTicks = 4;
		CPU::flags = 0;
		if (int(CPU::a) + ~CPU::b + 1 > 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + ~CPU::b + 1;
		if (CPU::a = 0) {
			CPU::flags |= 1;
		}
		counter += 1;
		break;
	case 74:
		//sub io
		clockTicks = 8;
		CPU::flags = 0;
		if (int(CPU::a) + ~(io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1])) + 1 > 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + ~(io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1])) + 1;
		if (CPU::a = 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 75:
		//sub [addr]
		clockTicks = 9;
		CPU::flags = 0;
		if (int(CPU::a) + ~RAM[RAM[counter + 1] + RAM[counter + 2] * 256] + 1 < 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + ~RAM[RAM[counter + 1] + RAM[counter + 2] * 256] + 1;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 76:
		//sub b, haddr
		clockTicks = 7;
		CPU::flags = 0;
		if (int(CPU::a) + ~RAM[CPU::b + RAM[counter + 1] * 256] + 1 < 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + ~RAM[CPU::b + RAM[counter + 1] * 256] + 1;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 79:
		//sub imm
		clockTicks = 5;
		CPU::flags = 0;
		if (int(CPU::a) + ~RAM[counter + 1] + 1 > 255) {
			CPU::flags |= 2;
		}
		CPU::a = CPU::a + ~RAM[counter + 1] + 1;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 80:
		//and a
		clockTicks = 3;
		if (CPU::a = 0) {
			CPU::flags = 1;
		}
		counter += 1;
		break;
	case 81:
		//and b:
		clockTicks = 4;
		CPU::flags = 0;
		CPU::a = CPU::a & CPU::b;
		if (CPU::a = 0) {
			CPU::flags = 1;
		}
		counter += 1;
		break;
	case 82:
		//and io
		clockTicks = 8;
		CPU::flags = 0;
		CPU::a = CPU::a & io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 83:
		//and [addr]
		clockTicks = 9;
		CPU::flags = 0;
		CPU::a = CPU::a & RAM[RAM[counter + 1] + RAM[counter + 2] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 84:
		//and b, [haddr]
		clockTicks = 7;
		CPU::flags = 0;
		CPU::a = CPU::a & RAM[CPU::b + RAM[counter + 1] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 87:
		//and imm
		clockTicks = 5;
		CPU::flags = 0;
		CPU::a = CPU::a & RAM[counter + 1];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 88:
		//or a
		clockTicks = 3;
		if (CPU::a = 0) {
			CPU::flags = 1;
		}
		counter += 1;
		break;
	case 89:
		//or b:
		clockTicks = 4;
		CPU::flags = 0;
		CPU::a = CPU::a | CPU::b;
		if (CPU::a = 0) {
			CPU::flags = 1;
		}
		counter += 1;
		break;
	case 90:
		//or io
		clockTicks = 8;
		CPU::flags = 0;
		CPU::a = CPU::a | io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 91:
		//or [addr]
		clockTicks = 9;
		CPU::flags = 0;
		CPU::a = CPU::a | RAM[RAM[counter + 1] + RAM[counter + 2] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 92:
		//or b, [haddr]
		clockTicks = 7;
		CPU::flags = 0;
		CPU::a = CPU::a | RAM[CPU::b + RAM[counter + 1] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 95:
		//or imm
		clockTicks = 5;
		CPU::flags = 0;
		CPU::a = CPU::a | RAM[counter + 1];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 96:
		//nor a
		clockTicks = 4;
		CPU::a = ~(CPU::a | CPU::a);
		if (CPU::a = 0) {
			CPU::flags = 1;
		}
		counter += 1;
		break;
	case 97:
		//nor b:
		clockTicks = 4;
		CPU::flags = 0;
		CPU::a = ~(CPU::a | CPU::b);
		if (CPU::a = 0) {
			CPU::flags = 1;
		}
		counter += 1;
		break;
	case 98:
		//nor io
		clockTicks = 8;
		CPU::flags = 0;
		CPU::a = ~(CPU::a | io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]));
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 99:
		//nor [addr]
		clockTicks = 9;
		CPU::flags = 0;
		CPU::a = ~(CPU::a | RAM[RAM[counter + 1] + RAM[counter + 2] * 256]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 100:
		//nor b, [haddr]
		clockTicks = 7;
		CPU::flags = 0;
		CPU::a = ~(CPU::a | RAM[CPU::b + RAM[counter + 1] * 256]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 103:
		//nor imm
		clockTicks = 5;
		CPU::flags = 0;
		CPU::a = ~(CPU::a | RAM[counter + 1]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 104:
		//inv a
		clockTicks = 4;
		CPU::a = ~CPU::a;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 1;
		break;
	case 105:
		//inv b
		clockTicks = 4;
		CPU::a = ~CPU::b;
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 1;
		break;
	case 106:
		//inv io
		clockTicks = 8;
		CPU::a = ~io.get(CPU::clusterSelect, CPU::RAM[CPU::counter + 1], CPU::RAM[CPU::counter + 1]);
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 107:
		//inv [addr]
		clockTicks = 9;
		CPU::a = ~RAM[RAM[counter + 1] + RAM[counter + 2] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 3;
		break;
	case 108:
		//inv b, haddr
		clockTicks = 7;
		CPU::a = ~RAM[CPU::b + RAM[counter + 1] * 256];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 111:
		//inv imm
		clockTicks = 5;
		CPU::flags = 0;
		CPU::a = ~RAM[counter + 1];
		if (CPU::a == 0) {
			CPU::flags |= 1;
		}
		counter += 2;
		break;
	case 112:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 113:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 114:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 115:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 116:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 117:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 118:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		break;
	case 119:
		//jmp
		clockTicks = 7;
		counter = RAM[counter + 1] + RAM[counter + 2] * 256;
		return vector<int>({ 0, 0 });
		break;
	case 120:
		//jz
		if(flags & 1){
			clockTicks = 7;
			counter = RAM[counter + 1] + RAM[counter + 2] * 256;
			return vector<int>({ 0, 0 });
		}
		else {
			clockTicks = 4;
			counter += 3;
		}
		break;
	case 121:
		//jnz
		if (!(flags & 1)) {
			clockTicks = 7;
			counter = RAM[counter + 1] + RAM[counter + 2] * 256;
			return vector<int>({ 0, 0 });
		}
		else {
			clockTicks = 4;
			counter += 3;
		}
		break;
	case 122:
		//jc
		if (flags & 2) {
			clockTicks = 7;
			counter = RAM[counter + 1] + RAM[counter + 2] * 256;
			return vector<int>({ 0, 0 });
		}
		else {
			clockTicks = 4;
			counter += 3;
		}
		break;
	case 123:
		//jz
		if (!(flags & 2)) {
			clockTicks = 7;
			counter = RAM[counter + 1] + RAM[counter + 2] * 256;
			return vector<int>({ 0, 0 });
		}
		else {
			clockTicks = 4;
			counter += 3;
		}
		break;
	default:
		//NOP
		clockTicks = 2;
		++CPU::counter;
		break;
	}
	while (duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() < clockTicks*1'000'000'000/clock_speed);
	return vector<int>();
}