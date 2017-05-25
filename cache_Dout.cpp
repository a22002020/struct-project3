#include<iostream>
#include<fstream>
#include<string>
#include <cmath>
#include "head.h"
using namespace std;
//-----------------------------cache----------------------------------
string IVA,IVPN_s,Ippn,IpAddr,DVA,DVPN_s,Dppn,DpAddr;
string* ITLB_tag,*Ippn_page,*Icache_tag,*Icache_VPN;
string* DTLB_tag,*Dppn_page,*Dcache_tag, *Dcache_VPN;
bool* ITLB_V,*Ipage_V,*Icache_v;
bool* DTLB_V,*Dpage_V,*Dcache_v;
bool ITLB_hit,Ipage_hit,Icahce_hit; 
bool DTLB_hit,Dpage_hit,Dcahce_hit;
int Ipage_num,ITLB_num,Icb_num;
int Dpage_num,DTLB_num,Dcb_num;
int IThs,ITms,IPhs,Ipms,IChs,ICms;
int DThs,DTms,DPhs,Dpms,DChs,DCms;
int* ITLB_LRU,*Ipage_LRU,*Icache_B_LRU;
int* DTLB_LRU, *Dpage_LRU, *Dcache_B_LRU;
int ITLB_zero,DTLB_zero;
int Ippn_now,Dppn_now;


string DtoB(int pc) {
	string result = "0000000000";
	int t = pc;
	int poi = 9;
	while (t != 0) {
		int num = t % 2;
		result[poi] = '0' + num;
		t = t / 2;
		poi--;
	}
	return result;

}

bool man,iman;

void cache_init() {
	IThs=0;ITms=0;IPhs=0;Ipms=0;IChs=0;ICms=0;
	
	Ipage_num=1024/I_P;//rukou
	ITLB_num=Ipage_num/4;//rukou
	Icb_num=I_CT/I_CB;
	ITLB_tag=new string[ITLB_num];
	ITLB_V=new bool[ITLB_num]; for (int i=0;i<ITLB_num;i++)ITLB_V[i]=false;
	Ippn_page=new string[Ipage_num];
	Ipage_V=new bool[Ipage_num]; for (int i=0;i<Ipage_num;i++)Ipage_V[i]=false;
	Icache_tag=new string[Icb_num];
	Icache_v=new bool[Icb_num]; for (int i=0;i<Icb_num;i++)Icache_v[i]=false;
	ITLB_LRU = new int[ITLB_num]; Ipage_LRU = new int[Ipage_num]; Icache_B_LRU = new int[I_CT / I_CB];
	Icache_VPN = new string[Icb_num];
	for (int i=0;i<ITLB_num;i++) ITLB_LRU[i]=0;
	ITLB_zero=ITLB_num;IThs=0;ITms=0;
	Ippn_now = 0; iman = false;

	
	
	DThs=0;DTms=0;DPhs=0;Dpms=0;DChs=0;DCms=0;
	Dpage_num=1024/D_P;
	DTLB_num=Dpage_num/4;
	Dcb_num=D_CT/D_CB;
	DTLB_tag=new string[DTLB_num];
	DTLB_V=new bool[DTLB_num]; for (int i=0;i<DTLB_num;i++)DTLB_V[i]=false;
	Dppn_page=new string[Dpage_num];
	Dpage_V=new bool[Dpage_num]; for (int i=0;i<Dpage_num;i++)Dpage_V[i]=false;
	Dcache_tag=new string[Dcb_num];
	Dcache_v=new bool[Dcb_num]; for (int i=0;i<Dcb_num;i++)Dcache_v[i]=false;
	DTLB_LRU = new int[DTLB_num]; Dpage_LRU = new int[Dpage_num]; Dcache_B_LRU = new int[D_CT / D_CB];
	Dcache_VPN = new string[Dcb_num];
	for (int i = 0; i<DTLB_num; i++) DTLB_LRU[i] = 0;
	DTLB_zero = DTLB_num; DThs = 0; DTms = 0;
	Dppn_now = 0; man = false;

}


