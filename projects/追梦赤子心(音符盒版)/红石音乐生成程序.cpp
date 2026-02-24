/*0:去
3:升8k或去
5:主旋律重复，去
7:主旋律重复，去
10:高潮主旋律重复，可去
12:衔接重复，去
14:高潮旁奏音符太少，去 
			  0
			  |
			  |
-PI	----------|---------- PI
			  |
			  |
			  |
			  |
			(+-)2PI
					volume	rotation
2:主旋律			127		180
1:副旋律			63		(+-)60
4:高潮副旋律		63		(+-)120
8:高潮主和音旋律	95		(+-)90
6:旁奏				63		(+-)150
11:和音				71		(+-)75
13:衔接				111		(+-)30
9:打击乐			95		(+-)120
*/
#include"../../MidiParse/5.0.0/headfile/MidiFun.h"
#include<map>
#include<time.h>
using namespace std;
#define PI 3.1415926535897931
map<int,const char*> channel2instru{
	{2,"bell"},
	{1,"harp"},
	{4,"bass"},
	{8,"harp"},
	{6,"harp"},
	{11,"flute"},
	{13,"harp"},
	{14,"flute"}
};
map<const char*,const char*> instru2block{
	{"bell","gold_block"},
	{"harp","grass"},
	{"flute","clay"},
	{"bass","planks"}
};
map<const char*,int> instru2low{
	{"harp",54},
	{"bass",30},
	{"flute",66},
	{"bell",78}
};
map<int,int> channel2volume{
	{2,127},
	{1,95},
	{4,87},
	{8,95},
	{6,63},
	{11,53},
	{13,87},
	{9,111}
};
map<int,int> channel2rotation{
	{2,180},
	{1,60},
	{4,120},
	{8,90},
	{6,150},
	{11,75},
	{13,30},
	{9,120}
};
map<int,const char*> channel9_pitch2instru{
	{50,"bassdrum"},
	{48,"bassdrum"},
	{47,"bassdrum"},
	{40,"snaredrum"},
	{35,"bassdrum"},
	{41,"bassdrum"},
	{36,"bassdrum"},
	{43,"bassdrum"},
	{38,"snaredrum"},
	{55,"snaredrum"},
	{57,"snaredrum"},
	{69,"hat"}
};
map<const char*,const char*> channel9_instru2block{
	{"bassdrum","stone"},
	{"snaredrum","sand"},
	{"hat","glass"}
};
FILE *set,*play;
MidiFun mid("midi/追梦赤子心.mid");
Note note[30000];
int noteNum;
void init(void){
	set=fopen("functions/set.mcfunction","w");
	play=fopen("functions/play.mcfunction","w");
	srand((unsigned)time(NULL));
	noteNum=mid.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].time/=47.939;
	}
	sort(note,note+noteNum,[](Note a,Note b){
		auto check=[](int channel,int volume,int pitch){
			return (channel==2||channel==1||channel==4||channel==8||channel==6||channel==11||channel==13||(channel==9&&(pitch==50||pitch==48||pitch==47||pitch==40||pitch==35||pitch==41||pitch==36||pitch==43||pitch==38||pitch==55||pitch==57||pitch==69)))&&volume!=0;
		};
		if(check(a.channel,a.volume,a.pitch)==check(b.channel,b.volume,b.pitch))
			return a.time<b.time;
		else if(check(a.channel,a.volume,a.pitch))
			return true;
		else
			return false;
	});
	auto check=[](int channel,int volume,int pitch){
		return (channel==2||channel==1||channel==4||channel==8||channel==6||channel==11||channel==13||(channel==9&&(pitch==50||pitch==48||pitch==47||pitch==40||pitch==35||pitch==41||pitch==36||pitch==43||pitch==38||pitch==55||pitch==57||pitch==69)))&&volume!=0;
	};
	for(int i=1;i<noteNum;i++){
		if(!check(note[i].channel,note[i].volume,note[i].pitch)){
			noteNum=i;
			break;
		}
		note[i].volume=channel2volume[note[i].channel];
	}
	[]{
		FILE*w=fopen("outFile\\所有音符.txt","w");
		fprintf(w,"time\ttrack\tchannel\tbar\tpitch\tvolume\tbar\tinstrument\n");
		for(int i=0; i<noteNum; i++) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].bar,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
		}
		fclose(w);
	}();
}
bool xyz[30000][48][48]={0};
void makeFun(void){
//	fprintf(play,"scoreboard players add @a[scores={tick=-10..%d}] tick 1\n",note[noteNum-1].time+10);
	for(int i=0;i<noteNum;i++){
		//position
		int x=note[i].time;
		int z=sin(PI*(int)channel2rotation[note[i].channel]/180.0)*(1-(int)channel2volume[note[i].channel]/127.0)*48;
		int y=cos(PI*(int)channel2rotation[note[i].channel]/180.0)*(1-(int)channel2volume[note[i].channel]/127.0)*48;
		if(xyz[x][y][z]){
			while(xyz[x][y][z++]);
		}
		xyz[x][y][z]=1;
		xyz[x][y][-z]=1;
		xyz[x][y-1][z]=1;
		xyz[x][y-1][-z]=1;
		if(note[i].channel==9&&(const char*)channel9_instru2block[channel9_pitch2instru[note[i].pitch]]=="sand"){
			xyz[x][y-2][z]=1;
			xyz[x][y-2][-z]=1;
		}
		y+=100;
		//pitch
		if(note[i].channel!=9){
			if((int)instru2low[channel2instru[note[i].channel]]>note[i].pitch){
				while(note[i].pitch<(int)instru2low[channel2instru[note[i].channel]]){
					note[i].pitch+=12;
				}
			}
			else if((int)instru2low[channel2instru[note[i].channel]]+24<note[i].pitch){
				while(note[i].pitch>(int)instru2low[channel2instru[note[i].channel]]+24){
					note[i].pitch-=12;
				}
			}
			note[i].pitch+=54-(int)instru2low[channel2instru[note[i].channel]];
			fprintf(set,"clone -100 4 %d -100 4 %d %d %d %d\n",note[i].pitch,note[i].pitch,x,y,z);
			if(z)
				fprintf(set,"clone -100 4 %d -100 4 %d %d %d %d\n",note[i].pitch,note[i].pitch,x,y,-z);
			fprintf(set,"setblock %d %d %d %s\n",x,y-1,z,(const char*)instru2block[channel2instru[note[i].channel]]);
			if(z)
				fprintf(set,"setblock %d %d %d %s\n",x,y-1,-z,(const char*)instru2block[channel2instru[note[i].channel]]);
		}
		else{
			fprintf(set,"clone -100 4 %d -100 4 %d %d %d %d\n",54,54,x,y,z);
			if(z)
				fprintf(set,"clone -100 4 %d -100 4 %d %d %d %d\n",54,54,x,y,-z);
			fprintf(set,"setblock %d %d %d %s\n",x,y-1,z,(const char*)channel9_instru2block[channel9_pitch2instru[note[i].pitch]]);
			if(z)
				fprintf(set,"setblock %d %d %d %s\n",x,y-1,-z,(const char*)channel9_instru2block[channel9_pitch2instru[note[i].pitch]]);
			if((const char*)channel9_instru2block[channel9_pitch2instru[note[i].pitch]]=="sand"){
				fprintf(set,"setblock %d %d %d %s\n",x,y-2,z,"barrier");
				if(z)
					fprintf(set,"setblock %d %d %d %s\n",x,y-2,-z,"barrier");
			}
		}
		
		fprintf(play,"%d execute @a[scores={tick=%d}] ~~~ setblock %d %d %d redstone_block\n",note[i].time,note[i].time,x-1,y,z);
		if(z)
			fprintf(play,"%d execute @a[scores={tick=%d}] ~~~ setblock %d %d %d redstone_block\n",note[i].time,note[i].time,x-1,y,-z);
//		fprintf(play,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].time+5,x,y,z);
//		if(z)
//			fprintf(play,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].time+5,x,y,-z);
		if((note[i].bar>=29&&note[i].bar<=44)||(note[i].bar>=69&&note[i].bar<=89)){
			if(note[i].channel==2){
				fprintf(play,"%d execute @a[scores={tick=%d}] ~~~ particle yellow_boom %d %d %d\n",note[i].time,note[i].time,x,y,z);
			}
			else if(note[i].channel==9){
				fprintf(play,"%d execute @a[scores={tick=%d}] ~~~ particle red_boom %d %d %d\n",note[i].time,note[i].time,x,y,z);
			}
		}
		if(note[i].beat==29*4&&note[i].channel==1){
			fprintf(play,"%d execute @a[scores={tick=%d}] ~~~ particle magic %d %d %d\n",note[i].time,note[i].time,x,y,z);
		}
		
	}
}
void end(void){
	fclose(set);
	fclose(play);
}
int main(void){
	init();
	makeFun();
	end();
	return 0;
}
