/*************************************************
*	tick	form
*	0		普通 
*	768		落沙钢琴
*	1152	旋转方块 
*	1536	line
*	1632	Phigros
*	1728	抛物线涟漪 
*	2400	下落起涟漪
*	2784	下落起涟漪+两侧烟花起
*************************************************/
#include"../../MidiParse/5.1.5/headfile/MidiFun.h"
#include"headFile/particle.h"
using namespace std;
using namespace particle;
MidiFun midi;
Note note[20000];
int noteNum;
FILE*effect;
void init(void) {
	effect=fopen("functions/effect.mcfunction","w");
	midi.parse("midi/Phigros-萤火虫の怨.mid",0);
	noteNum=midi.getNote(note);
	for(int i=0; i<noteNum; i++) {
		note[i].time/=40;
	}
	int flag=noteNum;
	for(int i=0;i<=flag;i++){
		if(note[i].volume==0){
			while(note[--flag].volume==0);
			std::swap(note[i],note[flag]);
		}
	}
	for(noteNum=0;note[noteNum++].volume!=0;){}
	std::sort(note,note+noteNum,[](Note a,Note b){
		if(a.time!=b.time)
			return a.time<b.time;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		return a.pitch<b.pitch;
	});
}
template<typename __Fun>
void drawline(Note*note,Note initPoint,__Fun cmd) {
	Note*lp=new Note[128];
	Note*np=new Note[128];
	int lpn=1,npn=1;
	lp[0]=initPoint;
	np[0]=note[0];
	for(int i=1; i<noteNum; i++) {
		if(!note[i].volume)
			continue;
		if(
		    np[0].channel!=note[i].channel||
		    np[0].track!=note[i].track
		) {
			if(lpn>npn) {
				int k=lpn/npn;
				if(k*npn==lpn)
					for(int i=1; i<=npn; i++)
						for(int j=k*(i-1)+1; j<=k*i; j++)
							cmd(lp[j-1],np[i-1]);
				else {
					for(int i=1; i<=lpn-k*npn; i++)
						for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
							cmd(lp[j-1],np[i-1]);
					for(int i=lpn-k*npn+1; i<=npn; i++)
						for(int j=k*(i-1-npn)+lpn+1; j<=k*(i-npn)+lpn; j++)
							cmd(lp[j-1],np[i-1]);
				}
			} else if(npn>lpn) {
				int k=npn/lpn;
				if(k*lpn==npn)
					for(int i=1; i<=lpn; i++)
						for(int j=k*i-k+1; j<=k*i; j++)
							cmd(lp[i-1],np[j-1]);
				else {
					for(int i=1; i<=npn-k*lpn; i++)
						for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
							cmd(lp[i-1],np[j-1]);
					for(int i=npn-k*lpn+1; i<=lpn; i++)
						for(int j=k*(i-1-lpn)+npn+1; j<=k*(i-lpn)+npn; j++)
							cmd(lp[i-1],np[j-1]);
				}
			} else
				for(int i=1; i<=npn; i++)
					cmd(lp[i-1],np[i-1]);
			lp[0]=initPoint;
			np[0]=note[i];
			lpn=1;
			npn=1;
		} else if(
		    np[0].channel==note[i].channel&&
		    np[0].track==note[i].track&&
		    np[0].time!=note[i].time
		) {
			if(lpn>npn) {
				int k=lpn/npn;
				if(k*npn==lpn)
					for(int i=1; i<=npn; i++)
						for(int j=k*(i-1)+1; j<=k*i; j++)
							cmd(lp[j-1],np[i-1]);
				else {
					for(int i=1; i<=lpn-k*npn; i++)
						for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
							cmd(lp[j-1],np[i-1]);
					for(int i=lpn-k*npn+1; i<=npn; i++)
						for(int j=k*(i-1-npn)+lpn+1; j<=k*(i-npn)+lpn; j++)
							cmd(lp[j-1],np[i-1]);
				}
			} else if(npn>lpn) {
				int k=npn/lpn;
				if(k*lpn==npn)
					for(int i=1; i<=lpn; i++)
						for(int j=k*i-k+1; j<=k*i; j++)
							cmd(lp[i-1],np[j-1]);
				else {
					for(int i=1; i<=npn-k*lpn; i++)
						for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
							cmd(lp[i-1],np[j-1]);
					for(int i=npn-k*lpn+1; i<=lpn; i++)
						for(int j=k*(i-1-lpn)+npn+1; j<=k*(i-lpn)+npn; j++)
							cmd(lp[i-1],np[j-1]);
				}
			} else
				for(int i=1; i<=npn; i++)
					cmd(lp[i-1],np[i-1]);
			for(int j=0; j<npn; j++)
				lp[j]=np[j];
			np[0]=note[i];
			lpn=npn;
			npn=1;
		} else
			np[npn++]=note[i];
	}
	if(lpn>npn) {
		int k=lpn/npn;
		if(k*npn==lpn)
			for(int i=1; i<=npn; i++)
				for(int j=k*(i-1)+1; j<=k*i; j++)
					cmd(lp[j-1],np[i-1]);
		else {
			for(int i=1; i<=lpn-k*npn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					cmd(lp[j-1],np[i-1]);
			for(int i=lpn-k*npn+1; i<=npn; i++)
				for(int j=k*(i-1-npn)+lpn+1; j<=k*(i-npn)+lpn; j++)
					cmd(lp[j-1],np[i-1]);
		}
	} else if(npn>lpn) {
		int k=npn/lpn;
		if(k*lpn==npn)
			for(int i=1; i<=lpn; i++)
				for(int j=k*i-k+1; j<=k*i; j++)
					cmd(lp[i-1],np[j-1]);
		else {
			for(int i=1; i<=npn-k*lpn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					cmd(lp[i-1],np[j-1]);
			for(int i=npn-k*lpn+1; i<=lpn; i++)
				for(int j=k*(i-1-lpn)+npn+1; j<=k*(i-lpn)+npn; j++)
					cmd(lp[i-1],np[j-1]);
		}
	} else
		for(int i=1; i<=npn; i++)
			cmd(lp[i-1],np[i-1]);
};
//general
void fun1(void){
	
}
//Line
void fun2(void) {
	//line
	[&] {
		useTemplate(
		    String<const char*>("particles/template/[线]通用.template"),
		    String<const char*>("particles/fun2/fun2_line.json"),
		    String<const char*>("fun2_line"),
		    384/20.0,
		    Offset(
		        String<const char*>("variable.emitter_age/variable.emitter_lifetime"),
		        String<int>(0),
		        String<int>(0)
		    ),
		    10,
		    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
		    Size(String<const char*>("(variable.particle_age<5)?0.6:-0.024*variable.particle_age*(variable.particle_age-10)"),String<const char*>("(variable.particle_age<5)?0.6:-0.024*variable.particle_age*(variable.particle_age-10)")),
		    UV(128,128,0,0,8,8),
		    Color(255,215,0,1)
		);
		fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun2_line %d %d %d\n",768-10,768,200,75);
		fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun2_line %d %d %d\n",768-10,768,220,75);
	}();
	//pitchLine
	[&] {
		useTemplate(
		    String<const char*>("particles/template/[线]通用.template"),
		    String<const char*>("particles/fun2/fun2_pitchLine1.json"),
		    String<const char*>("fun2_pitchLine1"),
		    10,
		    Offset(
		        String<int>(0),
		        String<const char*>("variable.emitter_age/variable.emitter_lifetime"),
		        String<int>(0)
		    ),
		    10,
		    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
		    Size(String<const char*>("(variable.particle_age<5)?0.6:-0.024*variable.particle_age*(variable.particle_age-10)"),String<const char*>("(variable.particle_age<5)?0.6:-0.024*variable.particle_age*(variable.particle_age-10)")),
		    UV(128,128,0,0,8,8),
		    Color(255,215,0,1)
		);
		useTemplate(
		    String<const char*>("particles/template/[线]通用.template"),
		    String<const char*>("particles/fun2/fun2_pitchLine2.json"),
		    String<const char*>("fun2_pitchLine2"),
		    10,
		    Offset(
		        String<int>(0),
		        String<const char*>("variable.emitter_age/variable.emitter_lifetime"),
		        String<int>(0)
		    ),
		    10,
		    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
		    Size(String<const char*>("(variable.particle_age<5)?0.6:-0.024*variable.particle_age*(variable.particle_age-10)"),String<const char*>("(variable.particle_age<5)?0.6:-0.024*variable.particle_age*(variable.particle_age-10)")),
		    UV(128,128,0,0,8,8),
		    Color(255,215,0,1)
		);
		for(int i=0; i<track1Num; i++) {
			if(track1[i].tick>=768&&track1[i].tick<1152) {
				fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun2_pitchLine %d %d %d\n",track1[i].tick,track1[i].tick,200,75);
				fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun2_pitchLine %d %d %d\n",track1[i].tick,track1[i].tick,220,75);
			}
		}
	}();
	//pitchPoint
	[&] {
		auto makeParticle=[](Color color) {
			useTemplate(
			    String<const char*>("particles/template/[点]通用.template"),
			    String<const char*,double,const char*,double,const char*,double,const char*>("particles/fun2/fun2_pitchPoint_",color.r,"_",color.g,"_",color.b,".json"),
			    String<const char*,double,const char*,double,const char*,double>("fun2_pitchPoint_",color.r,"_",color.g,"_",color.b),
			    3,
			    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
			    Size(String<const char*>("(variable.particle_age<0.1)?variable.particle_age*5:2"),String<const char*>("(variable.particle_age<0.1)?variable.particle_age*5:2")),
			    UV(128,128,0,32,8,8),
			    color
			);
		};
		int _min=128,_max=0,len;
		for(int i=0; i<track1Num; i++) {
			if(track1[i].tick>=768&&track1[i].tick<1152) {
				_min=min(_min,track1[i].pitch);
				_max=max(_max,track1[i].pitch);
			}
		}
		len=_max-_min;
		for(int i=0; i<track1Num; i++) {
			if(track1[i].tick>=768&&track1[i].tick<1152&&track1[i].tick!=track1[i-1].tick) {
				int r,g,b;
				switch(int((track1[i].pitch-_min)*1.0/len*6)){
					case 0:{
						r=255;
						g=(track1[i].pitch-_min)*1.0/len*3*255;
						b=0;
						break;
					}
					case 1:{
						r=255;
						g=(track1[i].pitch-_min)*1.0/len*3*255;
						b=0;
						break;
					}
					case 2:{
						r=(3-(track1[i].pitch-_min)*1.0/len*6)*255;
						g=255;
						b=0;
						break;
					}
					case 3:{
						r=0;
						g=255;
						b=((track1[i].pitch-_min)*1.0/len*6-3)*255;
						break;
					}
					case 4:{
						r=0;
						g=(5-(track1[i].pitch-_min)*1.0/len*6)*255;
						b=255;
						break;
					}
					case 5:{
						r=((track1[i].pitch-_min)*1.0/len*6-5)*255;
						g=0;
						b=255;
						break;
					}
					case 6:{
						r=255;
						g=0;
						b=255;
						break;
					}
					default:{
						printf("????");
						break;
					}
				}
				makeParticle(Color(r,g,b,1));
				fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun2_pitchPoint_%d_%d_%d %d %d %d\n",track1[i].tick,r,g,b,track1[i].tick,210,75);
			}
		}
	}();
}
//大风车 
void fun3(void){
	
}
void fun4(void){
	useTemplate(
	    String<const char*>("particles/template/[线]通用.template"),
	    String<const char*>("particles/fun4/fun4_line.json"),
	    String<const char*>("fun4_line"),
	    50/20.0,
	    Offset(
	        String<const char*>("50*variable.emitter_age/variable.emitter_lifetime"),
	        String<int>(0),
	        String<int>(0)
	    ),
	    192/20.0,
	    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
	    Size(String<const char*>("0.6"),String<const char*>("0.6")),
	    UV(128,128,0,0,8,8),
	    Color(255,255,255,1)
	);
	fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun4_line %d %d %d\n",1536-5,1536,200,48);
	fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun4_line %d %d %d\n",1536-5,1536,200,80);
	
	for(int i=0;i<track1Num;i++){
		if(track1[i].tick>=1536&&track1[i].tick<1632&&track1[i].tick!=track1[i-1].tick){
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun4_blueButton_1 %d %d %d\n",track1[i].tick-20,1550,200,80-10);
		}
		else if(track1[i].tick>=1632&&track1[i].tick<1728&&track1[i].tick!=track1[i-1].tick){
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun4_blueButton_2 %d %d %d\n",track1[i].tick-20,1550,200,48+10);
		}
	}
}
//抛物线涟漪 
void fun5(void){
	auto makeParticle=[](int x,int z){
		double a=(x*x+z*z)/4.0;
		useTemplate(
		    String<const char*>("particles/template/[线]通用.template"),
		    String<const char*,int,const char*,int,const char*>("particles/fun5/fun5_",x,"_",z,".json"),
		    String<const char*,int,const char*,int>("fun5_",x,"_",z),
		    x/20.0,
		    Offset(
		        String<int,const char*>(x,"*variable.emitter_age/variable.emitter_lifetime"),
		        String<double,const char*>(a,"*variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime-1)"),
		        String<int,const char*>(z,"*variable.emitter_age/variable.emitter_lifetime")
		    ),
		    1,
		    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
		    Size(String<const char*>("(variable.particle_age<0.5)?0.6:-2.4*variable.particle_age*(variable.particle_age-1)"),String<const char*>("(variable.particle_age<0.5)?0.6:-2.4*variable.particle_age*(variable.particle_age-1)")),
		    UV(128,128,0,0,8,8),
		    Color(255,215,0,1)
		);
	};
	Note initPoint;
	initPoint.tick=-10;
	initPoint.pitch=64;
	drawline(track1,initPoint,[&](Note a,Note b){
		if(a.tick>=1728&&a.tick<2400){
			makeParticle(b.tick-a.tick,b.pitch-a.pitch);
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun5_%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		}
	});
	drawline(track2,initPoint,[&](Note a,Note b){
		if(a.tick>=1728&&a.tick<2400){
			makeParticle(b.tick-a.tick,b.pitch-a.pitch);
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun5_%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		}
	});
	useTemplate(
	    String<const char*>("particles/template/[线]通用.template"),
	    String<const char*>("particles/fun5/fun5_circle1.json"),
	    String<const char*>("fun5_circle1"),
	    2,
	    Offset(
	        String<const char*>("math.cos(variable.emitter_age/variable.emitter_lifetime*360)*5"),
	        String<const char*>("0"),
	        String<const char*>("math.sin(variable.emitter_age/variable.emitter_lifetime*360)*5")
	    ),
	    10,
	    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
	    Size(String<const char*>("0.6"),String<const char*>("0.6")),
	    UV(128,128,0,0,8,8),
	    Color(255,0,70,1)
	);
	useTemplate(
	    String<const char*>("particles/template/[线]通用.template"),
	    String<const char*>("particles/fun5/fun5_circle2.json"),
	    String<const char*>("fun5_circle2"),
	    2,
	    Offset(
	        String<const char*>("math.cos(variable.emitter_age/variable.emitter_lifetime*360)*5"),
	        String<const char*>("0"),
	        String<const char*>("math.sin(variable.emitter_age/variable.emitter_lifetime*360)*5")
	    ),
	    10,
	    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
	    Size(String<const char*>("0.6"),String<const char*>("0.6")),
	    UV(128,128,0,0,8,8),
	    Color(0,242,255,1)
	);
	for(int i=0;i<track1Num;i++){
		if(track1[i].tick>=1728&&track1[i].tick<2400){
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun5_circle1 %d %d %d\n",track1[i].tick-40,track1[i].tick,200,track1[i].pitch);
		}
	}
	for(int i=0;i<track2Num;i++){
		if(track2[i].tick>=1728&&track2[i].tick<2400){
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun5_circle2 %d %d %d\n",track2[i].tick-40,track2[i].tick,200,track2[i].pitch);
		}
	}
}
//下落起涟漪(+两侧烟花起)
void fun6(void){
	useTemplate(
	    String<const char*>("particles/template/[线]通用.template"),
	    String<const char*>("particles/fun6/fun6_circle1.json"),
	    String<const char*>("fun6_circle1"),
	    2,
	    Offset(
	        String<const char*>("math.cos(variable.emitter_age/variable.emitter_lifetime*360)*5"),
	        String<const char*>("0"),
	        String<const char*>("math.sin(variable.emitter_age/variable.emitter_lifetime*360)*5")
	    ),
	    10,
	    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
	    Size(String<const char*>("0.6"),String<const char*>("0.6")),
	    UV(128,128,0,0,8,8),
	    Color(255,0,70,1)
	);
	useTemplate(
	    String<const char*>("particles/template/[线]通用.template"),
	    String<const char*>("particles/fun6/fun6_circle2.json"),
	    String<const char*>("fun6_circle2"),
	    2,
	    Offset(
	        String<const char*>("math.cos(variable.emitter_age/variable.emitter_lifetime*360)*5"),
	        String<const char*>("0"),
	        String<const char*>("math.sin(variable.emitter_age/variable.emitter_lifetime*360)*5")
	    ),
	    10,
	    Motion(String<int>(0),String<int>(0),String<int>(0),String<int>(0)),
	    Size(String<const char*>("0.6"),String<const char*>("0.6")),
	    UV(128,128,0,0,8,8),
	    Color(0,242,255,1)
	);
	auto flagflag=[](int value){
		return value==0||value==10;
	};
	auto flag=[&](int x,int y,int z){
		return flagflag(x)+flagflag(y)+flagflag(z)>=2;
	};
	auto block=[&](Note a){
		for(int x=0;x<=10;x+=2){
			for(int y=0;y<=10;y+=2){
				for(int z=0;z<=10;z+=2){
					if(flag(x,y,z)){
						fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun6_block %lf %lf %lf\n",a.tick-40,a.tick+x/10.0,220+y/10.0,a.pitch+z/10.0);
						fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun6_circle %d %d %d\n",a.tick-40,a.tick,220,a.pitch);
					}
				}
			}
		}
	};
	for(int i=0;i<track1Num;i++){
		if(track1[i].tick>=2400){
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun6_circle1 %d %d %d\n",track1[i].tick-40,track1[i].tick,200,track1[i].pitch);
			block(track1[i]);
		}
	}
	for(int i=0;i<track2Num;i++){
		if(track2[i].tick>=2400){
			fprintf(effect,"execute @p[scores={tick=%d}] ~~~ particle fun6_circle2 %d %d %d\n",track2[i].tick-40,track2[i].tick,200,track2[i].pitch);
			block(track2[i]);
		}
	}
	
}
void end(void){
	fclose(effect);
}
int main(void) {
	init();
	fun1();
	fun2();
	fun3();
	fun4();
	fun5();
	fun6();
	end();
	return 0;
}
