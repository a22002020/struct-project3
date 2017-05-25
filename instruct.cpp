//--------------------------------------ins----------------------------
#include<iostream>
#include<fstream>
#include<string>
#include "head.h"
using namespace std;
bool multy_deal = false;	bool go_halt = false;
void deal_ins(unsigned int num, int c_num) {
	string ins, outputs;
	if (((int)num - (int)moto_pc) >= 0)ins = instucts[(num - moto_pc) / 4 + 2]; else ins = "00000000000000000000000000000000";
	//cout<<"cycle "<<c_num <<" " <<ins<<endl;
	unsigned int Tres[100], TLo = Lo, THi = Hi, Tpc = pc;
	for (int i = 0; i < 32; i++) Tres[i] = res[i];
	//cout<<c_num<<"  "<<(int)Tres[17] <<" "<<endl;
	Tpc += 4;
	string opcode = ins.substr(0, 6);
	if (opcode == "000000") {
		string func = ins.substr(26);
		/*00sll*/if (func == "000000") {
			outputs = "sll"; int C = BtoD(ins.substr(21, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0 && !(rd == 0 && C == 0 && rt == 0)) int aaaa = 100; else  Tres[rd] = Tres[rt] << C; if (rd == 0 && C == 0 && rt == 0)outputs = "nop";
		}
		/*02srl*/if (func == "000010") {
			outputs = "srl"; int C = BtoD(ins.substr(21, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;
			else Tres[rd] = Tres[rt] >> C;
		}
		/*03sra*/if (func == "000011") {
			outputs = "sra"; int C = BtoD(ins.substr(21, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;
			else { Tres[rd] = (int)Tres[rt] >> C; }
		}
		/*08jr*/if (func == "001000") { outputs = "jr"; int rs = BtoD(ins.substr(6, 5)); Tpc = Tres[rs]; }
		/*10mfhi*/if (func == "010000") { outputs = "mfhi"; multy_deal = false; int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = THi; }
		/*12mflo*/if (func == "010010") { outputs = "mflo";  multy_deal = false; int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = TLo; }
		/*18mult*/if (func == "011000") { outputs = "mult"; if (multy_deal)int aaaa = 100; { multy_deal = true; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5));  unsigned long re = (long)(int)Tres[rs] * (long)(int)Tres[rt]; THi = re / 0x100000000; TLo = re % 0x100000000; } }
		/*19multu*/if (func == "011001") { outputs = "multu"; if (multy_deal)int aaaa = 100; { multy_deal = true; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); unsigned long re = (unsigned long)Tres[rs] * (unsigned long)Tres[rt];  THi = re / 0x100000000; TLo = re % 0x100000000; } }
		/*20add*/if (func == "100000") {
			outputs = "add"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100; {
				long re = (long)(int)Tres[rs] + (long)(int)Tres[rt];
				if (re > 2147483647 || re < intmin)int aaaa = 100;
				Tres[rd] = (unsigned int)((int)Tres[rs] + (int)Tres[rt]); }
		}
		/*21addu*/if (func == "100001") { outputs = "addu"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = Tres[rs] + Tres[rt]; }
		/*22sub*/if (func == "100010") {
			outputs = "sub";
			int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;
			{ unsigned int re = (unsigned int)((int)Tres[rs] - (int)Tres[rt]); int rss, rts, rees; if (Tres[rs] > 0x7FFFFFFF) rss = 1; else rss = 0; if (Tres[rt] > 0x80000000) rts = 0; else rts = 1; if (re > 0x7FFFFFFF) rees = 1; else rees = 0;
			//cout<<rss<<" "<<rts<<" "<<rees;
			if ((rss == 0 && rts == 0 && rees == 1) || (rss == 1 && rts == 1 && rees == 0))int aaaa = 100; Tres[rd] = (unsigned int)((int)Tres[rs] - (int)Tres[rt]); }
		}
		/*24and*/if (func == "100100") { outputs = "and"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = Tres[rs] & Tres[rt]; }
		/*25or*/if (func == "100101") { outputs = "or"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = Tres[rs] | Tres[rt]; }
		/*26xor*/if (func == "100110") { outputs = "xor"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = Tres[rs] ^ Tres[rt]; }
		/*27nor*/if (func == "100111") { outputs = "nor"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = ~(Tres[rs] | Tres[rt]); }
		/*28nand*/if (func == "101000") { outputs = "nand"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = ~(Tres[rs] & Tres[rt]); }
		/*2Aslt*/if (func == "101010") { outputs = "slt"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int rd = BtoD(ins.substr(16, 5)); if (rd == 0) int aaaa = 100;  Tres[rd] = (int)Tres[rs] < (int)Tres[rt]; }
	}


	/*02j*/if (opcode == "000010") { outputs = "j"; int add = BtoD(ins.substr(6)); Tpc = Tpc & 0xF0000000; Tpc = Tpc | (add * 4); }
	/*03jal*/if (opcode == "000011") { outputs = "jal"; int add = BtoD(ins.substr(6)); Tres[31] = Tpc; Tpc = Tpc & 0xF0000000; Tpc = Tpc | (add * 4); }
	/*04beq*/if (opcode == "000100") { outputs = "beq"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); long re = Tpc + 4 * imm; if (re > 2147483647)int aaaa = 100; if (Tres[rs] == Tres[rt])Tpc += 4 * imm; }
	/*05bne*/if (opcode == "000101") { outputs = "bne"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); long re = Tpc + 4 * imm; if (re > 2147483647)int aaaa = 100; if (Tres[rs] != Tres[rt])Tpc += 4 * imm; }
	/*07bgtz*/if (opcode == "000111") { outputs = "bgtz"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); long re = Tpc + 4 * imm; if (re > 2147483647)int aaaa = 100; if ((int)Tres[rs] > 0)Tpc += 4 * imm; }
	/*08addi*/if (opcode == "001000") {
		outputs = "addi";
		int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16));
		if (rt == 0) int aaaa = 100; {
			long re = ((long)(int)Tres[rs] + (long)(int)imm);
			if (re > 2147483647 || re < intmin)
			{
				int aaaa = 100;
			}
			Tres[rt] = (int)Tres[rs] + (int)imm; }
	}
	/*09addiu*/if (opcode == "001001") { outputs = "addiu"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16));  if (rt == 0) int aaaa = 100;  Tres[rt] = Tres[rs] + (unsigned int)imm; }
	/*0Aslti*/if (opcode == "001010") { outputs = "slti"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); if (rt == 0) int aaaa = 100;   Tres[rt] = ((int)Tres[rs]) < imm; }
	/*0Candi*/if (opcode == "001100") { outputs = "andi";  int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); unsigned int imm = BtoD(ins.substr(16)); if (rt == 0) int aaaa = 100;  Tres[rt] = Tres[rs] & imm; }
	/*0Dori*/if (opcode == "001101") { outputs = "ori"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); unsigned int imm = BtoD(ins.substr(16)); if (rt == 0) int aaaa = 100;  Tres[rt] = Tres[rs] | imm; }
	/*0Enori*/if (opcode == "001110") { outputs = "nori"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); unsigned int imm = BtoD(ins.substr(16)); if (rt == 0) int aaaa = 100;  Tres[rt] = ~(Tres[rs] | imm); }
	/*0Flui*/if (opcode == "001111") { outputs = "lui"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoD(ins.substr(16)); if (rt == 0) int aaaa = 100;  Tres[rt] = imm << 16; }
	/*20lb*/if (opcode == "100000") { outputs = "lb"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm; dcache(addrt, c_num); long addr = addrt + 8; if (rt == 0) int aaaa = 100; { long re = (long)Tres[rs] + (long)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)addrt > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; } else if (!go_halt) { Tres[rt] = HtoD(addr, 1, false); if (Tres[rt] >= 128) Tres[rt] = Tres[rt] | 0xFFFFFF00; } } }
	/*21lh*/if (opcode == "100001") { outputs = "lh"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm; dcache(addrt, c_num);  long addr = addrt + 8; if (rt == 0) int aaaa = 100; { long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)(addrt + 1) > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; }  if (addrt % 2 != 0) { int aaaa = 0; go_halt = true; } else if (!go_halt) { Tres[rt] = HtoD(addr, 2, false); if (Tres[rt] > 32767) Tres[rt] = Tres[rt] | 0xFFFF0000; } } }
	/*23lw*/if (opcode == "100011") { outputs = "lw"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm;  dcache(addrt, c_num); long addr = addrt + 8; if (rt == 0) int aaaa = 100; { long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)(addrt + 3) > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; }  if (addrt % 4 != 0) { int aaaa = 0; go_halt = true; } else if (!go_halt)Tres[rt] = HtoD(addr, 4, false); } }
	/*24lbu*/if (opcode == "100100") { outputs = "lbu"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm;  dcache(addrt, c_num); long addr = addrt + 8; if (rt == 0) int aaaa = 100; { long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)addrt > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; } else if (!go_halt)Tres[rt] = HtoD(addr, 1, false); } }
	/*25lhu*/if (opcode == "100101") { outputs = "lhu"; int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm;  dcache(addrt, c_num); long addr = addrt + 8; if (rt == 0) int aaaa = 100; { long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)(addrt + 1) > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; }  if (addrt % 2 != 0) { int aaaa = 0; go_halt = true; } else if (!go_halt)Tres[rt] = HtoD(addr, 2, false); } }
	/*28sb*/if (opcode == "101000") {
		outputs = "sb";
		int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm; long addr = addrt + 8; {long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)addrt > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; }
		else if (!go_halt) {
			save(addr, 1, (Tres[rt] & 0x000000FF), c_num); // cout<<addr<<" "<<(int)buffer[addr]<<" "<<(int)buffer[addr+1]<<" "<<(int)buffer[addr+2]<<" "<<(int)buffer[addr+3];
		}}
	}
	/*29sh*/if (opcode == "101001") {
		outputs = "sh";
		int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm; long addr = addrt + 8; {long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)(addrt + 1) > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; }  if (addrt % 2 != 0) { int aaaa = 0; go_halt = true; }
		else if (!go_halt) {
			save(addr, 2, (Tres[rt] & 0x0000FFFF), c_num);
			//cout<<addr<<" "<<(int)buffer[addr]<<" "<<(int)buffer[addr+1]<<" "<<(int)buffer[addr+2]<<" "<<(int)buffer[addr+3];
		}}
	}
	/*2Bswv */if (opcode == "101011") {
		outputs = "sw";
		int rs = BtoD(ins.substr(6, 5)); int rt = BtoD(ins.substr(11, 5)); int imm = BtoDs(ins.substr(16)); int addrt = (int)Tres[rs] + (int)imm; long addr = addrt + 8; {long re = (long)(int)Tres[rs] + (long)(int)imm; if (re > 2147483647 || re < intmin)int aaaa = 100; if ((unsigned long)(addrt + 3) > 1023 || addrt < 0) { int aaaa = 0; go_halt = true; }  if (addrt % 4 != 0) { int aaaa = 0; go_halt = true; }
		else  if (!go_halt) {
			save(addr, 4, Tres[rt],c_num);
			//cout<<addr<<" "<<(int)buffer[addr]<<" "<<(int)buffer[addr+1]<<" "<<(int)buffer[addr+2]<<" "<<(int)buffer[addr+3]; 
		}}
	}


	Tres[0] = 0;
	if (opcode == "111111" || go_halt) {
		outputs = "halt";
		go_halt = true; 
	}


	if (outfile.is_open()&&!go_halt) {
		outfile << "cycle " << c_num << endl;
		for (int i = 0; i < 32; i++) if (Tres[i] != res[i]) { res[i] = Tres[i]; outfile << "$"; if (i < 10) outfile << "0"; outfile << i << ": " << DtoH(res[i]) << endl; }
		if (THi != Hi) { Hi = THi; outfile << "$HI: " << DtoH(Hi) << endl; }
		if (TLo != Lo) { Lo = TLo; outfile << "$LO: " << DtoH(Lo) << endl; }
		if (Tpc != pc) { pc = Tpc; outfile << "PC: " << DtoH(pc) << endl; }
		outfile << endl;
		outfile << endl;

	}

	trfile << c_num << ", " << outputs;
	int s_n = 5 - outputs.length();
	for (int i = 1; i <= s_n; i++)trfile << " ";
	trfile << ": ";
	if (!ITLB_hit && !Icahce_hit)trfile << "Disk  ; ";
	else
	{
		if (ITLB_hit) trfile << "ITLB  ";
		if (Icahce_hit) trfile << "ICache  ";
		trfile << "; ";
	}
	if (DDD) {
		if (!DTLB_hit && !Dcahce_hit)trfile << " Disk ";
		else
		{
			if (DTLB_hit) trfile << " DTLB ";
			if (Dcahce_hit) trfile << " DCache ";

		}
	}
	trfile << endl;



}
