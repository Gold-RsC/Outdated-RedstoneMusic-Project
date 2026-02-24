//#include"E:\金良昊\初中\c++\笔记拍照\hsyy\craigsapp-midifile-988d55b\include\midi处理\MidiFile.cpp"
#ifndef __MIDI_FILE_H__
#define __MIDI_FILE_H__
#include<stdio.h>
namespace smf{
	template<typename T>
	class __DataVector{
		private:
			T*data;
			size_t _capacity;
			size_t _size;
			void resize(int t){
				T*newdata=new T[t];
				for(int i=0;i<_size;i++)
					newdata[i]=data[i];
				delete[] data;
				data=newdata;
				_capacity=t;
			}
		public:
			__DataVector(){
				data=new T[1];
				_capacity=1;
				_size=0;
			}
			__DataVector(int size){
				data=new T[size];
				_capacity=size;
				_size=0;
			}
			~__DataVector(){
				delete[] data;
			}
			void push_back(T sth){
				if(_size==_capacity)
					resize(int(2*_capacity));
				data[_size++]=sth;
			}
			void pop_back(){
				_size--;
				if(_size==_capacity/4)
					resize(_capacity/2);
			}
			size_t size(){
				return _size;
			}
			bool empty(){
				return !_size;
			}
			void clear(){
				_size=0;
			}
			T&operator[](int idex){
				return data[idex];
			}
	};
	typedef unsigned short ushort;
	typedef unsigned char uchar;
	typedef unsigned long ulong;

	class MidiEvent{
		private:
			
		public:
			
	};
	class NOTE:protected MidiEvent{
		private:
		public:
	};
	class MidiFile{
		private:
			int reterr(const char*err){
				return 0;
			}
			int VarLenQtt(FILE*FIN){
				int ret=0;
				int i;
				varlen=0;
				for(i=0;i<5;i++){
					ret<<=7;
					int a=fgetc(FIN);
					ret+=a;
					if(a<0x80){
						varlen=i+1;
						return ret;
					}
					ret-=0x80;
				}
				varlen=i;
				return ret;
			}
			int ImmLenQtt(FILE*FIN,int byte){
				int len=0;
				for(int i=1;i<=byte;i++)
					len=(len<<8)+fgetc(FIN);
				return len;
			}
		protected:
			ushort Type;
			ushort Ntrcks;
			ulong Tpqn;
			int varlen;
			ulong Event_Type;
		public:
			bool read(FILE*FIN){
				
				if(FIN==NULL)
					return reterr("文件为空");
				
				if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='h'||fgetc(FIN)!='d')
					return reterr("此midi文件非标准midi文件\n出错原因：头模块标识符");
				
				if(ImmLenQtt(FIN,4)!=6)
					return reterr("此midi文件非标准midi文件\n出错原因：头模块数据长度");
				
				ushort type=ImmLenQtt(FIN,2);
				switch(type){
					case 0:
					case 1:
					case 2:{
						Type=type;
						break;
					}
					default:{
						return reterr("此midi文件非标准midi文件\n出错原因：midi type");
					}
				}
				
				ushort ntrcks=ImmLenQtt(FIN,2);
				if(!type&&ntrcks!=1)
					return reterr("此midi文件非标准midi文件\n出错原因：midi type和音轨数量不对应");
				Ntrcks=ntrcks;
				
				ushort division=ImmLenQtt(FIN,2);
				if(division>=0x8000){
					int fps=0xFF-((division>>8)&0xFF)+1;
					int sf=division&0xFF;
					if(fps!=25&&fps!=24&&fps!=29&&fps!=30)
						return reterr("此midi文件非标准midi文件\n出错原因：midi type和音轨数量不对应");
					Tpqn=fps*sf;
				}
				else
					Tpqn=division;
				
				for(int i=0;i<Ntrcks;i++){
					
					if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='r'||fgetc(FIN)!='k')
						return reterr("此midi文件非标准midi文件\n出错原因：音轨块标识符");
					
