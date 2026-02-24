#include"MidiFun.hpp"
#include"../../GoldType/2.0.0/Algorithm.hpp"
#include<windows.h>
#include<unistd.h>
#pragma comment(lib,"winmm.lib")
using namespace MidiParse;
using namespace GoldType;
HMIDIOUT handle;
ushort instrument[16];
void fun(){
	MidiFun a("Æð·çÁË.mid");
	NoteList note=a.get_noteList();
	a.change_timeStandard(note);
	sort_quick(note.begin(),note.end());
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	
	for(int i=0; i<note.size()-1; i++) {
		if(note[i].instrument()!=instrument[note[i].channel()]){
			instrument[note[i].channel()]=note[i].instrument();
			midiOutShortMsg(handle,note[i].instrument()<<8|0xC0|note[i].channel());
		}
		midiOutShortMsg(handle,(note[i].velocity()<<16)|((note[i].pitch())<<8)|0x90|note[i].channel());
		if(note[i+1].time()-note[i].time())
			usleep(note[i+1].time()-note[i].time());
	}
	midiOutShortMsg(handle,(note[note.size()-1].velocity()<<16)|(note[note.size()-1].pitch()<<8)|0x90|note[note.size()-1].channel());
	usleep(5000000);
	midiOutClose(handle);
}
int main(){
	fun();
	return 0;
}
