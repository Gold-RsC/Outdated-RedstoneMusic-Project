#include "../../MidiParse/5.0.0/headfile/MidiFun.h"
#include <windows.h>
#pragma comment(lib,"winmm.lib")
Note note[30000];
int noteNum;
HMIDIOUT handle;
MidiFun mid("midi/×·ÃÎ³à×ÓÐÄ.mid");
int main(void){
	noteNum=mid.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].time/=47.939;
	}
//	int addnum=0;
//	for(int i=1; i<noteNum; i++) {
////		note[i].time/=2;
////		note[i].time*=2;
//		if(((note[i].time&1)==1)&&((note[i-1].time&1)==0)){
//			printf("%d\n",note[i].time);
//		}
//	
//	}
	std::sort(note,note+noteNum,[](Note a,Note b){
		return a.time<b.time;
	});
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].time*50);
	for(int i=0; i<noteNum-1; i++) {
//		printf("%d\n",note[i].channel);
//		if(((note[i].time&1)==1)&&((note[i-1].time&1)==0)){
//			printf("%d\n",note[i].time);
//		}
		if(
		note[i].channel==1||
		note[i].channel==2||
		note[i].channel==4||
		note[i].channel==8||
		note[i].channel==6||
		note[i].channel==11||
		note[i].channel==13||
		note[i].channel==14||
		note[i].channel==9
		)
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].time-note[i].time!=0)
			Sleep((note[i+1].time-note[i].time)*50);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	midiOutClose(handle);
	return 0;
}

