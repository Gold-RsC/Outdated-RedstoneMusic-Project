/***********************
*	channel	block
*	1		
***********************/
#include"../../MidiParse/5.2.1/headfile/MidiFun.h"
#include<map>
using namespace std;
MidiFun midi;
Note note[20000];
int noteNum;
FILE*file;
map<int,const char*> channel2block{
	{1,""}
};
void init(void) {
	file=fopen("functions/play.mcfunction","w");
	midi.parse("midi/The truth that you leave.mid");
	noteNum=midi.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].tick/=48;
	}
	for(int i=noteNum-1,f=-1; f<noteNum; i--) {
		if(note[i].volume){
			while(note[++f].volume);
			if(f>i){
				noteNum=f;
				break;
			}
			std::swap(note[f],note[i]);
		}
	}
	sort(note,note+noteNum,[](Note a,Note b) {
		if(a.volume==0||b.volume==0){
			printf("a");
		}
		if(a.tick!=b.tick)
			return a.tick<b.tick;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.pitch<b.pitch;
	});
	[&]{
		FILE*w=fopen("outFile/a.txt","w");
		fprintf(w,"time\ttrack\tchannel\tbeat\tpitch\tvolume\tbar\tinstrument\n");
		for(int i=0; i<noteNum; i++) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",note[i].tick,note[i].track,note[i].channel,note[i].beat,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
		}
		fclose(w);
	}();
}
void fun(void){
	
}
void end(void){
	fclose(file);
}
int main(void) {
	init();
	fun();
	end();
	return 0;
}
