#include<windows.h>
#include<string.h>
#include<string>
#include"header_file\\MidiFun.h"
#pragma conmment(lib,"winmm.lib")
MidiFun mid;
NOTE note;
int i;
using namespace std;
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
struct _cmd{
	string str;
	long long tick;
}cmd[50000];
int main(int arg,char*argv[]){
	mid.init("midi\\Dream It Possible.midi");
	mid.get_note(note);
	mergeSort(note.getVector(),0,note.size()-1);
	for(i=0;i<note.size();i++){
		char a[1000]={0};
		if(note[i].volume!=0)
			sprintf(a,"resource_packs\\%d\\%d.wav",note[i].channel==9?-1:0,note[i].pitch);
		else
			sprintf(a,"",note[i].channel==9?-1:0,note[i].pitch);
		cmd[i].str=a;
		if(note.size()!=i)
			cmd[i].tick=(note[i+1].tick-note[i].tick)*50;
	}
	for(i=0;i<note.size()-1;i++){
//		mciSendString(cmd[i].str.c_str(),NULL,0,NULL);
		if(cmd[i].str.length()){
			PlaySound(cmd[i].str.c_str(),NULL,SND_FILENAME|SND_ASYNC);
		}
		if(cmd[i].tick!=0)
			Sleep(cmd[i].tick);
	}
	mciSendString(cmd[i].str.c_str(),NULL,0,NULL);
	return 0;
}
