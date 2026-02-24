#ifndef MIDIFILE_H
#define MIDIFILE_H
#include"MidiEvent.h"
FILE*cao=fopen("cao.txt","w");
namespace midifile {
	class MidiFile {
		private:
			MidiEventList eventList;
			ulong VLValue(FILE*FIN,ulong&varlen) {
				ulong ret=0;
				int i;
				for(i=0; i<5; i++) {
					ret<<=7;
					int a=fgetc(FIN);
					ret|=(a&0x7F);
					if(a<0x80) {
						varlen+=i+1;
						return ret;
					}
				}
				varlen+=i;
				return ret;
			}
			ulong ILValue(FILE*FIN,int byte) {
				ulong len=0;
				for(int i=1; i<=byte; i++)
					len=(len<<8)+fgetc(FIN);
				return len;
			}
		ulong getMidiData(FILE*FIN,ulong tick,ulong&lastMidiData,MidiEvent&event,ulong&nRead) {
			uchar data=fgetc(FIN);
			nRead++;
			if(data<0x80) {
				if(lastMidiData==0) {
					MIDIERR<<"没有上次的事件命令";
					return 0;
				} else if(lastMidiData>=0xF0) {
					MIDIERR<<"元事件或系统码命令不能省略";
					return 0;
				}
				data=lastMidiData;
				fseek(FIN,-1,SEEK_CUR);
				nRead--;
			}
			switch(data&0xF0) {
				case 0x80:	//note off
				case 0x90:{	//note on
					ulong pitch=fgetc(FIN),volume=fgetc(FIN);
					fprintf(cao,"%d %d\n",pitch,volume);
					event=MidiEvent(tick,{(uchar)pitch,(uchar)volume});
					break;
				};
				case 0xA0:	//after touch
				case 0xB0:	//controller
				case 0xE0: {	//pitch wheel
					event=MidiEvent(tick, {data,(uchar)fgetc(FIN),(uchar)fgetc(FIN)});
					nRead+=2;
					break;
				}
				case 0xC0:
				case 0xD0: {
					event=MidiEvent(tick, {data,(uchar)fgetc(FIN)});
					nRead++;
					break;
				}
				case 0xF0: {
					switch(data) {
						case 0xFF: {
							uchar type=fgetc(FIN);
							nRead++;
							ulong len=VLValue(FIN,nRead);
							nRead+=len;
							MidiMessage message;
							message.push_back(data);
							message.push_back(type);
							for(int i=0; i<len; i++)
								message.push_back(fgetc(FIN));
							event=MidiEvent(tick,message);
							break;
						}
						case 0xF0:
						case 0xF7: {
							ulong len=VLValue(FIN,nRead);
							nRead+=len;
							MidiMessage message;
							message.push_back(data);
							for(int i=0; i<len; i++)
								message.push_back(fgetc(FIN));
							event=MidiEvent(tick,message);
							break;
						}
						default: {
							MIDIERR<<"未知的事件命令";
							return 0;
						}
					}
					break;
				}
				default: {
					MIDIERR<<"未知的事件命令";
					return 0;
				}
			}
			lastMidiData=data;
			return data;
		}
		public:
			MidiFile(void)=default;
			MidiFile(const char*filename) {
				read(filename);
			}
			MidiFile(FILE*FIN) {
				read(FIN);
			}
			MidiFile(MidiFile&another)=default;
			MidiFile&operator=(MidiFile&another)=default;
			void read(const char*filename) {
				FILE*FIN=fopen(filename,"rb");
				read(FIN);
				fclose(FIN);
			}
			void read(FILE*FIN) {
				if(FIN==NULL) {
					MIDIERR<<"文件为空";
					return;
				}
				if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='h'||fgetc(FIN)!='d') {
					MIDIERR<<"头模块出错";
					return;
				}
				if(ILValue(FIN,4)!=6) {
					MIDIERR<<"头模块数据长度出错";
					return;
				}
				ulong format=ILValue(FIN,2);
				if(format>2||format<0) {
					MIDIERR<<"头模块数据出错";
					return;
				}
				ulong nTrcks=ILValue(FIN,2);
				if(format==0&&nTrcks!=1) {
					MIDIERR<<"midi type和音轨数量不对应(type 0的文件只可以有一个轨道)";
					return;
				}
				ulong division=ILValue(FIN,2);
				ulong tpqn;
				if(division>=0x8000) {
					int fps=0xFF-((division>>8)&0xFF)+1;
					int sf=division&0xFF;
					if(fps!=25&&fps!=24&&fps!=29&&fps!=30) {
						MIDIERR<<"未知的FBS值";
						return;
					}
					tpqn=fps*sf;
				} else
					tpqn=division;
				eventList(format,nTrcks,tpqn);

