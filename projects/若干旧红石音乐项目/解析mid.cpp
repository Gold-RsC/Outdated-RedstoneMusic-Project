#include<stdio.h>
#include<math.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;
int Number_of_Tracks,Event_Type;
void reterror(const char*str)
{
	printf("小问号你是否有很多朋友\n出错内容：%s\n--------------\n",str);
}
const char*Chunk_Type(FILE*FIN,FILE*FOUT)
{
	if(fgetc(FIN)!='M')
	{
		reterror("头块标记出错");
		return NULL; 
	}
	if(fgetc(FIN)!='T')
	{
		reterror("头块标记出错");
		return NULL;
	}
	unsigned char a=fgetc(FIN),b=fgetc(FIN);
	if(
		a=='h'&&b=='d'
	){
		fprintf(FOUT,"MThd头块\n");
		return "MThd";
	}
	else if(
		a=='r'&&b=='k'
	){
		fprintf(FOUT,"MTrk头块\n");
		return "MTrk";
	}
	else
	{
		reterror("头块标记出错");
		return NULL;
	}
}

int Immutable_Length(FILE*FIN,int n)
{
	int len=0;
	for(int i=1;i<=n;i++)
		len=len*256+fgetc(FIN);
	return len;
}
int Variable_Length(FILE*FIN,int&len)
{
	int ret=0;
	for(int i=1;i<=5;i++)
	{
		ret*=128;
		int a=fgetc(FIN);
		ret+=a;
		if(a<128)
		{
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
bool Header_Chunk_Data_Length(FILE*FIN)
{
	if(Immutable_Length(FIN,4)!=6)
	{
		reterror("头块数据长度必须为6");
		return 0;
	}
	return 1;
}
int Header_Chunk_Format(FILE*FIN,FILE*FOUT)
{
	if(fgetc(FIN)!=0)
	{
		reterror("头块格式错误");
		return -1;
	}
	switch(fgetc(FIN))
	{
		case 0:
			fprintf(FOUT,"单音轨\n");
			return 0;
		case 1:
			fprintf(FOUT,"多音轨，且同步\n");
			return 1;
		case 2:
			fprintf(FOUT,"多音轨，但不同步\n");
			return 2;
		default:
			reterror("头块格式错误");
			return -1;
	}
}
bool Header_Chunk_Division(FILE*FIN,FILE*FOUT)
{
	int a=fgetc(FIN),b=fgetc(FIN);
	if(a>0)
	{
		if(a*256+b>480||a*256+b<48)
		{
			reterror("tpqn出错");
			return 0;
		}
		fprintf(FOUT,"tick per quarter note:%d\n",a*256+b);
		return 1;
	}
	else
	{
		reterror("因为我不会SMPTE，所以将tpqn记为120(吐舌)\n但实际情况确实需要记为120(皱眉)");
		fprintf(FOUT,"tick per quarter note:120\n");
		return 1;
	}
}
int Header_Chunk_Track_Number(FILE*FIN,FILE*FOUT)
{
	Number_of_Tracks=Immutable_Length(FIN,2);
	fprintf(FOUT,"文件中有%d个音轨块\n",Number_of_Tracks);
	return Number_of_Tracks;
}
int Header_Chunk_Data(FILE*FIN,FILE*FOUT)
{
	if(Header_Chunk_Format(FIN,FOUT)==-1)
		return 0;
	Header_Chunk_Track_Number(FIN,FOUT);
	if(!Header_Chunk_Division(FIN,FOUT))
		return 0;
	return 1;
}
bool Header_Chunk(FILE*FIN,FILE*FOUT)
{
	if(stricmp(Chunk_Type(FIN,FOUT),"MThd"))
		return 0;
	if(!Header_Chunk_Data_Length(FIN))
		return 0;
	if(!Header_Chunk_Data(FIN,FOUT))
		return 0;
	return 1;
}
int Track_Chunk_Data_Length(FILE*FIN){return Immutable_Length(FIN,4);}
int Delta_Tick(FILE*FIN,FILE*FOUT,int&Read_Len)
{
	int len=0;
	int datalen=Variable_Length(FIN,len);
	Read_Len+=len;
	return datalen;
}

int General_Event(FILE*FIN,FILE*FOUT,int&Read_Len)
{
	fprintf(FOUT,"channel:%d\tevent:",Event_Type%0x10);
	switch(Event_Type/0x10)
	{
		case 0x8:
			fprintf(FOUT,"note_off\tvelocity:%d\tpitch:%d\n",fgetc(FIN),fgetc(FIN));
			Read_Len+=2;
			break;
		case 0x9:
			fprintf(FOUT,"note_on\tvelocity:%d\tpitch:%d\n",fgetc(FIN),fgetc(FIN));
			Read_Len+=2;
			break;
		case 0xA:
			fprintf(FOUT,"key_after_touch\tpitch:%d\tvelocity:%d\n",fgetc(FIN),fgetc(FIN));
			Read_Len+=2;
			break;
		case 0xB:
			fprintf(FOUT,"controller\tcontrol:%d\tvalue:%d\n",fgetc(FIN),fgetc(FIN));
			Read_Len+=2;
			break;
		case 0xC:
			fprintf(FOUT,"program_changes\tvalue:%d\n",fgetc(FIN));
			Read_Len++;
			break;
		case 0xD:
			fprintf(FOUT,"aftertouch\tvalue:%d\n",fgetc(FIN));
			Read_Len++;
			break;
		case 0xE:
			fprintf(FOUT,"pitchwheel\tpitch:%d %d\n",fgetc(FIN),fgetc(FIN));
			Read_Len+=2;
			break;
		default:
			char str[100];
			sprintf(str,"未知的状态符：%X",Event_Type);
			reterror(str);
			return 0;
	}
	return Event_Type;
}
int Meta_Event(FILE*FIN,FILE*FOUT,int&Read_Len)
{
	if(Event_Type!=0xFF)
	{
		char str[100];
		sprintf(str,"未知状态符：%X",Event_Type);
		reterror(str);
		return 0;
	}
	int type=fgetc(FIN);
	Read_Len++;
	int len=Variable_Length(FIN,Read_Len);
	Read_Len+=len;
	fprintf(FOUT,"Metaevent\ttype:%X\t",type);
	switch(type)
	{
		case 0x0:
			fprintf(FOUT,"设置轨道音序\t音序号:%X %X\n",fgetc(FIN),fgetc(FIN));
			break;
		case 0x1:
			fprintf(FOUT,"歌曲备注or音轨文本\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x2:
			fprintf(FOUT,"歌曲版权\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x3:
			fprintf(FOUT,"歌曲标题or音轨名称\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x4:
			fprintf(FOUT,"乐器名称\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x5:
			fprintf(FOUT,"歌词\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x6:
			fprintf(FOUT,"标记(用文本标记)\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x7:
			fprintf(FOUT,"开始点(用文本记录开始点)\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x8:
			fprintf(FOUT,"歌曲文件名\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x9:
			fprintf(FOUT,"设备名\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		case 0x20:
			fprintf(FOUT,"midi通道\tvalue:%d\n",fgetc(FIN));
			break;
		case 0x21:
			fprintf(FOUT,"接口号码\tvalue:%d\n",fgetc(FIN));
			break;
		case 0x2F:
			fprintf(FOUT,"音轨结束标志\n");
			break;
		case 0x51:
			fprintf(FOUT,"速度\tvalue:%d\n",fgetc(FIN)*256*256+fgetc(FIN)*256+fgetc(FIN));
			break;
		case 0x54:
			fprintf(FOUT,"SMPTE开始的时间\thour:%d\tminute:%d\tsecond:%d\tframe:%d\tMultiframe:%d\n",fgetc(FIN),fgetc(FIN),fgetc(FIN),fgetc(FIN),fgetc(FIN));
			break;
		case 0x58:
			fprintf(FOUT,"节拍Time_Signature\tnumerator:%d\tdenominator:%d\t节拍器时钟:%d\t一个4分音符包括的32分音符的个数:%d\n",fgetc(FIN),(int)pow(2,fgetc(FIN)),fgetc(FIN),fgetc(FIN));
			break;
		case 0x59:
			fprintf(FOUT,"调号Key_Signature\t共标注升号or降号的数目:%d\t%s\n",fgetc(FIN),fgetc(FIN)?"小调":"大调");
			break;
		case 0x7F:
			fprintf(FOUT,"音符特定信息\ttext:");
			for(int i=1;i<=len;i++)
			{
				fprintf(FOUT,"%c",fgetc(FIN));
			}
			fprintf(FOUT,"\n");
			break;
		default:
			char str[100];
			sprintf(str,"未知元事件类型：%X",type);
			reterror(str);
			return 0;
	}
	return type;
}
int Sysex_Event(FILE*FIN,FILE*FOUT,int&Read_Len)
{
	if(Event_Type!=0xF7&&Event_Type!=0xF0)
	{
		char str[100];
		sprintf(str,"未知状态符：%X",Event_Type);
		reterror(str);
		return 0;
	}
	int len=Variable_Length(FIN,Read_Len);
	Read_Len+=len;
	fprintf(FOUT,"sysex event\tvalue:");
	for(int i=1;i<=len;i++)
		fprintf(FOUT,"%X ",fgetc(FIN));
	fprintf(FOUT,"\n");
	return 1;
}
int Midi_Event(FILE*FIN,FILE*FOUT,int&Read_Len)
{
	int data=fgetc(FIN);
	
	if(data>=0x80)
	{
		Event_Type=data;
		Read_Len++;
	}
	else
	{
		fseek(FIN,-1,SEEK_CUR);
	}
	
	if(Event_Type>=0x80&&Event_Type<=0xEF)
	{
		return General_Event(FIN,FOUT,Read_Len);
	}
	else if(Event_Type==0xFF)
	{
		return Meta_Event(FIN,FOUT,Read_Len);
	}
	else if(Event_Type==0xF0||Event_Type==0xF7)
	{
		return Sysex_Event(FIN,FOUT,Read_Len); 
	}
	else
	{
		char str[100];
		sprintf(str,"不可省略的状态符或未知的状态符：%X",Event_Type);
		reterror(str);
		return 0;
	}
}
int MTrk_Event(FILE*FIN,FILE*FOUT,int&Read_Len)
{
	Delta_Tick(FIN,FOUT,Read_Len);
	if(!Midi_Event(FIN,FOUT,Read_Len))
		return 0;
	
}
bool Track_Chunk_Data(FILE*FIN,FILE*FOUT,int len)
{
	int Read_Len=0;
	while(Read_Len<len)
	{
		MTrk_Event(FIN,FOUT,Read_Len);
	}
	if(Read_Len>len)
	{
		printf("%d %d\n",Read_Len,len);
		reterror("已读取字符数大于预设字符数"); 
		return 0;
	}
	return 1;
}
bool Track_Chunk(FILE*FIN,FILE*FOUT)
{
	if(stricmp(Chunk_Type(FIN,FOUT),"MTrk"))
		return 0;
	Track_Chunk_Data(FIN,FOUT,Track_Chunk_Data_Length(FIN));
	
	
	
	
	
	
	
	
	
	
	return 1;
}
int main()
{
//	FILE*r=fopen("midi\\恋爱循环.mid","rb");
//	FILE*w=fopen("magic\\A.txt","w");
//	if(!Header_Chunk(r,w))
//		return 0;
//	for(int i=1;i<=Number_of_Tracks;i++)
//	{
//		if(!Track_Chunk(r,w))
//			return 0;
//	}
//	fclose(r);
//	fclose(w);
	
	
	return 0;
}
