#define MIDI_DEBUG
#include"MidiPlay.hpp"
#include<iostream>
using namespace GoldType;
using namespace MidiParse;
using namespace std;
string filename[10];
//int n=0;
uchar pitch[128];
void fun1(){
	MidiFun a("13.mid");
    a.change_allTimeState();
    NoteList c=a.noteList();
    sort(c.begin(),c.end());
    NotePairList d,b;
    d.match(c);
    for(const auto&p:d){
//    	if(p.channel()==2&&p.time()>=19578949llu){
    		b.emplace_back(p);
//		}
	}
	for(auto&p:b){
		p.time()=(p.time()-b[0].time())/1000;
		p.duration()/=1000;
	}
    FILE*f=fopen("esp_write_cmd.txt","w");
    if(b.size()){
    	fprintf(f,"    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);\n"
				  "    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);\n");
    	fprintf(f,"    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0,%u);\n",uint(440*pow(2,(b[0].pitch()-69)/12.0)));
    	fprintf(f,"    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 128);\n"
				  "    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);\n");
	}
	for(int i=1;i<b.size();++i){
		if(b[i].time()-b[i-1].time()-b[i-1].duration()>20){
			fprintf(f,"    vTaskDelay(pdMS_TO_TICKS(%llu));\n",b[i-1].duration());
		}
		else{
			fprintf(f,"    vTaskDelay(pdMS_TO_TICKS(%llu));\n",b[i-1].duration()-20);
		}
		fprintf(f,"    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);\n"
				  "    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);\n");
		if(b[i].time()-b[i-1].time()-b[i-1].duration()>20){
			fprintf(f,"    vTaskDelay(pdMS_TO_TICKS(%llu));\n",b[i].time()-b[i-1].time()-b[i-1].duration());
		}
		else{
			fprintf(f,"    vTaskDelay(pdMS_TO_TICKS(20));\n");
		}
		
		fprintf(f,"    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0,%u);\n",uint(440*pow(2,(b[i].pitch()-69)/12.0)));
		fprintf(f,"    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 128);\n"
				  "    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);\n");
	}
	if(b.size()){
		fprintf(f,"    vTaskDelay(pdMS_TO_TICKS(%llu));\n",b[b.size()-1].duration());
    	fprintf(f,"    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);\n"
				  "    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);\n");
	}
    fclose(f);
}
void fun2(){
	MidiFun midi("14.mid");
	midi.change_allTimeState();
    NoteList c=midi.noteList(),d;
    sort(c.begin(),c.end());
    for(auto&i:c){
//    	if(i.channel()==1&&i.track()==1){
    		d.emplace_back(i);
//		}
	}
    for(auto&i:d){
    	i.time()/=1000;
	}
	FILE*w=fopen("esp_write_cmd.txt","w");
    if(d.size()){
    	if(!d[0].velocity()){
    		fprintf(w,"{%llu,0x%2.2X},\n",0,d[0].pitch()|0x80);
		}
		else{
			fprintf(w,"{%llu,0x%2.2X},\n",0,d[0].pitch());
		}
	}
    for(int i=1;i<d.size();++i){
    	if(!d[i].velocity()){
    		fprintf(w,"{%llu,0x%2.2X},\n",d[i].time()-d[i-1].time(),d[i].pitch()|0x80);
		}
		else{
    		fprintf(w,"{%llu,0x%2.2X},\n",d[i].time()-d[i-1].time(),d[i].pitch());
		}
	}
	fclose(w);
}
void parse(void){
    MidiFun midi("14.mid");
    midi.change_allTimeState();
    NoteList c=midi.noteList();
    sort(c.begin(),c.end());
    for(auto&i:c){
        i.time()/=1000;
    }
    FILE*w=fopen("esp_write_cmd.txt","w");
    if(c.size()){
        if(!c[0].velocity()){
            fprintf(w,"{%llu,0x%2.2X},\n",0,c[0].pitch()|0x80);
        }
        else{
            fprintf(w,"{%llu,0x%2.2X},\n",0,c[0].pitch());
        }
    }
    for(int i=1;i<c.size();++i){
        if(!c[i].velocity()){
            fprintf(w,"{%llu,0x%2.2X},\n",c[i].time()-c[i-1].time(),c[i].pitch()|0x80);
        }
        else{
            fprintf(w,"{%llu,0x%2.2X},\n",c[i].time()-c[i-1].time(),c[i].pitch());
        }
    }
    fclose(w);
}
int main(){
	fun2();
	
	return 0;
}
