#include"headfile\\MidiFile.h"
#include<windows.h>
#pragma comment(lib,"winmm.lib")
struct Note {
	long long time;
	int pitch;
	int velocity;
	int channel;
} note[60000];
int main() {
	MidiFile a;
	char filename[1000];
	scanf("%[^\n]",filename);
	a.read(filename);
	a.deltaTick2absTick();
	a.tick2time();
	int noteIdx=0;
	for(int track=0; track<a.getNTrcks(); track++) {
		for(int idx=0; idx<a[track].size(); idx++) {
			if(a[track][idx][0]>=0x80&&a[track][idx][0]<0xA0) {
				note[noteIdx].time=a[track][idx].getTime();
				note[noteIdx].channel=a[track][idx][0]%16;
				note[noteIdx].pitch=a[track][idx][1];
				note[noteIdx++].velocity=a[track][idx][2];
			}
		}
	}
	sort(note,note+noteIdx,[](Note&aa,Note&bb) {
		return aa.time<bb.time;
	});
	HMIDIOUT handle;
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].time);
	for(int i=0; i<noteIdx-1; i++) {
		midiOutShortMsg(handle,(note[i].velocity<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].time-note[i].time!=0)
			Sleep((note[i+1].time-note[i].time));
	}
	midiOutShortMsg(handle,(note[noteIdx-1].velocity<<16)+(note[noteIdx-1].pitch<<8)+0x90+note[noteIdx-1].channel);
	midiOutClose(handle);
	return 0;
}
