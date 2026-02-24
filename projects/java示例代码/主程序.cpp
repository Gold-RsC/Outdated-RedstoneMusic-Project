#include"headfile/MidiFun.h"
#include<string>
using namespace std;
MidiFun mid("midi/A Little Story.mid");
FILE*set=fopen("functions/set.mcfunction","w");;
string file[10000];
Note note[30000];
int noteNum;
long long maxTime;
int lamta=5;
void init(void){
	//获取音符事件，并获取音符数量 
	noteNum=mid.getNote(note);

	
	//-------------------------------------------
	//在循环中会执行两个操作，第一个：单位转换（由ms转换成gametick） ，第二个：寻找最大的gametick值（用于连接mcfunction） 
	for(int i=0; i<noteNum; i++) {
		note[i].time/=48.14;//本来应该除以50，但除以50后误差比48.14大得多（具体表现是播放时出现非系统造成的卡顿）
		maxTime=max(maxTime,note[i].time);
	}
	
	
	//-------------------------------------------
	//以track->channel->time的优先级来排序（箭头指向的优先级低） 
//	bool compare(Note&a,Note&b){
//		if(a.track!=b.track)
//			return a.track<b.track;
//		if(a.channel!=b.channel)
//			return a.channel<b.channel;
//		return a.time<b.time;
//	}
//	sort(note,note+noteNum,compare); 
	//以下代码等价于上面的代码，下面代码就是上面代码的一种简便写法(其实也没多简便) 
	sort(note,note+noteNum,[](Note&a,Note&b){
		if(a.track!=b.track)
			return a.track<b.track;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.time<b.time;
	});
	
	
	//-------------------------------------------
	//写set.mcfunction中的指令
	for(int i=0;i<noteNum;i++){
		//这里用的是海晶灯，可以换成其他的方块 
		fprintf(set,"setblock %d %d %d sea_lantern\n",note[i].time,200,note[i].pitch);
	}
}


