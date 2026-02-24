#ifndef __GEV_H__
#define __GEV_H__ 
#include<cstdio>
#include<string>
#include<cstring>
#include<sstream> 
#include<stdlib.h>
namespace gev { 
	struct EVENT{ 
		const char*type; 
		int value[5]; 
		/**************************************** 
		type(const char*)				value(int)							the number of value(int)
		
		ts(time_signature)				tick,tickdvl,numerator,denominator	4 
		tpqn(ticks_per_quarter_note)	tpqn								1 
		tempo							tick,tickdvl,tempo					3 
		note							tick,tickdvl,channel,pitch,velocity	5 
		endtrack						channel								1 
		other							tick,tickdvl						2 
		end																	0
		***************************************/ 
	};
}; 
class GEV{ 
	private: 
		gev::EVENT event[2000],error;
		int tsn,tpqnn,tempon,noten,othern;
		int EVNUM; 
		bool judge(char s1[],int __begin,int __len,std::string s2);
		bool chcmpstr(char s1[],std::string s2);
		std::string asn(char s[],int __begin,int __len);
		char*stradd(const char*s1,const char*s2);
	public: 
		GEV();//initialize
		int evnum();//the number of event
		gev::EVENT getev(const int _idex,const char*type);//find the (_idex)th event in (type)
		int evin(const char*filename,const char*suffix); //read the events
		int evout(const char*gevfilename); //print the events
		bool evcmp(GEV oth);//compare this event and another one
		int gevout(const char*FIN,const char*FOUT);
}; 
int GEV::gevout(const char*FIN,const char*FOUT)
{
	char line[500]={'\0'}; 
	unsigned long long tick=0; 
	int channel=-1;
	FILE*r=fopen(stradd(FIN,".kev"),"r");
	FILE*w=fopen(stradd(FOUT,".gev"),"w");
	if(r==NULL)return -1;
	while(fscanf(r,"%[^\n]\n",line)!=EOF){ 
		char command[40]={'\0'}; 
		int end_cmdidex; 
		for(int i=0;i<strlen(line);i++){ 
			if(line[i]==':'&&line[i+1]=='{'){ 
				end_cmdidex=i+2; 
				break;
			} 
			command[i]=line[i]; 
		} 
		if(chcmpstr(command,"ticks_per_beat")){ 
			int begin_valueidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,6,"value:")&&begin_valueidex==0)begin_valueidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				}
			}
			int ticks_per_quarter_note=0; 
			std::string strtpqn=asn(line,begin_valueidex+6,begin_timeidex-begin_valueidex-7); 
			std::stringstream strmtpqn; 
			strmtpqn<<strtpqn; 
			strmtpqn>>ticks_per_quarter_note;
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			
			tpqnn++; 
			fprintf(w,"tpqn %d\n",ticks_per_quarter_note);
		} 
		else if(chcmpstr(command,"set_tempo")){ 
			int begin_tempoidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,6,"tempo:")&&begin_tempoidex==0)begin_tempoidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				} 
			} 
			int tempo=0; 
			std::string strtmp=asn(line,begin_tempoidex+6,begin_timeidex-begin_tempoidex-7); 
			std::stringstream strmtmp; 
			strmtmp<<strtmp; 
			strmtmp>>tempo; 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
		
			tempon++; 
			fprintf(w,"tempo %llu %d %d\n",tick,tickdvl,tempo);
		} 
		else if(chcmpstr(command,"time_signature")){ 
			int begin_numeratoridex=0,begin_denominatoridex=0,begin_clocks_per_clickidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,10,"numerator:")&&begin_numeratoridex==0)begin_numeratoridex=i; 
				else if(judge(line,i,12,"denominator:")&&begin_denominatoridex==0)begin_denominatoridex=i; 
				else if(judge(line,i,17,"clocks_per_click:")&&begin_clocks_per_clickidex==0)begin_clocks_per_clickidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}') { 
					end_lineidex=i; 
					break; 
				} 
			} 
			int numerator=0,denominator=0; 
			std::string strnumerator=asn(line,begin_numeratoridex+10,begin_denominatoridex-begin_numeratoridex-11); 
			std::stringstream strmnumerator; 
			strmnumerator<<strnumerator; 
			strmnumerator>>numerator; 
			std::string strdenominator=asn(line,begin_denominatoridex+12,begin_clocks_per_clickidex-begin_denominatoridex-13); 
			std::stringstream strmdenominator; 
			strmdenominator<<strdenominator; 
			strmdenominator>>denominator; 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			
			tsn++; 
			fprintf(w,"ts %llu %d %d %d\n",tick,tickdvl,numerator,denominator);
		} 
		else if(chcmpstr(command,"end_of_track")){ 
			tick=0; 
			fprintf(w,"endtrack %d\n",channel);
		} 
		else if(chcmpstr(command,"note_on")||chcmpstr(command,"note_off")){ 
			int begin_channelidex=0,begin_noteidex=0,begin_velocityidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,8,"channel:")&&begin_channelidex==0)begin_channelidex=i; 
				else if(judge(line,i,5,"note:")&&begin_noteidex==0)begin_noteidex=i; 
				else if(judge(line,i,9,"velocity:")&&begin_velocityidex==0)begin_velocityidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				} 
			} 
			std::string strchannel=asn(line,begin_channelidex+8,begin_noteidex-begin_channelidex-9); 
			std::stringstream strmchannel; 
			channel=0; 
			strmchannel<<strchannel; 
			strmchannel>>channel; 
			std::string strnote=asn(line,begin_noteidex+5,begin_velocityidex-begin_noteidex-6); 
			std::stringstream strmnote; 
			short note=0; 
			strmnote<<strnote; 
			strmnote>>note; 
			short velocity=0; 
			if(chcmpstr(command,"note_on")){ 
				std::string strvelocity=asn(line,begin_velocityidex+9,begin_timeidex-begin_velocityidex-10); 
				std::stringstream strmvelocity; 
				strmvelocity<<strvelocity; 
				strmvelocity>>velocity; 
			} 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			
			noten++; 
			fprintf(w,"note %llu %d %d %d %d\n",tick,tickdvl,channel,note,velocity);
		}
