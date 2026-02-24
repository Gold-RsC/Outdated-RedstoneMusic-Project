#include"../../MidiParse/5.1.5/headfile/MidiFun.h"
#include<string>
using namespace std;
MidiFun midi;
FILE*set=fopen("functions/set.mcfunction","w");;
string file[10000];
Note note[10000];
int noteNum;
long long maxTime;

void init(void){
	midi.parse("midi/Æð·çÁË.mid",0);
	noteNum=midi.getNote(note);

	for(int i=0; i<noteNum; i++) {
		note[i].time/=7.5;
		maxTime=max(maxTime,note[i].time);
	}
	
	sort(note,note+noteNum,[](Note&a,Note&b){
		if(a.volume!=b.volume)
			return a.volume<b.volume;
		if(a.track!=b.track)
			return a.track<b.track;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.time<b.time;
	});
}

void end(void){
	fclose(set);
}

int main(void){
	init();
	end();
	return 0;
}
