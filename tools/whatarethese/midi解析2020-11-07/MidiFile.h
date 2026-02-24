#ifndef __MIDIFILE_H__
#define __MIDIFILE_H__

#include<stdio.h>
#include<stdlib.h> 
#include<vector>
#include<windows.h>

#define MIDIERR -1
#define NULL_TICK -2 
#define NULL_CHNL -2 
#define NULL_TRK -2 
#define NULL_PITCH -2 
#define NULL_VELO -2 
#define NULL_INSTRU -2 
class NOTEev{
	public:
		int tick,
			channel,track,
			pitch,velocity,
			instrument;
		NOTEev(int tick=-2,int channel=-2,int track=-2,int pitch=-2,int velocity=-2,int instrument=-2);
		NOTEev(const NOTEev&note);
		void set(int tick=-2,int channel=-2,int track=-2,int pitch=-2,int velocity=-2,int instrument=-2);
		void clear();
		void copy(const NOTEev&note);
		void move(NOTEev&note);
		bool operator==(NOTEev&note);
		bool operator!=(NOTEev&note);
};
NOTEev null_of_note_event;
NOTEev error_of_note_event(-1,-1,-1,-1,-1,-1);
#define NULL_Nev null_of_note_event
#define ERR_Nev error_of_note_event
class NOTE{
	private:
		std::vector<NOTEev>note;
		bool cmp(NOTEev&a,NOTEev&b);
		void mergeSort(std::vector<NOTEev>&arr,int start,int end);
	public:
		int tick,
			channel,track,
			pitch,velocity,
			instrument;
		NOTE();
		NOTE(const NOTE&sth);
		void push(NOTEev&sth);
		void pop();
		void copy(NOTE&sth);
		void move(NOTE&sth);
		void clear();
		bool empty();
		int size();
		void sort();
		NOTEev&operator[](int idx);
};

#define NULL_BPM -2 
class BPMev{
	public:
		int tick,
			value;
		BPMev(int tick=-2,int value=-2);
		BPMev(const BPMev&bpm);
		void set(int tick=-2,int value=-2);
		void clear();
		void copy(BPMev&bpm);
		void move(BPMev&bpm);
		bool operator==(BPMev&bpm);
		bool operator!=(BPMev&bpm);
};
BPMev null_of_bpm_event;
BPMev error_of_bpm_event(-1,-1);
#define NULL_Bev null_of_bpm_event
#define ERR_Bev error_of_bpm_event
class BPM{
	private:
		std::vector<BPMev>bpm;
		bool cmp(BPMev&a,BPMev&b);
		void mergeSort(std::vector<BPMev>&arr,int start,int end);
	public:
		int tick,
			value;
		BPM();
		BPM(const BPM&sth);
		void push(BPMev&sth);
		void pop();
		void copy(BPM&sth);
		void move(BPM&sth);
		void clear();
		bool empty();
		int size();
		void sort();
		BPMev&operator[](int idx);
};

class MidiFile{
	private:
		int Format,Number_of_Tracks,TPQN;
		
		int Instrument;
		int channel; 
		int Abs_Tick;
		
		int ts_nnn,ts_ddd;
		
		bool ISREAD;
		
		NOTE note;
		BPM bpm;
		
		void init();
		
		void reterr(const char*str);
		
		int Immutable_Length(FILE*FIN,int n);
		int Variable_Length(FILE*FIN,int&len);
		int MyPow(int x,int n);
	public:
		MidiFile();
		MidiFile(FILE*FIN);
		MidiFile(const char*filename);
		MidiFile(const MidiFile&other);
		
		int read(FILE*FIN);
		int read(const char*filename);
		
		bool is_read();
		
		int get_tpqn();
		int get_fmt();
		int get_ntrk();
		int get_note(NOTE&sth);
		int get_bpm(BPM&sth);
		int get_ts_n();
		int get_ts_d();
		
		
		
		~MidiFile();
};