				for(int track=0; track<nTrcks; track++) {
					if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='r'||fgetc(FIN)!='k') {
						MIDIERR<<"音轨块标识符出错";
						return;
					}
					ulong nByte=ILValue(FIN,4);
					ulong nRead=0;
					ulong absTick=0;
					ulong lastMidiData=0;
					while(!feof(FIN)) {
						ulong deltaTick=VLValue(FIN,nRead);
						absTick+=deltaTick;
						MidiEvent event;
						if(getMidiData(FIN,absTick,lastMidiData,event,nRead)==0) {
							return;
						}
						eventList[track].push_back(event);
						if(event.isMeta()&&event[1]==0x2F) {
							break;
						}
					}
				}
			}
			ulong getFormat(void) {
				return eventList.getFormat();
			}
			ulong getNTrcks(void) {
				return eventList.getNTrcks();
			}
			ulong getTPQN(void) {
				return eventList.getTPQN();
			}
			MidiEventList&getEvent(void) {
				return eventList;
			}
			TrackEvent&operator[](int idx) {
				return eventList[idx];
			}
	};
	int operator<<(Note*noteArr,MidiFile&midi) {
		ulong noteIdx=0;
		for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
			ulong instrument=0;
			for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
				if(midi.getEvent()[track][eventIdx][0]>=0x80&&midi.getEvent()[track][eventIdx][0]<0xA0) {
					noteArr[noteIdx++]= Note {midi.getEvent()[track][eventIdx].getTick(),
					                          track,
					                          (ulong)midi.getEvent()[track][eventIdx][0]&0xF,
					                          midi.getEvent()[track][eventIdx][1],
					                          (midi.getEvent()[track][eventIdx][0]&0xF0==0x80)?0:(ulong)midi.getEvent()[track][eventIdx][2],
					                          instrument
					                         };
				} else if(midi.getEvent()[track][eventIdx][0]&0xF0==0xC0) {
					instrument=midi.getEvent()[track][eventIdx][1];
				}
			}
		}
		return noteIdx;
	}
	int operator>>(MidiFile&midi,Note*noteArr) {
		ulong noteIdx=0;
		for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
			ulong instrument=0;
			for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
				if(midi.getEvent()[track][eventIdx][0]>=0x80&&midi.getEvent()[track][eventIdx][0]<0xA0) {
					noteArr[noteIdx++]= Note {midi.getEvent()[track][eventIdx].getTick(),
					                          track,
					                          (ulong)midi.getEvent()[track][eventIdx][0]&0xF,
					                          midi.getEvent()[track][eventIdx][1],
					                          (midi.getEvent()[track][eventIdx][0]&0xF0==0x80)?0:(ulong)midi.getEvent()[track][eventIdx][2],
					                          instrument
					                         };
				} else if(midi.getEvent()[track][eventIdx][0]&0xF0==0xC0) {
					instrument=midi.getEvent()[track][eventIdx][1];
				}
			}
		}
		return noteIdx;
	}
	int operator<<(Bpm*bpmArr,MidiFile&midi) {
		ulong bpmIdx=0;
		for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
				if(midi.getEvent()[track][eventIdx][0]==0xFF&&midi.getEvent()[track][eventIdx][1]==0x51) {
					bpmArr[bpmIdx++]=Bpm {midi.getEvent()[track][eventIdx].getTick(),
					                      6e7/(midi.getEvent()[track][eventIdx][2]<<16|midi.getEvent()[track][eventIdx][3]<<8|midi.getEvent()[track][eventIdx][4])
					                     };
				}
			}
		}
		return bpmIdx;
	}
	int operator>>(MidiFile&midi,Bpm*bpmArr) {
		ulong bpmIdx=0;
		for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
				if(midi.getEvent()[track][eventIdx][0]==0xFF&&midi.getEvent()[track][eventIdx][1]==0x51) {
					bpmArr[bpmIdx++]=Bpm {midi.getEvent()[track][eventIdx].getTick(),
					                      6e7/(midi.getEvent()[track][eventIdx][2]<<16|midi.getEvent()[track][eventIdx][3]<<8|midi.getEvent()[track][eventIdx][4])
					                     };
				}
			}
		}
		return bpmIdx;
	}
	int operator<<(Ts*tsArr,MidiFile&midi) {
		ulong tsIdx=0;
		for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
				if(midi.getEvent()[track][eventIdx][0]==0xFF&&midi.getEvent()[track][eventIdx][1]==0x58) {
					tsArr[tsIdx++]=Ts {midi.getEvent()[track][eventIdx].getTick(),
					midi.getEvent()[track][eventIdx][2],
					                   (ulong)pow(2,midi.getEvent()[track][eventIdx][3]),
					                   midi.getEvent()[track][eventIdx][4],
					                   midi.getEvent()[track][eventIdx][5]
					                  };
				}
			}
		}
		return tsIdx;
	}
	int operator>>(MidiFile&midi,Ts*tsArr) {
		ulong tsIdx=0;
		for(ulong track=0; track<midi.getEvent().getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<midi.getEvent()[track].size(); eventIdx++) {
				if(midi.getEvent()[track][eventIdx][0]==0xFF&&midi.getEvent()[track][eventIdx][1]==0x58) {
					tsArr[tsIdx++]=Ts {midi.getEvent()[track][eventIdx].getTick(),
					midi.getEvent()[track][eventIdx][2],
					                   (ulong)pow(2,midi.getEvent()[track][eventIdx][3]),
					                   midi.getEvent()[track][eventIdx][4],
					                   midi.getEvent()[track][eventIdx][5]
					                  };
				}
			}
		}
		return tsIdx;
	}
}
#endif