void ITLB(string addr,int c_n){
	int Ipage_bits=log(Ipage_num)/log(2);
	IVPN_s=addr.substr(0,Ipage_bits);
	//cout<<IVPN_s<<" "<<Ipage_num<<endl;
	int tag_bits=log(ITLB_num)/log(2);
	//string tag=addr.substr(0,10-tag_bits);
	string tag=IVPN_s;
	int locate=-1;
	for (int i=0;i<ITLB_num;i++){
		if (ITLB_V[i])
			if (ITLB_tag[i]==tag) locate=i;
	}
	if (locate!=-1) {IThs++;ITLB_LRU[locate]=c_n;ITLB_hit=true;}//hit
	else{ITLB_hit=false;
		ITms++;
			int insert=2000;
			for (int i=0;i<ITLB_num;i++) if (!ITLB_V[i]&&i<insert)insert=i;
			if (insert!=2000){//have null
			ITLB_V[insert]=true;
			ITLB_tag[insert]=tag;
			ITLB_LRU[insert]=c_n;
			}else{//LRU
				int min=600000;
				for (int i=0;i<ITLB_num;i++) if ( ITLB_LRU[i]<min) {insert=i;min=ITLB_LRU[i];}
				ITLB_V[insert]=true;
				ITLB_tag[insert]=tag;
				ITLB_LRU[insert]=c_n;
			}
			
		}
		
	
	/*itlbfile << c_n;
	for (int i = 0; i < ITLB_num; i++)itlbfile << " " << ITLB_tag[i] << " " << ITLB_V[i] << " ";
	itlbfile << endl;*/
}


void Ipage(int cn){
	int page_n=BtoD(IVPN_s);
	if (!Ipage_V[page_n]){
		Ipms++;
		Ipage_hit=false;

		
		if (Ippn_now == I_M / I_P) { iman = true; Ippn_now = 0;}
		if (iman){
			int min = 5000000; int loc = 0;
			for (int i = 0; i<Ipage_num; i++)
				if (Ipage_V[i] && Ipage_LRU[i]<min) {
					min = Ipage_LRU[i];
					loc = i;
				}
			Ipage_V[loc] = false;
			

			for (int i = 0; i < ITLB_num; i++)
				if (BtoD(ITLB_tag[i]) == loc)
					ITLB_V[i] = false;
		}


		int page_nuim_bit = (int)(log(I_M / I_P) / log(2));
		string ppn_t= DtoB(Ippn_now);
		ppn_t = ppn_t.substr(ppn_t.length() - page_nuim_bit);
		Ippn_page[page_n] = ppn_t;
		Ippn_now = Ippn_now + 1;
		
		
		
		Ipage_V[page_n]=true;
		Ipage_LRU[page_n]=cn;

			
		
	}
	//ipagefile << cn;
	//for (int i = 0; i < Ipage_num; i++)ipagefile << " " << Ipage_V[i] << " " << Ippn_page[i];
	//ipagefile << endl;
}

void Icache_insert(int kara, int made,string tag) {
	Icahce_hit = false;
	for (int i = kara; i <= made; i++)if (Icache_tag[i] == tag) { 
		Icahce_hit = true; 
		IChs++;
		int inv = i; Icache_B_LRU[inv] = 1;
		bool full = true;
		for (int i = kara; i <= made; i++)if (Icache_B_LRU[i] != 1)full = false;
		if (full) { for (int i = kara; i <= made; i++)  Icache_B_LRU[i] = 0; Icache_B_LRU[inv] = 1; }


	}
	if (!Icahce_hit) 
	{
		ICms++;
		int inv = -1;
		for (int i = made; i >= kara; i--)if (!Icache_v[i])inv = i;
		if (inv != -1) {
			Icache_v[inv] = true;
			Icache_VPN[inv] = IVPN_s;
			Icache_tag[inv] = tag;
			Icache_B_LRU[inv] = 1; bool full = true;
			for (int i = kara; i <= made; i++)if (Icache_B_LRU[i] != 1)full = false;
			if (full){for (int i = kara; i <= made; i++)  Icache_B_LRU[i] = 0; Icache_B_LRU[inv] = 1; }

		}
		else
		{
			for (int i = made; i >= kara; i--)if (Icache_B_LRU[i]==0)inv = i;
			if (made == kara)inv = made;
			Icache_v[inv] = true;
			Icache_VPN[inv] = IVPN_s;
			Icache_tag[inv] = tag;
			Icache_B_LRU[inv] = 1; bool full = true;
			for (int i = kara; i <= made; i++)if (Icache_B_LRU[i] != 1)full = false;
			if (full) { for (int i = kara; i <= made; i++)  Icache_B_LRU[i] = 0; Icache_B_LRU[inv] = 1; }
		}
	}


}



