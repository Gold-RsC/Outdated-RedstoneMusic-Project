#include"../../MidiParse/5.1.5/headfile/MidiFun.h"
#include<windows.h>
#pragma comment(lib,"winmm.lib")
Note note[60000];
int noteNum;
HMIDIOUT handle;
int main(void){
	MidiFun mid;
	mid.parse("midi/起风了.mid",0);
	noteNum=mid.getNote(note);
	for(int i=0;i<noteNum;i++){
		if(note[i].time%2==1){
			note[i].time++;
		}
	}
	for(int i=0;i<noteNum;i++){
		note[i].time/=7.5;
	}
	[&](){
		FILE*w=fopen("outFile/所有音符(包括打击乐).txt","w");
		fprintf(w,"time\ttrack\tchannel\tpitch\tvolume\tinstrument\n");
		for(int i=0; i<noteNum; i++) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].pitch,note[i].volume,note[i].instrument);
		}
		fclose(w);
	}();
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].time*50);
	for(int i=0; i<noteNum-1; i++) {
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].time-note[i].time!=0)
			Sleep((note[i+1].time-note[i].time)*50);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	midiOutClose(handle);
	return 0;
}

