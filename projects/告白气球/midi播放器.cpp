#include<windows.h>
#include<iostream>
#include<string>
#include"header_file\\MidiFun.h"
#pragma comment(lib,"winmm.lib")
using namespace std;
MidiFun mid;
NOTE note;
int track_idx[128];
int track_num=1;
HMIDIOUT handle;
bool cmp(NOTEev&a,NOTEev&b){
	if(a.tick!=b.tick)
		return a.tick<b.tick;
	if(a.track!=b.track)
		return a.track<b.track;
	if(a.channel!=b.channel)
		return a.channel<b.channel;
	if(a.pitch!=b.pitch)
		return a.pitch<b.pitch;
	return 1;
}
void mergeSort(std::vector<NOTEev>&arr,int start,int end){
    if(start>=end)
        return;
    int i=start;
    int mid=(start+end)/2;
    int j=mid+1;
    mergeSort(arr,i,mid);
    mergeSort(arr,j,end);
    NOTEev*temp=(NOTEev*)malloc((end-start+1)*sizeof(NOTEev));
    int index = 0;
    while(i<=mid&&j<=end){
        if(cmp(arr[i],arr[j]))
            temp[index++]=arr[i++];
    	else
            temp[index++]=arr[j++];
    }
    while(i<=mid)
        temp[index++]=arr[i++];
    while(j<=end)
        temp[index++]=arr[j++];
    int k=start;
    for(;k<=end;k++)
        arr[k] = temp[k-start];
    free(temp);
}
int main() {
	mid.init("midi\\告白气球.mid");
	mid.get_note(note);
	mergeSort(note.getVector(),0,note.size()-1);
//	FILE*w=fopen("aaaaaaaaaaaaaaaaaachina-xaaaaaaaaaaaaaaaaaa.txt","w");
//	for(int i=0;i<note.size();i++)
//		fprintf(w,"idx:\t%u\ttick:\t%u\ttrack:\t%u\tchannel:\t%u\tpitch:\t%u\tvolume\t%u\n",i,note[i].tick,note[i].track,note[i].channel,note[i].pitch,note[i].volume);
//	fclose(w);
//	FILE*w=fopen("caonima","w");
//	for(int i=0;i<note.size();i++){
//		fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\n",i,note[i].tick,note[i].track,note[i].channel,note[i].pitch,note[i].volume);
//	}
//	fclose(w);
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	int size=note.size();
	std::string a;
	do{
		a[0]='\0';
		system("cls");
		if(note[0].tick>0)
			Sleep(note[0].tick*50);
		for(int i=0; i<size-1; i++) {
//			if(note[i].volume!=0)
//			if(note[i].channel==8||(note[i].channel==9&&(note[i].pitch==36||note[i].pitch==38))||note[i].channel==0||note[i].channel==6)
			if(note[i].channel==8||(note[i].channel==9/*&&(note[i].pitch==36||note[i].pitch==38)*/)||note[i].channel==0||note[i].channel==6)
				midiOutShortMsg(handle,((note[i].channel==9?int(note[i].volume*0.8):note[i].volume)<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
			if(note[i+1].tick-note[i].tick!=0)
				Sleep((note[i+1].tick-note[i].tick)*50);
		}
		midiOutShortMsg(handle,(note[size-1].volume<<16)+(note[size-1].pitch<<8)+0x90+note[size-1].channel);
		cout<<"再来亿遍？(yes/no)"<<endl;
		cin>>a;
	}while(a=="Yes"||a=="yes"||a=="1");
	midiOutClose(handle);
	return 0;
}
