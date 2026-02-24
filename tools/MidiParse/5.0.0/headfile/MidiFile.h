#ifndef MIDIFILE_H
#define MIDIFILE_H

#include<stdio.h>
#include<stdarg.h>
#include<vector>
#include<algorithm>
//错误处理
FILE*__midiErrorFile=fopen("CON","w");
//输出错误信息 
int printErr(const char*format,...) {
	fprintf(__midiErrorFile,"MIDI Error:\n");
	va_list args;
	va_start(args,format);
	int ret=vfprintf(__midiErrorFile,format,args);
	va_end(args);
	return ret;
}
//更改错误输出路径 
void freErr(const char*filename) {
	freopen(filename,"w",__midiErrorFile);
}

//MidiMessage类
typedef std::vector<unsigned char> MidiMessage;

//MidiEvent类
class MidiEvent {
		long long tick;
		long long time;
		MidiMessage message;
	public:
		MidiEvent(void) {
			tick=0;
			time=0;
			message.clear();
		}
		MidiEvent(const MidiEvent&another)=default;
		MidiEvent(long long _tick,long long _time,MidiMessage _message) {
			tick=_tick,time=_time,message=_message;
		}
		long long getTick(void) {
			return tick;
		}
		void setTick(long long _tick){
			tick=_tick;
		}
		long long getTime(void){
			return time;
		}
		void setTime(long long _time){
			time=_time;
		}
		unsigned char getType(void) {
			return message[0];
		}
		MidiMessage&getMessage(void) {
			return message;
		}
		bool isMeta(void) {
			return message[0]==0xFF;
		}
		int size(void){
			return message.size();
		}
		int getEventBeginIdx(void) {
			if(message[0]==0xF0||message[0]==0xF7) {
				for(int i=1; i<6; i++) {
					if(message[i]<0x80) {
						return i+1;
					}
				}
				return -1;
			} else if(!isMeta()) {
				return 1;
			} else {
				for(int i=2; i<7; i++) {
					if(message[i]<0x80) {
						return i+1;
					}
				}
				return -1;
			}
		}
		unsigned char&operator[](int idx) {
			return message[idx];
		}
		MidiEvent&operator=(const MidiEvent&another)=default;
};

//MidiTrack类
typedef std::vector<MidiEvent> MidiTrack;

