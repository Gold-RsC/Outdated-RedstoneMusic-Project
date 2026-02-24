#include<bits/stdc++.h>
#include"headfile\notepoint.h"
using namespace std;
using namespace notepoint;
int main()
{
	getnote("gev\\´º½ÚÐòÇú.gev");
	FILE*w=fopen("cjxq.mcfunction","w");
	int mm=0;
	for(int i=1;i<=noteidx;i++)
	{
		if(note[i].velocity==0)continue;
		if(note[i].channel==9)continue;
		mm=max(mm,note[i].tick-note[i-1].tick);
		fprintf(w,"execute @a[scores={tick=%d}] ~~~ playsound note.harp @s ~~~ %lf %lf\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
	}
	cout<<mm;
	fclose(w);
	return 0;
}
