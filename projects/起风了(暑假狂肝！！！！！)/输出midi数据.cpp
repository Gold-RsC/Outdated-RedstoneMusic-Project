#include"../../MidiParse/6.0.0/MidiFun.hpp"
#include"../../GoldType/2.0.0/Algorithm.hpp"
#include"../../GoldType/2.0.0/Pair.hpp"
using namespace MidiParse;
using namespace GoldType;
MidiFun midi("midi/Æð·çÁË.mid");
NoteList note;
uint abstick2time(uint a){
	return a*8/5;
}
void out(void){
	note=midi.get_noteList();
//	midi.change_timeStandard(note);
	sort_quick(note.begin(),note.end());
	
	FILE*pitchFile=fopen("midi/data/pitch.txt","w");
	fprintf(pitchFile,"tick\ttime\tchannel\ttrack\tpitch\tvelocity\tinstrument\n");
	for(const auto&i:note){
		fprintf(pitchFile,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i.time(),abstick2time(i.time()),i.channel(),i.track(),i.pitch(),i.velocity(),i.instrument());
	}
	fclose(pitchFile);
	
	FILE*messageFile=fopen("midi/data/message.txt","w");
	for(const auto&i:note){
		fprintf(pitchFile,"%d\t%d\n",abstick2time(i.time()),(i.velocity()<<16)|((i.pitch())<<8)|0x90|i.channel());
	}
	fclose(messageFile);
}
int main(void){
	out();
	return 0;
}
