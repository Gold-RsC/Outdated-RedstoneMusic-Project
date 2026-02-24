#include<windows.h>
#include"header_file\\MidiFun.h"
#pragma comment(lib,"winmm.lib")
using namespace std;
MidiFun mid;
NOTE note;
int track_idx[128];
int track_num=1;
HMIDIOUT handle;
int main() {
	mid.init("midi\\夜航星《我的三体》.mid");
	mid.get_note(note);
	note.sort();
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	int size=note.size();
	for(int i=0; i<size-1; i++) {
		midiOutShortMsg(handle,(note[i].volume<<16)+(note[i].pitch<<8)+0x90+note[i].channel);
		if(note[i+1].tick-note[i].tick!=0)
			Sleep((note[i+1].tick-note[i].tick)*50);
	}
	midiOutShortMsg(handle,(note[size-1].volume<<16)+(note[size-1].pitch<<8)+0x90+note[size-1].channel);
	midiOutClose(handle);
	return 0;
}
