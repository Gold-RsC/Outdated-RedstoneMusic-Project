#include"header_file\\MidiFun.h"
using namespace std;
int main()
{
	MidiFun mid("midi\\¹´Ö¸ÆðÊÄ.mid");
	NOTE note;
	mid.get_note(note);
	FILE*w=fopen("functions\\gzqs.mcfunction","w");
	for(int i=0;i<note.size();i++){
		note[i].tick=note[i].tick*7.0/5.0;
	}
	for(int i=0;i<note.size();i++)
	{
		if(note[i].volume==0)continue;
		fprintf(w,"execute @a[scores={tick=%d}] ~~~ setblock 28 8 %d concretepowder %d\n",note[i].tick,96-note[i].pitch,note[i].track&1?3:6);
	}
	
	fclose(w);
	return 0;
}