NOTEev::NOTEev(int tick,int channel,int track,int pitch,int velocity,int instrument){
	this->tick=tick;
	this->channel=channel;
	this->track=track;
	this->pitch=pitch;
	this->velocity=velocity;
	this->instrument=instrument;
}
NOTEev::NOTEev(const NOTEev&note){
	*this=note;
}
void NOTEev::set(int tick,int channel,int track,int pitch,int velocity,int instrument){
	this->tick=tick;
	this->channel=channel;
	this->track=track;
	this->pitch=pitch;
	this->velocity=velocity;
	this->instrument=instrument;
}
void NOTEev::clear(){
	this->tick=-2;
	this->channel=-2;
	this->track=-2;
	this->pitch=-2;
	this->velocity=-2;
	this->instrument=-2;
}
void NOTEev::copy(const NOTEev&note){
	*this=note;
}
void NOTEev::move(NOTEev&note){
	*this=note;
	note.clear();
}
bool NOTEev::operator==(NOTEev&note){
	return (note.tick==this->tick&&note.channel==this->channel&&note.track==this->track&&note.pitch==this->pitch&&note.velocity==this->velocity&&note.instrument==this->instrument);
}
bool NOTEev::operator!=(NOTEev&note){
	return !(note==*this);
}

bool NOTE::cmp(NOTEev&a,NOTEev&b){
	if(a.channel!=b.channel)
		return a.channel<b.channel;
	if(a.track!=b.track)
		return a.track<b.track;
	if(a.tick!=b.tick)
		return a.tick<b.tick;
	if(a.pitch!=b.pitch)
		return a.pitch<b.pitch;
	return 1;
}
void NOTE::mergeSort(std::vector<NOTEev>&arr,int start,int end){
    if(start>=end)
        return;
    int i=start;
    int mid=(start+end)/2;
    int j=mid+1;
    mergeSort(arr,i,mid);
    mergeSort(arr,j,end);
    NOTEev*temp=(NOTEev*)malloc((end-start+1)*sizeof(NOTEev));
    int index = 0;
    while(i<=mid&&j<=end){
        if(cmp(arr[i],arr[j]))
            temp[index++]=arr[i++];
    	else
            temp[index++]=arr[j++];
    }
    while(i<=mid)
        temp[index++]=arr[i++];
    while(j<=end)
        temp[index++]=arr[j++];
    int k=start;
    for(;k<=end;k++)
        arr[k] = temp[k-start];
    free(temp);
}
NOTE::NOTE(){
	tick=-2;
	channel=-2;
	track=-2;
	pitch=-2;
	velocity=-2;
	instrument=-2;
}
NOTE::NOTE(const NOTE&sth){
	this->note=sth.note;
	tick=-2;
	channel=-2;
	track=-2;
	pitch=-2;
	velocity=-2;
	instrument=-2;
}
void NOTE::push(NOTEev&sth){
	note.push_back(sth);
}
void NOTE::pop(){
	note.pop_back();
}
void NOTE::copy(NOTE&sth){
	this->note.assign(sth.note.begin(),sth.note.end());
}
void NOTE::move(NOTE&sth){
	this->copy(sth);
	sth.clear();
}
void NOTE::clear(){
	note.clear();
}
bool NOTE::empty(){
	note.empty();
}
int NOTE::size(){
	return note.size();
}
void NOTE::sort(){
	mergeSort(note,0,note.size()-1);
}
NOTEev&NOTE::operator[](int idx){
	if(idx>=note.size())
		return ERR_Nev;
	this->tick=note[idx].tick;
	this->channel=note[idx].channel;
	this->track=note[idx].track;
	this->pitch=note[idx].pitch;
	this->velocity=note[idx].velocity,
	this->instrument=note[idx].instrument;
	return note[idx];
}


