#include"header_file\\MidiFun.h"
#include<stdio.h>
#include<iostream>
#pragma comment(lib,"winmm.lib")
using namespace std;
int main(void){
	MidiFun mid("midi\\China-X.mid");
	return 0;
	mid.tick2ms();
	Note note[10000];
	int size=note<<mid;
	sort(note,note+size,[](Note a,Note b){
		return a.tick<=b.tick;
	});
//	for(int i=0;i<size;i++)
//		printf("%d %d\n",i,note[i].tick);
	HMIDIOUT handle;
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	if(note[0].tick>0)
		Sleep(note[0].tick*50);
	for(int i=0; i<size-1; i++) {
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		printf("%d %d %d %d\n",note[i].tick,note[i].pitch,note[i].volume,note[i].channel);
		if(note[i+1].tick-note[i].tick!=0)
			Sleep((note[i+1].tick-note[i].tick)*50);
	}
	midiOutShortMsg(handle,(note[size-1].volume<<16)+(note[size-1].pitch<<8)+0x90+note[size-1].channel);
	midiOutClose(handle);
	return 0;
}
