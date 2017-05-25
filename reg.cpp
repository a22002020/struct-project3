#include<iostream>
#include<fstream>
#include<string>
#include "head.h"
using namespace std;
//-----------------------------Res----------------------------------
unsigned int res[32], Lo=0, Hi=0, pc=0,moto_pc=0;
void Res_init() {
	pc = BtoD(instucts[0]); moto_pc = pc;
	res[29] = datas[0];
	
	if (outfile.is_open()) {
		outfile << "cycle 0" << endl;
		for (int i = 0; i < 32; i++) { outfile << "$"; if (i < 10) outfile << "0"; outfile << i << ": " << DtoH(res[i]) << endl; }
		outfile << "$HI: " << DtoH(Hi) << endl;
    outfile << "$LO: " << DtoH(Lo) << endl;
		outfile << "PC: " << DtoH(pc) << endl;
		outfile << endl;
		outfile << endl;
	}
}

//------------------------------------------------------------------