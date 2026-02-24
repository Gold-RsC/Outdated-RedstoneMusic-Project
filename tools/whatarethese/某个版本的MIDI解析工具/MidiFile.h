#ifndef MIDIFILE_H
#define MIDIFILE_H
#include<stdio.h>
#include<stdarg.h>
#include<string>
#include<vector>
#include<iostream>
using namespace std;
//错误处理
FILE*__midiErrorFile=fopen("CON","w");
int printErr(const char*format,...) {
	fprintf(__midiErrorFile,"MIDI Error:\n");
	va_list args;
	va_start(args,format);
	int ret=vfprintf(__midiErrorFile,format,args);
	va_end(args);
	return ret;
}
void freErr(const char*filename) {
	freopen(filename,"w",__midiErrorFile);
}
class MidiEvent {
		long long tick;
		long long time;
		vector<unsigned char> message;
	public:
		MidiEvent(void){
			message.resize(3);
		}
		MidiEvent(long long _tick,long long _time,vector<unsigned char>_message) {
			tick=_tick;
			time=_time;
			message=_message;
		}
		~MidiEvent(void)=default;
		long long getTick(void) {
			return tick;
		}
		void setTick(long long _tick) {
			tick=_tick;
		}
		long long getTime(void) {
			return time;
		}
		void setTime(long long _time) {
			time=_time;
		}
		int getCommand(void) {
			return message[0];
		}
		void setCommand(unsigned char command) {
			message[0]=command;
		}
		bool isMeta(void) {
			return message[0]!=0xFF;
		}
		int getMetaType(void) {
			if(!isMeta()) {
				return -1;
			}
			return message[1];
		}
		void addData(int data){
			message.push_back(data);
		}
		template<typename T>
		void setMessage(initializer_list<T> _msg){
			message.resize(_msg.size());
			message.clear();
			for(auto a:_msg){
				message.push_back(a);
			}
		}
		vector<unsigned char> getMessage(void){
			return message;
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
		int size(void) {
			return message.size();
		}
		int operator[](int idx) {
			return message[idx];
		}
};

class MidiFile {
	protected:
		int format;
		int nTracks;
		int tpqn;
		vector<MidiEvent>*events;
	private:
		unsigned int ILValue(FILE*FIN,int len) {
			unsigned int ret=0;
			for(int i=0; i<len; i++) {
				unsigned int a=fgetc(FIN);
				cerr<<"["<<a<<"]";
				ret=(ret<<8)+a;
			}
			cerr<<ret;
			return ret;
		}
		unsigned char VLVData[6]= {0};
		//[0]:count
		//[1:5]:data
		long long VLValue(FILE*FIN) {
			long long ret=0;
			int i;
			for(i=0; i<5; i++) {
				ret<<=7;
				int a=fgetc(FIN);
				VLVData[i+1]=a;
				ret|=(a&0x7F);
				if(a<0x80) {
					break;
				}
			}
			VLVData[0]=i+1;
			return ret;
		}
		int getMidiData(FILE*FIN,MidiEvent&event,int&lastData,unsigned int&byteNow) {
			int data=fgetc(FIN);
			byteNow++;
			if(data<0x80) {
				if(lastData==0) {
					printErr("没有上次的事件命令");
					return 0;
				} else if(lastData>=0xF0) {
					printErr("元事件或系统码命令不能省略");
					return 0;
				}
				data=lastData;
				fseek(FIN,-1,SEEK_CUR);
				byteNow--;
			}
			switch(data&0xF0) {
				case 0x80:		//note off
				case 0x90:		//note on
				case 0xA0:		//after touch
				case 0xB0:		//controller
				case 0xE0: {	//pitch wheel
					event.setMessage({data,fgetc(FIN),fgetc(FIN)});
					byteNow+=2;
					break;
				}
				case 0xC0:
				case 0xD0: {
					event.setMessage({data,fgetc(FIN)});
					byteNow++;
					break;
				}
				case 0xF0: {
					switch(data) {
						case 0xFF: {
							int type=fgetc(FIN);
							event.setMessage({data,type});
							byteNow++;
							int len=VLValue(FIN);
							byteNow+=VLVData[0];
							for(int i=1;i<=VLVData[0];i++){
								event.addData(VLVData[i]);
							}
							for(int i=0; i<len; i++){
								event.addData(fgetc(FIN));
							}
							break;
						}
						case 0xF0:
						case 0xF7: {
							event.setMessage({data});
							byteNow++;
							int len=VLValue(FIN);
							byteNow+=VLVData[0];
							for(int i=1;i<=VLVData[0];i++){
								event.addData(VLVData[i]);
							}
							for(int i=0; i<len; i++){
								event.addData(fgetc(FIN));
							}
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
			lastData=data;
			return data;
		}
		int readFile(FILE*FIN) {
			cerr<<"0\n";
			if(FIN==NULL) {
				printErr("文件为空");
				return -1;
			}
			cerr<<"0.1\n";
			if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='h'||fgetc(FIN)!='d') {
				printErr("头模块标识符错误");
				return -1;
			}
			cerr<<"0.2\n";
			if(ILValue(FIN,4)!=6) {
				printErr("头模块数据长度出错");
				return -1;
			}
			cerr<<"1\n";
			format=fgetc(FIN);
			if(format<0||format>2) {
				printErr("头模块format数据出错");
				return -1;
			}
			cerr<<"ha?";
			nTracks=ILValue(FIN,2);
			if(format==0&&nTracks!=1) {
				cerr<<"yuanlairuci";
				printErr("头模块format数据与nTrcks数据不对应");
				cerr<<"ahaahahahah";
				return -1;
			}
			cerr<<"???";
			if(format==0&&nTracks!=1) {
				printErr("midi type和音轨数量不对应(type 0的文件只可以有一个轨道)");
				return -1;
			}
			cerr<<"cao?";
			events=new vector<MidiEvent>[nTracks];
			int division=ILValue(FIN,2);
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
	cerr<<"2\n";
			for(int track=0; track<nTracks; track++) {
				if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='r'||fgetc(FIN)!='k') {
					printErr("第%d音轨(从0开始)标识符出错",track);
					return -1;
				}
				unsigned int byteMax=ILValue(FIN,4);
				unsigned int byteNow=0;
				int lastData=0;
				cerr<<"3\n";
				while(!feof(FIN)) {
					cerr<<"4";
					unsigned long long deltaTick=VLValue(FIN);
					byteNow+=VLVData[0];
					MidiEvent event;
					event.setTick(deltaTick);
					getMidiData(FIN,event,lastData,byteNow);
					events[track].push_back(event);
					if(event.isMeta()&&event[1]==0x2F) {
						break;
					}
				}
			}
		}
	public:
		MidiFile(void)=default;
		MidiFile(const char*name) {
			read(name);
		}
		~MidiFile(void) {
			delete[] events;
		}
		int read(const char*name) {
			FILE*FIN=fopen(name,"rb");
			cerr<<"??";
			int ret=readFile(FIN);
			fclose(FIN);
			cerr<<"a!";
			return ret;
		}
		vector<MidiEvent> operator[](int idx){
			return events[idx];
		}
};
#endif

