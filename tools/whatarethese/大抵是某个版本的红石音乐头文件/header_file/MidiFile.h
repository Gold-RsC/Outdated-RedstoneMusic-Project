/*
	Name: MidiFile.h
	Author: Gold_RsC
	Starting Date: 2020/3/20
	Complete Date: 2020/10/4
	Author's website in Bilibili:
		https://space.bilibili.com/361846321?from=search&seid=3387696492724899772
	Description: 
		This is a header file that can analyze midi file.
		You can use this header file to parse midi files into gev files.
		Copyright belongs to Gold_RsC.
		No reprint without permissio!!!
*/
/*
	gev格式:
		分为两部分，每一部分都有标识符（MThd和MTrk）
		第一部分只有一个；第二部分有多个，每块第一个和最后一个标识符分别为MTrk和end 
	具体解释： 
	//前四行为Header Chunk
	MThd
		fmt [%d,0~1]
		ntrks [%d,0~127]
		tpqn [%d,48,96,120,240,360,480]
	//下面的ntrks(上面有该数据)个块均为以下的格式 
	MTrk
	//下面会有三大类格式  
	M txt [tick,%d] [type,%X] [len,%d] [text,%s[len=len]]
	  tempo [tick,%d] [value,%d]
	  ts [tick,%d] [分子,%d] [分母,%d] [clock时间,%d] [四分音符里有__个三十二分音符(弱智问题),%d(不是8我吃屎)]
	  sn [tick,%d] [%d] [%d]
	  channel [tick,%d] [value,%d]
	  port [tick,%d] [%d]
	  endtrack
	  SMPTE [tick,%d] [%d] [%d] [%d] [%d] [%d]
	  ks [tick,%d] [%d] [%d]
	S [tick,%d] [len,%d] [text,%d[len=len]]
	G note [tick,%d] [type(按下1,松开0),bool] [channel,%d] [pitch,%d] [velocity,%d] [program,%d]
	  kat [tick,%d] [channel,%d] [pitch,%d] [velocity,%d] [program,%d]
	  ctrl [tick] [channel] [control] [value]
	  prgrm [tick] [channel] [value]
	  at [tick] [channel] [value]
	  pw [tick] [channel] [pitch]
	//每个track块结束的标志符 
	end
*/
#ifndef __MIDIFILE_H__
#define __MIDIFILE_H__
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<windows.h>
#define ERROR_EVENT -1
typedef unsigned char uch;
class NOTE{
	public:
		//variable
		int tick,channel,pitch,velocity;
		//on or off
		bool type;
		//program
		int prgrm;
		//function
		void set(int tick,int channel,int pitch,int velocity,bool type,int prgrm);
		void set(NOTE&note);
		bool operator==(NOTE&note);
		bool operator!=(NOTE&note);
		void*operator new(size_t size);
		void operator delete(void*me);
		void*operator new[](size_t size);
		void operator delete[](void*me);
};
void NOTE::set(int tick,int channel,int pitch,int velocity,bool type,int prgrm){
	this->tick=tick;
	this->channel=channel;
	this->pitch=pitch;
	this->velocity=velocity;
	this->type=type;
	this->prgrm=prgrm;
}
void NOTE::set(NOTE&note){
	this->tick=note.tick;
	this->channel=note.channel;
	this->pitch=note.pitch;
	this->velocity=note.velocity;
	this->type=note.type;
	this->prgrm=note.prgrm;
}
bool NOTE::operator==(NOTE&note){
	return (this->tick==note.tick&&this->channel==note.channel&&this->pitch==note.pitch&&this->velocity==note.velocity&&this->type==note.type&&this->prgrm==note.prgrm);
}
bool NOTE::operator!=(NOTE&note){
	return !(*this==note);
}
void*NOTE::operator new(size_t size){
	NOTE*me=(NOTE*)malloc(size);
	return me;
}
void NOTE::operator delete(void*me){
	free(me);
}
void*NOTE::operator new[](size_t size){
	NOTE*me=(NOTE*)malloc(size);
	return me;
}
void NOTE::operator delete[](void*me){
	free(me);
}
class BPM{
	public:
		//variable
		int tick,value;
		//function
		void set(int tick,int value);
		void set(BPM&bpm);
		bool operator==(BPM&bpm);
		bool operator!=(BPM&bpm);
		void*operator new(size_t size);
		void operator delete(void*me);
		void*operator new[](size_t size);
		void operator delete[](void*me);
};
void BPM::set(int tick,int value){
	this->tick=tick;
	this->value=value;
}
void BPM::set(BPM&bpm){
	this->tick=bpm.tick;
	this->value=bpm.value;
}
bool BPM::operator==(BPM&bpm){
	return (this->tick==bpm.tick&&this->value==bpm.value);
}
bool BPM::operator!=(BPM&bpm){
	return !(*this==bpm);
}
void*BPM::operator new(size_t size){
	BPM*me=(BPM*)malloc(size);
	return me;
}
void BPM::operator delete(void*me){
	free(me);
}
void*BPM::operator new[](size_t size){
	BPM*me=(BPM*)malloc(size);
	return me;
}
void BPM::operator delete[](void*me){
	free(me);
}
NOTE end_of_note={-1,-1,-1,-1,0,-1},null_of_note;
BPM end_of_bpm={-1,-1},null_of_bpm;
#define EONOTE end_of_note
#define EOBPM end_of_bpm
#define NONOTE null_of_note
#define NOBPM null_of_bpm
int EVlen(NOTE*note){
	for(int i=0;i<=INT_MAX;i++)
		if(note[i]==EONOTE)
			return i;
	return -1;
}
int EVlen(BPM*bpm){
	for(int i=0;i<=INT_MAX;i++)
		if(bpm[i]==EOBPM)
			return i;
	return -1;
}
class MidiEvent{
	protected:
		//variable
		int Format,Number_of_Tracks,TPQN;
		int ABST,DTT,trkidx,Event_Type,channel,prgrm[16];
		int ISREAD;
		char EVFILE[50];
		//error warning
		void reterror(const char*str);
		void retwar(const char*str);
		//get length
		int Immutable_Length(FILE*FIN,int n);
		int Variable_Length(FILE*FIN,int&len);
		//print text
		void outtxt(FILE*FIN,FILE*FOUT,int len);
		//return Chunk Type
		const char*Chunk_Type(FILE*FIN,FILE*FOUT);
		const char*Chunk_Type(FILE*FIN);
		//Header Chunk
		bool Header_Chunk(FILE*FIN,FILE*FOUT);
		bool Header_Chunk(FILE*FIN);
		bool Header_Chunk_Data_Length(FILE*FIN);
		int Header_Chunk_Data(FILE*FIN,FILE*FOUT);
		int Header_Chunk_Data(FILE*FIN);
			//Header Chunk data
			int Header_Chunk_Format(FILE*FIN,FILE*FOUT);
			int Header_Chunk_Format(FILE*FIN);
			int Header_Chunk_Track_Number(FILE*FIN,FILE*FOUT);
			int Header_Chunk_Track_Number(FILE*FIN);
			bool Header_Chunk_Division(FILE*FIN,FILE*FOUT);
			bool Header_Chunk_Division(FILE*FIN);
		//Track Chunk
		bool Track_Chunk(FILE*FIN,FILE*FOUT);
		int Track_Chunk_Data_Length(FILE*FIN);
		bool Track_Chunk_Data(FILE*FIN,FILE*FOUT,const int len);
			//Track Chunk data
			int MTrk_Event(FILE*FIN,FILE*FOUT,int&Read_Len);
				int Delta_Tick(FILE*FIN,int&Read_Len);
				int Midi_Event(FILE*FIN,FILE*FOUT,int&Read_Len);
				int General_Event(FILE*FIN,FILE*FOUT,int&Read_Len);
				int Meta_Event(FILE*FIN,FILE*FOUT,int&Read_Len);
				int Sysex_Event(FILE*FIN,FILE*FOUT,int&Read_Len);
		int gevout(FILE*FIN,FILE*FOUT);
		int read(FILE*FIN);
	public:
		//init
		MidiEvent();
		//read
		//if you use this function,it will generate a file named "__MIDI_FILE__"
		//when the program finish,the new file will be removed
		int read(const char*filename);
		//input in GEV format
		int gevin(const char*filename);
		//output in GEV format
		int gevout(const char*finname,const char*foutname);
		//wheather the mid is read
		bool is_read();
		//get balabala
		int getfmt();
		int getntrks();
		int gettpqn();
		
