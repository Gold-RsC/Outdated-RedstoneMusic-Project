#include"../../MidiParse/6.1.0(based_on_stl)/MidiFun.hpp"
#include<algorithm>
using namespace GoldType;
using namespace MidiParse;
using namespace std;

void fun0(){
    MidiFun mf("HD.mid");
    
    NotePairList a(mf.noteList());
    sort(a.begin(),a.end());
    FILE*w1=fopen("noteList_tick.txt","w");
    fprintf(w1,"time\tduration\ttrack\tchannel\tinstrument\tpitch\tvelocity\tbar\tbeat\n");
    for(auto it:a){
        fprintf(w1,"%llu\t%llu\t%d\t%d\t%d\t%d\t%d\t%.6f\t%.6f\n",it.time(),it.duration(),it.track(),it.channel(),it.instrument(),it.pitch(),it.velocity(),it.bar(),it.beat());
    }
    fclose(w1);
    
    mf.change_allTimeState();
    
    NotePairList b(mf.noteList());
    sort(b.begin(),b.end());
    FILE*w2=fopen("noteList_mis.txt","w");
    fprintf(w2,"time\tduration\ttrack\tchannel\tinstrument\tpitch\tvelocity\tbar\tbeat\n");
    for(auto it:b){
        fprintf(w2,"%llu\t%llu\t%d\t%d\t%d\t%d\t%d\t%.6f\t%.6f\n",it.time(),it.duration(),it.track(),it.channel(),it.instrument(),it.pitch(),it.velocity(),it.bar(),it.beat());
    }
    fclose(w2);
}
void fun1(){
    NotePairList n;
    FILE*r=fopen("noteList_mix.txt","r");
    for(int i=0;i<6307;++i){
        ull tick,duration;
        ushort track,channel,instrument,pitch,velocity;
        fscanf(r,"%llu %llu %hu %hu %hu %hu %hu",&tick,&duration,&track,&channel,&instrument,&pitch,&velocity);
        n.emplace_back(tick,duration,track,channel,pitch,velocity,instrument);
    }
    fclose(r);
    //À©´óÊ±¼ä²î 
    bool flag=0;
    ull time=0;
    ull t[6307]={0};
    for(int i=1;i<6307;++i){
        if(flag){
            n[i].duration()*=2;
            time+=(n[i].time()-n[i-1].time())*2;
        }
        else{
            time+=(n[i].time()-n[i-1].time());
        }
        t[i]=time;
        if(n[i].time()!=n[i-1].time()){
            switch(n[i].time()){
                case 128000:
                case 160000:
                case 352000:
                case 368000:
                case 432000:
                case 440000:{
                    flag=!flag;
                    break;
                }
            }
        }
    }
    for(int i=0;i<6307;++i){
        n[i].time()=t[i];
    }
    FILE*w=fopen("noteList_extend.txt","w");
//    fprintf(w,"tick\tduration\ttrack\tchannel\tinstrument\tpitch\tvelocity\n");
    for(auto it:n){
        fprintf(w,"%llu\t%llu\t%hu\t%hu\t%hu\t%hu\t%hu\n",it.time(),it.duration(),it.track(),it.channel(),it.instrument(),it.pitch(),it.velocity());
    }
    fclose(w);
    
}
void fun2(){
    NotePairList n;
    FILE*r=fopen("noteList_extend.txt","r");
    for(int i=0;i<6307;++i){
        ull tick,duration;
        ushort track,channel,instrument,pitch,velocity;
        fscanf(r,"%llu %llu %hu %hu %hu %hu %hu",&tick,&duration,&track,&channel,&instrument,&pitch,&velocity);
        n.emplace_back(tick,duration,track,channel,pitch,velocity,instrument);
    }
    fclose(r);
    for(int i=0;i<6307;++i){
        n[i].time()=n[i].time()*0.008;
        n[i].duration()=n[i].duration()*0.008;
    }
    FILE*w=fopen("noteList_mctick.txt","w");
//    fprintf(w,"tick\tduration\ttrack\tchannel\tinstrument\tpitch\tvelocity\n");
    for(auto it:n){
        fprintf(w,"%llu\t%llu\t%hu\t%hu\t%hu\t%hu\t%hu\n",it.time(),it.duration(),it.track(),it.channel(),it.instrument(),it.pitch(),it.velocity());
    }
    fclose(w);
}
int main(void){
    fun2();
    return 0;
}
