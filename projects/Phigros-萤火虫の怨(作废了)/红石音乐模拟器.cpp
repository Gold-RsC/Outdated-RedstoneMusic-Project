#include"../../MidiParse/5.2.1/headfile/MidiFun.h"
#include<windows.h>
#pragma comment(lib,"winmm.lib")
Note note[60000];
int noteNum;
HMIDIOUT handle;
int main(void){
	MidiFun mid;
	mid.parse("midi/Phigros-Ó©»ð³æ¤ÎÔ¹.mid");
	noteNum=mid.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].tick/=40;
	}
	int flag=noteNum;
	for(int i=0;i<=flag;i++){
		if(note[i].volume==0){
			while(note[--flag].volume==0);
			std::swap(note[i],note[flag]);
		}
	}
	for(noteNum=0;note[noteNum++].volume!=0;){}
	std::sort(note,note+noteNum,[](Note a,Note b){
		if(a.tick!=b.tick)
			return a.tick<b.tick;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.pitch<b.pitch;
	});
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].tick*50);
	for(int i=0; i<noteNum-1; i++) {
//		if(note[i].tick<768)
//			continue;
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].tick-note[i].tick!=0)
			Sleep((note[i+1].tick-note[i].tick)*50);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	Sleep(1000);
	midiOutClose(handle);
	
	return 0;
}

