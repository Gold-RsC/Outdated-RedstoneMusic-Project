std::pair<int,MidiEvent> allEvent[evnum];
int initidx=0;
int noteIdx=0;
for(int track=0; track<nTrcks; track++) {
	for(int idx=0; idx<trackEvent[track].size(); idx++) {
		allEvent[initidx++]=std::pair<int,MidiEvent>(track,trackEvent[track][idx]);
	}
}
std::sort(allEvent,allEvent+evnum,[](std::pair<int,MidiEvent>&a,std::pair<int,MidiEvent>&b) {
	if(a.second.getTick()!=b.second.getTick())
		return a.second.getTick()<b.second.getTick();
	return a.first<b.first;
});
double beatPerMinute=120.0;
double timePerTick=120.0;
double ts_nnn=4;
double ts_ddd=4;
int instruments[16];
double lastBeat=0;
double lastBar=0;
memset(instruments,-1,sizeof(instruments));
for(int idx=0; idx<evnum; idx++) {
	allEvent[idx].second.setTime((idx==0?0:allEvent[idx-1].second.getTime()+(allEvent[idx].second.getTick()-allEvent[idx-1].second.getTick())/1000.0*timePerTick));
	if(allEvent[idx].second[0]==0xFF&&allEvent[idx].second[1]==0x51) {
		beatPerMinute=6e7/((allEvent[idx].second[3]<<16)+(allEvent[idx].second[4]<<8)+allEvent[idx].second[5]);
		timePerTick=((allEvent[idx].second[3]<<16)+(allEvent[idx].second[4]<<8)+allEvent[idx].second[5])*1.0/tpqn;
	} else if(allEvent[idx].second[0]==0xFF&allEvent[idx].second[1]==0x58) {
		ts_nnn=allEvent[idx].second[3];
		ts_ddd=pow(2,allEvent[idx].second[4]);
	} else if(allEvent[idx].second[0]&0xF0==0xC0) {
		instruments[allEvent[idx].second[0]&0xF]=allEvent[idx].second[1];
	} else if(allEvent[idx].second[0]&0xF0==0x80||allEvent[idx].second[0]&0xF0=0x90) {
		note[noteIdx++]=Note {
			allEvent[idx].second.getTime(),
			allEvent[idx].first,
			allEvent[idx].second[0]&0xF0,
			lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4),
			lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn,
			allEvent[idx].second[1],
			allEvent[idx].second[2],
			instruments[allEvent[idx].second[0]&0xF0]
		};
	}
	lastBeat=lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4);
	lastBar=lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn;
}
for(int i=0; i<nTrcks; i++) {
	trackEvent[i].clear();
}
for(int idx=0; idx<evnum; idx++) {
	trackEvent[allEvent[idx].first].push_back(allEvent[idx].second);
}