//MidiFile类
class MidiFile {
	private:
		int format,nTrcks,tpqn;
		MidiTrack*trackEvent;
		int evnum;
		void resize(int t) {
			int l_ntrcks=(nTrcks>t?t:nTrcks);
			MidiTrack*newOne=new MidiTrack[t];
			for(int i=0; i<l_ntrcks; ++i) {
				newOne[i]=trackEvent[i];
			}
			delete[] trackEvent;
			trackEvent=newOne;
			nTrcks=t;
		}
		void dataInit(void) {
			format=0,nTrcks=16,tpqn=120,evnum=0;
		}
		long long vValue(FILE*FIN,int&varlen) {
			long long ret=0;
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
		long long vValue(FILE*FIN,int&varlen,MidiMessage&message) {
			long long ret=0;
			int i;
			for(i=0; i<5; i++) {
				ret<<=7;
				int a=fgetc(FIN);
				message.push_back(a);
				ret|=(a&0x7F);
				if(a<0x80) {
					varlen+=i+1;
					return ret;
				}
			}
			varlen+=i;
			return ret;
		}
		long long iValue(FILE*FIN,int byte) {
			long long len=0;
			for(int i=1; i<=byte; i++)
				len=(len<<8)+fgetc(FIN);
			return len;
		}
		long long getMidiData(FILE*FIN,long long tick,unsigned char&lastMidiData,MidiEvent&event,int&nRead) {
			unsigned char data=fgetc(FIN);
			nRead++;
			if(data<0x80) {
				if(lastMidiData==0) {
					printErr("没有上次的事件命令");
					return 0;
				} else if(lastMidiData>=0xF0) {
					printErr("元事件或系统码命令不能省略");
					return 0;
				}
				data=lastMidiData;
				fseek(FIN,-1,SEEK_CUR);
				nRead--;
			}
			switch(data&0xF0) {
				case 0x80:		//note off
				case 0x90:		//note on
				case 0xA0:		//after touch
				case 0xB0:		//controller
				case 0xE0: {	//pitch wheel
					event=MidiEvent(tick, 0,{data,(unsigned char)fgetc(FIN),(unsigned char)fgetc(FIN)});
					nRead+=2;
					break;
				}
				case 0xC0:
				case 0xD0: {
					event=MidiEvent(tick, 0,{data,(unsigned char)fgetc(FIN)});
					nRead++;
					break;
				}
				case 0xF0: {
					switch(data) {
						case 0xFF: {
							MidiMessage message;
							message.push_back(data);
							unsigned char type=fgetc(FIN);
							message.push_back(type);
							nRead++;
							int len=vValue(FIN,nRead,message);
							nRead+=len;
							for(int i=0; i<len; i++)
								message.push_back(fgetc(FIN));
							event=MidiEvent(tick,0,message);
							break;
						}
						case 0xF0:
						case 0xF7: {
							MidiMessage message;
							message.push_back(data);
							int len=vValue(FIN,nRead,message);
							nRead+=len;
							for(int i=0; i<len; i++)
								message.push_back(fgetc(FIN));
							event=MidiEvent(tick,0,message);
							break;
						}
						default: {
							printErr("未知的事件命令");
							return 0;
						}
					}
					break;
				}
				default: {
					printErr("未知的事件命令");
					return 0;
				}
			}
			lastMidiData=data;
			return data;
		}
		int read(FILE*FIN) {
			if(FIN==NULL) {
				printErr("文件为空");
				return -1;
			}
			if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='h'||fgetc(FIN)!='d') {
				printErr("头模块出错");
				return -1;
			}
			if(iValue(FIN,4)!=6) {
				printErr("头模块数据长度出错");
				return -1;
			}
			format=iValue(FIN,2);
			if(format>2||format<0) {
				printErr("头模块数据出错");
				return -1;
			}
			nTrcks=iValue(FIN,2);
			trackEvent=new MidiTrack[nTrcks];
			if(format==0&&nTrcks!=1) {
				printErr("midi type和音轨数量不对应(type 0的文件只可以有一个轨道)");
				return -1;
			}
			int division=iValue(FIN,2);
			if(division>=0x8000) {
				int fps=0xFF-((division>>8)&0xFF)+1;
				int sf=division&0xFF;
				if(fps!=25&&fps!=24&&fps!=29&&fps!=30) {
					printErr("未知的FBS值");
					return -1;
				}
				tpqn=fps*sf;
			} else {
				tpqn=division;
			}
			int eventNum=0;
			for(int track=0; track<nTrcks; track++) {
				if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='r'||fgetc(FIN)!='k') {
					printErr("音轨块标识符出错");
					return -1;
				}
				int nByte=iValue(FIN,4);
				int nRead=0;
				unsigned char lastMidiData=0;
				while(!feof(FIN)) {
					long long deltaTick=vValue(FIN,nRead);
					MidiEvent event;
					if(getMidiData(FIN,deltaTick,lastMidiData,event,nRead)==0) {
						return -1;
					}
					trackEvent[track].push_back(event);
					eventNum++;
					if(event.isMeta()&&event[1]==0x2F) {
						break;
					}
				}
			}
			return eventNum;
		}
	public:
		MidiFile(void) {
			dataInit();
		}
		MidiFile(const char*filename) {
			dataInit();
		}
		int read(const char*filename) {
			FILE*FIN=fopen(filename,"rb");
			evnum=read(FIN);
			fclose(FIN);
			return evnum;
		}
		int&getFormat(void) {
			return format;
		}
		int&getTracksNum(void) {
			return nTrcks;
		}
		int&getTPQN(void) {
			return tpqn;
		}
		MidiTrack*getEvent(void) {
			return trackEvent;
		}
		int getEventsNum(void){
			return evnum;
		}
		void deltaTick2absTick(void) {
			for(int track=0; track<nTrcks; track++) {
				for(int ev=1; ev<trackEvent[track].size(); ev++) {
					trackEvent[track][ev].setTick(trackEvent[track][ev].getTick()+trackEvent[track][ev-1].getTick());
				}
			}
		}
		void tick2time(void) {
			std::pair<int,MidiEvent> allEvent[evnum];
			int initidx=0;
			for(int track=0;track<nTrcks;track++){
				for(int idx=0;idx<trackEvent[track].size();idx++){
					allEvent[initidx++]=std::pair<int,MidiEvent>(track,trackEvent[track][idx]);
				}
			}
			std::sort(allEvent,allEvent+evnum,[](std::pair<int,MidiEvent>&a,std::pair<int,MidiEvent>&b){
				if(a.second.getTick()!=b.second.getTick())
					return a.second.getTick()<b.second.getTick();
				return a.first<b.first;
			});
			double timePerTick=120.0;
			for(int idx=0; idx<evnum; idx++) {
				allEvent[idx].second.setTime(idx==0?0:allEvent[idx-1].second.getTime()+(allEvent[idx].second.getTick()-allEvent[idx-1].second.getTick())/1000.0*timePerTick);
				if(allEvent[idx].second[0]==0xFF&&allEvent[idx].second[1]==0x51){
					timePerTick=((allEvent[idx].second[3]<<16)+(allEvent[idx].second[4]<<8)+allEvent[idx].second[5])*1.0/tpqn;
				}
			}
			for(int i=0;i<nTrcks;i++){
				trackEvent[i].clear();
			}
			for(int idx=0;idx<evnum;idx++){
				trackEvent[allEvent[idx].first].push_back(allEvent[idx].second);
			}
		}
		MidiFile&operator=(MidiFile&another) {
			format=another.format,nTrcks=another.nTrcks,tpqn=another.tpqn,evnum=another.evnum;
			delete[] trackEvent;
			trackEvent=new MidiTrack[nTrcks];
			for(int i=0; i<nTrcks; ++i) {
				trackEvent[i]=another[i];
			}
			return *this;
		}
		MidiTrack&operator[](int idx) {
			return trackEvent[idx];
		}
};
#endif

