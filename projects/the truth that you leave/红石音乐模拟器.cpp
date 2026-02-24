#include"../../MidiParse/5.2.1/headfile/MidiFun.h"
#include<windows.h>
#pragma comment(lib,"winmm.lib")
Note note[60000];
int noteNum;
HMIDIOUT handle;
int main(void){
	MidiFun mid;
	mid.parse("midi/The truth that you leave.mid");
	noteNum=mid.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].tick/=48;
	}
//	for(int i=noteNum-1,f=-1; f<noteNum; i--) {
//		if(note[i].volume){
//			while(note[++f].volume);
//			if(f>i){
//				noteNum=f;
//				break;
//			}
//			std::swap(note[f],note[i]);
//		}
//	}
//	std::sort(note,note+noteNum,[](Note a,Note b) {
//		if(a.volume==0||b.volume==0){
//			printf("a");
//		}
//		if(a.tick!=b.tick)
//			return a.tick<b.tick;
//		if(a.channel!=b.channel)
//			return a.channel<b.channel;
//		return a.pitch<b.pitch;
//	});
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].tick*50);
	for(int i=0; i<noteNum-1; i++) {
//		if(note[i].tick<768)
//			continue;
		midiOutShortMsg(handle,int((note[i].volume<<16) *0.2)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].tick-note[i].tick!=0)
			Sleep((note[i+1].tick-note[i].tick)*50);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	Sleep(1000);
	midiOutClose(handle);
	
	return 0;
}

