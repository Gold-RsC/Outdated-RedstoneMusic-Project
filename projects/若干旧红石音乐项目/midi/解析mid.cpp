//目测系统码出错！！！ 
#include<stdio.h>
#include<math.h>
#define MIDI_ERROR 0xEE
using namespace std;
int reterror(const char*str)
{
	printf("小问号你是否有很多朋友\n出错内容：%s\n--------------\n",str);
	return MIDI_ERROR;
}
int Header_Chunk_Type(FILE*FIN)
{
	if(fgetc(FIN)!='M')
	{
		reterror("头块标记出错");
		return -1; 
	}
	if(fgetc(FIN)!='T')
	{
		reterror("头块标记出错");
		return -1;
	}
	char a=fgetc(FIN),b=fgetc(FIN);
	if(
		a=='h'&&b=='d'
	){
		return 0;
	}
	else if(
		a=='r'&&b=='k'
	){
		return 1;
	}
	else
	{
		reterror("头块标记出错");
		return -1;
	}
}
bool Header_Chunk_Length(FILE*FIN)
{
	int a=0;
	for(int i=1;i<=4;i++)
		a=a*256+fgetc(FIN);
	if(a==6)
	{
		return 1;
	}
	else
	{
		reterror("头块长度必为6");
		return 0;
	}
}
bool Header_Chunk_Format(FILE*FIN,FILE*FOUT)
{
	if(fgetc(FIN)!=0)
	{
		reterror("头块格式错误");
		return 0;
	}
	switch(fgetc(FIN))
	{
		case 0:
			fprintf(FOUT,"单音轨\n");
			return 1;
		case 1:
			fprintf(FOUT,"多音轨，且同步\n");
			return 1;
		case 2:
			fprintf(FOUT,"多音轨，但不同步\n");
			return 1;
		default:
			reterror("头块格式错误");
			return 0;
	}
}
int Header_Chunk_Track_Number(FILE*FIN,FILE*FOUT)
{
	int len=fgetc(FIN)*256+fgetc(FIN);
	fprintf(FOUT,"文件中有%d个音轨块\n",len);
	return len;
}
int Header_Chunk_Division(FILE*FIN,FILE*FOUT)
{
	int a=fgetc(FIN),b=fgetc(FIN);
	if(a>0)
	{
		switch(a*256+b)
		{
			case 120:
			case 240:
			case 360:
			case 480:
			case 96:
				fprintf(FOUT,"tick per quarter note:%d\n",a*256+b);
				return 1;
			default:
				reterror("tpqn出错");
				fprintf(FOUT,"tick per quarter note:%d\n",a*256+b);
				return -1;
		}
	}
	else
	{
		reterror("因为我不会（SMPTE），所以记为120(吐舌)");
		fprintf(FOUT,"tick per quarter note:120\n");
		return 2;
	}
}
int Variable_Length_Quantity(FILE*FIN)
{
	int ret=0;
	while(ret<=0xFFFFFFF)
	{
		ret*=128;
		int a=fgetc(FIN);
		ret+=a;
		if(a<128)
			break;
		ret-=128;
	}
	return ret;
}
int Meta_Event(FILE*FIN,FILE*FOUT)
{
	int type=fgetc(FIN);
	
	int len;
	switch(type)
	{
		case 0x00:
			return type;
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			len=Variable_Length_Quantity(FIN);
			fprintf(FOUT,"text:");
			for(int i=1;i<=len;i++)
				fprintf(FOUT,"%c",fgetc(FIN));
			fprintf(FOUT,"\n");
			return type;
		case 0x20:
			len=fgetc(FIN);
			if(len!=1)
			{
				return reterror("元事件type为0x20时字符长度一定为1");
			}
			fprintf(FOUT,"通道：%d\n",fgetc(FIN));
			return type;
		case 0x21:
			len=fgetc(FIN);
			if(len!=1)
			{
				return reterror("元事件type为0x21时字符长度一定为1");
			}
			fprintf(FOUT,"接口号吗：%d\n",fgetc(FIN));
			return type;
		case 0x2F:
			len=fgetc(FIN);
			if(len!=0)
			{
				return reterror("元事件type为0x2F时字符长度一定为0");
			}
			fprintf(FOUT,"end of track\n");
			return type;
		case 0x51:
			len=fgetc(FIN);
			if(len!=3)
			{
				return reterror("元事件type为0x51时字符长度一定为3");
				
			}
			fprintf(FOUT,"速度：%llu\n",fgetc(FIN)*256*256+fgetc(FIN)*256+fgetc(FIN));
			return type;
		case 0x54:
			len=fgetc(FIN);
			if(len!=5)
			{
				return reterror("元事件type为0x54时字符长度一定为5");
				
			}
			fprintf(FOUT,"SMPTE时间，时：%d，分%d，秒%d，帧%d，复帧%d\n",fgetc(FIN),fgetc(FIN),fgetc(FIN),fgetc(FIN),fgetc(FIN));
			return type;
		case 0x58:
			len=fgetc(FIN);
			if(len!=4)
			{
				return reterror("元事件type为0x58时字符长度一定为4");
				
			}
			fprintf(FOUT,"节拍，分子%d，分母%d，节拍器时钟%d，一个四分音符包括的三十二分音符的个数%d\n",fgetc(FIN),fgetc(FIN),fgetc(FIN),fgetc(FIN));
			return type;
		case 0x59:
			len=fgetc(FIN);
			if(len!=2)
			{
				return reterror("元事件type为0x59时字符长度一定为2");
				
			}
			fprintf(FOUT,"调号 升降号数%d 大小调%d\n",fgetc(FIN),fgetc(FIN));
			return type;
		case 0x7F:
			len=Variable_Length_Quantity(FIN);
			fprintf(FOUT,"text:");
			for(int i=1;i<=len;i++)
				fprintf(FOUT,"%c",fgetc(FIN));
			fprintf(FOUT,"\n");
			return type;
		default:
			reterror("鬼知道这元事件类型是啥");
	}
}
void Sysex_Event(FILE*FIN,FILE*FOUT)
{
	fprintf(FOUT,"系统码，data:");
	
	int len=fgetc(FIN);
	for(int i=1;i<=len;i++)
		fprintf(FOUT,"%X ",fgetc(FIN));
	fprintf(FOUT,"\n");
}
int last_type;
int Midi_Event(FILE*FIN,FILE*FOUT)
{
	int type=fgetc(FIN);
	
	
	if(type<=0x7F)
	{
		fprintf(FOUT,"事件类型：%X\t",last_type);
		switch(last_type/16)
		{
			case 0x8:
				fprintf(FOUT,"松开音符，音符：%d，力度：%d\n",type,fgetc(FIN));
				
				return last_type; 
			case 0x9:
				fprintf(FOUT,"按下音符，音符：%d，力度：%d\n",type,fgetc(FIN));
				
				return last_type;
			case 0xA:
				fprintf(FOUT,"key after touch，音符：%d，力度：%d\n",type,fgetc(FIN));
				
				return last_type;
			case 0xB:
				fprintf(FOUT,"controller，control：%d，value：%d\n",type,fgetc(FIN));
				
				return last_type;
			case 0xC:
				fprintf(FOUT,"program changes，乐器号码：%d\n",type);
				
				return last_type;
			case 0xD:
				fprintf(FOUT,"aftertouch，值：%d\n",type);
				
				return last_type;
			case 0xE:
				
				fprintf(FOUT,"滑音，低位：%d，高位%d\n",type,fgetc(FIN));
				
				return last_type;
			case 0xF:
				if(type==0xF0||type==0xF7)
				{
					Sysex_Event(FIN,FOUT);
					
					return last_type;
				}
				else if(type==0xFF)
				{
					
					return 0xFF00+Meta_Event(FIN,FOUT);
					
				}
				else
				{
					char errorstr[100];
					sprintf(errorstr,"鬼知道%X是什么",type);
					return reterror(errorstr); 
				}
				
			default:
				char errorstr[100];
				sprintf(errorstr,"鬼知道%X是什么",type);
				return reterror(errorstr); 
		}
	}
	fprintf(FOUT,"事件类型：%X\t",type);
	switch(type/16)
	{
		case 0x8:
			fprintf(FOUT,"松开音符，音符：%d，力度：%d\n",fgetc(FIN),fgetc(FIN));
			last_type=type;
			return type; 
		case 0x9:
			fprintf(FOUT,"按下音符，音符：%d，力度：%d\n",fgetc(FIN),fgetc(FIN));
			last_type=type;
			return type;
		case 0xA:
			fprintf(FOUT,"key after touch，音符：%d，力度：%d\n",fgetc(FIN),fgetc(FIN));
			last_type=type;
			return type;
		case 0xB:
			fprintf(FOUT,"controller，control：%d，value：%d\n",fgetc(FIN),fgetc(FIN));
			last_type=type;
			return type;
		case 0xC:
			fprintf(FOUT,"program changes，乐器号码：%d\n",fgetc(FIN));
			last_type=type;
			return type;
		case 0xD:
			fprintf(FOUT,"aftertouch，值：%d\n",fgetc(FIN));
			last_type=type;
			return type;
		case 0xE:
			
			fprintf(FOUT,"滑音，低位：%d，高位%d\n",fgetc(FIN),fgetc(FIN));
			last_type=type;
			return type;
		case 0xF:
			if(type==0xF0||type==0xF7)
			{
				Sysex_Event(FIN,FOUT);
				last_type=type;
				return type;
			}
			else if(type==0xFF)
			{
				last_type=type;
				return 0xFF00+Meta_Event(FIN,FOUT);
				
			}
			else
			{
				char errorstr[100];
				sprintf(errorstr,"鬼知道%X是什么",type);
				return reterror(errorstr); 
			}
			
		default:
			char errorstr[100];
			sprintf(errorstr,"鬼知道%X是什么",type);
			return reterror(errorstr); 
	}
}
int main()
{
	FILE*r=fopen("追梦赤子心.mid","rb");
	FILE*w=fopen("ddddd.txt","w");
	
	Header_Chunk_Type(r);
	Header_Chunk_Length(r);
	Header_Chunk_Format(r,w);
	int track=Header_Chunk_Track_Number(r,w);
	Header_Chunk_Division(r,w);
	
	for(int i=1;i<=track;i++)
	{
		Header_Chunk_Type(r);
		for(int j=1;j<=4;j++)
			fgetc(r);
		int midievent;
		do
		{
			fprintf(w,"时间差：%d\t",Variable_Length_Quantity(r));
			midievent=Midi_Event(r,w);
			if(midievent==0xFF2F)break;
			else if(midievent%256==0xEE)
			{
				printf("%X",midievent);
				return 0;
			}
		}while(midievent!=0xFF2F);
	}
	fclose(r);
	fclose(w);
	return 0;
}


















