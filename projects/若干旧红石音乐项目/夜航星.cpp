#include<bits/stdc++.h>
#include"headfile\notepoint.h"
using namespace std;
using namespace notepoint;
int main()
{
	getnote("gev\\Ò¹º½ÐÇ.gev");
	FILE*w=fopen("mcfunction\\yhx.mcfunction","w");
	
	for(int i=1;i<=noteidx;i++)
	{
		if(note[i].velocity==0)continue;
		fprintf(w,"execute @a[scores={tick=%d}] ~~~ setblock 28 8 %d redstone_block\n",note[i].tick,96-note[i].pitch);
	}
	
	fclose(w);
	return 0;
}
