#include"headfile/MidiFun.h"
#include<windows.h>
#pragma comment(lib,"winmm.lib")
Note note[60000];
int noteNum;
HMIDIOUT handle;
int main(void){
	char filename[100];
	scanf("%[^\n]",filename);
	MidiFun mid(filename);
	noteNum=mid.getNote(note);
	double speed,errDegree=-1;
	for(double v=47;v<=53;v+=0.02){
		double degree=0;
		double sum=0;
		for(int i=0;i<noteNum;i++){
			sum+=note[i].time-note[i].time/v;
		}
		sum/=noteNum;
		for(int i=0;i<noteNum;i++){
			degree+=(note[i].time-int(note[i].time/v)*v)*(note[i].time-int(note[i].time/v)*v);
		}
		degree/=noteNum;
		if(degree<errDegree||errDegree<0){
			speed=v;
			errDegree=degree;
		}
	}
	printf("speed:%lf degree:%lf\n",speed,errDegree);
	for(int i=0; i<noteNum; i++) {
		note[i].time/=speed;
	}
	std::sort(note,note+noteNum,[](Note a,Note b){
		return a.time<b.time;
	});
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

