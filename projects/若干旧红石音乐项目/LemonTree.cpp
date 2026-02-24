//引用头文件 
//万能头文件 
#include<bits/stdc++.h> 
//引用particle.h头文件 
#include"header_file\\particle.h"
//引用notepoint.h头文件 
//#include"headfile\\notepoint.h"//这行可不加，因为noteline.h里引用过了 
//引用noteline.h头文件 
#include"header_file\\noteline.h"
//使用命名空间 
using namespace std;
using namespace ptc;
using namespace notepoint;
//dtcl函数（根据颜色，返回颜色数值）（particle中的函数需要此函数） 
char*dtcl(const char*color){
	char*colorvalue=(char*)malloc(50);
	strcpy(colorvalue,"0, 0, 0, 0");
	if(!stricmp(color,"Yellow"))
		strcpy(colorvalue,"1, 0.92941, 0, 1");
	else if(!stricmp(color,"Purple"))
		strcpy(colorvalue,"0.35686, 0, 1, 1");
	else if(!stricmp(color,"Pink"))
		strcpy(colorvalue,"0, 1, 0.74902, 1");
	else if(!stricmp(color,"Green"))
		strcpy(colorvalue,"0.10588, 1, 0, 1");
	else if(!stricmp(color,"Red"))
		strcpy(colorvalue,"1, 0, 0, 1");
	else if(!stricmp(color,"Blue"))
		strcpy(colorvalue,"0.5, 0.92941, 0, 1");
	else if(!stricmp(color,"Orange"))
		strcpy(colorvalue,"1, 0.39216, 0, 1");
	else if(!stricmp(color,"White"))
		strcpy(colorvalue,"1, 0.39216, 0, 1");
	else
		strcpy(colorvalue,"error");
	return colorvalue;
}
//初始点
Point pinit;
//输出文件的变量名 
FILE*reset,*play,*music;
//通道和颜色名的对应表 
const char*cc[17];
//初始化 
void init(){
	//打开文件 
//	play=fopen("LemonTreePlay.mcfunction","w");
//	reset=fopen("LemonTreeReset.mcfunction","w");
	music=fopen("LemonTreeMusic.mcfunction","w");
	//初始化起始点 
	pinit.pitch=64;
	pinit.channel=-1;
	pinit.tick=-10;
	//定义channel所对应的颜色名 
	cc[3]="Orange";
	cc[4]="Green";
	cc[6]="Yellow";
	cc[11]="Red";
	cc[9]="White";
	
}
//结束 
void end(){
	//关闭文件 
	fclose(play);
	fclose(reset);
	fclose(music);
}
//输出指令 
int aaaa;
void writecmd1(Point a,Point b){
//	aaaa++;
//	if(aaaa%60==0)cout<<aaaa<<endl;
	if(b.channel==4||b.channel==3)
	{
		if(b.channel==4)
		{
			fprintf(play,"execute @a[scores={tick=%d}] %d %d %d particle Rainbow2Parabola%d_%d_%lf ~~~\n",a.tick,a.tick/2,200,a.pitch,(b.tick-a.tick)/2,b.pitch-a.pitch,0.5);
			fprintf(play,"execute @a[scores={tick=%d}] %d %d %d function %sCircle\n",a.tick,a.tick/2,200,a.pitch,cc[b.channel]);
		}
		else
		{
			fprintf(play,"execute @a[scores={tick=%d}] %d %d %d particle Rainbow2Parabola%d_%d_%lf ~~~\n",a.tick,a.tick/2,200,a.pitch+35,(b.tick-a.tick)/2,b.pitch-a.pitch,0.5);
			fprintf(play,"execute @a[scores={tick=%d}] %d %d %d function %sCircle\n",a.tick,a.tick/2,200,a.pitch+35,cc[b.channel]);
		}
		//根据需要创造.particle.json
		parabola4("e:\\程序文件\\程序文件\\particle\\particles\\",(b.tick-a.tick)/2,b.pitch-a.pitch,1,0.5);
		circle("e:\\程序文件\\程序文件\\particle\\particles\\",cc[b.channel],dtcl);
	}
	else if(b.channel==6||b.channel==11)
	{
		fprintf(play,"execute @a[scores={tick=%d}] %d %d %d particle %sBeeline%d_%d_%d_%lf ~~~\n",a.tick,a.tick/2,240,a.pitch,cc[b.channel],(b.tick-a.tick)/2,0,b.pitch-a.pitch,0.5);
		beeline("e:\\程序文件\\程序文件\\particle\\particles\\",cc[b.channel],dtcl,(b.tick-a.tick)/2,0,b.pitch-a.pitch,0.5);
	}
	else if(b.channel==9)
	{
		fprintf(play,"execute @a[scores={tick=%d}] %d %d %d particle %sBeeline%d_%d_%d_%lf ~~~\n",a.tick,a.pitch+171,30,cc[b.channel],dtcl,(b.tick-a.tick)/2,b.pitch-a.pitch,0,0.5);
		fprintf(play,"execute @a[scores={tick=%d}] %d %d %d particle %sBeeline%d_%d_%d_%lf ~~~\n",a.tick,a.pitch+171,105,cc[b.channel],dtcl,(b.tick-a.tick)/2,b.pitch-a.pitch,0,0.5);
		beeline("e:\\程序文件\\程序文件\\particle\\particles\\",cc[b.channel],dtcl,(b.tick-a.tick)/2,b.pitch-a.pitch,0,0.5);
	}
}
void resetblock()
{
	for(int i=1;i<=noteidx;i++)
	{
		if(note[i].channel==4)
		{
			fprintf(reset,"setblock %d %d %d kami_chan_6\n",note[i].tick/2,200,note[i].pitch);
		}
		else if(note[i].channel==3)
		{
			fprintf(reset,"setblock %d %d %d kami_chan_8\n",note[i].tick/2,200,note[i].pitch+35);
		}
		else if(note[i].channel==6)
		{
			fprintf(reset,"setblock %d %d %d kami_chan_13\n",note[i].tick/2,240,note[i].pitch);
		}
		else if(note[i].channel==11)
		{
			fprintf(reset,"setblock %d %d %d kami_chan_11\n",note[i].tick/2,240,note[i].pitch);
		}
		else if(note[i].channel==9)
		{
			fprintf(reset,"setblock %d %d %d kami_chan_12\n",note[i].tick/2,note[i].pitch+171,30);
			fprintf(reset,"setblock %d %d %d kami_chan_12\n",note[i].tick/2,note[i].pitch+171,105);
		}
	}
}
void resetmusic()
{
	for(int i=1;i<=noteidx;i++)
	{
		
		if(note[i].velocity==0)continue;
		
		//主旋律 
		if(note[i].channel==3)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ~~-1~ playsound 7a.%d @s ~~~ %lf 1 0\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
		}
		//主伴 
		else if(note[i].channel==0||note[i].channel==1||note[i].channel==4||note[i].channel==12)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ~5~-5~ playsound note.harp @s ~~~ %lf %lf 0\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
			fprintf(music,"execute @a[scores={tick=%d}] ~-5~-5~ playsound note.harp @s ~~~ %lf %lf 0\n",note[i].tick,note[i].velocity/127.0,pow(2,(note[i].pitch-66)/12.0));
		}
		//添油加醋 
		else if(note[i].channel==5||note[i].channel==6||note[i].channel==7||note[i].channel==10||note[i].channel==11||note[i].channel==15)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ~~5~ playsound 34a.%d @s ~~~ %lf 1 0\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
		}
		//次伴 
