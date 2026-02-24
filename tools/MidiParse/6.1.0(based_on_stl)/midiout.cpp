#define MIDI_DEBUG
#include"MidiPlay.hpp"
#include<iostream>
using namespace GoldType;
using namespace MidiParse;
using namespace std;
string filename[10];
int n=0;
void fun1(){
	for(int i=0;i<n;++i){
	    getline(cin,filename[i]);
    }
    for(int i=0;i<n;++i){
        cout<<"This is:"<<filename[i]<<endl;
	    play(filename[i]);
    }
}
void fun2(){
    for(int i=0;i<n;++i){
	    getline(cin,filename[i]);
	    MidiFun a(filename[i]);
	    printf("format:%d\n",a.midi().format());
	    printf("tick_per_qn:%d\n",a.midi().tpqn());
	    printf("tick time mis_per_qn(6e7/bpm):\n");
	    if(a.midi().format()==2){
	        for(int i=0;i<a.midi().ntracks();++i){
	            printf("track%d\n",i);
	            auto c=a.timeMap();
	            for(auto it:c[i]){
	                printf("(%llu %llu %d)\n",it.time(),it.timeNode(),it.mispqn());
                }
            }
        }
        else if(a.midi().format()==0||a.midi().format()==1){
            printf("track0\n");
            auto c=a.timeMap();
            for(auto it:c[0]){
                printf("(%llu %llu %d)\n",it.time(),it.timeNode(),it.mispqn());
            }
        }
	    TsList b=a.tsList();
	    printf("tick n d:\n");
	    for(auto it:b){
	        printf("(%llu %d %d)\n",it.time(),it.numerator(),it.denominator());
        }
        a.change_allTimeState();
        sort(a.noteList().begin(),a.noteList().end());
        FILE*f=fopen("testwrite.txt","w");
        a.write_noteList(f,"@t\t@r\t@c\t@i\t@p\t@v\t@B\t@b");
        fclose(f);
    }
}
void fun3(){
    for(int i=0;i<n;++i){
	    getline(cin,filename[i]);
	    MidiFun a(filename[i]);
        a.change_allTimeState();
	    NotePairList b(a.noteList());
	    sort(b.begin(),b.end());
        FILE*f=fopen("write_notePairList.txt","w");
        fprintf(f,"time\tduration\ttrack\tchannel\tinstrument\tpitch\tvelocity\tbar\tbeat\n");
        for(auto it:b){
            fprintf(f,"%llu\t%llu\t%d\t%d\t%d\t%d\t%d\t%.6f\t%.6f\n",it.time(),it.duration(),it.track(),it.channel(),it.instrument(),it.pitch(),it.velocity(),it.bar(),it.beat());
        }
        fclose(f);
    }
}
void fun4(){
	for(int i=0;i<n;++i){
	    getline(cin,filename[i]);
    }
	for(int i=0;i<n;++i){
        cout<<"This is:"<<filename[i]<<endl;
        MidiFun midi(filename[i]);
        ShortMessageList s(midi);
	    s.play_if([](const ShortMessage&a){
			if((a.message()&0xF)==1){
				return true;
			}
		});
    }
}
//windmill.midÓÐÎÊÌâ 
int main(){
	printf("Input filename num:\t");
	scanf("%d",&n);
	printf("Input filenames:\n");
	cin.ignore();
	
	fun3();
	return 0;
}
