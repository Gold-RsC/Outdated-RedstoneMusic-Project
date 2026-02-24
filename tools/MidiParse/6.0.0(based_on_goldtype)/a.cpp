//#define WRITE_MIDI_ERROR
#include"MidiFun.hpp"
#include"../../GoldType/2.0.0/Algorithm.hpp"
#include<windows.h>
#include<algorithm>
#pragma comment(lib,"winmm.lib")
using namespace MidiParse;
HMIDIOUT handle;
void t1(){
//	midierror("error.txt");
	MidiFile a;
	FILE*w=fopen("a.txt","w");
	a.read("起风了.mid");
	for(int trackIdx=0;trackIdx<a.ntracks();++trackIdx){
		fprintf(w,"track:%.2X\n",trackIdx);
		fprintf(w,"time\tdata\n");
		for(int eventIdx=0;eventIdx<a[trackIdx].size();++eventIdx){
			fprintf(w,"%llu\t",a[trackIdx][eventIdx].time());
			for(int i=0;i<a[trackIdx][eventIdx].message().size();++i){
				fprintf(w,"%.2X ",a[trackIdx][eventIdx][i]);
			}
			fprintf(w,"\n");
		}
		fprintf(w,"----------------------------\n");
	}
	fclose(w);
}
void t2(){
//	midierror("error.txt");
	MidiFun a("起风了.mid");
	NoteList b=a.get_noteList();
	a.change_timeStandard(b);
	FILE*w=fopen("b.txt","w");
	for(auto&i:b){
		fprintf(w,"%llu\t%d\t%d\t%d\t%d\t%d\n",i.time(),i.track(),i.channel(),i.pitch(),i.velocity(),i.instrument());
	}
	fclose(w);
}
void t3(){
//	midierror("error.txt");
	MidiFun a("See you again.mid");
	LyricList b=a.get_lyricList();
	FILE*w=fopen("c.txt","w");
	for(auto&i:b){
		fprintf(w,"%llu\t%d\t%d\t%s\n",i.time(),i.track(),i.channel(),i.text().c_str());
	}
	fclose(w);
}
void t4(){
//	midierror("error.txt");
	MidiFun a("AIR 鸟之诗（完美版）.mid");
	TempoList b=a.get_tempoList();
	FILE*w=fopen("d.txt","w");
	for(auto&i:b){
		fprintf(w,"%llu\t%d\t%d\t%llu\n",i.time(),i.track(),i.mispqn(),i.timeBase());
	}
	fclose(w);
	system("d.txt");
}
void t5(){
//	midierror("error.txt");
	MidiFun a("春节序曲.mid");
	TsList b=a.get_tsList();
	FILE*w=fopen("e.txt","w");
	for(auto&i:b){
		fprintf(w,"%llu\t%d\t%d\t%d\t%d\t%d\n",i.time(),i.track(),i.numerator(),i.denominator(),i.tickPerMidiclock(),i.num32ndNotePer24Midiclock());
	}
	fclose(w);
	system("e.txt");
}
void t6(){
//	midierror("error.txt");
	MidiFun a("春节序曲.mid");
	NoteList note=a.get_noteList();
	a.change_timeStandard(note);
//	FILE*w=fopen("b.txt","w");
//	for(auto&i:b){
//		fprintf(w,"%llu\t%d\t%d\t%d\t%d\t%d\n",i.time(),i.track(),i.channel(),i.pitch(),i.velocity(),i.instrument());
//	}
	GoldType::sort_quick(note.begin(),note.end(),[](Note a,Note b)->bool{
		if(a.time()!=b.time()){
			return a.time()<b.time();
		}
		return a.track()<b.track();
	});
	printf("%d\n",GoldType::is_sorted(note.begin(),note.end(),[](Note a,Note b)->bool{
		if(a.time()!=b.time()){
			return a.time()<b.time();
		}
		return a.track()<b.track();
	}));
	
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	for(int i=0; i<note.size()-1; i++) {
		midiOutShortMsg(handle,(note[i].velocity()<<16)|((note[i].pitch())<<8)|0x90|note[i].channel());
		if((note[i+1].time()-note[i].time())/1000)
			Sleep((note[i+1].time()-note[i].time())/1000);
	}
	midiOutShortMsg(handle,(note[note.size()-1].velocity()<<16)|(note[note.size()-1].pitch()<<8)|0x90|note[note.size()-1].channel());
	Sleep(5000);
	midiOutClose(handle);
//	fclose(w);
}
int main(){
	t6();
	return 0;
}