					ulong bytemax=ImmLenQtt(FIN,4);
					ulong bytenow=0;
					int abstick=0;
					
					while(bytenow<bytemax){
						ulong dtt=VarLenQtt(FIN);
						abstick+=dtt;
						bytenow+=varlen;
						ulong data=fgetc(FIN);
						if(data>=0x80){
							Event_Type=data;
							bytenow++;
						}
						else
							fseek(FIN,-1,SEEK_CUR);
						if(
							(Event_Type>=0x80&&Event_Type<=0xEF)||
							(Event_Type==0xF0||Event_Type==0xF7)
						){
							int channel=Event_Type%0x10;
							switch(Event_Type/0x10){
								case 0x8:{
									bytenow+=2;
									int pitch=fgetc(FIN),velocity=fgetc(FIN);
									NOTEev ev(Abs_Tick,channel,Track,pitch,0,Instrument);
									note.push(ev);
									break;
								}
								case 0x9:{
									bytenow+=2;
									int pitch=fgetc(FIN),velocity=fgetc(FIN);
									NOTEev ev(Abs_Tick,channel,Track,pitch,velocity,Instrument);
									note.push(ev);
									break;
								}
								case 0xA:{
									bytenow+=2;
									fseek(FIN,2,SEEK_CUR);
									break;
								}
								case 0xB:{
									bytenow+=2;
									fseek(FIN,2,SEEK_CUR);
									break;
								}
								case 0xC:{
									bytenow++;
									Instrument=fgetc(FIN);
									break;
								}
								case 0xD:{
									bytenow++;
									fseek(FIN,1,SEEK_CUR);
									break;
								}
								case 0xE:{
									bytenow+=2;
									fseek(FIN,2,SEEK_CUR);
									break;
								}
								case 0xF:{
									switch(Event_Type){
										case 0xF7:
										case 0xF0:{
											int len=Variable_Length(FIN,bytenow);
											fseek(FIN,len,SEEK_CUR);
											break;
										}
										default:{
											reterr("此midi文件非标准midi文件\n出错原因：不明事件类型");
											return -2;
										}
									}
									break;
								}
								default:{
									reterr("此midi文件非标准midi文件\n出错原因：不明事件类型");
									return -2;
								}
							}
						}
						else if(Event_Type==0xFF){
							int FFType=fgetc(FIN);
							bytenow++;
							int len=Variable_Length(FIN,bytenow);
							bytenow+=len;
							switch(FFType){
								case 0x0:
								case 0x1:
								case 0x2:
								case 0x3:
								case 0x4:
								case 0x5:
								case 0x6:
								case 0x7:
								case 0x8:
								case 0x9:{
									fseek(FIN,len,SEEK_CUR);
									break;
								}
								case 0x20:{
									channel=fgetc(FIN);
									break;
								}
								case 0x21:{
									fseek(FIN,len,SEEK_CUR);
									break;
								}
								case 0x2F:{
									Abs_Tick=0;
									break;
								}
								case 0x51:{
									BPMev ev(Abs_Tick,6e7/Immutable_Length(FIN,3));
									bpm.push(ev);
									break;
								}
								case 0x54:{
									fseek(FIN,len,SEEK_CUR);
									break;
								}
								case 0x58:{
									ts_nnn=fgetc(FIN);
									ts_ddd=MyPow(2,fgetc(FIN));
									fseek(FIN,len-2,SEEK_CUR);
									break;
								}
								case 0x59:{
									fseek(FIN,len,SEEK_CUR);
									break;
								}
								case 0x7F:{
									fseek(FIN,len,SEEK_CUR);
									break;
								}
								default:{
									reterr("此midi文件非标准midi文件\n出错原因：不明事件类型");
									return -2;
								}
							}
						}
						else{
							reterr("此midi文件非标准midi文件\n出错原因：不明事件类型");
							return -2;
						}
					}
				}
			}
	};
}
#endif
