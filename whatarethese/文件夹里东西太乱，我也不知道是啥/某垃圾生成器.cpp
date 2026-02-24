#include"header_file\\MidiFun.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
using namespace std;
MidiFile mid;
NOTE note;
int main() {
	printf("注意事项:\n本生成器仅可在特制的地图中使用\n特制地图：超平坦地图里在-100 4 z的地方(z表示音高编号，范围0～127)依次放置发音方块，并设置[-100,4,0]至[-100,4,127]为常加载区(/tickingarea)\n");
	char tin[100],tout[100];
	cout<<"调用的文件名（加后缀）："<<endl;
	cin.getline(tin,100);
	mid.read(tin);
	mid.get_note(note);
	cout<<"导出的文件名（加后缀,必须是.mcfunction才可在mc中运行）："<<endl;
	cin.getline(tout,100);
	BPM _bpm;
	mid.get_bpm(_bpm);
	int bpm=_bpm[0].value;
	int midtk=mid.get_tpqn();
	FILE*fout=fopen(tout,"w");
	int tk,td,yg,yl;//四要素
	int ltk=-1;//上一个tk
	int nn=1;//notenumber
	int num=3;//函数计数君
	int xn=0,zn;//x,z轴坐标
	fprintf(fout,"setblock -10 4 0 quartz_block\nsetblock -10 5 0 stone_button\nfill -9 4 0 -1 4 0 unpowered_repeater 1\n");
	for(int i=0;i<note.size();i++) {
		tk=note[i].tick;
		td=note[i].channel;
		yg=note[i].pitch;
		yl=note[i].velocity;
		if(yl==0)continue;
		xn+=(tk-ltk)*1200/midtk/bpm/2;
		if(ltk!=-1) {
			if(ltk==tk)nn++;
			else {
				nn=1;
				xn++;
			}
		}
		zn=nn/2*((nn-1)%2?1:-1);
		fprintf(fout,"clone -100 4 %d -100 4 %d %d 4 %d\n",yg,yg,xn,zn);
		fprintf(fout,"setblock %d 5 %d redstone_wire\n",xn,zn);
		ltk=tk;
		num+=2;
	}
	int li=0;
	for(int i=1; i<=xn; i+=60) {
		fprintf(fout,"fill %d 4 0 %d 4 0 unpowered_repeater 1 replace air\n",li,i);
		li=i;
	}
	fprintf(fout,"fill %d 4 0 %d 4 0 unpowered_repeater 1 replace air\n",li,xn);
	num+=li+1;
	cout<<"生成成功！！！共生成"<<num<<"个指令\n";
	fclose(fout);
	system("pause");
	return 0;
}