//		else if(note[i].channel==2||note[i].channel==8||note[i].channel==13||note[i].channel==14)
//		{
//			fprintf(music,"execute @a[scores={tick=%d}] ~7~5~ playsound 1a.%d @s ~~~ %lf\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
//			fprintf(music,"execute @a[scores={tick=%d}] ~-7~5~ playsound 1a.%d @s ~~~ %lf\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
//		}
		//打击乐 
		else if(note[i].channel==9)
		{
			fprintf(music,"execute @a[scores={tick=%d}] ~10~~ playsound 0a.%d @s ~~~ %lf 1 0\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
			fprintf(music,"execute @a[scores={tick=%d}] ~-10~~ playsound 0a.%d @s ~~~ %lf 1 0\n",note[i].tick,note[i].pitch,note[i].velocity/127.0);
		}
	}
} 
void setb()
{
	FILE*www=fopen("LemonTreeSet.mcfunction","w");
	for(int i=1;i<=noteidx;i++)
	{

		if(note[i].channel==4)
		{
			fprintf(www,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].tick,note[i].tick/2,200,note[i].pitch);
		}
		else if(note[i].channel==3)
		{
			fprintf(www,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].tick,note[i].tick/2,200,note[i].pitch+35);
		}
		else if(note[i].channel==6)
		{
			fprintf(www,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].tick,note[i].tick/2,240,note[i].pitch);
		}
		else if(note[i].channel==11)
		{
			fprintf(www,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].tick,note[i].tick/2,240,note[i].pitch);
		}
		else if(note[i].channel==9)
		{
			fprintf(www,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].tick,note[i].tick/2,note[i].pitch+171,30);
			fprintf(www,"execute @a[scores={tick=%d}] ~~~ setblock %d %d %d air\n",note[i].tick,note[i].tick/2,note[i].pitch+171,105);
		}
	}
	fclose(www);
}
int main(){ 
//	clock_t start,endd;
//	start=clock();
	init();
//	cout<<1;
	getnote("e:\\程序文件\\程序文件\\gev\\LemonTree.gev");
//	cout<<2;
//	makeline(pinit,writecmd1); 
//	resetblock();
	resetmusic();
//	setb();
//	cout<<3;
//	cout<<GAME_TICK_MAX;
	end();
//	endd=clock();
//	cout<<(double)(endd-start)/1000;
//	circle("e:\\程序文件\\程序文件\\particle\\particles\\","Yellow",dtcl);
	return 0; 
}
