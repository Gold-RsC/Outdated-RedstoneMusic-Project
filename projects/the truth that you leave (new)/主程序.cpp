#include<stdio.h>
#include"../../MidiParse/5.2.1/headfile/MidiFun.h"
Note note[5000];
int noteNum;
void init(){
	noteNum=MidiFun("midi/Thetruththatyouleave.mid").getNote(note);
	for(int i=0;i<noteNum;i++){
		note[i].time/=48;
	}
}
void block(){
	
}
void end(){
	
}
int main(){
	init();
	block();
	end();
	return 0;
}
