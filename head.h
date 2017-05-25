#include<iostream>
#include<fstream>
#include<string>
using namespace std;
void Res_init();
void deal_ins(unsigned int num,int c_num);
extern string instucts[50000];
extern unsigned int datas[50000];
extern int ins_num,data_num;
extern unsigned char buffer[50000];
extern unsigned int res[32], Lo, Hi, pc,moto_pc;
extern long intmin;
extern ofstream outfile;
extern ofstream errorfile;
extern ofstream trfile;
extern ofstream itlbfile;
extern ofstream ipagefile;
extern ofstream icachefile;
extern int ccc;
extern bool DDD;
extern	bool go_halt;
unsigned int BtoD(string st);
int BtoDs(string st);
void HtoB(int i, int j, int num);
string DtoH(unsigned int number);
unsigned int HtoD(int n,int byt_num,bool isInit);
void save(int addr, int byt_num,unsigned int input,int c_n);
void icache(int pc,int c_n);
void dcache(int addr, int c);
void cache_init();
extern int I_M,D_M,I_P,D_P,I_CT,I_CB,I_S,D_CT,D_CB,D_S;

extern string IVA,IVPN_s,Ippn,IpAddr,DVA,DVPN_s,Dppn,DpAddr;
extern string* ILTB_tag,*Ippn_page,*Icache_tag;
extern string* DLTB_tag,*Dppn_page,*Dcache_tag;
extern bool* ILTB_V,*Ipage_V,*Icache_v;
extern bool* DLTB_V,*Dpage_V,*Dcache_v;
extern bool ITLB_hit,Ipage_hit,Icahce_hit; 
extern bool DTLB_hit,Dpage_hit,Dcahce_hit;
extern int Ipage_num,ITLB_num,Icb_num;
extern int Dpage_num,DTLB_num,Dcb_num;
extern int IThs,ITms,IPhs,Ipms,IChs,ICms;
extern int DThs,DTms,DPhs,Dpms,DChs,DCms;