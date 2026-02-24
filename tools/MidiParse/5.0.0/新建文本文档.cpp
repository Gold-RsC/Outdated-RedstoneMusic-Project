#include"headfile/MidiFun.h"
using namespace std;
int main(){
    MidiFun a("midi/Faded.mid");
    Note arr[10000];
    int n=a.getNote(arr);
    sort(arr,arr+n,[](const Note&a,const Note&b){
        return a.time<b.time;
    });
    FILE*f=fopen("agadsfhikhasuod.txt","w");
    fprintf(f,"time\ttrack\tbar\tbeat\n");
    
    for(int i=0;i<n;++i){
        fprintf(f,"%llu\t%d\t%d\t%d\n",arr[i].time,arr[i].track,arr[i].bar,arr[i].beat);
    }
    fclose(f);
    return 0;
}
