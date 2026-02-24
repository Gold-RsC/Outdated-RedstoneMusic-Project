#include"header_file\MidiFun.h"

MidiFun mid;
NOTE note;
FILE*line,*music;
void init(){
	mid.init("midi\\ÌøÎèµÄÏß-³óÐ¡Ñ¼.mid");
	mid.get_note(note);
	line=fopen("mcfunction\\line_UglyDuckling.mcfunction","w");
	music=fopen("mcfunction\\music_UglyDuckling.mcfunction","w");
}
void makemusic(){
	int size=note.size();
	for(int i=0;i<size;i++){
		if(note[i].velocity)
			fprintf(music,"execute @a[scores={tick=%d}] ~~~ playsound %dc.%d @s ~~~ %lf 1 0\n",note[i].tick,0,note[i].pitch,note[i].velocity/127.0-0.2);
		else
			fprintf(music,"execute @a[scores={tick=%d}] ~~~ stopsound @s %dc.%d\n",note[i].tick,0,note[i].pitch);
	}
	fprintf(music,"scoreboard players add @a tick 1");
}
void makeline(){
	bool flag[3000]={0};
	bool direction=0;
	int size=note.size();
	for(int i=0;i<size;i++)
		if(note[i].track==1&&note[i].velocity){
			if(flag[note[i].tick]==1)
				continue;
			flag[note[i].tick]=1;
			int j;
			bool f=0;
			for(j=note[i].tick-1;j>=0;j--)
				if(flag[j]==1){
					f=1;
					break;
				}
			if(f==0)
				j=-10;
			fprintf(line,"execute @a[scores={tick=%d..%d}] ~~~ execute @e[type=armor_stand,tag=line] ~~~ tp @s %s\n",j,note[i].tick,direction?"~~~0.2":"~0.2~~");
			direction=!direction;
		}
}
void end(){
	fclose(line);
	fclose(music);
	FILE*end=fopen("mcfunction\\UglyDuckling.mcfunction","w");
	fprintf(end,"function line_UglyDuckling\nfunction music_UglyDuckling");
	fclose(end);
}
int main(){
	init();
	makemusic();
	makeline();
	end();
	return 0;
}
