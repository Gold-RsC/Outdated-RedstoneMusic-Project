#include<windows.h>
#include"headfile\\MidiFun.h"
using namespace std;
//定义
MidiFun mid;
Note note[30000];
Lyric lyric[1000];
int noteNum;
int lyricNum;
//初始操作
void init(void) {
	//初始化
	mid.parse("midi\\windmill.mid");
	noteNum=mid.getNote(note);
	lyricNum=mid.getLyric(lyric);

	//转化单位
	//time单位是毫秒，与gametick的进率为：1gt=50ms
	//time除以50后可能会导致精度不够，做出来的音乐一会儿快一会儿慢，所以可以用第二种方法来做
	//法一
	//下面被lambda框起来的代码不会执行，如果想用这种方法可以把lambda的首尾去掉，并把前面法二的代码删掉
	[&]{
		for(int i=0; i<noteNum; i++) {
			note[i].time/=50;
		}
		for(int i=0; i<lyricNum; i++) {
			lyric[i].time/=50;
		}
	};
	//法二
	//基本思路：找出一个合适的进率(可理解为速度，50附近，可以为小数) ，使几乎所有音符的time除以进率的尾数近乎相同(尾数：小数部分) ，那么就可以整体忽略掉小数部分 
	double speed,errDegree=-1;
	for(double v=48;v<=52;v+=0.05){
		double degree=0;
		double sum=0;
		for(int i=0;i<noteNum;i++){
			sum+=note[i].time-note[i].time/v;
		}
		sum/=noteNum;
		for(int i=0;i<noteNum;i++){
			degree+=(note[i].time-int(note[i].time/v)*v)*(note[i].time-int(note[i].time/v)*v);
		}
		degree/=noteNum;
		if(degree<errDegree||errDegree<0){
			speed=v;
			errDegree=degree;
		}
	}
	for(int i=0; i<noteNum; i++) {
		note[i].time/=speed;
	}
	for(int i=0; i<lyricNum; i++) {
		lyric[i].time/=speed;
	}
	
	//以绝对时间从小到大进行排序
	sort(note,note+noteNum,[](Note&a,Note&b) {
		return a.time<b.time;
	});
	[&](){
		FILE*w=fopen("outFile/a.txt","w");
		for(int i=0;i<noteNum;i++){
			fprintf(w,"%d %d %d\n",note[i].time,note[i].beat,note[i].bar);
		}
		fclose(w);
	}();
}

//新加的部分，NotePair类(将两个Note对象合并到一个对象里)相关用法 
void fun6(void){
	NotePair notepair[30000];
	int num=change(note,notepair,noteNum);
	
	//输出 
	FILE*w=fopen("outFile\\音符对.txt","w");
	fprintf(w,"beginTime\tendTime\ttrack\tchannel\tbar\tbeat\tpitch\tvolume\tinstrument\n");
	for(int i=0; i<num; i++) {
		fprintf(w,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",notepair[i].beginTime,notepair[i].endTime,notepair[i].track,notepair[i].channel,notepair[i].bar,notepair[i].beat,notepair[i].pitch,notepair[i].volume,notepair[i].instrument);
//		fprintf(w,"%d\t%d\t%d\t%d\n",notepair[i].beginTime,notepair[i].pitch,notepair[i].volume,notepair[i].instrument);
	}
	fclose(w);
}
int main() {
	init();
	fun6();
	return 0;
}