		int getnote(NOTE*note,int num);
		int getbpm(BPM*bpm,int num);
		//end
		~MidiEvent();
};
MidiEvent::MidiEvent(){
	TPQN=120;
	Event_Type=-1;
	channel=-1;
	for(int i=0;i<16;i++)
		prgrm[i]=-1;
}
MidiEvent::~MidiEvent(){
	if(ISREAD==1){
		remove("__MIDI_FILE__");
	}
}
int MidiEvent::getnote(NOTE*note,int num){
	if(!is_read()){
		reterror("你还没有读取mid，何谈note");
		return ERROR_EVENT;
	}
	FILE*FIN=fopen(EVFILE,"rb");
	int noteidx=0;
	for(int i=1;i<=4;i++){
		char*ynwg=new char[50];
		fscanf(FIN,"%[^\n]\n",ynwg);
		delete[]ynwg;
	}
	for(int i=1;i<=Number_of_Tracks;i++){
		char*HC=new char[10];
		fscanf(FIN,"%s",HC);
		if(stricmp(HC,"MTrk")){
			return ERROR_EVENT;
		}
		delete[] HC;
		while(1){
			char*type=new char[10];
			fscanf(FIN,"%s",type);
			if(!stricmp(type,"G")){
				char*t=new char[10];
				fscanf(FIN,"%s",t);
				if(!stricmp(t,"note")){
					NOTE n;
					fscanf(FIN,"%d %d %d %d %d %d",&n.type,&n.tick,&n.channel,&n.pitch,&n.velocity,&n.prgrm);
					if(noteidx>=num){
						reterror("音符数量过大，或给定空间过小");
						return ERROR_EVENT;
					}
					note[noteidx++]=n;
				}
				else{
					char*value=new char[50];
					fscanf(FIN,"%[^\n]",value);
					delete[] value;
				}
				delete[] t;
			}
			else if(!stricmp(type,"M")||!stricmp(type,"S")){
				char*value=new char[50];
				fscanf(FIN,"%[^\n]\n",value);
				delete[] value;
			}
			else if(!stricmp(type,"end")){
				delete[] type;
				break;
			}
			else{
				return ERROR_EVENT;
			}
			delete[] type;
		}
	}
	fclose(FIN);
	note[noteidx]=EONOTE;
	return noteidx;
}
int MidiEvent::getbpm(BPM*bpm,int num){
	if(!is_read()){
		reterror("你还没有读取mid，何谈bpm");
		return ERROR_EVENT;
	}
	FILE*FIN=fopen(EVFILE,"rb");
	int bpmidx=0;
	for(int i=1;i<=4;i++){
		char*ynwg=new char[50];
		fscanf(FIN,"%[^\n]\n",ynwg);
		delete[]ynwg;
	}
	for(int i=1;i<=Number_of_Tracks;i++){
		char*HC=new char[10];
		fscanf(FIN,"%s",HC);
		if(stricmp(HC,"MTrk")){
			return ERROR_EVENT;
		}
		delete[] HC;
		while(1){
			char*type=new char[10];
			fscanf(FIN,"%s",type);
			if(!stricmp(type,"M")){
				char*t=new char[10];
				fscanf(FIN,"%s",t);
				if(!stricmp(t,"tempo")){
					BPM n;
					fscanf(FIN,"%d %d\n",&n.tick,&n.value);
					n.value=6e7/n.value;
					if(bpmidx>=num){
						reterror("音符数量过大，或给定空间过小");
						return ERROR_EVENT;
					}
					bpm[bpmidx++]=n;
				}
				else{
					char*value=new char[50];
					fscanf(FIN,"%[^\n]",value);
					delete[] value;
				}
				delete[] t;
			}
			else if(!stricmp(type,"G")||!stricmp(type,"S")){
				char*value=new char[50];
				fscanf(FIN,"%[^\n]\n",value);
				delete[] value;
			}
			else if(!stricmp(type,"end")){
				delete[] type;
				break;
			}
			else{
				return ERROR_EVENT;
			}
			delete[] type;
		}
	}
	fclose(FIN);
	bpm[bpmidx]=EOBPM;
	return bpmidx;
}
void MidiEvent::reterror(const char*str){	
	char s[100];
	sprintf(s,"小问号你是否有很多朋友\n出错内容：\n\t%s",str);
	MessageBox(NULL,s,"Midi解析器",MB_ICONQUESTION|MB_OK);
}
void MidiEvent::retwar(const char*str){
	char s[100];
	sprintf(s,"小问号你是否有很多朋友\n警告内容：\n\t%s",str);
	MessageBox(NULL,s,"Midi解析器",MB_ICONQUESTION|MB_OK);
}
int MidiEvent::Immutable_Length(FILE*FIN,int n){
	int len=0;
	for(int i=1;i<=n;i++)
		len=len*256+fgetc(FIN);
	return len;
}
int MidiEvent::Variable_Length(FILE*FIN,int&len){
	int ret=0;
	for(int i=1;i<=5;i++){
		ret*=128;
		int a=fgetc(FIN);
		ret+=a;
		if(a<128){
			len+=i;
			return ret;
		}
		ret-=128;
	}
	char str[100];
	sprintf(str,"可变长度超出范围，记为%d",ret);
	reterror(str);
	return ret;
}
void MidiEvent::outtxt(FILE*FIN,FILE*FOUT,int len){
	for(int i=1;i<=len;i++)
		fprintf(FOUT,"%c",fgetc(FIN));
}
int MidiEvent::gevin(const char*filename){
	if(ISREAD){
		reterror("the midi file has already been read");
		return ERROR_EVENT; 
	}
	strcpy(EVFILE,filename);
	FILE*FIN=fopen(filename,"r");
	char ynwg[50];
	fscanf(FIN,"%[^\n]\n",ynwg);
	fscanf(FIN,"%s",ynwg);
	fscanf(FIN,"%d",&Format);
	fscanf(FIN,"%s",ynwg);
	fscanf(FIN,"%d",&Number_of_Tracks);
	fscanf(FIN,"%s",ynwg);
	fscanf(FIN,"%d",&TPQN);
	fclose(FIN);
	ISREAD=3;
	return 1;
}
int MidiEvent::read(FILE*FIN){
	FILE*FOUT=fopen("__MIDI_FILE__","wb");
	strcpy(EVFILE,"__MIDI_FILE__");
	gevout(FIN,FOUT);
	fclose(FOUT);
	ISREAD=1;
	return 1;
}
int MidiEvent::read(const char*filename){
	if(ISREAD){
		reterror("the midi file has already been read");
		return ERROR_EVENT; 
	}
	FILE*FIN=fopen(filename,"rb");
	read(FIN);
	fclose(FIN);
	ISREAD=1;
	return 1;
}
int MidiEvent::gevout(FILE*FIN,FILE*FOUT){
	if(!Header_Chunk(FIN,FOUT))
		return 0;
	for(int i=1;i<=Number_of_Tracks;i++){
		if(!Track_Chunk(FIN,FOUT))
			return 0;
		trkidx++;
	}
	ISREAD=2;
	return 1;
}
int MidiEvent::gevout(const char*finname,const char*foutname){
	if(ISREAD){
		reterror("the midi file has already been read");
		return ERROR_EVENT; 
	}
	FILE*FIN=fopen(finname,"rb");
	FILE*FOUT=fopen(foutname,"w");
	strcpy(EVFILE,foutname);
	gevout(FIN,FOUT);
	fclose(FIN);
	fclose(FOUT);
	ISREAD=2;
	return 1;
}
int MidiEvent::gettpqn(){
	if(!is_read()){
		reterror("你还没有读取mid，何谈tpqn");
		return ERROR_EVENT;
	}
	return TPQN;
}
int MidiEvent::getntrks(){
	if(!is_read()){
		reterror("你还没有读取mid，何谈ntrks");
		return ERROR_EVENT;
	}
	return Number_of_Tracks;
}
int MidiEvent::getfmt(){
	if(!is_read()){
		reterror("你还没有读取mid，何谈fmt");
		return ERROR_EVENT;
	}
	return Format;
}
bool MidiEvent::is_read(){
	return ISREAD;
}
const char*MidiEvent::Chunk_Type(FILE*FIN,FILE*FOUT){
	if(fgetc(FIN)!='M'){
		reterror("头块标记出错");
		return NULL; 
	}
	if(fgetc(FIN)!='T'){
		reterror("头块标记出错");
		return NULL;
	}
	uch a=fgetc(FIN),b=fgetc(FIN);
	if(a=='h'&&b=='d'){
		fprintf(FOUT,"MThd\n");
		return "MThd";
	}
	else if(a=='r'&&b=='k'){
		fprintf(FOUT,"MTrk\n");
		return "MTrk";
	}
	else{
		return NULL;
	}
}
const char*MidiEvent::Chunk_Type(FILE*FIN){
	if(fgetc(FIN)!='M'){
		reterror("头块标记出错");
		return NULL; 
	}
	if(fgetc(FIN)!='T'){
		reterror("头块标记出错");
		return NULL;
	}
	uch a=fgetc(FIN),b=fgetc(FIN);
	if(a=='h'&&b=='d'){
		return "MThd";
	}
	else if(a=='r'&&b=='k'){
		return "MTrk";
	}
	else{
		return NULL;
	}
}
bool MidiEvent::Header_Chunk(FILE*FIN,FILE*FOUT){
	if(stricmp(Chunk_Type(FIN,FOUT),"MThd"))
		return 0;
	if(!Header_Chunk_Data_Length(FIN))
		return 0;
	if(!Header_Chunk_Data(FIN,FOUT))
		return 0;
	return 1;
}
bool MidiEvent::Header_Chunk(FILE*FIN){
	if(stricmp(Chunk_Type(FIN),"MThd"))
		return 0;
	if(!Header_Chunk_Data_Length(FIN))
		return 0;
	if(!Header_Chunk_Data(FIN))
		return 0;
	return 1;
}
bool MidiEvent::Header_Chunk_Data_Length(FILE*FIN){
	if(Immutable_Length(FIN,4)!=6){
		reterror("头块数据长度必须为6");
		return 0;
	}
	return 1;
}
int MidiEvent::Header_Chunk_Data(FILE*FIN,FILE*FOUT){
	if(Header_Chunk_Format(FIN,FOUT)==-1)
		return 0;
	Header_Chunk_Track_Number(FIN,FOUT);
	if(!Header_Chunk_Division(FIN,FOUT))
		return 0;
	return 1;
}
int MidiEvent::Header_Chunk_Data(FILE*FIN){
	if(Header_Chunk_Format(FIN)==-1)
		return 0;
	Header_Chunk_Track_Number(FIN);
	if(!Header_Chunk_Division(FIN))
		return 0;
	return 1;
}
int MidiEvent::Header_Chunk_Format(FILE*FIN,FILE*FOUT){
	if(fgetc(FIN)!=0){
		reterror("头块格式错误");
		return ERROR_EVENT;
	}
	Format=fgetc(FIN);
	switch(Format){
		case 0:
		case 1:
		case 2:
			fprintf(FOUT,"fmt %d\n",Format);
			return Format;
		default:
			reterror("头块格式错误");
			return ERROR_EVENT;
	}
}
int MidiEvent::Header_Chunk_Format(FILE*FIN){
	if(fgetc(FIN)!=0){
		reterror("头块格式错误");
		return ERROR_EVENT;
	}
	Format=fgetc(FIN);
	switch(Format){
		case 0:
		case 1:
		case 2:
			return Format;
		default:
			reterror("头块格式错误");
			return ERROR_EVENT;
	}
}
int MidiEvent::Header_Chunk_Track_Number(FILE*FIN,FILE*FOUT){
	Number_of_Tracks=Immutable_Length(FIN,2);
	fprintf(FOUT,"ntrks %d\n",Number_of_Tracks);
	return Number_of_Tracks;
}
int MidiEvent::Header_Chunk_Track_Number(FILE*FIN){
	Number_of_Tracks=Immutable_Length(FIN,2);
	return Number_of_Tracks;
}
bool MidiEvent::Header_Chunk_Division(FILE*FIN,FILE*FOUT){
	int a=fgetc(FIN),b=fgetc(FIN);
	if(a>0){
		TPQN=a*256+b;
		fprintf(FOUT,"tpqn %d\n",TPQN);
		return 1;
	}
	else{
		reterror("因为我不会SMPTE，所以将tpqn记为120(吐舌)\n但实际情况确实应记为120(皱眉)");
		fprintf(FOUT,"tpqn %d\n",TPQN);
		return 1;
	}
}
bool MidiEvent::Header_Chunk_Division(FILE*FIN){
	int a=fgetc(FIN),b=fgetc(FIN);
	if(a>0){

		TPQN=a*256+b;
		return 1;
	}
	else{
		retwar("因为我不会SMPTE，所以将tpqn记为120(吐舌)\n但实际情况确实应记为120(皱眉)");
		return 1;
	}
}
bool MidiEvent::Track_Chunk(FILE*FIN,FILE*FOUT){
	if(stricmp(Chunk_Type(FIN,FOUT),"MTrk"))
		return 0;
	Track_Chunk_Data(FIN,FOUT,Track_Chunk_Data_Length(FIN));
	return 1;
}
int MidiEvent::Track_Chunk_Data_Length(FILE*FIN){
	return Immutable_Length(FIN,4);
}
bool MidiEvent::Track_Chunk_Data(FILE*FIN,FILE*FOUT,const int len){
	int Read_Len=0;
	while(Read_Len<len)
		MTrk_Event(FIN,FOUT,Read_Len);
	if(Read_Len>len){
		reterror("已读取字符数大于预设字符数"); 
		return 0;
	}
	fprintf(FOUT,"end\n");
	return 1;
}
int MidiEvent::MTrk_Event(FILE*FIN,FILE*FOUT,int&Read_Len){
	DTT=Delta_Tick(FIN,Read_Len);
	ABST+=DTT;
	if(!Midi_Event(FIN,FOUT,Read_Len))
		return 0;
}
int MidiEvent::Delta_Tick(FILE*FIN,int&Read_Len){
	return Variable_Length(FIN,Read_Len);
}
int MidiEvent::Midi_Event(FILE*FIN,FILE*FOUT,int&Read_Len){
	int data=fgetc(FIN);
	if(data>=0x80){
		Event_Type=data;
		Read_Len++;
	}
	else
		fseek(FIN,-1,SEEK_CUR);
	if(Event_Type>=0x80&&Event_Type<=0xEF)
		return General_Event(FIN,FOUT,Read_Len);
	else if(Event_Type==0xFF)
		return Meta_Event(FIN,FOUT,Read_Len);
	else if(Event_Type==0xF0||Event_Type==0xF7)
		return Sysex_Event(FIN,FOUT,Read_Len); 
	else{
		char str[100];
		sprintf(str,"不可省略的状态符或未知的状态符：%X",Event_Type);
		reterror(str);
		return 0;
	}
}
int MidiEvent::General_Event(FILE*FIN,FILE*FOUT,int&Read_Len){
	channel=Event_Type%0x10;
	int value;
	fprintf(FOUT,"G ");
	switch(Event_Type/0x10){
		case 0x8:{
			//note off
			int pitch=fgetc(FIN),velocity=fgetc(FIN);
			fprintf(FOUT,"note 0 %d %d %d %d %d\n",ABST,channel,pitch,velocity,prgrm[channel]);
			Read_Len+=2;
			break;
		}
		case 0x9:{
			//note on
			int pitch=fgetc(FIN),velocity=fgetc(FIN);
			fprintf(FOUT,"note 1 %d %d %d %d %d\n",ABST,channel,pitch,velocity,prgrm[channel]);
			Read_Len+=2;
			break;
		}
		case 0xA:{
			//Key after touch
			int a=fgetc(FIN),b=fgetc(FIN);
			fprintf(FOUT,"kat %d %d %d %d %d\n",ABST,channel,a,b,prgrm[channel]);
			Read_Len+=2;
			break;
		}
		case 0xB:{
			//controller
			int a=fgetc(FIN),b=fgetc(FIN);
			fprintf(FOUT,"ctrl %d %d %d %d\n",ABST,channel,a,b);
			Read_Len+=2;
			break;
		}
		case 0xC:{
			//program changes
			prgrm[channel]=fgetc(FIN);
			fprintf(FOUT,"prgrm %d %d %d\n",ABST,channel,prgrm[channel]);
			Read_Len++;
			break;
		}
		case 0xD:{
			//aftertouch
			fprintf(FOUT,"at %d %d %d\n",ABST,channel,fgetc(FIN));
			Read_Len++;
			break;
		}
		case 0xE:{
			//pitch wheel
			int a=fgetc(FIN),b=fgetc(FIN);
			fprintf(FOUT,"pw %d %d %d\n",ABST,channel,b*128+a-8192);
			Read_Len+=2;
			break;
		}
		default:{
			char str[100];
			sprintf(str,"未知的状态符：%X",Event_Type);
			reterror(str);
			return 0;
		}
	}
	return Event_Type;
}
int MidiEvent::Meta_Event(FILE*FIN,FILE*FOUT,int&Read_Len){
	if(Event_Type!=0xFF){
		char str[100];
		sprintf(str,"未知状态符：%X",Event_Type);
		reterror(str);
		return 0;
	}
	int type=fgetc(FIN);
	Read_Len++;
	int len=Variable_Length(FIN,Read_Len);
	Read_Len+=len;
	fprintf(FOUT,"M ");
	switch(type){
		case 0x0:{
			if(!len){
				fprintf(FOUT,"sn %d %d %d\n",ABST,trkidx-1,trkidx);
				break;
			}
			int a=fgetc(FIN),b=fgetc(FIN);
			fprintf(FOUT,"sn %d %d %d\n",ABST,a,b);
			break;
		}
		case 0x1:{
			//歌曲备注or音轨文本
			fprintf(FOUT,"txt %d 01 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x2:{
			//歌曲版权
			fprintf(FOUT,"txt %d 02 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x3:{
			//歌曲标题or音轨名称
			fprintf(FOUT,"txt %d 03 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x4:{
			//乐器名称
			fprintf(FOUT,"txt %d 04 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x5:{
			//歌词
			fprintf(FOUT,"txt %d 05 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x6:{
			//标记
			fprintf(FOUT,"txt %d 06 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x7:{
			//开始点
			fprintf(FOUT,"txt %d 07 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x8:{
			//歌曲文件名
			fprintf(FOUT,"txt %d 08 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x9:{
			//设备名
			fprintf(FOUT,"txt %d 09 %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		case 0x20:{
			channel=fgetc(FIN);
			fprintf(FOUT,"channel %d %d\n",ABST,channel);
			break;
		}
		case 0x21:{
			fprintf(FOUT,"port %d %d\n",ABST,fgetc(FIN));
			break;
		}
		case 0x2F:{
			fprintf(FOUT,"endtrack %d\n",channel);
			ABST=0;
			break;
		}
		case 0x51:{
			fprintf(FOUT,"tempo %d %d\n",ABST,Immutable_Length(FIN,3));
			break;
		}
		case 0x54:{
			int sh=fgetc(FIN),sm=fgetc(FIN),ss=fgetc(FIN),sf=fgetc(FIN),smf=fgetc(FIN);
			fprintf(FOUT,"SMPTE %d %d %d %d %d %d\n",ABST,sh,sm,ss,sf,smf);
			break;
		}
		case 0x58:{
			int nnn=fgetc(FIN),ddd=(int)pow(2,fgetc(FIN)),ccc=fgetc(FIN),n32=fgetc(FIN);
			fprintf(FOUT,"ts %d %d %d %d %d\n",ABST,nnn,ddd,ccc,n32);
			break;
		}
		case 0x59:{
			int a=fgetc(FIN),b=fgetc(FIN);
			fprintf(FOUT,"ks %d %d %d\n",ABST,a,b);
			break;
		}
		case 0x7F:{
			//音符特定信息
			fprintf(FOUT,"txt %d 7F %d ",ABST,len);
			outtxt(FIN,FOUT,len);
			fprintf(FOUT,"\n");
			break;
		}
		default:{
			char str[100];
			sprintf(str,"未知元事件类型：%X",type);
			reterror(str);
			return 0;
		}
	}
	return type;
}
int MidiEvent::Sysex_Event(FILE*FIN,FILE*FOUT,int&Read_Len){
	if(Event_Type!=0xF7&&Event_Type!=0xF0){
		char str[100];
		sprintf(str,"未知状态符：%X",Event_Type);
		reterror(str);
		return 0;
	}
	int len=Variable_Length(FIN,Read_Len);
	Read_Len+=len;
	fprintf(FOUT,"S %d %d",ABST,len);
	for(int i=1;i<=len;i++)
		fprintf(FOUT," %d",fgetc(FIN));
	fprintf(FOUT,"\n");
	return 1;
}

#endif
