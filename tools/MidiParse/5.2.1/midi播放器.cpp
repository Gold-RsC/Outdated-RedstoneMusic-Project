#include"headfile/MidiFun.h"
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
Note note[60000];
int noteNum;
HMIDIOUT handle;
int main(void){
	char filename[100];
	scanf("%[^\n]",filename);
	MidiFun mid(filename);
	noteNum=mid.getNote(note);
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].time);
	for(int i=0; i<noteNum-1; i++) {
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].time-note[i].time!=0)
			Sleep(note[i+1].time-note[i].time);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	midiOutClose(handle);
	return 0;
}

