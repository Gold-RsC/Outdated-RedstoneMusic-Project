#include"../../MidiParse/6.1.0(based_on_stl)/MidiPlay.hpp"
#include"ParticleTemplate.hpp"
using namespace GoldType;
using namespace MidiParse;
using namespace std;
NotePairList n;
FILE*mcfunction,*testF;
double PI=3.14159265358979323846;
void init(){
	system("clear_effect.exe");
    FILE*r=fopen("noteList_mctick.txt","r");
    for(int i=0;i<6307;++i){
        ull tick,duration;
        ushort track,channel,instrument,pitch,velocity;
        fscanf(r,"%llu %llu %hu %hu %hu %hu %hu",&tick,&duration,&track,&channel,&instrument,&pitch,&velocity);
        n.emplace_back(tick,duration,track,channel,pitch,velocity,instrument);
    }
    fclose(r);
    
    FILE*initF=fopen("mcpack/hsyy_hopes_and_dreams/b/functions/init.mcfunction","w");
    fprintf(initF,
                "scoreboard objectives add tick dummy tick\n"
                "scoreboard objectives setdisplay sidebar tick descending\n"
                "scoreboard players set GoldRsC tick -100\n"
                "tp GoldRsC -4 99 0 -90 0\n"
//                "gamerule sendcommandblockfeedback false\n"
//                "gamerule commandblockoutput false\n"
                );
    fclose(initF);
    mcfunction=fopen("mcpack/hsyy_hopes_and_dreams/b/functions/play.mcfunction","w");
    fprintf(mcfunction,"scoreboard players add GoldRsC tick 1\n");
    testF=fopen("mcpack/hsyy_hopes_and_dreams/b/functions/test.mcfunction","w");
    fprintf(testF,
				"scoreboard players set GoldRsC tick -100\n"
                "tp GoldRsC -4 99 0 -90 0\n");
}
template<typename...Args>
void addFun(const char*text,Args&&...args){
    fprintf(mcfunction,text,args...);
}
template<typename...Args>
void addTest(const char*text,Args&&...args){
    fprintf(testF,text,args...);
}
//bar[0,16)
void A1(){
    /********************
            red
    ********************/
    vector<int> time={0,16,32,64,80,96,128,144,160,184,192,208,224};
    vector<int> pos ={0,3, 1, 0 ,3 ,-3,-2 ,0  ,2  ,3  ,2  ,1  ,0  };
    //appear
    red_appear();
    addFun("execute as GoldRsC at @s[scores={tick=0}] anchored eyes run particle red_appear 0 100 0\n");
    //move
    for(int i=1;i<time.size();++i){
        red_move("red_move_"+to_string(pos[i]-pos[i-1]),8*0.05,(pos[i]-pos[i-1]));
        addFun("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle red_move_%d 0 100 %d\n",time[i],(pos[i]-pos[i-1]),pos[i-1]);
    }
    //static
    for(int i=0;i<time.size()-1;++i){
        if(!(time[i+1]-time[i]-8)){
            continue;
        }
        red_static("red_static_"+to_string(time[i+1]-time[i]-8),(time[i+1]-time[i]-8)*0.05);
        addFun("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle red_static_%d 0 100 %d\n",time[i]+8,(time[i+1]-time[i]-8),pos[i]);
    }
    //change
    red_change();
    addFun("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle red_change 0 100 0\n",time[time.size()-1]+8);
    //float
    red_float("red_float_256",256*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=256}] anchored eyes run particle red_float_256 0 98 0\n");
    
    
    /********************
          white_rain
    ********************/
    //rain
    white_rain1("white_rain_1");
    white_rain2("white_rain_2");
    time=vector<int>{256,260,268,272,276,284,288,292,300,304,308,312,316,320,324,332,336,340,348,352,356,364,368,372,376,380,384,388,396,400,404,412,416,420,428,432,436,440,448,464,480};
    for(int i=0;i<time.size();++i){
        addFun("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle white_rain_1 2 104 -8\n",time[i]-1);
        addFun("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle white_rain_2 2 104 8\n",time[i]-1);
    }
    
    
    /********************
          asriel
    ********************/
    //appear
    asriel_appear();
    addFun("execute as GoldRsC at @s[scores={tick=448}] anchored eyes run particle asriel_appear 1 101 0\n");
    //1float
    asriel_1float("asriel_1float_32",32*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=480}] anchored eyes run particle asriel_1float_32 1 101 0\n");
    
    /********************
          6 heart
    ********************/
    //appear
    heartAll_appear();
    for(int i=0;i<6;++i){
        addFun("execute as GoldRsC at @s[scores={tick=416}] anchored eyes run particle heart%d_appear 0 101 0\n",i);
    }
    //rotate
    heartAll_rotate();
    for(int i=0;i<6;++i){
        addFun("execute as GoldRsC at @s[scores={tick=448}] anchored eyes run particle heart%d_rotate 0 101 0\n",i);
    }
    
}
//bar[16,32)
void A2(){
    /********************
            red
    ********************/
    red_float("red_float_512",512*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=512}] anchored eyes run particle red_float_512 0 98 0\n");
    
    /********************
           asriel
    ********************/
    //8float
    //8float_shadow
    asriel_8float("asriel_8float_512",512*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=512}] anchored eyes run particle asriel_8float_512 1 101 0\n\n");
    asriel_8float_shadow("asriel_8float_shadow_512",512*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=512}] anchored eyes run particle asriel_8float_shadow_512 2.0 101 0\n\n");
    
    /********************
         background
    ********************/
    background1("background1_512",512*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=512}] anchored eyes run particle background1_512 9 98 -4\n");
    background2("background2_512",512*0.05);
    addFun("execute as GoldRsC at @s[scores={tick=512}] anchored eyes run particle background2_512 9 98 4\n");
}
//bar[32,40)
void B1(){
	/********************
            asriel
    ********************/
//    asriel_1float("asriel_1float_32",32*0.05);
//    addFun("execute as GoldRsC at @s[scores={tick=480}] anchored eyes run particle asriel_1float_32 1 101 0\n");
    
    /********************
             gun
    ********************/
    
    
}
//bar[40,48)
void B2(){
    
}
//bar[48,56)
void B3(){
    
}
//bar[56,72)
void C1(){
    
}
//bar[72,88)
void C2(){
    
}
//bar[88,92)
void D1(){
    
}
//bar[92,108)
void D2(){
    
}
//bar[108,...)
void E(){
    
}
void test(){
	auto gun=[](int tick,int angle,double x,double y,double z){
    	gun_outline("gun_outline_"+to_string(angle),angle);
    	addTest("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle gun_outline_%d %f %f %f\n",tick,angle,x,y,z);
    	gun_rainbowray("gun_rainbowray_"+to_string(angle),angle);
    	addTest("execute as GoldRsC at @s[scores={tick=%d}] anchored eyes run particle gun_rainbowray_%d %f %f %f\n",tick,angle,x+0.05,y-0.5*cos(angle*PI/180),z+0.5*sin(angle*PI/180));
	};
	gun(-100,30,0,102,-3.5);
	gun(-100,-30,0,102,3.5+0.95);
}
void end(){
    fclose(mcfunction);
    fclose(testF);
    system("powershell -File to_zip.ps1");
}
int main(){
    init();
    A1();
    A2();
    B1();
    B2();
    B3();
    C1();
    C2();
    D1();
    D2();
    E();
    test();
    end();
    return 0;
}
