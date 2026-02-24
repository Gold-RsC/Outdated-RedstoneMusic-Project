#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<string.h>
#include<cstring>
using namespace std;
int main(){
    cout<<"欢迎使用红石音乐生成器"<<endl;
    cout<<"=====作者：Gold_RsC====="<<endl;
    printf("注意事项:\n本生成器仅可在特制的地图中使用\n特制地图：超平坦地图里在-100 4 z的地方(z表示音高编号，范围0～127)依次放置发音方块，并设置[-100,4,0]至[-100,4,127]为常加载区(/tickingarea)\n使用方法:\n先用Midi Voyager复制出MidiEvent中的NOTE然后用文本编辑器处理成四列数据，\n分别是:miditick channel pitch volume，\n然后每行一个这样的事件，保存为xx.txt的格式\n");
    cout<<"eg："<<endl<<"2928 7 71 100"<<endl<<"2928 8 71 100"<<endl<<"2976 7 70 100"<<endl<<"2976 8 70 100"<<endl<<"3024 7 66 100"<<endl<<"3024 8 66 100"<<endl<<"3072 7 66 100"<<endl<<"3072 8 66 100"<<endl<<"3120 7 68 100"<<endl<<"3120 8 68 100"<<endl<<"3144 7 68 100"<<endl<<"3144 8 68 100"<<endl<<"3216 7 66 100"<<endl<<"3216 8 66 100"<<endl<<"3264 7 66 100"<<endl<<"3264 8 66 100"<<endl<<"3312 7 68 100"<<endl<<"3312 8 68 100"<<endl<<"3336 7 68 100"<<endl<<"3336 8 68 100"<<endl<<"3408 7 66 100"<<endl<<"3408 8 66 100"<<endl<<"3456 7 66 100"<<endl;
    char tin[100],tout[100];
    cout<<"调用的文件名（不加后缀）："<<endl;
    cin>>tin;
    cout<<"导出的文件名（不加后缀）："<<endl;
    cin>>tout;
    cout<<"BPM："<<endl;
    int bpm;
    cin>>bpm;
    cout<<"一个四分音符的miditick值："<<endl;
    int midtk;
    cin>>midtk;
    FILE*fin=fopen(strcat(tin,".txt"),"r");
    FILE*fout=fopen(strcat(tout,".mcfunction"),"w");
    if(fin==NULL){
        cout<<"请检查您的文件名是否正确或检查文件中是否有信息";
        return 1;
    }int tk,td,yg,yl;//四要素
    int ltk=-1;//上一个tk
    int nn=1;//notenumber
    int num=3;//函数计数君
    int xn=0,zn;//x,z轴坐标
    fprintf(fout,"setblock -10 4 0 quartz_block\nsetblock -10 5 0 stone_button\nfill -9 4 0 -1 4 0 unpowered_repeater 1\n");
    while(fscanf(fin,"%d%d%d%d",&tk,&td,&yg,&yl)>0){
        if(yl==0)continue;
        xn+=(tk-ltk)*1200/midtk/bpm/2;
        if(ltk!=-1){
            if(ltk==tk)nn++;
            else{
                nn=1;
                xn++;
        }}zn=nn/2*((nn-1)%2?1:-1);
        fprintf(fout,"clone -100 4 %d -100 4 %d %d 4 %d\n",yg,yg,xn,zn);
        fprintf(fout,"setblock %d 5 %d redstone_wire\n",xn,zn);
        ltk=tk;
        num+=2;
    }
    int li=0;
    for(int i=1;i<=xn;i+=60){
        fprintf(fout,"fill %d 4 0 %d 4 0 unpowered_repeater 1 replace air\n",li,i);
        li=i;
    }fprintf(fout,"fill %d 4 0 %d 4 0 unpowered_repeater 1 replace air\n",li,xn);
    num+=li+1;
    cout<<"生成成功！！！共生成"<<num<<"个指令";
    fclose(fin);
    fclose(fout);
    return 0;
}