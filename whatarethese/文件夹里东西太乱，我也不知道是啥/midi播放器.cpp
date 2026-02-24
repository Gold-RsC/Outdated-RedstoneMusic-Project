#include<windows.h>
#include<iostream>
#include<string>
#include"header_file\\MidiFun.h"
#pragma comment(lib,"winmm.lib")
using namespace std;
MidiFun mid;
NOTE note;
int track_idx[128];
int track_num=1;
HMIDIOUT handle;
int main() {
	mid.init("midi\\春节序曲.mid");
	mid.get_note(note);
	return 0;
	note.sort();
//	FILE*w=fopen("aaaaaaaaaaaaaaaaaachina-xaaaaaaaaaaaaaaaaaa.txt","w");
//	for(int i=0;i<note.size();i++)
//		fprintf(w,"idx:\t%u\ttick:\t%u\ttrack:\t%u\tchannel:\t%u\tpitch:\t%u\tvolume\t%u\n",i,note[i].tick,note[i].track,note[i].channel,note[i].pitch,note[i].volume);
//	fclose(w);
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	int size=note.size();
	std::string a;
	do{
		a[0]='\0';
		system("cls");
		if(note[0].tick>0)
			Sleep(note[0].tick*50);
		for(int i=0; i<size-1; i++) {
//			if(note[i].channel==9){
				midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
//				if(note[i].volume)
//					printf("%d\n",note[i].pitch);
//			}
			if(note[i+1].tick-note[i].tick!=0)
				Sleep((note[i+1].tick-note[i].tick)*50);
		}
		midiOutShortMsg(handle,(note[size-1].volume<<16)+(note[size-1].pitch<<8)+0x90+note[size-1].channel);
		cout<<"再来亿遍？(yes/no)"<<endl;
		cin>>a;
	}while(a=="Yes"||a=="yes"||a=="1");
	midiOutClose(handle);
	return 0;
}
