#include"header_file\\noteline.h"
#include<math.h>
using namespace std;
using namespace notepoint;
int main(){
	getnote("gev\\—©÷Æ√Œ.gev");
	FILE*w=fopen("mcfunction\\snow.mcfunction","w");
	int n=0;
	for(int i=1;i<=noteidx;i++){
		if(note[i].velocity==0)continue;
		fprintf(w,"execute @a[scores={tick=%d}] ~~~ playsound 1c.%d @s ~~~ %lf 1 0\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
		n+=10;
		n%=360;
	}
	fclose(w);
	return 0;
}
