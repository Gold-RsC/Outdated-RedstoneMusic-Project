#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<iostream>
bool parabola4(const char*path,const char*color,char* (*dtcl)(const char*),int x,int z,double high) {
	char*n=(char*)malloc(200);
	sprintf(n,"%s%sParabola%d_%d.json",path,color,x,z);
	FILE*ff=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	//																									   +		 +  +																																																																				+																										+		  +																																								  +																																																																																																																						  +
	fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sParabola%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",
	        color,x,z,
	        2.5,
	        x,-dis*high,z,
	        dtcl(color));
	fclose(ff);
	return 1;
}
bool parabola5(const char*path,const char*color,char* (*dtcl)(const char*),double lifetime,int x,int z,double high) {
	char*n=(char*)malloc(200);
	sprintf(n,"%s%sParabola%.2lf_%d_%d.json",path,color,lifetime,x,z);
	FILE*ff=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	//																									   +		 +  +																																																																				+																										+		  +																																								  +																																																																																																																						  +
	fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sParabola%.2lf_%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",
	        color,lifetime,x,z,
	        lifetime,
	        x,-4.0*high,z,
	        dtcl(color));
	fclose(ff);
	return 1;
}
bool parabola6(const char*path,double lifetime,int x,int z,double high) {
	char*n=(char*)malloc(200);
	sprintf(n,"%sRainbowParabola%.2lf_%d_%d.json",path,lifetime,x,z);
	FILE*ff=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"RainbowParabola%.2lf_%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"%lf * variable.emitter_age/variable.emitter_lifetime*(variable.emitter_age/variable.emitter_lifetime - 1)\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 2},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": {\"interpolant\": \"variable.particle_age/variable.particle_lifetime\",\"gradient\": {\"0.0\": \"#FF0000\",\"0.17\": \"#FF6D00\",\"0.33\": \"#FFF500\",\"0.5\": \"#40FF00\",\"0.67\": \"#00DBFF\",\"0.83\": \"#9B00FF\",\"1.0\": \"#FF0000\"}}}}}}",
	        lifetime,x,z,
	        lifetime,
	        x,-4.0*high,z
			);
	fclose(ff);
	return 1;
}
void line(const char*path,const char*color,char*(*dtcl)(const char*),double lifetime,int x,int z) {
	char*n=(char*)malloc(200);
	sprintf(n,"%s%sLine%.2lf_%d_%d.json",path,color,lifetime,x,z);
	FILE*ff=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	//																									   +		 +  +																																																																				+																										+		  +																																								  +																																																																																																																						  +
	fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"%sLine%.2lf_%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"0\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": [%s]}}}}",
	        color,lifetime,x,z,
	        lifetime,
	        x,z,
	        dtcl(color));
	fclose(ff);
}
void line2(const char*path,double lifetime,int x,int z) {
	char*n=(char*)malloc(200);
	sprintf(n,"%sRainbowLine%.2lf_%d_%d.json",path,lifetime,x,z);
	FILE*ff=fopen(n,"w");
	double dis=sqrt(pow(x*1.00,2)+pow(z*1.00,2));
	//																									   +		 +  +																																																																				+																										+		  +																																								  +																																																																																																																						  +
	fprintf(ff,"{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"RainbowLine%.2lf_%d_%d\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/particles\"}},\"components\": {\"minecraft:emitter_rate_steady\": {\"spawn_rate\": 32,\"max_particles\": 10000},\"minecraft:emitter_lifetime_looping\": {\"active_time\": %lf},\"minecraft:emitter_shape_point\": {\"offset\": [\"variable.emitter_age/variable.emitter_lifetime * %d\", \"0\", \"variable.emitter_age/variable.emitter_lifetime * %d\"]},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": 1},\"minecraft:particle_initial_speed\": 0,\"minecraft:particle_motion_dynamic\": {},\"minecraft:particle_appearance_billboard\": {\"size\": [\"0.4*(1-variable.particle_age)\", \"0.4*(1-variable.particle_age)\"],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {\"texture_width\": 128,\"texture_height\": 128,\"uv\": [33, 88],\"uv_size\": [8, 8]}},\"minecraft:particle_appearance_tinting\": {\"color\": {\"interpolant\": \"variable.particle_age/variable.particle_lifetime\",\"gradient\": {\"0.0\": \"#FF0000\",\"0.17\": \"#FF6D00\",\"0.33\": \"#FFF500\",\"0.5\": \"#40FF00\",\"0.67\": \"#00DBFF\",\"0.83\": \"#9B00FF\",\"1.0\": \"#FF0000\"}}}}}}",
	        lifetime,x,z,
	        lifetime,
	        x,z);
	fclose(ff);
}
char*dtcl(const char*color) {
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
		strcpy(colorvalue,"0, 0.5882, 1, 1");
	else if(!stricmp(color,"Orange"))
		strcpy(colorvalue,"1, 0.39216, 0, 1");
	else if(!stricmp(color,"White"))
		strcpy(colorvalue,"1, 1, 1, 1");
	else
		strcpy(colorvalue,"error");
	return colorvalue;
}
int main() {
	for(double lifetime=1.7; lifetime<=3.3; lifetime+=0.1) {
		parabola6("aba\\",lifetime,0,-20,1.7*1.2+(lifetime-1.7)*10+20);
		line2("aba\\",lifetime,0,-20);
	}
//	//1:
//	for(int x=17;x<=73;x++)
//	{
//		parabola4("pt\\particles\\","Yellow",dtcl,x-87,22-0);
//		parabola4("pt\\particles\\","Yellow",dtcl,0,42-22);
//	}
//	for(int x=20;x<=72;x++)
//	{
//		parabola4("pt\\particles\\","Yellow",dtcl,x-87,22-0);
//		parabola4("pt\\particles\\","Yellow",dtcl,0,45-22);
//	}
//	//2:
//	for(int x=2;x<=58;x++)
//	{
//		parabola4("pt\\particles\\","Yellow",dtcl,x+1,27-0);
//		parabola4("pt\\particles\\","Yellow",dtcl,0,58-27);
//	}
//	for(int x=2;x<=58;x++)
//	{
//		parabola4("pt\\particles\\","Yellow",dtcl,x+1,27-0);
//		parabola4("pt\\particles\\","Yellow",dtcl,0,61-27);
//	}
//	for(int i=-100;i<=100;i++)
//	{
//		for(int j=22;j<=70;j++)
//		{
//			parabola4("pt\\particles\\","Yellow",dtcl,i,j);
//		}
//	}
//	for(int i=-100;i<=100;i++){
//		for(int j=-100;j<=100;j++){
//			char a[100];
//			sprintf(a,"pt\\particles\\YellowParabola%d_%d.json",i,j);
//			remove(a);
//		}
//	}
	return 0;
}
