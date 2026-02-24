#ifndef NOTE_H
#define NOTE_H
#include<algorithm>
class Note {
	public:
		long long time;
		long long tick;
		int track;
		int channel;
		int beat;
		int bar;
		int pitch;
		int volume;
		int instrument;
};
class NotePair {
	public:
		long long beginTime;
		long long endTime;
		long long beginTick;
		long long endTick;
		int track;
		int channel;
		int beat;
		int bar;
		int pitch;
		int volume;
		int instrument;
	public:
		static bool check(Note a,Note b) {
			return a.track==b.track&&a.channel==b.channel&&a.pitch==b.pitch&&a.instrument==b.instrument&&a.time<=b.time&&a.volume>=b.volume;
		}
	public:
		NotePair(void) {
			beginTime=endTime=beginTick=endTick=track=channel=beat=bar=pitch=volume=instrument=0;
		}
		NotePair(Note a,Note b) {
			if(check(a,b)) {
				beginTime=a.time;
				endTime=b.time;
				beginTick=a.tick;
				endTick=b.tick;
				track=a.track;
				channel=a.channel;
				beat=b.beat-a.beat;
				bar=b.bar-a.bar;
				pitch=a.pitch;
				volume=a.volume-b.volume;
				instrument=a.instrument;
			}
		}
		bool set(Note a,Note b){
			if(check(a,b)){
				beginTime=a.time;
				endTime=b.time;
				beginTick=a.tick;
				endTick=b.tick;
				track=a.track;
				channel=a.channel;
				beat=b.beat-a.beat;
				bar=b.bar-a.bar;
				pitch=a.pitch;
				volume=a.volume-b.volume;
				instrument=a.instrument;
				return 1;
			}
			return 0;
		}
};
int change(Note*note,NotePair*notepair,int num){
	int end=-1;
	for(int i=0;i<num;i++){
		if(!note[i].volume){
			end=i;
			break;
		}
	}
	if(end==-1){
		for(int i=0;i<num;i++){
			notepair[i].set(note[i],note[i]);
		}
		return num;
	}
	bool*flag=new bool[num+1]{0};
	int pairnum=0;
	for(int i=0;i<num;i++){
		if(note[i].volume){
			for(int j=end;j<num;j++){
				if((!note[j].volume)&&NotePair::check(note[i],note[j])){
					notepair[pairnum++].set(note[i],note[j]);
					flag[j]=1;
					flag[i]=1;
					while(flag[end]||(note[end].volume)){
						end++;
					}
					break;
				}
			}
		}
	}
	std::sort(notepair,notepair+pairnum,[](NotePair&a,NotePair&b){
		return a.beginTime<b.beginTime;
	});
	return pairnum;
}
#endif

