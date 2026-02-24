#include"header_file\MidiFun.h"
#include<iostream>
void test1(){
	MidiFile mid("mid\\tmp.mid");
	NOTE note;
	FILE*w=fopen("out_file\\test1.txt","w");
	mid.get_note(note);
	std::cout<<mid.get_tpqn();
	int count_note=note.size();
	for(int idex=0;idex<count_note;idex++)
		if(note[idex].velocity)
			fprintf(w,"execute @a[scores={tick=%d}] ~~~ playsound %da.%d @s ~~~ %lf 1 0\n",note[idex].tick,1,note[idex].pitch,(note[idex].velocity/127.0));		
	fprintf(w,"scoreboard players add @a tick 1");
	fclose(w);
}

FILE*test2_file;
void test2_writecmd(NOTEev a,NOTEev b){
	if(b.channel!=9)
		fprintf(test2_file,"(%d,%d)---(%d,%d)\n",a.tick,a.pitch,b.tick,b.pitch);
}
void test2(){
	MidiFun mid;
	mid.init("mid\\tmp.mid");
	test2_file=fopen("out_file\\test2.txt","w");
	NOTEev pinit(-10,-2,-2,64,128,-2);
	mid.makeline(pinit,test2_writecmd);
	fclose(test2_file);
}


void test3(){
	MidiFun mid("mid\\tmp.mid");
	NOTE note;
	mid.get_note(note);
	FILE*w=fopen("out_file\\test3.txt","w");
	//音符数 
	int size=note.size();
	//音轨数
	int ntrk=mid.get_ntrk();
	for(int i=0;i<size;i++){
		bool flag=0;
		if(note[i].channel==9){
			fprintf(w,"(%d,%d)在通道9(以0为第一通道)上,乐器编号为%d\n",note[i].tick,note[i].pitch,note[i].instrument);
			flag=1;
		}
		if(note[i].track==1){
			fprintf(w,"(%d,%d)在音轨1(以0为第一音轨)上,乐器编号为%d\n",note[i].tick,note[i].pitch,note[i].instrument);
			flag=1;
		}
		if(flag)
			fprintf(w,"\n");
	}
	fclose(w);
}

void test4(){
	FILE*w=fopen("out_file\\test4.txt","w");
	NOTEev a;
	if(a==NULL_Nev)
		fprintf(w,"这是个空NOTEev事件\n");
	if(a.tick==NULL_TICK)
		fprintf(w,"此tick为空\n");
	//空的note动态数组 
	NOTE arr;
	if(arr[114514]==ERR_Nev)
		fprintf(w,"此NOTEev事件错误\n");
}
int main(){
	//制作函数音乐(基本) 
	test1();
	//制作粒子特效(makeline函数) 
	test2();
	//track和channel的使用 
	test3();
	//宏用法 
	test4();
	MessageBox(NULL,"需要特别注意的一点:\n通道编号为9时 [ 一般 ] (有二般情况)为打击乐,此时音符编号为打击乐乐器编号,与NOTEev的instrument成员无关!!!","Gold_RsC温馨提示",MB_ICONEXCLAMATION|MB_OK);
	int ret=MessageBox(NULL,"记得给我的视频三连","Gold_RsC",MB_ICONEXCLAMATION|MB_YESNO);
	while(ret==7)
		ret=MessageBox(NULL,"请求失败\n记得给我的视频三连","Gold_RsC",MB_ICONEXCLAMATION|MB_YESNO);
	MessageBox(NULL,"谢谢惠顾","Gold_RsC",MB_ICONEXCLAMATION|MB_YESNO);
	return 0;
}
