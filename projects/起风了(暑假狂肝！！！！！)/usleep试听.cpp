#include"../../MidiParse/6.0.0/MidiFun.hpp"
#include"../../GoldType/2.0.0/Algorithm.hpp"
#include"../../GoldType/2.0.0/Pair.hpp"
#include<windows.h>
#include<unistd.h>
#pragma comment(lib,"winmm.lib")
using namespace MidiParse;
using namespace GoldType;
HMIDIOUT handle;
List<Pair<uint,ull>> message;
void fun(){
	ushort instrument[16];
	for(int i=0;i<16;++i){
		instrument[i]=0;
	}
	MidiFun midi("midi/起风了.mid");
	NoteList note=midi.get_noteList();
	midi.change_timeStandard(note);
	sort_quick(note.begin(),note.end());
	ull lastTime=0;
	for_each(note.begin(),note.end(),[&](Note&a){
		a.time()-=lastTime;
		if(a.instrument()==39){
			a.instrument()=1;
			if(a.velocity()){
				a.velocity()+=10;
			}
		}
		lastTime+=a.time();
		a.time()/=2;
	});
	for(int i=0; i<note.size(); ++i) {
		if(note[i].instrument()!=instrument[note[i].channel()]){
			instrument[note[i].channel()]=note[i].instrument();
			message.emplace_back(note[i].instrument()<<8|0xC0|note[i].channel(),0);
		}
		message.emplace_back((note[i].velocity()<<16)|((note[i].pitch())<<8)|0x90|note[i].channel(),note[i].time()*2);
	}
}
void write(){
	FILE*w=fopen("最终试听.cpp","w");
	fprintf(w,
	"#include<windows.h>\n"
	"#include<unistd.h>\n"
	"#pragma comment(lib,\"winmm.lib\")\n"
	"HMIDIOUT handle;\n"
	"int main(void){\n"
	"\tmidiOutOpen(&handle,0,0,0,CALLBACK_NULL);\n"
	);
	for(const auto&a:message) {
		if(a.second())
			fprintf(w,"\tusleep(%llu);\n",a.second());
		fprintf(w,"\tmidiOutShortMsg(handle,%d);\n",a.first());
	}
	fprintf(w,
	"\tusleep(5000000);\n"
	"\tmidiOutClose(handle);\n"
	"\treturn 0;\n"
	"}"
	);
	fclose(w);
}
int main(){
	fun();
	write();
	return 0;
}
