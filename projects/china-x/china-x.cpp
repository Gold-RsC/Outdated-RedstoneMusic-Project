#include"header_file\\MidiFun.h"
#include"header_file\\particle.h"
#include<algorithm>
#include<math.h>
using namespace std;
using namespace particle;
#define PI 3.1415926535897931
MidiFun midi;
NOTE note;
FILE*music,*reset,*play;
void init(void) {
	music=fopen("functions\\music.mcfunction","w");
	reset=fopen("functions\\reset.mcfunction","w");
	play=fopen("functions\\play.mcfunction","w");
	midi.init("midi\\China-X.mid");
	midi.get_note(note);
	for(int i=0; i<note.size(); i++) {
		if(note[i].channel==3) {
			note[i].track=2;
			note[i].channel=0;
		}
	}
	sort(&note[0],&note[note.size()],[](NOTEev a,NOTEev b) {
		if(a.track!=b.track)
			return a.track<b.track;
		if(a.channel!=b.channel)
			return a.channel<b.channel;
		if(a.tick!=b.tick)
			return a.tick<b.tick;
		if(a.pitch!=b.pitch)
			return a.pitch<b.pitch;
	});
}
void writecmd(NOTEev a,NOTEev b) {
	if(a.channel==9||b.channel==9||a.track==1||b.track==1||a.volume==0||b.volume==0)
		return;
	if(a.tick>=0&&a.tick<640) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\circularFun",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("circularFun",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<const char*,double,const char*,double>("Math.cos(360*T+180)*",len/4,"+",len/4),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		            15,
		            Size(String<const char*>("(T>0.5?0.2*(1-T):0.1)"),
		                 String<const char*>("(T>0.5?0.2*(1-T):0.1)")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle circularFun%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	} else if(a.tick>=640&&a.tick<1280) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\parabola",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("parabola",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<double,const char*>(-3*len,"*T*(T-1)"),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		            15,
		            Size(String<const char*>("(T>0.5?0.2*(1-T):0.1)"),
		                 String<const char*>("(T>0.5?0.2*(1-T):0.1)")),
		            Color(255,254,186,1)
		           );
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\peak",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("peak",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<double,const char*>(-3*len,"*T*(T-1)"),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle parabola%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle peak%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	} else if(a.tick>=1280&&a.tick<1935) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\peak",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("peak",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<double,const char*>(-3*len,"*T*(T-1)"),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle peak%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	} else if(a.tick>=1935&&a.tick<2560) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\circularFun",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("circularFun",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<const char*,double,const char*,double>("Math.cos(360*T+180)*",len/4,"+",len/4),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		            15,
		            Size(String<const char*>("(T>0.5?0.2*(1-T):0.1)"),
		                 String<const char*>("(T>0.5?0.2*(1-T):0.1)")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle circularFun%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	} else if(a.tick>=2560&&a.tick<3200) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\parabola",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("parabola",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<double,const char*>(-3*len,"*T*(T-1)"),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		            15,
		            Size(String<const char*>("(T>0.5?0.2*(1-T):0.1)"),
		                 String<const char*>("(T>0.5?0.2*(1-T):0.1)")),
		            Color(255,254,186,1)
		           );
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\peak",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("peak",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<double,const char*>(-3*len,"*T*(T-1)"),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle parabola%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle peak%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	} else if(a.tick>=3200&&a.tick<4160) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\peak",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("peak",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<double,const char*>(-3*len,"*T*(T-1)"),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle circularFun%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	} else if(a.tick>=4160) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
		            String<const char*,int,const char*,int,const char*>("particles\\particles\\circularFun",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
		            String<const char*,int,const char*,int>("circularFun",b.tick-a.tick,"_",b.pitch-a.pitch),
		            (b.tick-a.tick)*2,
		            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
		                   String<const char*,double,const char*,double>("Math.cos(360*T+180)*",len/4,"+",len/4),
		                   String<int,const char*>(b.pitch-a.pitch,"*T")),
		            15,
		            Size(String<const char*>("(T>0.5?0.2*(1-T):0.1)"),
		                 String<const char*>("(T>0.5?0.2*(1-T):0.1)")),
		VariousColor({{0,"#FF0000"},
			{0.2,"#FF6D00"},
			{0.5,"#FFF500"}
		})
		           );
		fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle circularFun%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
	}
}
void makepoint(void) {
	//打击粒子
	//cang
	useTemplate(String<const char*>("particles\\template\\[点]通用.template"),
	            String<const char*>("particles\\particles\\goldenPoint.json"),
	            String<const char*>("goldenPoint"),
	            15,
	            Size(String<const char*>("0.2"),
	                 String<const char*>("0.2")),
	            Color(255,215,0,1)
	           );
	//pa
	useTemplate(String<const char*>("particles\\template\\[点]通用.template"),
	            String<const char*>("particles\\particles\\redPoint.json"),
	            String<const char*>("redPoint"),
	            15,
	            Size(String<const char*>("0.2"),
	                 String<const char*>("0.2")),
	            Color(255,0,0,1)
	           );
	//平1&2粒子
	useTemplate(String<const char*>("particles\\template\\[点]上升倒锥steady.template"),
	            String<const char*>("particles\\particles\\coneSteady.json"),
	            String<const char*>("coneSteady"),
	            15,
	            Color(255,215,0,1)
	           );
	//平3粒子
	 useTemplate(String<const char*>("particles\\template\\[点]扩散球.template"),
	            String<const char*>("particles\\particles\\sphere.json"),
	            String<const char*>("sphere"),
	            15,
	            Color(255,215,0,1)
	           );
	//高1粒子
	for(int angle=0; angle<360; angle++) {
		useTemplate(String<const char*>("particles\\template\\[点]粒子方向.template"),
		            String<const char*,int,const char*>("particles\\particles\\circle",angle,".json"),
		            String<const char*,int>("circle",angle),
		            15,
		            Motion(String<double>(sin(PI*angle/360)*2),
		                   String<int>(0),
		                   String<double>(cos(PI*angle/360))*2),
		            Color(255,215,0,1)
		           );
	}
	//高2粒子
	useTemplate(String<const char*>("particles\\template\\[点]渐慢上升.template"),
	            String<const char*>("particles\\particles\\slowlyRising.json"),
	            String<const char*>("slowlyRising"),
	            15,
	            Color(255,215,0,1)
	           );
	
	for(int i=0; i<note.size(); i++) {
		if(note[i].volume==0||note[i].track==1) {
			continue;
		}
		if(note[i].channel==9) {
			//cang~
			if(note[i].pitch==49) {
				for(int angle=0; angle<360; angle++) {
					fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle goldenPoint ~5~%lf~%lf\n",note[i].tick,sin(PI*angle/360)*2,cos(PI*angle/360)*2);
				}
			}
			//pa~
			else if(note[i].pitch==39) {
				for(int angle=0; angle<360; angle++) {
					fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle redPoint ~5~%lf~%lf\n",note[i].tick,sin(PI*angle/360)*2,cos(PI*angle/360)*2);
				}
			}
			continue;
		}
		if(note[i].tick>=0&&note[i].tick<640) {
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle coneSteady %d %d %d\n",note[i].tick,note[i].tick,200,note[i].pitch);
		} else if(note[i].tick>=640&&note[i].tick<1280) {
		} else if(note[i].tick>=1280&&note[i].tick<1935) {
			fprintf(play,"execute @a[scores={tick=%d}] %d %d %d function blockRising\n",note[i].tick,note[i].tick,200,note[i].pitch);
		} else if(note[i].tick>=1935&&note[i].tick<2560) {
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle coneSteady %d %d %d\n",note[i].tick,note[i].tick,200,note[i].pitch);
		} else if(note[i].tick>=2560&&note[i].tick<3200) {
		} else if(note[i].tick>=3200&&note[i].tick<4160) {
			fprintf(play,"execute @a[scores={tick=%d}] %d %d %d function blockRising\n",note[i].tick,note[i].tick,200,note[i].pitch);
		} else if(note[i].tick>=4160) {
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle sphere %d %d %d\n",note[i].tick,note[i].tick,200,note[i].pitch);
		}
	}
}
/**********
track 2:主
track 3:辅
**********/
void makeParticle(void) {
	NOTEev pinit(-10,-1,-1,64,-1,-1);
	midi.makeline(pinit,writecmd);
	makepoint(void);
}
int main(void) {
	init();
	makeParticle();
	
	return 0;
}