BPMev::BPMev(int tick,int value){
	this->tick=tick;
	this->value=value;
}
BPMev::BPMev(const BPMev&bpm){
	*this=bpm;
}
void BPMev::set(int tick,int value){
	this->tick=tick;
	this->value=value;
}
void BPMev::clear(){
	this->tick=-2;
	this->value=-2;
}
void BPMev::copy(BPMev&bpm){
	*this=bpm;
}
void BPMev::move(BPMev&bpm){
	*this=bpm;
	bpm.clear();
}
bool BPMev::operator==(BPMev&bpm){
	return (bpm.tick==this->tick&&bpm.value==this->value);
}
bool BPMev::operator!=(BPMev&bpm){
	return !(bpm==*this);
}

bool BPM::cmp(BPMev&a,BPMev&b){
	return a.tick<=b.tick;
}
void BPM::mergeSort(std::vector<BPMev>&arr,int start,int end){
    if(start>=end)
        return;
    int i=start;
    int mid=(start+end)/2;
    int j=mid+1;
    mergeSort(arr,i,mid);
    mergeSort(arr,j,end);
    BPMev*temp=(BPMev*)malloc((end-start+1)*sizeof(BPMev));
    int index = 0;
    while(i<=mid&&j<=end){
        if(cmp(arr[i],arr[j]))
            temp[index++]=arr[i++];
    	else
            temp[index++]=arr[j++];
    }
    while(i<=mid)
        temp[index++]=arr[i++];
    while(j<=end)
        temp[index++]=arr[j++];
    int k=start;
    for(;k<=end;k++)
        arr[k] = temp[k-start];
    free(temp);
}
BPM::BPM(){
	tick=-2;
	value=-2;
}
BPM::BPM(const BPM&sth){
	this->bpm=sth.bpm;
	tick=-2;
	value=-2;
}
void BPM::push(BPMev&sth){
	bpm.push_back(sth);
}
void BPM::pop(){
	bpm.pop_back();
}
void BPM::copy(BPM&sth){
	this->bpm.assign(sth.bpm.begin(),sth.bpm.end());
}
void BPM::move(BPM&sth){
	this->copy(sth);
	sth.clear();
}
void BPM::clear(){
	bpm.clear();
}
bool BPM::empty(){
	bpm.empty();
}
int BPM::size(){
	return bpm.size();
}
void BPM::sort(){
	mergeSort(bpm,0,bpm.size()-1);
}
BPMev&BPM::operator[](int idx){
	if(idx>=bpm.size())
		return ERR_Bev;
	this->tick=bpm[idx].tick;
	this->value=bpm[idx].value;
	return bpm[idx];
}

