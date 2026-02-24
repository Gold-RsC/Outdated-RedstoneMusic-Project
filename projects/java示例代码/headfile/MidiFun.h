#ifndef MIDIFUN_H
#define MIDIFUN_H
#include"MidiFile.h"
#include"Note.h"
#include<string>
#include<math.h>

struct Lyric {
	long long time;
	int track;
	double beat;
	double bar;
	std::string text;
};
struct TimeSignature{
	long long time;
	int track;
	double beat;
	double bar;
	int numerator;
	int denominator;
};
struct BeatPerMinute{
	long long time;
	int track;
	double beat;
	double bar;
	double value;
};
class MidiFun:public MidiFile {
		std::vector<Note> noteArr;
		std::vector<Lyric> lyricArr;
		std::vector<TimeSignature> tsArr;
		std::vector<BeatPerMinute> bpmArr;
	public:
		MidiFun(void) {

		}
		MidiFun(const char*filename) {
			parse(filename);
		}
		~MidiFun(void) {
		}
		void parse(const char*filename) {
			read(filename);
			deltaTick2absTick();
			std::pair<int,MidiEvent>*allEvent=new std::pair<int,MidiEvent>[getEventsNum()];
			int initidx=0;
			for(int track=0; track<getTracksNum(); track++) {
				for(int idx=0; idx<operator[](track).size(); idx++) {
					allEvent[initidx++]=std::pair<int,MidiEvent>(track,operator[](track)[idx]);
				}
			}
			std::sort(allEvent,allEvent+getEventsNum(),[](std::pair<int,MidiEvent>&a,std::pair<int,MidiEvent>&b) {
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
			for(int i=0; i<16; i++) {
				instruments[i]=-1;
			}
			noteArr.clear();
			lyricArr.clear();
			tsArr.clear();
			bpmArr.clear();
			for(int idx=0; idx<getEventsNum(); idx++) {
				allEvent[idx].second.setTime(idx==0?0:allEvent[idx-1].second.getTime()+(allEvent[idx].second.getTick()-allEvent[idx-1].second.getTick())/1000.0*timePerTick);
				if(allEvent[idx].second[0]==0xFF&&allEvent[idx].second[1]==0x51) {
					beatPerMinute=6e7/((allEvent[idx].second[3]<<16)+(allEvent[idx].second[4]<<8)+allEvent[idx].second[5]);
					timePerTick=((allEvent[idx].second[3]<<16)+(allEvent[idx].second[4]<<8)+allEvent[idx].second[5])*1.0/getTPQN();
					bpmArr.push_back(BeatPerMinute{
						allEvent[idx].second.getTime(),
						allEvent[idx].first,
						idx==0?0:(lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)),
						idx==0?0:(lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn),
						beatPerMinute
					});
				} else if(allEvent[idx].second[0]==0xFF&&allEvent[idx].second[1]==0x58) {
					ts_nnn=allEvent[idx].second[3];
					ts_ddd=pow(2,allEvent[idx].second[4]);
					tsArr.push_back(TimeSignature{
						allEvent[idx].second.getTime(),
						allEvent[idx].first,
						idx==0?0:(lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)),
						idx==0?0:(lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn),
						(int)ts_nnn,
						(int)ts_ddd
					});
				} else if((allEvent[idx].second[0]&0xF0)==0xC0) {
					instruments[allEvent[idx].second[0]&0xF]=allEvent[idx].second[1];
				} else if((allEvent[idx].second[0]&0xF0)==0x80||(allEvent[idx].second[0]&0xF0)==0x90) {
					noteArr.push_back(Note {
						allEvent[idx].second.getTime(),
						allEvent[idx].first,
						allEvent[idx].second[0]&0xF,
						idx==0?0:(lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)),
						idx==0?0:(lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn),
						allEvent[idx].second[1],
						((allEvent[idx].second[0]&0xF0)==0x80)?0:allEvent[idx].second[2],
						instruments[allEvent[idx].second[0]&0xF0]
					});
				} else if(allEvent[idx].second[0]==0xFF&&allEvent[idx].second[1]==0x5) {
					std::string tmp;
					for(int i=allEvent[idx].second.getEventBeginIdx(); i<allEvent[idx].second.size(); i++)
						tmp+=allEvent[idx].second[i];
					lyricArr.push_back(Lyric {
						allEvent[idx].second.getTime(),
						allEvent[idx].first,
						idx==0?0:(lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)),
						idx==0?0:(lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn),
						tmp
					});
				}
				lastBeat=idx==0?0:lastBeat+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4);
				lastBar=idx==0?0:lastBar+(allEvent[idx].second.getTime()-allEvent[idx-1].second.getTime())*(beatPerMinute/6e4)/ts_nnn;
			}
			for(int i=0; i<getTracksNum(); i++) {
				operator[](i).clear();
			}
			for(int idx=0; idx<getEventsNum(); idx++) {
				operator[](allEvent[idx].first).push_back(allEvent[idx].second);
			}
			delete[] allEvent;
		}
		int getNote(Note*arr) {
			for(int i=0; i<noteArr.size(); i++) {
				arr[i]=noteArr[i];
			}
			return noteArr.size();
		}
		int getNoteNum(void) {
			return noteArr.size();
		}

		int getLyric(Lyric*arr) {
			for(int i=0; i<lyricArr.size(); i++) {
				arr[i]=lyricArr[i];
			}
			return lyricArr.size();
		}
		int getLyricNum(void) {
			return lyricArr.size();
		}
		
		int getTS(TimeSignature*arr){
			for(int i=0; i<tsArr.size(); i++) {
				arr[i]=tsArr[i];
			}
			return tsArr.size();
		}
		int getTSNum(void){
			return tsArr.size();
		}
		
		int getBPM(BeatPerMinute*arr){
			for(int i=0; i<bpmArr.size(); i++) {
				arr[i]=bpmArr[i];
			}
			return bpmArr.size();
		}
		int getBPMNum(void){
			return bpmArr.size();
		}
};
#endif