void I_cache(string addr,int c) {
	IpAddr = addr.substr((int)(log(Ipage_num) / log(2)));
	IpAddr = Ippn_page[BtoD(IVPN_s)] + IpAddr;
	int bl_num = I_CT / I_CB;
	int index_max = bl_num / I_S;
	int id_bit = (int)(log(index_max) / log(2));
	for (int i = 0; i < bl_num; i++)
		if (Icache_v[i])
			Icache_v[i] = Ipage_V[BtoD(Icache_VPN[i])];
	string tag = IpAddr.substr(0, IpAddr.length() - id_bit);

	if (id_bit != 0) {
		string id_s = IpAddr.substr(IpAddr.length() - id_bit);
		int index = BtoD(id_s);

		Icache_insert(index*I_S,index*I_S+I_S-1,tag);
	}
	else Icache_insert(0, bl_num-1,tag);
	//icachefile << c;
	//for (int i = 0; i < bl_num; i++)icachefile << " " << Icache_v[i] << " " << Icache_tag[i] << " " << Icache_VPN[i] << " " << Icache_B_LRU[i]<<"---";
	////for (int i = 0; i < bl_num; i++)icachefile<< Icache_B_LRU[i]<<" ";
	//icachefile << endl;


}
























void DTLB(string addr, int c_n) {
	int Dpage_bits = log(Dpage_num) / log(2);
	DVPN_s = addr.substr(0, Dpage_bits);
	//cout<<DVPN_s<<" "<<Dpage_num<<endl;
	int tag_bits = log(DTLB_num) / log(2);
	//string tag=addr.substr(0,10-tag_bits);
	string tag = DVPN_s;
	int locate = -1;
	for (int i = 0; i<DTLB_num; i++) {
		if (DTLB_V[i])
			if (DTLB_tag[i] == tag) locate = i;
	}
	if (locate != -1) { DThs++; DTLB_LRU[locate] = c_n; DTLB_hit = true; }//hit
	else {
		DTLB_hit = false;
		DTms++;
		int insert = 2000;
		for (int i = 0; i<DTLB_num; i++) if (!DTLB_V[i] && i<insert)insert = i;
		if (insert != 2000) {//have null
			DTLB_V[insert] = true;
			DTLB_tag[insert] = tag;
			DTLB_LRU[insert] = c_n;
		}
		else {//LRU
			int min = 600000;
			for (int i = 0; i<DTLB_num; i++) if (DTLB_LRU[i]<min) { insert = i; min = DTLB_LRU[i]; }
			DTLB_V[insert] = true;
			DTLB_tag[insert] = tag;
			DTLB_LRU[insert] = c_n;
		}

	}


	itlbfile << DVPN_s<<" - "<<c_n;
	for (int i = 0; i < DTLB_num; i++)itlbfile << " " << DTLB_tag[i] << " " << DTLB_V[i] << " ";
	itlbfile << endl;
}


void Dpage(int cn) {
	int page_n = BtoD(DVPN_s);
	if (!Dpage_V[page_n]) {
		Dpms++;
		Dpage_hit = false;


		if (Dppn_now == D_M / D_P) { man = true; Dppn_now = 0;}
		if (man){
			int min = 5000000; int loc = 0;
			for (int i = 0; i<Dpage_num; i++)
				if (Dpage_V[i] && Dpage_LRU[i]<min) {
					min = Dpage_LRU[i];
					loc = i;
				}
			Dpage_V[loc] = false;
			
			//int Dpage_bits = log(Dpage_num) / log(2);
			//string changedVPN = DtoB(loc).substr(10-Dpage_bits);
			//itlbfile << loc << "   " << changedVPN << "-----";
			for (int i = 0; i < DTLB_num; i++)
				if ( BtoD( DTLB_tag[i]) ==loc)
					DTLB_V[i] = false;
		}


		int page_nuim_bit = (int)(log(D_M / D_P) / log(2));
		string ppn_t = DtoB(Dppn_now);
		ppn_t = ppn_t.substr(ppn_t.length() - page_nuim_bit);
		Dppn_page[page_n] = ppn_t;
		Dppn_now = Dppn_now + 1;



		Dpage_V[page_n] = true;
		Dpage_LRU[page_n] = cn;
		itlbfile << DVPN_s << " - " << cn;
		for (int i = 0; i < DTLB_num; i++)itlbfile << " " << DTLB_tag[i] << " " << DTLB_V[i] << " ";
		itlbfile << endl;


	}
	ipagefile << cn;
	for (int i = 0; i < Dpage_num; i++)ipagefile << " " << Dpage_V[i] << " " << Dppn_page[i];
	ipagefile << endl;


}

