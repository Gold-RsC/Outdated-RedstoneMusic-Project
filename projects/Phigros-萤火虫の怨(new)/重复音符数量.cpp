#include"../../MidiParse/5.1.5/headfile/MidiFun.h"
Note note[60000];
int noteNum;
int main(void){
	MidiFun midi;
	midi.parse("midi/Phigros-Ó©»ð³æ¤ÎÔ¹.mid",0);
	noteNum=midi.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].time/=40;
	}
	int flag=noteNum;
	for(int i=0;i<=flag;i++){
		if(note[i].volume==0){
			while(note[--flag].volume==0);
			std::swap(note[i],note[flag]);
		}
	}
	for(noteNum=0;note[noteNum++].volume!=0;){}
	std::sort(note,note+noteNum,[](Note a,Note b){
		if(a.time!=b.time)
			return a.time<b.time;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.pitch<b.pitch;
	});
	FILE*w=fopen("outFile/ÖØ¸´µÄÒô·ûÊýÁ¿.txt","w");
	int memory=0,num=0;	
	for(int i=0;i<noteNum;i++){
		if(note[i].time!=memory){
			fprintf(w,"%d\t%d\n",memory,num);
			num=0;
			memory=note[i].time;
		}
		num++;
	}
	fprintf(w,"%d\t%d\n",memory,num);
	fclose(w);
	
	return 0;
}

