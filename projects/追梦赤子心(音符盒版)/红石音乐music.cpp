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
map<const char*,int> instru2low{
	{"harp",54},
	{"bass",30},
	{"flute",66},
	{"bell",78}
};
map<int,int> channel2volume{
	{2,111},
	{1,95},
	{4,71},
	{8,71},
	{6,64},
	{11,54},
	{13,83},
	{9,95}
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
	{50,"bd"},
	{48,"bd"},
	{47,"bd"},
	{40,"snare"},
	{35,"bd"},
	{41,"bd"},
	{36,"bd"},
	{43,"bd"},
	{38,"snare"},
	{55,"snare"},
	{57,"snare"},
	{69,"hat"}
};
FILE *music;
MidiFun mid("midi/追梦赤子心.mid");
Note note[30000];
int noteNum;
void init(void){
	music=fopen("functions/music.mcfunction","w");
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
		//pitch
		if(note[i].channel!=9){
//			if((int)instru2low[channel2instru[note[i].channel]]>note[i].pitch){
//				while(note[i].pitch<(int)instru2low[channel2instru[note[i].channel]]){
//					note[i].pitch+=12;
//				}
//			}
//			else if((int)instru2low[channel2instru[note[i].channel]]+24<note[i].pitch){
//				while(note[i].pitch>(int)instru2low[channel2instru[note[i].channel]]+24){
//					note[i].pitch-=12;
//				}
//			}
//			printf("%d\t",note[i].pitch);
//			note[i].pitch+=54-(int)instru2low[channel2instru[note[i].channel]];
//			printf("%d\n",note[i].pitch);
			fprintf(music,"%d execute @a[scores={tick=%d}] ~5~%d~%d playsound note.%s @s ~~~ 1 %lf 0\n",note[i].time,note[i].time,y,z,channel2instru[note[i].channel],pow(2,(note[i].pitch-66)*1.0/12));
			if(z)
				fprintf(music,"%d execute @a[scores={tick=%d}] ~5~%d~%d playsound note.%s ~~~ @s 1 %lf 0\n",note[i].time,note[i].time,y,-z,channel2instru[note[i].channel],pow(2,(note[i].pitch-66)*1.0/12));
		}
		else{
			fprintf(music,"%d execute @a[scores={tick=%d}] ~5~%d~%d playsound note.%s @s ~~~ 1 %lf 0\n",note[i].time,note[i].time,y,z,channel9_pitch2instru[note[i].pitch],pow(2,(54-66)*1.0/12));
			if(z)
				fprintf(music,"%d execute @a[scores={tick=%d}] ~5~%d~%d playsound note.%s @s ~~~ 1 %lf 0\n",note[i].time,note[i].time,y,-z,channel9_pitch2instru[note[i].pitch],pow(2,(note[i].pitch-66)*1.0/12));
		}
//		fprintf(play,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].time+5,x,y,z);
//		if(z)
//			fprintf(play,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].time+5,x,y,-z);
	}
}
void end(void){
	fclose(music);
}
int main(void){
	init();
	makeFun();
	end();
	return 0;
}
