#include"header_file\NotePoint.h"
//变量
MidiFun midi;
//装函数的文件
FILE*function;
//音符事件，相当于一个点，所以类型名为Point
Point note[30000];
//数量
int note_num;
//BPM，这里只是做辅助作用，写上即可
PBPM bpm[1000];
//数量
int bpm_num;
//需要的乐器，下标与乐器编号错开了1位
bool program[129];
void init() {
	//初始化
	midi.init("midi\\跳舞的线-丑小鸭.mid",note,bpm);
	//获取事件
	midi.getev(30000,1400);
	//获取数量
	note_num=midi.note_num();
	//这不用说吧
	function=fopen("mcfunction\\fnbdct.mcfunction","w");
}
//音乐部分
void makemusic() {
	//note从0开始，note_num-1结束
	for(int i=0; i<note_num; i++) {
		//有些mid以type为判断是否为松开音符
		//(如windmill中事件为noteoff，但音量不为0)
		//有些以velocity
		//(如春节序曲只有noteon，没有noteoff，它以音量是否为0作为判断是否为noteoff的标志)，
		//所以写成这样最为保守
		if(!note[i].type||!note[i].velocity)
			continue;
		//一般情况，通道9为打击乐专用通道，其编号在神羽资源包中为-1
		//当然有二般情况
		if(note[i].channel==9)
			note[i].prgrm=-1;
		//记录需要的乐器编号
		program[note[i].prgrm+1]=1;
		//输出指令
		fprintf(function,"execute @a[scores={tick=%d}] ~~~ playsound %da.%d @s ~~~ %lf 1 0\n",note[i].gt,note[i].prgrm,note[i].pitch,note[i].velocity/127.0);
	}
}
//be特有自定义粒子，雨Java无瓜
bool rainbow_particle(const char*path,int x,int z) {
	char*n=(char*)malloc(200);
	sprintf(n,"%sRainbow%d_%d.json",path,x,z);
	FILE*particle_file=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	fprintf(particle_file,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"Rainbow%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 2},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age/variable.particle_lifetime)+0.2\", \"0.4*(1-variable.particle_age/variable.particle_lifetime)+0.2\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": {\"interpolant\": \"variable.particle_age/variable.particle_lifetime\",\"gradient\": {\"0.0\": \"#FF0000\",\"0.17\": \"#FF6D00\",\"0.33\": \"#FFF500\",\"0.5\": \"#40FF00\",\"0.67\": \"#00DBFF\",\"0.83\": \"#9B00FF\",\"1.0\": \"#FF0000\"}}}}}}",
	        x,z,
	        x*1.000/20,
	        x,-dis*2,z);
	fclose(particle_file);
	return 1;
}
//makeline的一部分，已知两点输出连线
void writecmd(Point a,Point b) {
	fprintf(function,"execute @a[scores={tick=%d}] %d %d %d particle Rainbow%d_%d ~~~\n",a.gt,a.gt,200,a.pitch,b.gt-a.gt,b.pitch-a.pitch);
	//为防止你不看程序就运行而让电脑爆炸，我把这行注释掉了
//	rainbow_particle("particles\\",b.gt-a.gt,b.pitch-a.pitch);
}
//绘线
void drawline() {
	Point pinit= {
		0,-10,-1,64,127,
		1,
		1,
	};
	midi.makeline(pinit,writecmd);
}
//通知需要的乐器
void outprogram() {
	printf("请自行准备以下编号的乐器\n");
	for(int i=0; i<=128; i++)
		if(program[i])
			printf("%d ",i-1);
}
//结束
void end() {
	fclose(function);
}
int main() {
	//初始化
	init();
	//音乐部分
	makemusic();
	//绘制线线
//	drawline();
	//输出需要的乐器资源包编号
	outprogram();
	//结束
	end();
	return 0;
}

