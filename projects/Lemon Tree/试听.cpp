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
	MidiFun midi("midi/LemonTree.mid");
	NoteList note=midi.get_noteList();
//	midi.change_timeStandard(note);
	sort_quick(note.begin(),note.end());
	for_each(note.begin(),note.end(),[](Note&a){
//		a.time()/=1000;
		if(a.channel()==2||a.channel()==8){
			a.velocity()/=1.2;
		}
		else if(a.channel()==3){
			a.velocity()+=30;
		}
	});
	int i=0;
	for(; i<note.size()-1; ++i) {
		if(note[i].instrument()!=instrument[note[i].channel()]){
			instrument[note[i].channel()]=note[i].instrument();
			if(note[i].channel()==3){
				message.emplace_back(1<<8|0xC0|note[i].channel(),0);
			}
			else{
				message.emplace_back(note[i].instrument()<<8|0xC0|note[i].channel(),0);
			}
		}
		message.emplace_back((note[i].velocity()<<16)|((note[i].pitch())<<8)|0x90|note[i].channel(),(note[i+1].time()-note[i].time())/*/1000*/);
	}
	if(note[i].instrument()!=instrument[note[i].channel()]){
		instrument[note[i].channel()]=note[i].instrument();
		if(note[i].channel()==3){
			message.emplace_back(1<<8|0xC0|note[i].channel(),0);
		}
		else{
			message.emplace_back(note[i].instrument()<<8|0xC0|note[i].channel(),0);
		}
	}
	message.emplace_back((note[note.size()-1].velocity()<<16)|(note[note.size()-1].pitch()<<8)|0x90|note[note.size()-1].channel(),5000);
}
int main(){
	fun();
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	for(const auto&a:message) {
		midiOutShortMsg(handle,a.first());
		if(a.second())
			Sleep(a.second());
	}
	midiOutClose(handle);
	return 0;
}
