#include"header_file\\MidiFile.h"
int main(){
	BPM bpm;
	MidiFile mid("midi\\China-X.mid");
	mid.get_bpm(bpm);
	for(int i=0;i<bpm.size();i++)
	printf("%d %d\n",bpm[i].tick,bpm[i].value);
	return 0;
}
