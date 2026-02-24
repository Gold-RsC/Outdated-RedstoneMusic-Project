#ifndef MIDIEVENT_H
#define MIDIEVENT_H
#include<windows.h>
#include<fstream>
#include<math.h>
#include<vector>
#include<stdio.h>
namespace midifile {
	namespace midierr {
		class __MIDIERR {
			private:
				std::ofstream out;
				int _mode;
			public:
				__MIDIERR(void) {
					_mode=0;
					out.open("CON");
				}
				__MIDIERR(const __MIDIERR&)=delete;
				__MIDIERR(const char*filename) {
					_mode=0;
					out.open(filename);
				}
				__MIDIERR(int mode) {
					_mode=mode;
					out.open("CON");
				}
				__MIDIERR&operator()(const __MIDIERR&)=delete;
				__MIDIERR&operator()(const char*filename) {
					_mode=0;
					out.close();
					out.open(filename);
					return*this;
				}
				__MIDIERR&operator()(int mode) {
					_mode=mode;
					return*this;
				}
				__MIDIERR&mode(int num) {
					_mode=num;
					return *this;
				}
				__MIDIERR&operator<<(const char*err) {
					switch(_mode) {
						case 0: {
							out<<"Error about the midi: "<<err<<"\n";
							break;
						}
						case 1: {
							MessageBox(NULL,err,"Error about the midi",MB_OK|MB_ICONINFORMATION);
							break;
						}
						default: {
							break;
						}
					}
					return *this;
				}
		};
	}
	midierr::__MIDIERR MIDIERR;
	typedef unsigned long ulong;
	typedef unsigned long long ull;
	typedef unsigned char uchar;
	struct Note {
		ulong tick,track:7,channel:4,pitch:7,volume:7,instrument:7;
	};
	struct Bpm {
		ulong tick;
		double value;
	};
	struct Ts {
		ulong tick,numerator:4,denominator:4,cpc,n32pqn:4;
	};
	typedef std::vector<uchar> MidiMessage;
	class MidiEvent {
		private:
			ulong tick;
			MidiMessage message;
		public:
			MidiEvent(void) {
				tick=0;
				message.clear();
			}
			MidiEvent(const MidiEvent&another)=default;
			MidiEvent(ulong _tick,MidiMessage _message) {
				tick=_tick,message=_message;
			}
			MidiEvent&operator()(ulong _tick,MidiMessage _message) {
				tick=_tick,message=_message;
				return *this;
			}
			MidiEvent&operator()(MidiEvent&another) {
				*this=another;
				return*this;
			}
			MidiEvent&operator=(const MidiEvent&another)=default;
			void clear(void) {
				tick=0;
				message.clear();
			}
			MidiMessage getMessage(void) {
				return message;
			}
			uchar getType(void) {
				return message[0];
			}
			bool isMeta(void) {
				return message[0]==0xFF;
			}
			ulong getTick(void) {
				return tick;
			}
			uchar&operator[](int idx) {
				return message[idx];
			}
	};
	typedef std::vector<MidiEvent> TrackEvent;
	class MidiEventList {
		private:
			ulong format,nTrcks,tpqn;
			TrackEvent*trackEvent;
			void resize(int t) {
				ulong l_ntrcks=(nTrcks>t?t:nTrcks);
				TrackEvent*newOne=new TrackEvent[t];
				for(int i=0; i<l_ntrcks; ++i) {
					newOne[i]=trackEvent[i];
				}
				delete[] trackEvent;
				trackEvent=newOne;
				nTrcks=t;
			}
		public:
			MidiEventList(void) {
				format=0,nTrcks=16,tpqn=120;
				trackEvent=new TrackEvent[nTrcks];
			}
			MidiEventList(ulong _format,ulong _ntrcks,ulong _tpqn) {
				format=_format,nTrcks=_ntrcks,tpqn=_tpqn;
				trackEvent=new TrackEvent[nTrcks];
			}
			MidiEventList(MidiEventList&another) {
				format=another.format,nTrcks=another.nTrcks,tpqn=another.tpqn;
				trackEvent=new TrackEvent[nTrcks];
				for(int i=0; i<nTrcks; ++i) {
					trackEvent[i]=another[i];
				}
			}
			~MidiEventList(void) {
				delete[] trackEvent;
			}
			ulong getFormat(void) {
				return format;
			}
			ulong getNTrcks(void) {
				return nTrcks;
			}
			ulong getTPQN(void) {
				return tpqn;
			}
			TrackEvent*getEvent(void) {
				return trackEvent;
			}
			MidiEventList&operator()(MidiEventList&another) {
				format=another.format,nTrcks=another.nTrcks,tpqn=another.tpqn;
				delete[] trackEvent;
				trackEvent=new TrackEvent[nTrcks];
				for(int i=0; i<nTrcks; ++i) {
					trackEvent[i]=another[i];
				}
				return *this;
			}
			MidiEventList&operator()(ulong _format,ulong _ntrcks,ulong _tpqn) {
				format=_format,tpqn=_tpqn;
				resize(_ntrcks);
				return*this;
			}
			MidiEventList&operator=(MidiEventList&another) {
				format=another.format,nTrcks=another.nTrcks,tpqn=another.tpqn;
				delete[] trackEvent;
				trackEvent=new TrackEvent[nTrcks];
				for(int i=0; i<nTrcks; ++i) {
					trackEvent[i]=another[i];
				}
				return *this;
			}
			TrackEvent&operator[](int idx) {
				return trackEvent[idx];
			}
	};
	int operator<<(Note*noteArr,MidiEventList&eventList) {
		ulong noteIdx=0;
		for(ulong track=0; track<eventList.getNTrcks(); track++) {
			ulong instrument=0;
			for(int eventIdx=0; eventIdx<eventList[track].size(); eventIdx++) {
				if(eventList[track][eventIdx][0]>=0x80&&eventList[track][eventIdx][0]<0xA0) {
					noteArr[noteIdx++]= Note {eventList[track][eventIdx].getTick(),
					                          track,
					                          (ulong)eventList[track][eventIdx][0]&0xF,
					                          eventList[track][eventIdx][1],
					                          (eventList[track][eventIdx][0]&0xF0==0x80)?0:(ulong)eventList[track][eventIdx][2],
					                          instrument
					                         };
				} else if(eventList[track][eventIdx][0]&0xF0==0xC0) {
					instrument=eventList[track][eventIdx][1];
				}
			}
		}
		return noteIdx;
	}
	int operator>>(MidiEventList&eventList,Note*noteArr) {
		ulong noteIdx=0;
		for(ulong track=0; track<eventList.getNTrcks(); track++) {
			ulong instrument=0;
			for(int eventIdx=0; eventIdx<eventList[track].size(); eventIdx++) {
				if(eventList[track][eventIdx][0]>=0x80&&eventList[track][eventIdx][0]<0xA0) {
					noteArr[noteIdx++]= Note {eventList[track][eventIdx].getTick(),
					                          track,
					                          (ulong)eventList[track][eventIdx][0]&0xF,
					                          eventList[track][eventIdx][1],
					                          (eventList[track][eventIdx][0]&0xF0==0x80)?0:(ulong)eventList[track][eventIdx][2],
					                          instrument
					                         };
				} else if(eventList[track][eventIdx][0]&0xF0==0xC0) {
					instrument=eventList[track][eventIdx][1];
				}
			}
		}
		return noteIdx;
	}
	int operator<<(Bpm*bpmArr,MidiEventList&eventList) {
		ulong bpmIdx=0;
		for(ulong track=0; track<eventList.getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<eventList[track].size(); eventIdx++) {
				if(eventList[track][eventIdx][0]==0xFF&&eventList[track][eventIdx][1]==0x51) {
					bpmArr[bpmIdx++]=Bpm {eventList[track][eventIdx].getTick(),
					                      6e7/(eventList[track][eventIdx][2]<<16|eventList[track][eventIdx][3]<<8|eventList[track][eventIdx][4])
					                     };
				}
			}
		}
		return bpmIdx;
	}
	int operator>>(MidiEventList&eventList,Bpm*bpmArr) {
		ulong bpmIdx=0;
		for(ulong track=0; track<eventList.getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<eventList[track].size(); eventIdx++) {
				if(eventList[track][eventIdx][0]==0xFF&&eventList[track][eventIdx][1]==0x51) {
					bpmArr[bpmIdx++]=Bpm {eventList[track][eventIdx].getTick(),
					                      6e7/(eventList[track][eventIdx][2]<<16|eventList[track][eventIdx][3]<<8|eventList[track][eventIdx][4])
					                     };
				}
			}
		}
		return bpmIdx;
	}
	int operator<<(Ts*tsArr,MidiEventList&eventList) {
		ulong tsIdx=0;
		for(ulong track=0; track<eventList.getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<eventList[track].size(); eventIdx++) {
				if(eventList[track][eventIdx][0]==0xFF&&eventList[track][eventIdx][1]==0x58) {
					tsArr[tsIdx++]=Ts {eventList[track][eventIdx].getTick(),
					                   eventList[track][eventIdx][2],
					                   (ulong)pow(2,eventList[track][eventIdx][3]),
					                   eventList[track][eventIdx][4],
					                   eventList[track][eventIdx][5]
					                  };
				}
			}
		}
		return tsIdx;
	}
	int operator>>(MidiEventList&eventList,Ts*tsArr) {
		ulong tsIdx=0;
		for(ulong track=0; track<eventList.getNTrcks(); track++) {
			for(int eventIdx=0; eventIdx<eventList[track].size(); eventIdx++) {
				if(eventList[track][eventIdx][0]==0xFF&&eventList[track][eventIdx][1]==0x58) {
					tsArr[tsIdx++]=Ts {eventList[track][eventIdx].getTick(),
					                   eventList[track][eventIdx][2],
					                   (ulong)pow(2,eventList[track][eventIdx][3]),
					                   eventList[track][eventIdx][4],
					                   eventList[track][eventIdx][5]
					                  };
				}
			}
		}
		return tsIdx;
	}
}
#endif


