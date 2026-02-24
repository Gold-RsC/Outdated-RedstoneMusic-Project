#ifndef MIDIFUN_H
#define MIDIFUN_H
#include"MidiFile.h"
#include<algorithm>
using namespace midifile;
class MidiFun {
	private:
		MidiFile midi;
		bool hasChangedTick;
		std::vector<Note>note;
	public:
		MidiFun(void)=default;
		MidiFun(MidiFile&mid) {
			midi=mid;
			hasChangedTick=0;
		}
		MidiFun(const char*filename) {
			midi.read(filename);
			hasChangedTick=0;
		}
		MidiFun(FILE*FIN) {
			midi.read(FIN);
			hasChangedTick=0;
		}
		MidiFun(MidiFun&another)=default;
		MidiFun&operator=(MidiFun&another)=default;
		void read(const char*filename) {
			midi.read(filename);
		}
		void read(FILE*FIN) {
			midi.read(FIN);
		}
		void tick2ms(void) {
			hasChangedTick=1;
			//初始化
			std::vector<Note>noteArr;
			std::vector<Bpm>bpmArr;
			std::vector<Ts>tsArr;
			//获取事件信息并转换
			for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
				ulong instrument=0;
				for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
					if(midi.getEvent()[track][eventIdx][0]>=0x80&&midi.getEvent()[track][eventIdx][0]<0xA0) {
						noteArr.push_back({midi.getEvent()[track][eventIdx].getTick(),
						                   track,
						                   (ulong)midi.getEvent()[track][eventIdx][0]&0xF,
						                   midi.getEvent()[track][eventIdx][1],
						                   (midi.getEvent()[track][eventIdx][0]&0xF0==0x80)?0:(ulong)midi.getEvent()[track][eventIdx][2],
						                   instrument
						                  });
					} else if(midi.getEvent()[track][eventIdx][0]&0xF0==0xC0) {
						instrument=midi.getEvent()[track][eventIdx][1];
					} else if(midi.getEvent()[track][eventIdx][0]==0xFF&&midi.getEvent()[track][eventIdx][1]==0x51) {
						bpmArr.push_back({midi.getEvent()[track][eventIdx].getTick(),
						                  6e7/(midi.getEvent()[track][eventIdx][2]<<16|midi.getEvent()[track][eventIdx][3]<<8|midi.getEvent()[track][eventIdx][4])
						                 });
					} else if(midi.getEvent()[track][eventIdx][0]==0xFF&&midi.getEvent()[track][eventIdx][1]==0x58) {
						tsArr.push_back({midi.getEvent()[track][eventIdx].getTick(),
						                 midi.getEvent()[track][eventIdx][2],
						                 (ulong)pow(2,midi.getEvent()[track][eventIdx][3]),
						                 midi.getEvent()[track][eventIdx][4],
						                 midi.getEvent()[track][eventIdx][5]
						                });
					}
				}
			}
			//开始转换
			sort(bpmArr.begin(),bpmArr.end(),[](Bpm a,Bpm b) {
				return a.tick<=b.tick;
			});
			sort(tsArr.begin(),tsArr.end(),[](Ts a,Ts b) {
				return a.tick<=b.tick;
			});
			auto findBpmIdx=[&](ulong tick) {
				ulong l=0,r=bpmArr.size()-1;
				if(tick>=bpmArr[r].tick)
					return r;
				if(tick<bpmArr[l].tick)
					return (ulong)0;
				while(l<=r) {
					ulong mid=(l+r)/2;
					if(tick<bpmArr[mid].tick) {
						r=mid;
					} else if(tick>=bpmArr[mid+1].tick) {
						l=mid+1;
					} else {
						return mid;
					}
				}
				return (ulong)0;
			};
			auto findTsIdx=[&](ulong tick) {
				ulong l=0,r=tsArr.size()-1;
				if(tick>=tsArr[r].tick)
					return r;
				if(tick<tsArr[l].tick)
					return (ulong)0;
				while(l<=r) {
					ulong mid=(l+r)/2;
					if(tick<tsArr[mid].tick) {
						r=mid;
					} else if(tick>=tsArr[mid+1].tick) {
						l=mid+1;
					} else {
						return mid;
					}
				}
				return (ulong)0;
			};
			note.clear();
			std::vector<ulong>tmp;
			for(int i=0; i<bpmArr.size(); i++) {
				ulong tsIdx=findTsIdx(bpmArr[i].tick);
				tmp.push_back((i==0?0:tmp[i-1])
				              +(bpmArr[i].tick-(i==0?0:bpmArr[i-1].tick))
				              *300*tsArr[tsIdx].denominator
				              /midi.getTPQN()
				              /(i==0?1:bpmArr[i-1].value)
				             );
			}
			for(int i=0; i<noteArr.size(); i++) {
				ulong bpmIdx=findBpmIdx(noteArr[i].tick);
				ulong tsIdx=findTsIdx(noteArr[i].tick);
				ulong tick=tmp[bpmIdx]+
				           ulong((noteArr[i].tick-bpmArr[bpmIdx].tick)
				                 *300*tsArr[tsIdx].denominator
				                 /midi.getTPQN()
				                 /bpmArr[bpmIdx].value);
				note.push_back({tick,
				                noteArr[i].track,
				                noteArr[i].channel,
				                noteArr[i].pitch,
				                noteArr[i].volume,
				                noteArr[i].instrument
				               });
			}
		}
		int operator>>(Note*_note) {
			for(int i=0; i<note.size(); i++)
				_note[i]=note[i];
			return note.size();
		}
};
int operator<<(Note*note,MidiFun&midi) {
	return midi>>note;
}
#endif

