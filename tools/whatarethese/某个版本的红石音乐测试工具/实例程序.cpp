#include<windows.h>
#include"headfile\\MidiFun.h"
#pragma comment(lib,"winmm.lib")
using namespace std;
//定义
MidiFun mid;
Note note[60000];
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
}

//实例
//打印midi所有事件
void fun1(void) {
	FILE*w=fopen("outFile\\midi所有事件.txt","w");
	for(int track=0; track<mid.getTracksNum(); track++) {
		fprintf(w,"-------------%d-------------\n",track);
		for(int ev=0; ev<mid[track].size(); ev++) {
			for(int idx=0; idx<mid[track][ev].size(); idx++) {
				fprintf(w,"%2.2X ",mid[track][ev][idx]);
			}
			fprintf(w,"\n");
		}
	}
	fclose(w);
}

//打印音符事件(挑了几个重要的，其他不重要的没放进来)
void fun2(void) {
	FILE*w=fopen("outFile\\所有音符(包括打击乐).txt","w");
	fprintf(w,"time\ttrack\tchannel\tbar\tpitch\tvolume\tinstrument\n");
	for(int i=0; i<noteNum; i++) {
		fprintf(w,"%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].bar,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
	}
	fclose(w);
}

//单另把打击乐挑出来成为一个事件
void fun3(void) {
	FILE*w=fopen("outFile\\只有打击乐.txt","w");
	fprintf(w,"time\ttrack\tchannel\tbar\tpitch\tvolume\tinstrument\n");
	for(int i=0; i<noteNum; i++) {
		if(note[i].channel==9) {
			fprintf(w,"%d\t%d\t%d\t%d\t%d\n",note[i].time,note[i].track,note[i].channel,note[i].bar,note[i].pitch,note[i].volume,note[i].bar,note[i].instrument);
		}
	}
	fclose(w);
}

//试听音乐
void fun4(void){
	HMIDIOUT handle;
	midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	Sleep(note[0].time*50);
	for(int i=0; i<noteNum-1; i++) {
		midiOutShortMsg(handle,(note[i].volume<<16)+((note[i].pitch)<<8)+0x90+note[i].channel);
		if(note[i+1].time-note[i].time!=0)
			Sleep((note[i+1].time-note[i].time)*50);
	}
	midiOutShortMsg(handle,(note[noteNum-1].volume<<16)+(note[noteNum-1].pitch<<8)+0x90+note[noteNum-1].channel);
	midiOutClose(handle);
}

//打印歌词
void fun5(void){
	FILE*w=fopen("outFile\\歌词.txt","w");
	fprintf(w,"time\ttext\n");
	for(int i=0;i<lyricNum;i++){
		fprintf(w,"%d\t%s\n",lyric[i].time,lyric[i].text.c_str());
	}
	fclose(w);
}
int main() {
	init();
	fun1();
	fun2();
	fun3();
//	fun4();
	fun5();
	return 0;
}
