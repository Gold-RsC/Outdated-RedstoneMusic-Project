#include"NotePoint.h"
int main(){
	Point p[50000];
	PBPM bpm[10000];
	MidiFun mid;
	mid.init("midifile\\´º½ÚÐòÇú-Ver.1.mid",p,bpm);
	mid.getev(30000,1000);
	for(int i=0;i<mid.note_num();i++){
		printf("%d %d\n",p[i].gt,p[i].pitch);
	}
	return 0;
}
