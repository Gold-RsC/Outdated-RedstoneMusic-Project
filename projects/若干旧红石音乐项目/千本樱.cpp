#include"header_file\\NotePoint.h"
int main(){
	FILE*w=fopen("mcfunction\\qby.mcfunction","w");
	MidiFun m;
	Point note[10000];
	PBPM bpm[100];
	m.init("midi\\Ç§±¾Ó£.mid",note,bpm);
	m.getev();
	int num=m.get_note();
	for(int i=0;i<num;i++){
		if(!note[i].type)continue;
		fprintf(w,"execute @a[scores={tick=%d}] ~~~ playsound note.harp @s ~~~ %lf %lf 0\n",note[i].gt,note[i].velocity/127.0,pow(2.0,(note[i].pitch-66)/12.0));
	}
	fclose(w);
	return 0;
}
