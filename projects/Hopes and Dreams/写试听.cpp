#include"../../MidiParse/6.1.0(based_on_stl)/MidiPlay.hpp"
using namespace GoldType;
using namespace MidiParse;
using namespace std;

void fun0(){
    NotePairList n;
    FILE*r=fopen("noteList_mctick.txt","r");
    for(int i=0;i<6307;++i){
        ull tick,duration;
        ushort track,channel,instrument,pitch,velocity;
        fscanf(r,"%llu %llu %hu %hu %hu %hu %hu",&tick,&duration,&track,&channel,&instrument,&pitch,&velocity);
        n.emplace_back(tick,duration,track,channel,pitch,velocity,instrument);
    }
    fclose(r);
    
    for(int i=0;i<6307;++i){
        n[i].time()*=50000/4*3.5;
        n[i].duration()*=50000/4*3.5;
    }
    ShortMessageList l(n);
    l.sort();
    l.write("ÊÔÌý.cpp");
}
int main(void){
    fun0();
    return 0;
}