//-------------------------------------------
//连接两个点 
//CnctP=Connect (two) Points
void drawline_CnctP(Note a,Note b){
	char tmp[1000]={0};
	//time为x坐标,pitch为z坐标，无y坐标数据
	int deltaTime=b.time-a.time;
	double vx=(b.time-a.time)*1.0/deltaTime;
	double vz=(b.pitch-a.pitch)*1.0/deltaTime;
	for(int t=0;t<deltaTime;t++){
		for(int i=0;i<=lamta;i++){
			double xi=a.time+vx*(t+i*1.0/lamta)+0.5;
			double zi=a.pitch+vz*(t+i*1.0/lamta)+0.5;
			sprintf(tmp,"particle endrod %lf %lf %lf 0 0 0 0 1 force\n",xi,200,zi);
			file[t+a.time+10]+=tmp;
		}
	} 
}
//两组time值相同的点互相连接（即选出两点传入到drawline_CnctP函数中） 
//Cnct2PGrpWthIchOthr=Connect Two Groups Of Points With Each Other
void drawline_Cnct2PGrpWthIchOthr(Note*lp,Note*np,int lpn,int npn){
//for一个ample，方便梳理思路 
//**********************************
//两种情况，左和右 
// npn||lpn k					|  npn||lpn k
//	5   10  2					|	5   13  3(2)
//	1    2    3    4    5		|	1      2      3      4     5
//	1 2  3 4  5 6  7 8  9 10	|	1 2 3  4 5 6  7 8 9  10 11 12 13
//**********************************
	if(lpn>npn) {
		int k=lpn/npn;
		if(k*npn==lpn)
			for(int i=1; i<=npn; i++)
				for(int j=k*(i-1)+1; j<=k*i; j++)
					drawline_CnctP(lp[j-1],np[i-1]);
		else {
			for(int i=1; i<=lpn-k*npn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					drawline_CnctP(lp[j-1],np[i-1]);
			for(int i=lpn-k*npn+1; i<=npn; i++)
				for(int j=k*(i-1-npn)+lpn+1; j<=k*(i-npn)+lpn; j++)
					drawline_CnctP(lp[j-1],np[i-1]);
		}
	} else if(npn>lpn) {
		int k=npn/lpn;
		if(k*lpn==npn)
			for(int i=1; i<=lpn; i++)
				for(int j=k*i-k+1; j<=k*i; j++)
					drawline_CnctP(lp[i-1],np[j-1]);
		else {
			for(int i=1; i<=npn-k*lpn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					drawline_CnctP(lp[i-1],np[j-1]);
			for(int i=npn-k*lpn+1; i<=lpn; i++)
				for(int j=k*(i-1-lpn)+npn+1; j<=k*(i-lpn)+npn; j++)
					drawline_CnctP(lp[i-1],np[j-1]);
		}
	} else
		for(int i=1; i<=npn; i++)
			drawline_CnctP(lp[i-1],np[i-1]);
} 
//多次将原来的Note数组分解成两组time相同的note数组并调用drawline_Cnct2PGrpWthIchOthr函数 
//Grp2MnGrp=One Group To Many Goups
void drawline_Grp2MnGrp(Note initPoint) {
	Note*lp=new Note[128];
	Note*np=new Note[128];
	int lpn=1,npn=1;
	lp[0]=initPoint;
	np[0]=note[0];
	for(int i=1; i<noteNum; i++) {
		//音符中，有两个音符表示一个音符，第一个音符表示这个音符开始，第二个音符表示这个音符结束，一般情况第二个音符的音量为0，故可用此方法排除第二个音符 
		if(!note[i].volume)
			continue;
//		只要 np的channel与此时note的channel不同 
//			或者 np的track与此时note的track不同 
//		则说明 这个channel(通道)或这个track(轨道)的线已经造完 
//  		就 造线 并 重置lp和np
//		重置方法:
//		1.lp,np清空 
//		2.lp的第〇项为初始点
//		3.np的第〇项为note[i]
//		4.lpn,npn设为1
		if(
		    np[0].channel!=note[i].channel||
		    np[0].track!=note[i].track
		) {
			drawline_Cnct2PGrpWthIchOthr(lp,np,lpn,npn);
			lp[0]=initPoint;
			np[0]=note[i];
			lpn=1;
			npn=1;
		}
//		channel/track相同的前提下
//			则说明 此channel/track音符未遍历结束 
//		如果 tick不同
//			则 造线 并 重置lp,np 
//		重置方法:
//		1.lp清空 
//		2.np移植给lp
//		3.np清空
//		4.np的第〇项被note[i]赋值
//		5.lpn为npn，之后npn设为1 
		else if(
		    np[0].channel==note[i].channel&&
		    np[0].track==note[i].track&&
		    np[0].time!=note[i].time
		) {
			drawline_Cnct2PGrpWthIchOthr(lp,np,lpn,npn);
			for(int j=0;j<npn;j++)
				lp[j]=np[j];
			np[0]=note[i];
			lpn=npn;
			npn=1;
		} else
			np[npn++]=note[i];
	}
	drawline_Cnct2PGrpWthIchOthr(lp,np,lpn,npn);
}
void makeLine(void){
	//a代表起始点，可以参考特效红石音乐视频，一般的连线都会初始点开始 
	Note a;
	a.pitch=64;//代表z坐标
	a.time=-10;//代表x坐标 
	drawline_Grp2MnGrp(a);
}
void makePoint(void){
	//播放粒子，同时摧毁方块 
	char tmp[1000]={0};
	for(int i=0;i<noteNum;i++){
		sprintf(tmp,"summon minecraft:firework_rocket %d %d %d {\"Silent\":1b,\"Motion\":[0d,1d,0d],\"FireworksItem\":{\"id\":\"minecraft:firework_rocket\",\"Count\":1b,\"tag\":{\"Fireworks\":{\"Explosions\":[{\"Colors\":[I;16777215],\"Type\":4b,\"Trail\":1b},]}}}}\n",note[i].time,200,note[i].pitch);
		file[note[i].time+10]+=tmp;
		sprintf(tmp,"setblock %d %d %d air\n",note[i].time,200,note[i].pitch);
		file[note[i].time+10]+=tmp;
	}
}
void makeParticle(void){
	//这里分成两部分来写，第一部分：用于连线的粒子，第二部分：用于触发点的粒子 
	makeLine();
	makePoint();
}


//-------------------------------------------
//连接mcfunction，并关闭保存所有文件 
void linkFunction(void){
	char tmp[100]={0};
	for(int i=-10;i<maxTime;i++){
		sprintf(tmp,"gamerule gameLoopFunction lalala:%d\n",i+1);
		file[i+10]+=tmp;
	}
}


//-------------------------------------------
//关闭文件 
void end(void){
	char tmp[100]={0};
	for(int i=-10;i<=maxTime;i++){
		sprintf(tmp,"functions/%d.mcfunction",i);
		FILE*w=fopen(tmp,"w");
		fprintf(w,file[i].c_str());
		fclose(w);
	}
	fclose(set);
}

//-------------------------------------------
//main函数 
int main(void){
	init();
	makeParticle();
	linkFunction();
	end();
	return 0;
}
