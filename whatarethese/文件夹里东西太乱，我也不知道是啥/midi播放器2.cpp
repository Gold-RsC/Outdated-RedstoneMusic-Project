#include<windows.h>
#include<string.h>
#include"header_file\\MidiFun.h"
#pragma conmment(lib,"winmm.lib")
MidiFun mid;
NOTE note;
int i;
int main(int arg,char*argv[]){
	mid.init("midi\\Ò¹º½ÐÇ.mid");
	mid.get_note(note);
	note.sort();
//	Sleep(note[0].tick*30);
	for(i=0;i<note.size()-1;i++){
		char a[1000]={0};
		if(note[i].volume!=0)
			sprintf(a,"play resource_packs\\%d\\%d.mp3",note[i].channel==9?-1:0,note[i].pitch);
		else
			sprintf(a,"close resource_packs\\%d\\%d.mp3",note[i].channel==9?-1:0,note[i].pitch);
			mciSendString(a,NULL,0,NULL);
		if(note[i+1].tick-note[i].tick!=0)
				Sleep((note[i+1].tick-note[i].tick)*30);
	}
	char a[1000]={0};
	if(note[i].volume!=0)
		sprintf(a,"play resource_packs\\%d\\%d.mp3",note[i].channel==9?-1:0,note[i].pitch);
	else
		sprintf(a,"close resource_packs\\%d\\%d.mp3",note[i].channel==9?-1:0,note[i].pitch);
	mciSendString(a,NULL,0,NULL);
	return 0;
}
