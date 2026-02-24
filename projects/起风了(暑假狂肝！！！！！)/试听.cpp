#include"../../MidiParse/6.0.0/MidiFun.hpp"
#include"../../GoldType/2.0.0/Algorithm.hpp"
#include"../../GoldType/2.0.0/Pair.hpp"
#include<windows.h>
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
	MidiFun midi("midi/Æð·çÁË.mid");
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
		a.time()/=1000;
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
int main(){
	fun();
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	for(const auto&a:message) {
		if(a.second())
			Sleep(a.second());
		midiOutShortMsg(handle,a.first());
	}
	Sleep(5000);
	midiOutClose(handle);
	return 0;
}
