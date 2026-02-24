#include<bits/stdc++.h>
#include"headfile\notepoint.h"
using namespace std;
using namespace notepoint;
int main()
{
	
	getnote("E:\\程序文件\\程序文件\\gev\\杜鹃圆舞曲.gev");
	FILE*music=fopen("mcfunction\\dujuan.mcfunction","w");
	for(int i=1;i<=noteidx;i++)
	{
		if(note[i].velocity==0)continue;
		if(note[i].channel==9)continue;
//		if(note[i].tick==0)cout<<note[i].channel<<endl;
		if(note[i].channel==5)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ^^1^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
		else if(note[i].channel==4||note[i].channel==3)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ^-6^-1^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
			fprintf(music,"execute @a[scores={tick=%d}] ^6^-1^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
		else if(note[i].channel==11||note[i].channel==12||note[i].channel==6)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ^5^5^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
			fprintf(music,"execute @a[scores={tick=%d}] ^-5^5^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
		else if(note[i].channel==0||note[i].channel==1||note[i].channel==2)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ^5^2^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
			fprintf(music,"execute @a[scores={tick=%d}] ^-5^2^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
		else if(note[i].channel==7||note[i].channel==8)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ^-8^2^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
			fprintf(music,"execute @a[scores={tick=%d}] ^8^2^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
		else
		{
			fprintf(music,"execute @a[scores={tick=%d}] ^^7^ playsound note.harp @s ~~~ %.2lf %.2lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
	}
	fprintf(music,"scoreboard players add @a tick 1");
	fclose(music);
	return 0;
}
