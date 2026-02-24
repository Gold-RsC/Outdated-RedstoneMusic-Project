#include"MidiFun.h"
void test1(){
	MidiFun mid("midi\\China-x.mid");
	NOTE note;
	FILE*w=fopen("out_file\\test1.txt","w");
	mid.get_note(note);
	int count_note=note.size();
	for(int idex=0;idex<count_note;idex++)
		if(note[idex].instrument != 9 && note[idex].instrument <= 120) fprintf(w,"%d %d %d %lf\n",note[idex].tick,(note[idex].instrument == 0) ? 1 : note[idex].instrument,note[idex].pitch,(note[idex].volume/127.0));
	fclose(w);
}

FILE*test2_file;
void test2_writecmd(NOTEev a,NOTEev b){
	if(b.channel!=9 && b.channel <= 120)
		fprintf(test2_file,"%d %d %d %d %d\n",a.tick,a.pitch,b.tick,b.pitch,a.instrument);
}
void test2(){
	MidiFun mid;
	mid.init("midi\\China-x.mid");
	test2_file=fopen("out_file\\test2.txt","w");
	NOTEev pinit(-10,-2,0,64,128,0);
	mid.makeline1(pinit,test2_writecmd);
	fclose(test2_file);
}

int main(){
	//制作函数音乐
	test1();
	//制作粒子特效(makeline函数) 
	test2();
	
	return 0;
}
