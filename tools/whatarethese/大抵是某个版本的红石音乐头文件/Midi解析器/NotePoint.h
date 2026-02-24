/*
	Name: MidiFile.h
	Author: Gold_RsC
	Starting Date: 2020/10/3
	Complete Date: 2020/10/5
	Author's website in Bilibili:
		https://space.bilibili.com/361846321?from=search&seid=3387696492724899772
	Description: 
		Copyright belongs to Gold_RsC.
		No reprint without permissio!!!
		 
*/
#ifndef __NOTEPOINT_H__
#define __NOTEPOINT_H__
#include"MidiFile.h"
class Point{
	public:
		//midi tick,game tick,channel,pitch,velocity
		int tick,gt,channel,pitch,velocity;
		//note on(1) or off(0)
		bool type;
		//program
		int prgrm;
		bool operator==(Point&note);
		bool operator!=(Point&note);
		void*operator new(size_t size);
		void operator delete(void*me);
		void*operator new[](size_t size);
		void operator delete[](void*me);
};
bool Point::operator==(Point&note){
	return (this->tick==note.tick&&this->channel==note.channel&&this->pitch==note.pitch&&this->velocity==note.velocity&&this->type==note.type&&this->prgrm==note.prgrm);
}
bool Point::operator!=(Point&note){
	return !(*this==note);
}
void*Point::operator new(size_t size){
	Point*me=(Point*)malloc(size);
	return me;
}
void Point::operator delete(void*me){
	free(me);
}
void*Point::operator new[](size_t size){
	Point*me=(Point*)malloc(size);
	return me;
}
void Point::operator delete[](void*me){
	free(me);
}
class PBPM{
	public:
		//miditick,bpm 
		int tick,value;
		//相当于gt 
		int preva_sum;
		bool operator==(PBPM&bpm);
		bool operator!=(PBPM&bpm);
		void*operator new(size_t size);
		void operator delete(void*me);
		void*operator new[](size_t size);
		void operator delete[](void*me);
};
bool PBPM::operator==(PBPM&bpm){
	return (this->tick==bpm.tick&&this->value==bpm.value&&this->preva_sum==bpm.preva_sum);
}
bool PBPM::operator!=(PBPM&bpm){
	return !(*this==bpm);
}
void*PBPM::operator new(size_t size){
	PBPM*me=(PBPM*)malloc(size);
	return me;
}
void PBPM::operator delete(void*me){
	free(me);
}
void*PBPM::operator new[](size_t size){
	PBPM*me=(PBPM*)malloc(size);
	return me;
}
void PBPM::operator delete[](void*me){
	free(me);
}
class MidiFun{
	protected:
		int TPQN;
		MidiEvent midi;
		Point*notepoint;
		PBPM*pbpm;
		int nnum;
		int bnum;
		int GAME_TICK_MAX;
		int findbpmidx(PBPM*bpm,int len,int tick);
		void drawline(Point*lp,Point*np,int lpn,int npn,void (*writecmd)(Point,Point));
	public:
		//雨女无瓜 
		MidiFun();
		//初始化 
		void init(MidiEvent&midie,Point*note,PBPM*bpm);
		void init(const char*midiname,Point*note,PBPM*bpm);
		//将note和bpm事件屯到上面的note和bpm数组里 
		//读取成功返回1，失败返回0
		//失败原因：
		//1.数组开的过小
		//2.midi出错 
		bool getev(int note_num=10000,int bpm_num=100);
		//返回game tick最大值 
		int gt_max();
		//只获取note事件，返回note数量，失败返回-1(ERROR_EVENT) 
		int get_note(int note_num=10000);
		//与上一个函数类似
		int get_bpm(int bpm_num=100);
		//造线：起始点，函数(已知两点，将它们连起) 
		void makeline(Point&pinit,void (*writecmd)(Point,Point));
};
MidiFun::MidiFun(){
	nnum=ERROR_EVENT;
	bnum=ERROR_EVENT;
	GAME_TICK_MAX=ERROR_EVENT;
}
void MidiFun::init(MidiEvent&midie,Point*note,PBPM*bpm){
	midi=midie;
	notepoint=note;
	pbpm=bpm;
	TPQN=midi.gettpqn();
}
void MidiFun::init(const char*midiname,Point*note,PBPM*bpm){
	midi.read(midiname);
	notepoint=note;
	pbpm=bpm;
	TPQN=midi.gettpqn();
}
void MidiFun::drawline(Point*lp,Point*np,int lpn,int npn,void (*writecmd)(Point,Point)){
//for一个ample，方便梳理思路 
//**********************************
//两种情况，左和右 
// npn||lpn k					|  npn||lpn k
//	5   10  2					|	5   13  3(2)
//	1    2    3    4    5		|	1      2      3      4     5
//	1 2  3 4  5 6  7 8  9 10	|	1 2 3  4 5 6  7 8 9  10 11 12 13
//**********************************
	if(lpn*1.0/npn>1){//lpn为npn的k倍(k>1) 
		int k=lpn/npn;
		if(k*npn==lpn)
			for(int i=1;i<=npn;i++)
				for(int j=k*(i-1)+1;j<=k*i;j++)
					writecmd(lp[j-1],np[i-1]);
		else{
			for(int i=1;i<=lpn-k*npn;i++)
				for(int j=(k+1)*(i-1)+1;j<=(k+1)*i;j++)
					writecmd(lp[j-1],np[i-1]);
			for(int i=lpn-k*npn+1;i<=npn;i++)
				for(int j=k*(i-1-npn)+lpn+1;j<=k*(i-npn)+lpn;j++)
					writecmd(lp[j-1],np[i-1]);
		}
	}
	else if(npn*1.0/lpn>1){//npn为lpn的k倍(k>1)
		int k=npn/lpn;
		if(k*lpn==npn)
			for(int i=1;i<=lpn;i++)
				for(int j=k*i-k+1;j<=k*i;j++)
					writecmd(lp[i-1],np[j-1]);
		else{
			for(int i=1;i<=npn-k*lpn;i++)
				for(int j=(k+1)*(i-1)+1;j<=(k+1)*i;j++)
					writecmd(lp[i-1],np[j-1]);
			for(int i=npn-k*lpn+1;i<=lpn;i++)
				for(int j=k*(i-1-lpn)+npn+1;j<=k*(i-lpn)+npn;j++)
					writecmd(lp[i-1],np[j-1]);
		}
	}
	else//lpn等于npn 
		for(int i=1;i<=npn;i++)
			writecmd(lp[i-1],np[i-1]);
} 
void MidiFun::makeline(Point&pinit,void (*writecmd)(Point,Point)){
//	初始化:
//	1.lpn,npn为1
//	2.将lp第一项赋值为初始点，提前获取npn第一项 
	Point*lp=new Point[128],*np=new Point[128];
	int lpn=1,npn=1;
	lp[0]=pinit;
	for(int i=0;i<nnum;i++){
		if(!notepoint[i].type)continue;
//			只要 np的channel与此时note的channel不同 
//			或者 np的tick大于note的tick 
//			则说明 这个channel或此音轨的线已经造完 
//  	    就 造线 并 重置lp和np
//			重置方法:
//			1.lp,np清空 
//			2.lp的第一项为初始点
//			3.np的第一项为note[i]
//			4.lpn,npn设为1
		if(
			np[0].channel!=notepoint[i].channel||
			np[0].gt>notepoint[i].gt
		){
			drawline(lp,np,lpn,npn,writecmd);
			memset(lp,0,sizeof(class Point)*128);
			memset(np,0,sizeof(class Point)*128);
			lp[0]=pinit;
			np[0]=notepoint[i];
			lpn=1;
			npn=1;
		}
//		channel相同的前提下
//		则说明 此channel未结束 
//		如果 tk不同
//		则 造线 并 重置lp,np 
//		重置方法:
//		1.lp清空 
//		2.np移植给lp
//		3.np清空
//		4.np的第一项被note[i]赋值
//		5.lpn为npn，之后npn设为1
		else if(
			np[0].channel==notepoint[i].channel&&
			np[0].gt!=notepoint[i].gt
		){
			drawline(lp,np,lpn,npn,writecmd);
			memset(lp,0,sizeof(class Point)*128);
			for(int j=0;j<npn;j++)
				lp[j]=np[j];
			memset(np,0,sizeof(class Point)*128);
			np[0]=notepoint[i];
			lpn=npn;
			npn=1;
		}
//		channel相同的前提下 
//		如果tk相同 
//		即 else（其他情况） 
//		则 在np中添加note[i]
		else
			np[npn++]=notepoint[i];
	}
//	处理剩下的点 
	drawline(lp,np,lpn,npn,writecmd);
	delete[] np;
	delete[] lp;
}
int MidiFun::findbpmidx(PBPM*bpm,int len,int tick){
	int l=0,r=len-1;
	if(tick>=bpm[r].tick)
		return r;
	if(tick<bpm[l].tick)
		return ERROR_EVENT;
	while(l<=r){
		int mid=(l+r)/2;
		if(tick<bpm[mid].tick)
			r=mid;
		else if(tick>=bpm[mid+1].tick)
			l=mid+1;
		else
			return mid;
	}
	return ERROR_EVENT;
}
bool MidiFun::getev(int note_num,int bpm_num){
	NOTE*note=new NOTE[note_num];
	BPM*bpm=new BPM[bpm_num];
	nnum=midi.getnote(note,note_num);
	if(nnum==ERROR_EVENT)
		return 0;
	bnum=midi.getbpm(bpm,bpm_num);
	if(bnum==ERROR_EVENT)
		return 0;
	for(int bpmidx=0;bpmidx<bnum;bpmidx++){
		pbpm[bpmidx].value=bpm[bpmidx].value;
		pbpm[bpmidx].tick=bpm[bpmidx].tick;
		pbpm[bpmidx].preva_sum=
			(bpmidx==0?0:pbpm[bpmidx-1].preva_sum)
			+(pbpm[bpmidx].tick-(bpmidx==0?0:pbpm[bpmidx-1].tick))
			*1200/TPQN
			/(bpmidx==0?1:pbpm[bpmidx-1].value);
	}
	for(int noteidx=0;noteidx<nnum;noteidx++){
		notepoint[noteidx].tick=note[noteidx].tick;
		notepoint[noteidx].channel=note[noteidx].channel;
		notepoint[noteidx].pitch=note[noteidx].pitch;
		notepoint[noteidx].velocity=note[noteidx].velocity;
		notepoint[noteidx].type=note[noteidx].type;
		notepoint[noteidx].prgrm=note[noteidx].prgrm;
		int bpmidx=findbpmidx(pbpm,bnum,notepoint[noteidx].tick);
		if(bpmidx==ERROR_EVENT)
			return 0;
		notepoint[noteidx].gt=
			pbpm[bpmidx].preva_sum
			+(notepoint[noteidx].tick-pbpm[bpmidx].tick)
			*1200/TPQN/pbpm[bpmidx].value;
		GAME_TICK_MAX=(GAME_TICK_MAX<notepoint[noteidx].gt?notepoint[noteidx].gt:GAME_TICK_MAX);
	}
	delete[] note;
	delete[] bpm;
	return 1;
}
int MidiFun::get_note(int note_num){
	NOTE*note=new NOTE[note_num];
	nnum=midi.getnote(note,note_num);
	if(nnum==ERROR_EVENT)
		return 0;
	if(bnum==ERROR_EVENT)
		bnum=get_bpm(100);
	for(int noteidx=0;noteidx<nnum;noteidx++){
		notepoint[noteidx].tick=note[noteidx].tick;
		notepoint[noteidx].channel=note[noteidx].channel;
		notepoint[noteidx].pitch=note[noteidx].pitch;
		notepoint[noteidx].velocity=note[noteidx].velocity;
		notepoint[noteidx].type=note[noteidx].type;
		notepoint[noteidx].prgrm=note[noteidx].prgrm;
		int bpmidx=findbpmidx(pbpm,bnum,notepoint[noteidx].tick);
		if(bpmidx==ERROR_EVENT)
			return 0;
		notepoint[noteidx].gt=
			pbpm[bpmidx].preva_sum
			+(notepoint[noteidx].tick-pbpm[bpmidx].tick)
			*1200/TPQN/pbpm[bpmidx].value;
		GAME_TICK_MAX=(GAME_TICK_MAX<notepoint[noteidx].gt?notepoint[noteidx].gt:GAME_TICK_MAX);
	}
	delete[] note;
	return nnum;
}
int MidiFun::get_bpm(int bpm_num){
	BPM*bpm=new BPM[bpm_num];
	bnum=midi.getbpm(bpm,bpm_num);
	if(bnum==ERROR_EVENT)
		return 0;
	for(int bpmidx=0;bpmidx<bnum;bpmidx++){
		pbpm[bpmidx].value=bpm[bpmidx].value;
		pbpm[bpmidx].tick=bpm[bpmidx].tick;
		pbpm[bpmidx].preva_sum=
			(bpmidx==0?0:pbpm[bpmidx-1].preva_sum)
			+(pbpm[bpmidx].tick-(bpmidx==0?0:pbpm[bpmidx-1].tick))
			*1200/TPQN
			/(bpmidx==0?1:pbpm[bpmidx-1].value);
	}
	delete[] bpm;
	return bnum;
}
int MidiFun::gt_max(){
	if(GAME_TICK_MAX!=ERROR_EVENT)
		return GAME_TICK_MAX;
	nnum=get_note(30000);
	return GAME_TICK_MAX;
}
#endif

