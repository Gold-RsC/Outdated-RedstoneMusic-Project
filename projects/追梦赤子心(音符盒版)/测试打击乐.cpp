#include<stdio.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib")
HMIDIOUT handle;
int main(void){
	int a=0;
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	while(a>=0&&a<=127) {
		scanf("%d",&a);
		midiOutShortMsg(handle,(127<<16)+(a<<8)+0x90+9);
	}
	midiOutClose(handle);
	return 0;
}

