#ifndef PARTICLE_H
#define PARTICLE_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
namespace ptc{
	//parabola1:很基础，没啥特别的 
	bool parabola1(const char*path,const char*color,char* (*dtcl)(const char*),int x,int z,double speed){
		char*n=(char*)malloc(200);
		sprintf(n,"%s%s1Parabola%d_%d_%lf.json",path,color,x,z,speed);
		FILE*ff=fopen(n,"w");
		double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
		//																									   +		 +  +																																																																				+																										+		  +																																								  +																																																																																																																						  +		
		fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sParabola%d_%d_%lf\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",
																																																																																																																																																																																																																																																																																															color,x,z,speed,
																																																																																																																																																																																																																																																																																															x*1.0/20/speed,
																																																																																																																																																																																																																																																																																															x,-dis*4,z,
																																																																																																																																																																																																																																																																																															dtcl(color));
		fclose(ff);
		return 1;
	}
	//parabola2:在prbl1的基础上添加了y轴“方向”
	bool parabola2(const char*path,const char*color,char* (*dtcl)(const char*),int x,int z,bool direction,double speed){
		char*n=(char*)malloc(200);
		sprintf(n,"%s%s2Parabola%d_%d_%lf.json",path,color,x,z,speed);
		FILE*ff=fopen(n,"w");
		double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
		//																									   +		 +  +																																																																				+																										+		  +																																								  +																																																																																																																						  +		
		fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sParabola%d_%d_%lf\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",
																																																																																																																																																																																																																																																																																															color,x,z,speed,
																																																																																																																																																																																																																																																																																															x*1.000/20/speed,
																																																																																																																																																																																																																																																																																															x,-(direction?1:-1)*dis*4,z,
																																																																																																																																																																																																																																																																																															dtcl(color));
		fclose(ff);
		return 1;
	}
	//parabola3:在parabola2的基础上把颜色改成彩虹色(全身同时变) 
	//不建议使用 
	bool parabola3(const char*path,int x,int z,bool direction,double speed){
		char*n=(char*)malloc(200);
		sprintf(n,"%sRainbow1Parabola%d_%d_%lf.json",path,x,z,speed);
		FILE*ff=fopen(n,"w");
		double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
		fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"Rainbow1Parabola%d_%d_%lf\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": {\"interpolant\": \"variable.emitter_age/variable.emitter_lifetime\",\"gradient\": {\"0.0\": \"#FF0000\",\"0.17\": \"#FF6D00\",\"0.33\": \"#FFF500\",\"0.5\": \"#40FF00\",\"0.67\": \"#00DBFF\",\"0.83\": \"#9B00FF\",\"1.0\": \"#FF0000\"}}}}}}",
																																																																																																																																																																																																																																																																																																																																																									x,z,speed,
																																																																																																																																																																																																																																																																																																																																																									x*1.000/20/speed,
																																																																																																																																																																																																																																																																																																																																																									x,-(direction?1:-1)*dis*4,z);
		fclose(ff);
		return 1;
	}
	//parabola4:区别于parabola3，不同位置不同颜色 
	bool parabola4(const char*path,int x,int z,bool direction,double speed){
		char*n=(char*)malloc(200);
		sprintf(n,"%sRainbow2Parabola%d_%d_%lf.json",path,x,z,speed);
		FILE*ff=fopen(n,"w");
		double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
		fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"Rainbow2Parabola%d_%d_%lf\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 2},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age/variable.particle_lifetime)+0.2\", \"0.4*(1-variable.particle_age/variable.particle_lifetime)+0.2\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": {\"interpolant\": \"variable.particle_age/variable.particle_lifetime\",\"gradient\": {\"0.0\": \"#FF0000\",\"0.17\": \"#FF6D00\",\"0.33\": \"#FFF500\",\"0.5\": \"#40FF00\",\"0.67\": \"#00DBFF\",\"0.83\": \"#9B00FF\",\"1.0\": \"#FF0000\"}}}}}}",
																																																																																																																																																																																																																																																																																																																																																																											x,z,speed,
																																																																																																																																																																																																																																																																																																																																																																											x*1.000/20/speed,
																																																																																																																																																																																																																																																																																																																																																																											x,-(direction?1:-1)*dis*4,z);
		fclose(ff);
		return 1;
	}
	bool circle(const char*path,const char*color,char* (*dtcl)(const char*)){
		char*nname=(char*)malloc(500);
		sprintf(nname,"%s%sCircle.mcfunction",path,color);
		FILE*ww=fopen(nname,"w");
		for(int i=0;i<360;i+=5)
		{
			char*name=(char*)malloc(500);
			sprintf(name,"%s%sCircle_%d.json",path,color,i);
			FILE*ff=fopen(name,"w");
			fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sCircle_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_initialization\": {\"creation_expression\": \"variable.size = 0.08;variable.radius = 0.6;\"},\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": 2},\"minecraft:emitter_shape_point\": {\"offset\": [\"math.cos(%d)*(variable.emitter_age/variable.emitter_lifetime)*5\", 0, \"math.cos(%d)*(variable.emitter_age/variable.emitter_lifetime)*5\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 0.07},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [0.5, 0.5],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",color,i,i,90-i,dtcl(color));
			fclose(ff);
			fprintf(ww,"particle %sCircle_%d ~~~\n",color,i);
		}
		fclose(ww);
		return 1;
	}
	bool beeline(const char*path,const char*color,char* (*dtcl)(const char*),int x,int y,int z,double speed){
		char*n=(char*)malloc(200);
		sprintf(n,"%s%sBeeline%d_%d_%d_%lf.json",path,color,x,y,z,speed);
		FILE*ff=fopen(n,"w");
		double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
		fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sBeeline%d_%d_%d_%lf\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"variable.emitter_age/variable.emitter_lifetime * %d\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age/variable.particle_lifetime)\", \"0.4*(1-variable.particle_age/variable.particle_lifetime)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",
																																																																																																																																																																																																																																																																																			color,x,y,z,speed,
																																																																																																																																																																																																																																																																																			x*1.000/20/speed,
																																																																																																																																																																																																																																																																																			x,y,z,
																																																																																																																																																																																																																																																																																			dtcl(color));
		fclose(ff);
		return 1;
	}
};
#endif