//		if( 
//			chcmpstr(command,"identity")|| 
//			chcmpstr(command,"key_signature")|| 
//			chcmpstr(command,"track_name")|| 
//			chcmpstr(command,"control_change")|| 
//			chcmpstr(command,"program_change")|| 
//			chcmpstr(command,"text")||
//			chcmpstr(command,"pitchwheel")
//		) 
		else{ 
			int begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,5,"time:")&&begin_timeidex==0) 
					begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				} 
			} 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			
			fprintf(w,"other %llu %d\n",tick,tickdvl);
		} 
		memset(command,'\0',sizeof(command)); 
		memset(line,'\0',sizeof(line)); 
	}
	
	fprintf(w,"end");
	fclose(r); 
	fclose(w);
}
bool GEV::judge(char s1[],int __begin,int __len,std::string s2){ 
	int idex=0; 
	for(int i=__begin;i<__begin+__len;i++) 
		if(s2[idex++]!=s1[i]) 
			return 0; 
	return 1; 
}
bool GEV::chcmpstr(char s1[],std::string s2){
	if(strlen(s1)!=s2.length())return 0; 
	for(int i=0;i<strlen(s1);i++) 
		if(s1[i]!=s2[i]) 
			return 0; 
	return 1; 
} 
std::string GEV::asn(char s[],int __begin,int __len){ 
	std::string out=""; 
	for(int i=__begin;i<__begin+__len;i++)out+=s[i]; 
	return out; 
}
char*GEV::stradd(const char*s1,const char*s2){
	char*str=(char*)malloc(strlen(s1)+strlen(s2)+1);
	strcpy(str,s1);
	strcat(str,s2);
	return str;
}
GEV::GEV(){ 
	error.type="error"; 
	error.value[0]=-1; 
	error.value[1]=-1; 
	error.value[2]=-1; 
	error.value[3]=-1; 
	error.value[4]=-1; 
	tsn=0; 
	tpqnn=0; 
	tempon=0; 
	noten=0; 
	othern=0; 
	EVNUM=0; 
}; 
gev::EVENT GEV::getev(const int _idex,const char*type){ 
	if(type=="all")return (_idex<=0||_idex>EVNUM)?error:event[_idex];
	if( 
		_idex<=0|| 
		(type!="ts"&&type!="tpqn"&&type!="tempo"&&type!="note"&&type!="other")|| 
		(type=="ts"&&_idex>tsn)|| 
		(type=="tpqn"&&_idex>tpqnn)|| 
		(type=="tempo"&&_idex>tempon)|| 
		(type=="note"&&_idex>noten)|| 
		(type=="other"&&_idex>othern) 
	)return error; 
	int ridex=0; 
	for(int i=1;i<=_idex;i++)while(type!=event[++ridex].type); 
	return event[ridex]; 
}
int GEV::evin(const char*filename,const char*suffix){
	if(suffix!="gev"&&suffix!="kev")return -2;
	if(suffix=="gev"){
		FILE*r=fopen(stradd(filename,".gev"),"r"); 
		if(r==NULL)return -1;
		int i;
		for(i=1;;i++){
			char*type=(char*)malloc(20);
			if(fscanf(r,"%s",type)==EOF)break;
			event[i].type=(const char*)type;
			if(!strcmp(event[i].type,"end"))break;
			if(!strcmp(event[i].type,"other"))continue;
			fscanf(r,"%llu %d",&event[i].value[0],&event[i].value[1]); 
			if(!strcmp(event[i].type,"ts")) 
				for(int j=2;j<4;j++) 
					fscanf(r,"%d",&event[i].value[j]); 
			else if(!strcmp(event[i].type,"tpqn")) 
				for(int j=2;j<2;j++) 
					fscanf(r,"%d",&event[i].value[j]); 
			else if(!strcmp(event[i].type,"tempo")) 
				for(int j=2;j<3;j++) 
					fscanf(r,"%d",&event[i].value[j]); 
			else if(!strcmp(event[i].type,"note")) 
				for(int j=2;j<5;j++) 
					fscanf(r,"%d",&event[i].value[j]); 
			else if(!strcmp(event[i].type,"endtrack")) 
				for(int j=2;j<2;j++) 
					fscanf(r,"%d",&event[i].value[j]);
			fscanf(r,"\n");
		}
		EVNUM=i;
		fclose(r);
		return 1;
	}
	char line[500]={'\0'}; 
	unsigned long long tick=0; 
	int channel=-1;
	FILE*r=fopen(stradd(filename,".kev"),"r");
	if(r==NULL)return -1;
	while(fscanf(r,"%[^\n]\n",line)!=EOF){ 
		EVNUM++; 
		char command[40]={'\0'}; 
		int end_cmdidex; 
		for(int i=0;i<strlen(line);i++){ 
			if(line[i]==':'&&line[i+1]=='{'){ 
				end_cmdidex=i+2; 
				break;
			} 
			command[i]=line[i]; 
		} 
		if(chcmpstr(command,"ticks_per_beat")){ 
			int begin_valueidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,6,"value:")&&begin_valueidex==0)begin_valueidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				}
			}
			int ticks_per_quarter_note=0; 
			std::string strtpqn=asn(line,begin_valueidex+6,begin_timeidex-begin_valueidex-7); 
			std::stringstream strmtpqn; 
			strmtpqn<<strtpqn; 
			strmtpqn>>ticks_per_quarter_note;
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			event[EVNUM].type="tpqn"; 
			event[EVNUM].value[0]=ticks_per_quarter_note; 
			event[EVNUM].value[1]=-1; 
			event[EVNUM].value[2]=-1;
			event[EVNUM].value[3]=-1; 
			event[EVNUM].value[4]=-1; 
			tpqnn++; 
		} 
		else if(chcmpstr(command,"set_tempo")){ 
			int begin_tempoidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,6,"tempo:")&&begin_tempoidex==0)begin_tempoidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				} 
			} 
			int tempo=0; 
			std::string strtmp=asn(line,begin_tempoidex+6,begin_timeidex-begin_tempoidex-7); 
			std::stringstream strmtmp; 
			strmtmp<<strtmp; 
			strmtmp>>tempo; 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			event[EVNUM].type="tempo"; 
			event[EVNUM].value[0]=tick; 
			event[EVNUM].value[1]=tickdvl; 
			event[EVNUM].value[2]=tempo; 
			event[EVNUM].value[3]=-1; 
			event[EVNUM].value[4]=-1; 
			tempon++; 
		} 
		else if(chcmpstr(command,"time_signature")){ 
			int begin_numeratoridex=0,begin_denominatoridex=0,begin_clocks_per_clickidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,10,"numerator:")&&begin_numeratoridex==0)begin_numeratoridex=i; 
				else if(judge(line,i,12,"denominator:")&&begin_denominatoridex==0)begin_denominatoridex=i; 
				else if(judge(line,i,17,"clocks_per_click:")&&begin_clocks_per_clickidex==0)begin_clocks_per_clickidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}') { 
					end_lineidex=i; 
					break; 
				} 
			} 
			int numerator=0,denominator=0; 
			std::string strnumerator=asn(line,begin_numeratoridex+10,begin_denominatoridex-begin_numeratoridex-11); 
			std::stringstream strmnumerator; 
			strmnumerator<<strnumerator; 
			strmnumerator>>numerator; 
			std::string strdenominator=asn(line,begin_denominatoridex+12,begin_clocks_per_clickidex-begin_denominatoridex-13); 
			std::stringstream strmdenominator; 
			strmdenominator<<strdenominator; 
			strmdenominator>>denominator; 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			event[EVNUM].type="ts"; 
			event[EVNUM].value[0]=tick; 
			event[EVNUM].value[1]=tickdvl; 
			event[EVNUM].value[2]=numerator; 
			event[EVNUM].value[3]=denominator; 
			event[EVNUM].value[4]=-1; 
			tsn++; 
		} 
		else if(chcmpstr(command,"end_of_track")){ 
			event[EVNUM].type="endtrack"; 
			event[EVNUM].value[0]=channel; 
			event[EVNUM].value[1]=-1; 
			event[EVNUM].value[2]=-1; 
			event[EVNUM].value[3]=-1; 
			event[EVNUM].value[4]=-1; 
			tick=0; 
		} 
		else if(chcmpstr(command,"note_on")||chcmpstr(command,"note_off")){ 
			int begin_channelidex=0,begin_noteidex=0,begin_velocityidex=0,begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,8,"channel:")&&begin_channelidex==0)begin_channelidex=i; 
				else if(judge(line,i,5,"note:")&&begin_noteidex==0)begin_noteidex=i; 
				else if(judge(line,i,9,"velocity:")&&begin_velocityidex==0)begin_velocityidex=i; 
				else if(judge(line,i,5,"time:")&&begin_timeidex==0)begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				} 
			} 
			std::string strchannel=asn(line,begin_channelidex+8,begin_noteidex-begin_channelidex-9); 
			std::stringstream strmchannel; 
			channel=0; 
			strmchannel<<strchannel; 
			strmchannel>>channel; 
			std::string strnote=asn(line,begin_noteidex+5,begin_velocityidex-begin_noteidex-6); 
			std::stringstream strmnote; 
			short note=0; 
			strmnote<<strnote; 
			strmnote>>note; 
			short velocity=0; 
			if(chcmpstr(command,"note_on")){ 
				std::string strvelocity=asn(line,begin_velocityidex+9,begin_timeidex-begin_velocityidex-10); 
				std::stringstream strmvelocity; 
				strmvelocity<<strvelocity; 
				strmvelocity>>velocity; 
			} 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			event[EVNUM].type="note"; 
			event[EVNUM].value[0]=tick; 
			event[EVNUM].value[1]=tickdvl; 
			event[EVNUM].value[2]=channel; 
			event[EVNUM].value[3]=note; 
			event[EVNUM].value[4]=velocity; 
			noten++; 
		}