void MidiFile::init(){
	Format=-1;
	Number_of_Tracks=-1;
	TPQN=-1;
	Instrument=-1;
	Abs_Tick=0;
	channel=-1; 
	ts_nnn=-1;
	ts_ddd=-1;
	ISREAD=0;
}
MidiFile::MidiFile(){
	init();
}
MidiFile::MidiFile(FILE*FIN){
	init();
	read(FIN);
}
MidiFile::MidiFile(const char*filename){
	init();
	read(filename);
}
MidiFile::MidiFile(const MidiFile&other){
	*this=other;
}
void MidiFile::reterr(const char*str){
	MessageBox(NULL,str,"Midi解析器",MB_ICONQUESTION|MB_OK);
}
int MidiFile::Immutable_Length(FILE*FIN,int n){
	int len=0;
	for(int i=1;i<=n;i++)
		len=len*256+fgetc(FIN);
	return len;
}
int MidiFile::Variable_Length(FILE*FIN,int&len){
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
	return ret;
}
int MidiFile::MyPow(int x,int n){
	if(n==0)return 1;
	if(n==1)return x;
	if(n==2)return x*x;
	return MyPow(x,n/2)*MyPow(x,n-n/2);
}
int MidiFile::read(FILE*FIN){
	if(FIN==NULL){
		reterr("文件为空");
		return -1;
	}
	if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='h'||fgetc(FIN)!='d'){
		reterr("此midi文件非标准midi文件\n出错原因：头模块标识符");
		return -2;
	}
	if(Immutable_Length(FIN,4)!=6){
		reterr("此midi文件非标准midi文件\n出错原因：头模块数据长度");
		return -2;
	}
	Format=Immutable_Length(FIN,2);
	if(Format>2||Format<0){
		reterr("此midi文件非标准midi文件\n出错原因：头模块数据");
		return -2;
	}
	Number_of_Tracks=Immutable_Length(FIN,2);
	TPQN=Immutable_Length(FIN,2);
	if(TPQN>480||TPQN<48)
		TPQN=120;
	for(int Track=0;Track<Number_of_Tracks;Track++){
		int Abs_Tick=0;
		int Event_Type=0;
		if(fgetc(FIN)!='M'||fgetc(FIN)!='T'||fgetc(FIN)!='r'||fgetc(FIN)!='k'){
			reterr("此midi文件非标准midi文件\n出错原因：音轨块标识符");
			return -2;
		}
		int Byte=Immutable_Length(FIN,4);
		int Read_Byte=0;
		while(Read_Byte<Byte){
			int Delta_Tick=Variable_Length(FIN,Read_Byte);
			Abs_Tick+=Delta_Tick;
			int data=fgetc(FIN);
			if(data>=0x80){
				Event_Type=data;
				Read_Byte++;
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
						Read_Byte+=2;
						int pitch=fgetc(FIN),velocity=fgetc(FIN);
						NOTEev ev(Abs_Tick,channel,Track,pitch,0,Instrument);
						note.push(ev);
						break;
					}
					case 0x9:{
						Read_Byte+=2;
						int pitch=fgetc(FIN),velocity=fgetc(FIN);
						NOTEev ev(Abs_Tick,channel,Track,pitch,velocity,Instrument);
						note.push(ev);
						break;
					}
					case 0xA:{
						Read_Byte+=2;
						fseek(FIN,2,SEEK_CUR);
						break;
					}
					case 0xB:{
						Read_Byte+=2;
						if(!fgetc(FIN))
							printf("音色库编号为:%d\n",fgetc(FIN));
						else
							fseek(FIN,1,SEEK_CUR);
						break;
					}
					case 0xC:{
						Read_Byte++;
						Instrument=fgetc(FIN);
						break;
					}
					case 0xD:{
						Read_Byte++;
						fseek(FIN,1,SEEK_CUR);
						break;
					}
					case 0xE:{
						Read_Byte+=2;
						fseek(FIN,2,SEEK_CUR);
						break;
					}
					case 0xF:{
						switch(Event_Type){
							case 0xF7:
							case 0xF0:{
								int len=Variable_Length(FIN,Read_Byte);
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
				Read_Byte++;
				int len=Variable_Length(FIN,Read_Byte);
				Read_Byte+=len;
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
	note.sort();
	bpm.sort();
	ISREAD=1;
	return 1;
}
int MidiFile::read(const char*filename){
	FILE*FIN=fopen(filename,"rb");
	int ret=read(FIN);
	fclose(FIN);
	return ret;
}
bool MidiFile::is_read(){
	return ISREAD;
}
int MidiFile::get_tpqn(){
	if(!ISREAD)
		return -1;
	return TPQN;
}
int MidiFile::get_fmt(){
	if(!ISREAD)
		return -1;
	return Format;
}
int MidiFile::get_ntrk(){
	if(!ISREAD)
		return -1;
	return Number_of_Tracks;
}
int MidiFile::get_note(NOTE&sth){
	if(!ISREAD)
		return -1;
	sth=note;
	return note.size();
}
int MidiFile::get_bpm(BPM&sth){
	if(!ISREAD)
		return -1;
	sth=bpm;
	return bpm.size();
}
int MidiFile::get_ts_n(){
	if(!ISREAD)
		return -1;
	return ts_nnn;
}
int MidiFile::get_ts_d(){
	if(!ISREAD)
		return -1;
	return ts_ddd;
}
MidiFile::~MidiFile(){
	
}

#endif
