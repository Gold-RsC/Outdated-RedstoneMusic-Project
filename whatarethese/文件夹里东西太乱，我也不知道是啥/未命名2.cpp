#include"header_file\MidiFun.h"
#include<iostream>
using namespace std;
MidiFun mid;
NOTE note;
void music(){
	FILE*music=fopen("mcfunction\\DIP.mcfunction","w");
	for(int i=0;i<note.size();i++){
		if(note[i].velocity){
			if(note[i].track==2){
				fprintf(music,"execute @a[scores={tick=%d}]~~~playsound %dc.%d @s ^^^%lf 1 1 0 \n",note[i].tick,note[i].instrument,note[i].pitch,note[i].velocity/127.0*3);
			}
			else{
				fprintf(music,"execute @a[scores={tick=%d}]~~~playsound %dc.%d @s ^^^%lf 1 1 0 \n",note[i].tick,note[i].instrument,note[i].pitch,note[i].velocity/127.0*3-0.5);
			}
		}
		else
			fprintf(music,"execute @a[scores={tick=%d}]~~~stopsound @s %dc.%d\n",note[i].tick,note[i].instrument,note[i].pitch);
	}
	fprintf(music,"scoreboard players add @a tick 1");
	fclose(music);
}
void fall(){
	FILE*w=fopen("DIP_fall.mcfunction","w");
	for(int i=0;i<note.size();i++){
		if(note[i].velocity){
			if(note[i].track==2){
				fprintf(w,"execute @a[scores={tick=%d}]~~~setblock 28 8 %d concretepowder 1\n",note[i].tick,60+36-note[i].pitch);
			}
			else{
				fprintf(w,"execute @a[scores={tick=%d}]~~~setblock 28 8 %d concretepowder 3\n",note[i].tick,60+36-note[i].pitch);
			}
		}
		else
			fprintf(w,"execute @a[scores={tick=%d}]~~~stopsound @s %dc.%d\n",note[i].tick,note[i].instrument,note[i].pitch);
	}
	fprintf(w,"scoreboard players add @a tick 1");
	fclose(w);
}
int main() {
	mid.init("midi\\Dream It Possible.midi");
	mid.get_note(note);
	for(int i=0;i<note.size();i++)
		if(note[i].track>=5)
			note[i].track--;
//	music();
	fall();
	return 0;
}
