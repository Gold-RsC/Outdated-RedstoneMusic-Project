//使用头文件 
#include"NotePoint.h"
//be版特有的particle.json，这是写粒子文件的函数 
bool rb(const char*path,int x,int z){
	char*n=(char*)malloc(200);
	sprintf(n,"%sRainbow%d_%d.json",path,x,z);
	FILE*ff=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"Rainbow%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 2},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age/variable.particle_lifetime)+0.2\", \"0.4*(1-variable.particle_age/variable.particle_lifetime)+0.2\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": {\"interpolant\": \"variable.particle_age/variable.particle_lifetime\",\"gradient\": {\"0.0\": \"#FF0000\",\"0.17\": \"#FF6D00\",\"0.33\": \"#FFF500\",\"0.5\": \"#40FF00\",\"0.67\": \"#00DBFF\",\"0.83\": \"#9B00FF\",\"1.0\": \"#FF0000\"}}}}}}",
																																																																																																																																																																																																																																																																																																																																																																										x,z,
																																																																																																																																																																																																																																																																																																																																																																										x*1.000/20,
																																																																																																																																																																																																																																																																																																																																																																										x,-dis*2,z);
	fclose(ff);
	return 1;
}
//不作解释 
FILE*particle,*music;
//writecmd函数，配合MidiFun的makeline函数使用（作为makeline的参数） 
void writecmd(Point a,Point b){
	fprintf(particle,"execute @a[scores={tick=%d}] %d %d %d particle Rainbow%d_%d ~~~\n",a.gt,a.gt,200,a.pitch,b.gt-a.gt,b.pitch-a.pitch);
	rb("particles\\",b.gt-a.gt,b.pitch-a.pitch);
}
int main(){
	music=fopen("snow.mcfunction","w");
	particle=fopen("snowptc.mcfunction","w");
	//初始化 
	MidiFun m;
	Point note[10000];
	PBPM bpm[100];
	//函数初始化 
	m.init("程序文件\\midi\\雪之梦.mid",note,bpm);
	//获取事件 
	m.getev();
	//定义起始点 
		//参数顺序：tick,gt,channel,pitch,velocity,type,prgrm 
	Point pinit={0,-10,-1,64,0,1,-1};
	//造线 
	m.makeline(pinit,writecmd);
	//getnote返回值为note数量 
	for(int i=0;i<m.get_note();i++){
		//如果type为0，即此note为“松开键盘” 
		if(!note[i].type)
			continue;
		//输出 
		fprintf(music,
			"execute @a[scores={tick=%d}] ~~~ playsound %da.%d @s ~~~ %lf 1 0\n",
			note[i].gt,note[i].prgrm,note[i].pitch,note[i].velocity/127.0);
	}
	//关闭文件 
	fclose(particle);
	fclose(music);
	//程序华丽地结束 
	return 0;
}