//		if( 
//			chcmpstr(command,"identity")|| 
//			chcmpstr(command,"key_signature")|| 
//			chcmpstr(command,"track_name")|| 
//			chcmpstr(command,"control_change")|| 
//			chcmpstr(command,"program_change")|| 
//			chcmpstr(command,"text")||
//			chcmpstr(command,"pitchwheel")
//		) 
		else{ 
			int begin_timeidex=0,end_lineidex=0; 
			for(int i=end_cmdidex;i<strlen(line);i++){ 
				if(judge(line,i,5,"time:")&&begin_timeidex==0) 
					begin_timeidex=i; 
				else if(line[i]=='}'){ 
					end_lineidex=i; 
					break; 
				} 
			} 
			std::string strtick=asn(line,begin_timeidex+5,end_lineidex-begin_timeidex-5); 
			std::stringstream strmtick; 
			long tickdvl=0; 
			strmtick<<strtick; 
			strmtick>>tickdvl; 
			tick+=tickdvl; 
			event[EVNUM].type="other"; 
			event[EVNUM].value[0]=tick; 
			event[EVNUM].value[1]=tickdvl; 
			event[EVNUM].value[2]=-1; 
			event[EVNUM].value[3]=-1; 
			event[EVNUM].value[4]=-1; 
		} 
		memset(command,'\0',sizeof(command)); 
		memset(line,'\0',sizeof(line)); 
	}
	event[++EVNUM].type="end"; 
	event[EVNUM].value[0]=-1; 
	event[EVNUM].value[1]=-1; 
	event[EVNUM].value[2]=-1;
	event[EVNUM].value[3]=-1; 
	event[EVNUM].value[4]=-1; 
	fclose(r); 
	return 1;
} 
int GEV::evout(const char*gevfilename){ 
	if(EVNUM==0)return -1; 
	FILE*w=fopen(gevfilename,"w"); 
	int tickdvl=0; 
	for(int i=1;i<=EVNUM;i++){
		tickdvl+=event[i].value[1]; 
		fprintf(w,"%s",event[i].type);
		if(!strcmp(event[i].type,"other")){
			fprintf(w,"\n");
			continue;
		} 
		if(!strcmp(event[i].type,"end"))break;
		fprintf(w," %llu %d",event[i].value[0],event[i].value[1]); 
		if(!strcmp(event[i].type,"ts")) 
			for(int j=2;j<4;j++) 
				fprintf(w," %d",event[i].value[j]); 
		else if(!strcmp(event[i].type,"tpqn")) 
			for(int j=2;j<2;j++) 
				fprintf(w," %d",event[i].value[j]); 
		else if(!strcmp(event[i].type,"tempo")) 
			for(int j=2;j<3;j++) 
				fprintf(w," %d",event[i].value[j]); 
		else if(!strcmp(event[i].type,"note")) 
			for(int j=2;j<2;j++) 
				fprintf(w," %d",event[i].value[j]); 
		else if(!strcmp(event[i].type,"endtrack")) 
			for(int j=2;j<3;j++) 
				fprintf(w," %d",event[i].value[j]); 
		fprintf(w,"\n"); 
		tickdvl=0;
	}
	fclose(w); 
	return 1;
}
int GEV::evnum(){ 
	return EVNUM; 
}
bool GEV::evcmp(GEV oth){
	if(this->EVNUM!=oth.EVNUM)return 0;
	for(int i=1;i<=EVNUM;i++){
		if(strcmp(this->event[i].type,oth.event[i].type))return 0;
		for(int j=0;j<=4;j++)
			if(this->event[i].value[j]!=oth.event[i].value[j])return 0;
	}
	return 1;
}
#endif 
