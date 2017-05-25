
#include<iostream>
#include<fstream>
#include<string>
#include "head.h"
#include <sstream>
//123
using namespace std;
string instucts[50000];
unsigned int datas[50000];
int ins_num,data_num;
unsigned char buffer[50000];
long intmin = -1 * (long)2147483648;
ofstream outfile("snapshot.rpt");
ofstream rptfile("report.rpt");
ofstream trfile("trace.rpt");


ofstream itlbfile("itlb.rpt");
ofstream ipagefile("ipage.rpt");
ofstream icachefile("icache.rpt");

int ccc; bool DDD;
unsigned int BtoD(string st) {
	int result = 0, ratio = 1;
	for (int i = st.length()-1; i >= 0; i--) {
		result += ratio*(st[i] - 48);
		ratio *= 2;
	}
	return result;
}
int BtoDs(string st) {
	int result = 0, ratio = 1;
	string s = st.substr(1);
	if (st[0] == '1')  for (int i = 0; i < s.length(); i++) if (s[i] == '0') s[i] = '1'; else s[i] = '0';
	result = BtoD(s);
	if (st[0]== '1') result = -1 * (result+1);
	return result;
}
void HtoB(int i, int j, int num) {
	unsigned short chr = (unsigned short)buffer[num];
	int kara = j * 8, made = (j + 1) * 8 - 1;
	if (chr != 0) 
		chr = chr;
	for (int ii = made; ii >= kara; ii--) {
		int out = chr % 2;
		instucts[i][ii] = out + '0';
		chr = chr / 2;
	}
}

string DtoH(unsigned int number) {
	string result="00000000";
	unsigned int temp=number;
	for (int i = 7; i >= 0; i--) {
		int k = temp % 16;
		if (k < 10) result[i] = k + '0'; else result[i] = k - 10 + 'A';
		temp /= 16;
	}
	return "0x" + result;
}
unsigned int HtoD(int n,int byt_num,bool isInit) {
	int kara; if (isInit)kara = n * byt_num; else kara = n;
	int made = kara + byt_num-1;
	unsigned int ratio = 1;
	unsigned int result = 0;
	for (int i = made; i >= kara; i--) {
		result += buffer[i] * ratio;
		ratio *= 256;
	}
	return result;
}

void save(int addr, int byt_num,unsigned int input,int c_n) {
	dcache(addr-8, c_n);
	int kara = addr; int made = kara + byt_num - 1;
	unsigned int inn = input;
	for (int i = made; i >= kara; i--) {
		buffer[i] = input % 256;
		input /= 256;
	}

}

void input() {
	ifstream fin;
	fin.open("iimage.bin", ios::binary);
	if (!fin) {
		cout << "open error!" << endl;
	}
	//char buffer[5000];
	int len = 0;
	while (!fin.eof()/* && fin.peek() != EOF*/) {
		char tt;
		fin.get(tt); buffer[len] = (unsigned char)tt;
		len++;
	}
	buffer[len] = EOF; len++;
	//fin.read(buffer, 3073 * sizeof(char));
	ins_num = len / 4;
	int temp=0;
	for (int i = 0; i < ins_num; i++) {
		instucts[i] = "00000000000000000000000000000000";  
		for (int j = 0; j < 4; j++) {
			HtoB(i, j, temp);
			temp++;
		}
	}
	//for (int i = 0; i < ins_num; i++) cout << instucts[i]<< endl;
	ifstream fin2;
	fin2.open("dimage.bin", ios::binary);
	if (!fin2) {
		cout << "open error!" << endl;
	}

	len = 0;
	while (!fin2.eof()/* && fin.peek() != EOF*/) {
		char tt;
		fin2.get(tt); buffer[len] = (unsigned char)tt;
		len++;
	}
	buffer[len] = EOF; len++;
	//fin.read(buffer, 3073 * sizeof(char));
	data_num = len / 4;
	temp = 0;
	for (int i = 0; i < data_num; i++) {
		datas[i] = HtoD(i,4,true);
		
	}
 	for (int i=data_num*4;i<50000;i++)buffer[i]=(unsigned char)0;
	//for (int i = 0; i < data_num; i++) cout <<(int)buffer[16+i]<< endl;

}








int I_M=64,D_M=32,I_P=8,D_P=16,I_CT=16,I_CB=4,I_S=4,D_CT=16,D_CB=4,D_S=1;
void outRPT(){
rptfile<<"ICache :"<<endl;
rptfile<<"# hits: "<<IChs<<endl;
rptfile<<"# misses: "<<ICms<<endl;
rptfile<<""<<endl;
rptfile<<"DCache :"<<endl;
rptfile<<"# hits: "<<DChs<<endl;
rptfile<<"# misses: "<<DCms<<endl;
rptfile<<""<<endl;
rptfile<<"ITLB :"<<endl;
rptfile<<"# hits: " << IThs <<endl;
rptfile<<"# misses: " << ITms <<endl;
rptfile<<""<<endl;
rptfile<<"DTLB :"<<endl;
rptfile<<"# hits: "<<DThs<<endl;//IThs=0;ITms=0;
rptfile<<"# misses: "<<DTms<<endl;
rptfile<<""<<endl;
rptfile<<"IPageTable :"<<endl;
rptfile<<"# hits: "<<IPhs<<endl;
rptfile<<"# misses: "<<Ipms<<endl;
rptfile<<""<<endl;
rptfile<<"DPageTable :"<<endl;
rptfile<<"# hits: "<<DPhs<<endl;
rptfile<<"# misses: "<<Dpms<<endl;
rptfile<<""<<endl;

}


//---------------------------------------------------------------------
int main(int argc, char **argv)
{


if (argc==10){
	std::string temp=argv[1];
    std::stringstream ss;
	ss << temp;ss >> I_M; 
	temp=argv[2];ss.clear();ss<<temp;ss>>D_M;
	temp=argv[3];ss.clear();ss<<temp;ss>>I_P;
	temp=argv[4];ss.clear();ss<<temp;ss>>D_P;
	temp=argv[5];ss.clear();ss<<temp;ss>>I_CT;
	temp=argv[6];ss.clear();ss<<temp;ss>>I_CB;
	temp=argv[7];ss.clear();ss<<temp;ss>>I_S;
	temp=argv[8];ss.clear();ss<<temp;ss>>D_CT;
	temp=argv[9];ss.clear();ss<<temp;ss>>D_CB;
	temp=argv[10];ss.clear();ss<<temp;ss>>D_S;
	
	
	
 
	
}
int aaa=0;
	input();
	Res_init();
	cache_init();
	int inum = BtoD(instucts[1]);
	int c_n = 1; ccc = c_n;

	while (((int)pc -(int) moto_pc) / 4 < ins_num&&!go_halt) {
		ITLB_hit = false; Ipage_hit = false; Icahce_hit = false; DDD = false;

		icache(pc,c_n);
		deal_ins(pc, c_n);
		
		c_n++; ccc = c_n;
	}
	
	outRPT();
	outfile.close();
	rptfile.close();
	trfile.close();
	//errorfile.close();
    return 0;
}

