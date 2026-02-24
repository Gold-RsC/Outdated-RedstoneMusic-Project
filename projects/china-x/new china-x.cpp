#include"header_file\\MidiFun.h"
#include"header_file\\particle.h"
#include<algorithm>
#include<math.h>
#include<iostream>
using namespace std;
using namespace particle;
#define PI 3.1415926535897931
MidiFun midi;
NOTE note;
NOTEev pinit(-10,-1,-1,64,-1,-1);
FILE*play,*reset;
template<typename __FUNCTION>
void drawline(NOTE&lp,NOTE&np,__FUNCTION writecmd) {
	int lpn=lp.size(),npn=np.size();
	if(lpn*1.0/npn>1) {
		int k=lpn/npn;
		if(k*npn==lpn)
			for(int i=1; i<=npn; i++)
				for(int j=k*(i-1)+1; j<=k*i; j++)
					writecmd(lp[j-1],np[i-1]);
		else {
			for(int i=1; i<=lpn-k*npn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					writecmd(lp[j-1],np[i-1]);
			for(int i=lpn-k*npn+1; i<=npn; i++)
				for(int j=k*(i-1-npn)+lpn+1; j<=k*(i-npn)+lpn; j++)
					writecmd(lp[j-1],np[i-1]);
		}
	} else if(npn*1.0/lpn>1) { //npn为lpn的k倍(k>1)
		int k=npn/lpn;
		if(k*lpn==npn)
			for(int i=1; i<=lpn; i++)
				for(int j=k*i-k+1; j<=k*i; j++)
					writecmd(lp[i-1],np[j-1]);
		else {
			for(int i=1; i<=npn-k*lpn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					writecmd(lp[i-1],np[j-1]);
			for(int i=npn-k*lpn+1; i<=lpn; i++)
				for(int j=k*(i-1-lpn)+npn+1; j<=k*(i-lpn)+npn; j++)
					writecmd(lp[i-1],np[j-1]);
		}
	} else //lpn等于npn
		for(int i=1; i<=npn; i++)
			writecmd(lp[i-1],np[i-1]);
}
template<typename __FUNCTION>
void makeline(NOTEev&pinit,__FUNCTION writecmd) {
	NOTE lp,np;
	lp.push(pinit);
	np.push(note[0]);
	int size=note.size();
	for(int i=1; i<size; i++) {
		if(!note[i].volume||note[i].channel==9||note[i].track==1||note[i].channel==3)
			continue;
		if(
		    np[0].track!=note[i].track
		) {
			drawline(lp,np,writecmd);
			lp.clear();
			np.clear();
			lp.push(pinit);
			np.push(note[i]);
		} else if(
		    np[0].track==note[i].track&&
		    np[0].tick!=note[i].tick
		) {
			drawline(lp,np,writecmd);
			lp.clear();
			lp.move(np);
			np.push(note[i]);
		} else
			np.push(note[i]);
	}
	drawline(lp,np,writecmd);
}
void init(void) {
	reset=fopen("functions\\reset.mcfunction","w");
	play=fopen("functions\\play.mcfunction","w");
	midi.init("midi\\China-X.mid");
	midi.get_note(note);
	for(int i=0; i<note.size(); i++) {
		if(note[i].track==3){
			note[i].pitch-=12;
		}
	}
	note.sort();
}
void writecmd(NOTEev&a,NOTEev&b) {
	if(a.channel==9||b.channel==9||a.track==1||b.track==1||a.volume==0||b.volume==0||a.channel==3||b.channel==3)
		return;
	if(a.tick<640||(a.tick>=1935&&a.tick<2560)||a.tick>=4160) {
		if(b.track==2) {
			double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
			useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
			            String<const char*,int,const char*,int,const char*>("particles\\particles\\track2line",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
			            String<const char*,int,const char*,int>("track2:line",b.tick-a.tick,"_",b.pitch-a.pitch),
			            (b.tick-a.tick)/4.0,
			            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
			                   String<int>(0),
			                   String<int,const char*>(b.pitch-a.pitch,"*T")),
			            25,
			            Size(String<const char*>("(T>0.75?0.8*(1.25-T):0.4)"),
			                 String<const char*>("(T>0.75?0.8*(1.25-T):0.4)")),
			VariousColor({{0,"#FF0000"},
				{0.2,"#FF6D00"},
				{0.5,"#FFF500"},
				{1,"#FFF500"}
			})
			           );
			fprintf(play,"%d\t",a.tick);
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle track2:line%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		} else if(b.track==3) {
			double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
			useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
			            String<const char*,int,const char*,int,const char*>("particles\\particles\\track3line",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
			            String<const char*,int,const char*,int>("track3:line",b.tick-a.tick,"_",b.pitch-a.pitch),
			            (b.tick-a.tick)/4.0,
			            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
			                   String<int>(0),
			                   String<int,const char*>(b.pitch-a.pitch,"*T")),
			            25,
			            Size(String<const char*>("(T>0.75?0.8*(1.25-T):0.4)"),
			                 String<const char*>("(T>0.75?0.8*(1.25-T):0.4)")),
			VariousColor({{0,"#00FFDE"},
				{0.2,"#00B2FF"},
				{0.5,"#F65DC8"},
				{1,"#F65DC8"}
			})
			           );
			fprintf(play,"%d\t",a.tick);
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle track3:line%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		}
	} else if((a.tick>=640&&a.tick<1280)||(a.tick>=2560&&a.tick<3200)) {
		if(b.track==2) {
			double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
			useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
			            String<const char*,int,const char*,int,const char*>("particles\\particles\\track2parabola",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
			            String<const char*,int,const char*,int>("track2:parabola",b.tick-a.tick,"_",b.pitch-a.pitch),
			            (b.tick-a.tick)/4.0,
			            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
			                   String<double,const char*>(-3*len,"*T*(T-1)"),
			                   String<int,const char*>(b.pitch-a.pitch,"*T")),
			            25,
			            Size(String<const char*>("(T>0.75?0.8*(1.25-T):0.4)"),
			                 String<const char*>("(T>0.75?0.8*(1.25-T):0.4)")),
			VariousColor({{0,"#FF0000"},
				{0.2,"#FF6D00"},
				{0.5,"#FFF500"},
				{1,"#FFF500"}
			})
			           );
			           fprintf(play,"%d\t",a.tick);
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle track2:parabola%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		} else if(b.track==3) {
			double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
			useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
			            String<const char*,int,const char*,int,const char*>("particles\\particles\\track3parabola",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
			            String<const char*,int,const char*,int>("track3:parabola",b.tick-a.tick,"_",b.pitch-a.pitch),
			            (b.tick-a.tick)/4.0,
			            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
			                   String<double,const char*>(-3*len,"*T*(T-1)"),
			                   String<int,const char*>(b.pitch-a.pitch,"*T")),
			            25,
			            Size(String<const char*>("(T>0.75?0.8*(1.25-T):0.4)"),
			                 String<const char*>("(T>0.75?0.8*(1.25-T):0.4)")),
			VariousColor({{0,"#00FFDE"},
				{0.2,"#00B2FF"},
				{0.5,"#F65DC8"},
				{1,"#F65DC8"}
			})
			           );
			           fprintf(play,"%d\t",a.tick);
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle track3:parabola%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		}
	} else if((a.tick>=1280&&a.tick<1935)||(a.tick>=3200&&a.tick<4160)) {
		double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
		if(b.track==2) {
			double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
			useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
			            String<const char*,int,const char*,int,const char*>("particles\\particles\\track2circularFun",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
			            String<const char*,int,const char*,int>("track2:circularFun",b.tick-a.tick,"_",b.pitch-a.pitch),
			            (b.tick-a.tick)/4.0,
			            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
			                   String<const char*,double,const char*,double>("Math.cos(360*T+180)*",len/4,"+",len/4),
			                   String<int,const char*>(b.pitch-a.pitch,"*T")),
			            25,
			            Size(String<const char*>("(T>0.75?0.8*(1.25-T):0.4)"),
			                 String<const char*>("(T>0.75?0.8*(1.25-T):0.4)")),
			VariousColor({{0,"#FF0000"},
				{0.2,"#FF6D00"},
				{0.5,"#FFF500"},
				{1,"#FFF500"}
			})
			           );
			           fprintf(play,"%d\t",a.tick);
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle track2:circularFun%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		} else if(b.track==3) {
			double len=sqrt(pow(b.tick-a.tick,2)+pow(b.pitch-a.pitch,2));
			useTemplate(String<const char*>("particles\\template\\[线]通用.template"),
			            String<const char*,int,const char*,int,const char*>("particles\\particles\\track3circularFun",b.tick-a.tick,"_",b.pitch-a.pitch,".json"),
			            String<const char*,int,const char*,int>("track3:circularFun",b.tick-a.tick,"_",b.pitch-a.pitch),
			            (b.tick-a.tick)/4.0,
			            Offset(String<int,const char*>(b.tick-a.tick,"*T"),
			                   String<const char*,double,const char*,double>("Math.cos(360*T+180)*",len/4,"+",len/4),
			                   String<int,const char*>(b.pitch-a.pitch,"*T")),
			            25,
			            Size(String<const char*>("(T>0.75?0.8*(1.25-T):0.4)"),
			                 String<const char*>("(T>0.75?0.8*(1.25-T):0.4)")),
			VariousColor({{0,"#00FFDE"},
				{0.2,"#00B2FF"},
				{0.5,"#F65DC8"},
				{1,"#F65DC8"}
			})
			           );
			fprintf(play,"%d\t",a.tick);
			fprintf(play,"execute @a[scores={tick=%d}] ~~~ particle track3:circularFun%d_%d %d %d %d\n",a.tick,b.tick-a.tick,b.pitch-a.pitch,a.tick,200,a.pitch);
		}
	}
}
void makepoint(void) {
	//打击粒子
	//cang
	useTemplate(String<const char*>("particles\\template\\[点]通用.template"),
	            String<const char*>("particles\\particles\\goldenPoint.json"),
	            String<const char*>("goldenPoint"),
	            25,
	            Size(String<const char*>("0.2"),
	                 String<const char*>("0.2")),
	            Color(255,215,0,1)
	           );
	//pa
	useTemplate(String<const char*>("particles\\template\\[点]通用.template"),
	            String<const char*>("particles\\particles\\redPoint.json"),
	            String<const char*>("redPoint"),
	            25,
	            Size(String<const char*>("0.2"),
	                 String<const char*>("0.2")),
	            Color(255,0,0,1)
	           );
	//平1&3粒子
	useTemplate(String<const char*>("particles\\template\\[点]上升倒锥steady.template"),
	            String<const char*>("particles\\particles\\coneSteady.json"),
	            String<const char*>("coneSteady"),
	            25,
	            Color(255,215,0,1)
	           );
	//平2粒子
	useTemplate(String<const char*>("particles\\template\\[点]扩散球.template"),
	            String<const char*>("particles\\particles\\sphere.json"),
	            String<const char*>("sphere"),
	            25,
	            Color(255,215,0,1)
	           );
	//高1粒子
	for(int angle=0; angle<360; angle+=5) {
		useTemplate(String<const char*>("particles\\template\\[点]粒子方向.template"),
		            String<const char*,int,const char*>("particles\\particles\\circle",angle,".json"),
		            String<const char*,int>("circle",angle),
		            25,
		            Motion(String<double>(sin(PI*angle/180)),
		                   String<int>(0),
		                   String<double>(cos(PI*angle/180)),
		                   String<int>(4)),
		            Color(255,215,0,1)
		           );
	}
	auto circle=[&](NOTEev a) {
		for(int i=0; i<360; i+=5) {
			fprintf(play,"%d\texecute @a[scores={tick=%d}] ~~~ particle circle%d %d %d %d\n",a.tick,a.tick,i,a.tick,200,a.pitch);
		}
	};
	//高2粒子
	useTemplate(String<const char*>("particles\\template\\[点]渐慢上升.template"),
	            String<const char*>("particles\\particles\\slowlyRising.json"),
	            String<const char*>("slowlyRising"),
	            25,
	            Color(255,215,0,1)
	           );
	auto risingBlock=[&](NOTEev n) {
		struct A {
			int x,y,z;
		} a[]= {{0,0,0},{0,0,1},{0,0,2},{0,0,3},{0,0,4},{0,0,5},{0,0,6},{0,0,7},{0,0,8},{0,0,9},{0,0,10},{0,1,0},{0,1,10},{0,2,0},{0,2,10},{0,3,0},{0,3,10},{0,4,0},{0,4,10},{0,5,0},{0,5,10},{0,6,0},{0,6,10},{0,7,0},{0,7,10},{0,8,0},{0,8,10},{0,9,0},{0,9,10},{0,10,0},{0,10,1},{0,10,2},{0,10,3},{0,10,4},{0,10,5},{0,10,6},{0,10,7},{0,10,8},{0,10,9},{0,10,10},{1,0,0},{1,0,10},{1,10,0},{1,10,10},{2,0,0},{2,0,10},{2,10,0},{2,10,10},{3,0,0},{3,0,10},{3,10,0},{3,10,10},{4,0,0},{4,0,10},{4,10,0},{4,10,10},{5,0,0},{5,0,10},{5,10,0},{5,10,10},{6,0,0},{6,0,10},{6,10,0},{6,10,10},{7,0,0},{7,0,10},{7,10,0},{7,10,10},{8,0,0},{8,0,10},{8,10,0},{8,10,10},{9,0,0},{9,0,10},{9,10,0},{9,10,10},{10,0,0},{10,0,1},{10,0,2},{10,0,3},{10,0,4},{10,0,5},{10,0,6},{10,0,7},{10,0,8},{10,0,9},{10,0,10},{10,1,0},{10,1,10},{10,2,0},{10,2,10},{10,3,0},{10,3,10},{10,4,0},{10,4,10},{10,5,0},{10,5,10},{10,6,0},{10,6,10},{10,7,0},{10,7,10},{10,8,0},{10,8,10},{10,9,0},{10,9,10},{10,10,0},{10,10,1},{10,10,2},{10,10,3},{10,10,4},{10,10,5},{10,10,6},{10,10,7},{10,10,8},{10,10,9},{10,10,10}};
		for(auto i:a) {
			fprintf(play,"%d\texecute @a[scores={tick=%d}] ~~~ particle slowlyRising %lf %lf %lf\n",n.tick,n.tick,n.tick+i.x/10.0,200+i.y/10.0,n.pitch+i.z/10.0);
		}
	};
	for(int i=0; i<note.size(); i++) {
		if(note[i].volume==0||note[i].track==1||note[i].channel==3) {
			continue;
		}
		if(note[i].channel==9) {
			//cang~
			if(note[i].pitch==49) {
				for(int angle=0; angle<360; angle+=5)
					fprintf(play,"%d\texecute @a[scores={tick=%d}] ~~~ particle goldenPoint %d %lf %lf\n",note[i].tick,note[i].tick,note[i].tick+5,220.5+sin(PI*angle/180)*2,64.5+cos(PI*angle/180)*2);
			}
			//pa~
			else if(note[i].pitch==39) {
				for(int angle=0; angle<360; angle+=5)
					fprintf(play,"%d\texecute @a[scores={tick=%d}] ~~~ particle redPoint %d %lf %lf\n",note[i].tick,note[i].tick,note[i].tick+5,220.5+sin(PI*angle/180)*2,64.5+cos(PI*angle/180)*2);
			}
			continue;
		}
		fprintf(play,"%d\t execute @a[scores={tick=%d}] ~~~ setblock %d 200 %d air\n",note[i].tick,note[i].tick,note[i].tick,note[i].pitch);
			fprintf(reset,"setblock %d 200 %d redstone_block\n",note[i].tick,note[i].pitch);
		if(note[i].tick<640||note[i].tick>=4160) {
			fprintf(play,"%d\texecute @a[scores={tick=%d}] ~~~ particle coneSteady %d %d %d\n",note[i].tick,note[i].tick,note[i].tick,200,note[i].pitch);
		} else if((note[i].tick>=640&&note[i].tick<1280)||(note[i].tick>=2560&&note[i].tick<3200)) {
			circle(note[i]);
		} else if((note[i].tick>=1280&&note[i].tick<1935)||(note[i].tick>=3200&&note[i].tick<4160)) {
			risingBlock(note[i]);
		} else if(note[i].tick>=1935&&note[i].tick<2560) {
			fprintf(play,"%d\texecute @a[scores={tick=%d}] ~~~ particle sphere %d %d %d\n",note[i].tick,note[i].tick,note[i].tick,200,note[i].pitch);
		}
	}
}
/**********
track 2:主
track 3:辅
**********/
void makeParticle(void) {
	makeline(pinit,writecmd);
	makepoint();
}
void end(void) {
	fclose(play);
	fclose(reset);
}
int main(void) {
	init();
	makeParticle();
	end();
	return 0;
}
