#include"../../MidiParse/5.1.5/headfile/MidiFun.h"
using namespace std;
MidiFun midi;
Note note[20000];
int noteNum;
void init(void){
	midi.parse("midi/骄傲的少年.mid",0);
	noteNum=midi.getNote(note);
	for(int i=0;i<noteNum;i++){
		note[i].time/=30;
	}
	[]{
		FILE*w=fopen("outFile\\所有音符.txt","w");
		fprintf(w,"time\ttrack\tchannel\tpitch\tvolume\tinstrument\n");
		for(int i=0; i<noteNum; i++) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
		}
		fclose(w);
	}();
}

int main(void){
	init();
//	block();
//	particle();
	return 0;
}