void Dcache_insert(int kara, int made, string tag) {
	Dcahce_hit = false;
	for (int i = kara; i <= made; i++)if (Dcache_tag[i] == tag) {
		Dcahce_hit = true;
		DChs++;
		int inv = i; Dcache_B_LRU[inv] = 1;
		bool full = true;
		for (int i = kara; i <= made; i++)if (Dcache_B_LRU[i] != 1)full = false;
		if (full) { for (int i = kara; i <= made; i++)  Dcache_B_LRU[i] = 0; Dcache_B_LRU[inv] = 1; }


	}
	if (!Dcahce_hit)
	{
		DCms++;
		int inv = -1;
		for (int i = made; i >= kara; i--)if (!Dcache_v[i])inv = i;
		if (inv != -1) {
			Dcache_v[inv] = true;
			Dcache_VPN[inv] = DVPN_s;
			Dcache_tag[inv] = tag;
			Dcache_B_LRU[inv] = 1; bool full = true;
			for (int i = kara; i <= made; i++)if (Dcache_B_LRU[i] != 1)full = false;
			if (full) { for (int i = kara; i <= made; i++)  Dcache_B_LRU[i] = 0; Dcache_B_LRU[inv] = 1; }

		}
		else
		{
			for (int i = made; i >= kara; i--)if (Dcache_B_LRU[i] == 0)inv = i;
			if (made == kara)inv = made;
			Dcache_v[inv] = true;
			Dcache_VPN[inv] = DVPN_s;
			Dcache_tag[inv] = tag;
			Dcache_B_LRU[inv] = 1; bool full = true;
			for (int i = kara; i <= made; i++)if (Dcache_B_LRU[i] != 1)full = false;
			if (full) { for (int i = kara; i <= made; i++)  Dcache_B_LRU[i] = 0; Dcache_B_LRU[inv] = 1; }
		}
	}


}



void D_cache(string addr, int c) {
	DpAddr = addr.substr((int)(log(Dpage_num) / log(2)));
	DpAddr = Dppn_page[BtoD(DVPN_s)] + DpAddr;
	int bl_num = D_CT / D_CB;
	int index_max = bl_num / D_S;
	int id_bit = (int)(log(index_max) / log(2));
	for (int i = 0; i < bl_num; i++)
		if (Dcache_v[i])
			Dcache_v[i] = Dpage_V[BtoD(Dcache_VPN[i])];
	string tag = DpAddr.substr(0, DpAddr.length() - id_bit);

	if (id_bit != 0) {
		string id_s = DpAddr.substr(DpAddr.length() - id_bit);
		int index = BtoD(id_s);

		Dcache_insert(index*D_S, index*D_S + D_S - 1, tag);
	}
	else Dcache_insert(0, bl_num - 1, tag);
	icachefile << c<<" "<<addr;
	for (int i = 0; i < bl_num; i++)icachefile << " " << Dcache_v[i] << " " << Dcache_tag[i] << " " << Dcache_VPN[i] << " " << Dcache_B_LRU[i] << "---";
	//for (int i = 0; i < bl_num; i++)icachefile<< Dcache_B_LRU[i]<<" ";
	icachefile << endl;


}




























void icache(int pc,int c_n){
IVA=DtoB(pc);
ITLB(IVA,c_n);
if (!ITLB_hit) Ipage(c_n);
IVA = DtoB(pc / I_CB * 4);
I_cache(IVA,c_n);

}








void dcache(int addr, int c_n) {
	itlbfile << addr<<"  : ";
	DDD = true;
	DVA = DtoB(addr);
	DTLB(DVA, c_n);
	if (!DTLB_hit) Dpage(c_n);
	DVA = DtoB(addr / D_CB);
	D_cache(DVA, c_n);
}




void end_cache(){
	delete[] ITLB_tag; delete[] ITLB_V; delete[] ITLB_LRU; delete[] Icache_B_LRU; delete[] Icache_VPN;
	delete[] Ippn_page;delete[] Ipage_V;
	delete[] Icache_tag;delete[] Icache_v;
	delete[] DTLB_tag;delete[] DTLB_V;
	delete[] Dppn_page;delete[] Dpage_V;
	delete[] Dcache_tag;delete[] Dcache_v;
}
//------------------------------------------------------------------int I_M=64,D_M=32,I_P=8,D_P=16,I_CT=16,I_CB=4,I_S=4,D_CT=16,D_CB=4,D_S=1;