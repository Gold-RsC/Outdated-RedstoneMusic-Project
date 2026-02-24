#include"../../MidiParse/5.0.0/headfile/MidiFun.h"
Note note[30000];
int noteNum;
MidiFun mid("midi/追梦赤子心.mid");
int main(void){
	noteNum=mid.getNote(note);
//	double speed,errDegree=-1;
//	for(double v=37;v<=40;v+=0.0001){
//		double degree=0;
////		double sum=0;
////		for(int i=0;i<noteNum;i++){
////			sum+=note[i].time-note[i].time/v;
////		}
////		sum/=noteNum;
//		for(int i=0;i<noteNum;i++){
//			degree+=(note[i].time-int(note[i].time/v)*v)*(note[i].time-int(note[i].time/v)*v);
//			
//		}
////		degree+=jinum/100000.0;
//		degree/=noteNum;
//		if(degree<errDegree||errDegree<0){
//			speed=v;
//			errDegree=degree;
//		}
//	}
//	for(int i=0; i<noteNum; i++) {
//		note[i].time/=speed;
//	}
	for(int i=0; i<noteNum; i++) {
//		note[i].time/=38.3512;
//		note[i].time=note[i].time*4.0/5;
		note[i].time/=47.939;
	}

//	printf("%lf %lf",speed,errDegree);
//	for(int i=0; i<noteNum; i++) {
//		note[i].time/=42.615;
//	}
//	for(int i=0; i<noteNum; i++) {
//		note[i].time/=39;
//	}
	std::sort(note,note+noteNum,[](Note a,Note b){
		return a.time<b.time;
	});
	[]{
		FILE*w=fopen("outFile\\所有音符.txt","w");
		fprintf(w,"time\ttrack\tchannel\tbar\tpitch\tvolume\tinstrument\n");
		for(int i=0; i<noteNum; i++) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].bar,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
		}
		fclose(w);
	}();
	[]{
		bool check[128]={0};
		FILE*w=fopen("outFile\\打击音符.txt","w");
		for(int i=0; i<noteNum; i++) {
			if(note[i].channel==9&&note[i].volume!=0)
			check[note[i].pitch]=1;
		}
		for(int i=0;i<128;i++){
			if(check[i])
			fprintf(w,"%d\n",i);
		}
		fprintf(w,"time\ttrack\tchannel\tbar\tpitch\tvolume\tinstrument\n");
		for(int i=0; i<noteNum; i++) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].bar,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
		}
		fclose(w);
	}();
	return 0;
}


