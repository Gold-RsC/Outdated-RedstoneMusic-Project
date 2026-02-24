#include"../../MidiParse/5.1.5/headfile/MidiFun.h"
#include<windows.h>
#pragma comment(lib,"winmm.lib")
Note note[60000];
int noteNum;
HMIDIOUT handle;
int main(void){
	int begin,end;
	scanf("%d%d",&begin,&end);
	MidiFun mid;
	mid.parse("midi/Phigros-Ó©»ð³æ¤ÎÔ¹.mid",0);
	noteNum=mid.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].time/=40;
		if(note[i].volume==0){
			note[i].time+=12;
		}
	}
	
//	[]{
//		FILE*w=fopen("outFile/allPitch.txt","w");
//		fprintf(w,"time	track	channel	pitch	volume	instrument\n");
//		for(int i=0;i<noteNum;i++){
//			if(note[i].volume!=0)
//			fprintf(w,"%d	%d	%d	%d	%d	%d\n",note[i].time,note[i].track,note[i].channel,note[i].pitch,note[i].volume,note[i].instrument);
//		}
//		fclose(w);
//	}();

//	int flag=noteNum;
//	for(int i=0;i<=flag;i++){
//		if(note[i].volume==0){
//			while(note[--flag].volume==0);
//			std::swap(note[i],note[flag]);
//		}
//	}
//	for(noteNum=0;note[noteNum++].volume!=0;){}

	std::sort(note,note+noteNum,[](Note a,Note b){
		if(a.time!=b.time)
			return a.time<b.time;
		if(a.volume!=b.volume)
			return a.volume<b.volume;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.pitch<b.pitch;
	});
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].time*50);
	for(int i=0; i<noteNum-1; i++) {
		if(note[i].time<begin)
			continue;
		if(note[i].time>=end)
			break;
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].time-note[i].time!=0)
			Sleep((note[i+1].time-note[i].time)*50);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	Sleep(5000);
	midiOutClose(handle);
	return 0;
}

