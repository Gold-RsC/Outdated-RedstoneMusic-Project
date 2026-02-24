#include<string>
#include<stdio.h>
using namespace std;
void red_appear(){
    FILE*r=fopen("particle_template/red_appear.particle.template","r");
    FILE*w=fopen("mcpack/hsyy_hopes_and_dreams/r/particles/red_appear.particle.json","w");
    while(!feof(r)){
        char c=getc(r);
        fputc(c,w);
    }
    fclose(r);
    fclose(w);
}
void red_static(const string&name,double second){
    FILE*r=fopen("particle_template/red_static.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void red_move(const string&name,double second,double speed){
    FILE*r=fopen("particle_template/red_move.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                case 's':{
                    fprintf(w,"%f",speed);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void red_change(){
    FILE*r=fopen("particle_template/red_change.particle.template","r");
    FILE*w=fopen("mcpack/hsyy_hopes_and_dreams/r/particles/red_change.particle.json","w");
    while(!feof(r)){
        char c=getc(r);
        fputc(c,w);
    }
    fclose(r);
    fclose(w);
}
void red_float(const string&name,double second){
    FILE*r=fopen("particle_template/red_float.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void white_rain1(const string&name){
    FILE*r=fopen("particle_template/white_rain1.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void white_rain2(const string&name){
    FILE*r=fopen("particle_template/white_rain2.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}

void asriel_appear(){
    FILE*r=fopen("particle_template/asriel_appear.particle.template","r");
    FILE*w=fopen("mcpack/hsyy_hopes_and_dreams/r/particles/asriel_appear.particle.json","w");
    while(!feof(r)){
        char c=getc(r);
        fputc(c,w);
    }
    fclose(r);
    fclose(w);
}
void asriel_1float(const string&name,double second){
    FILE*r=fopen("particle_template/asriel_1float.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void asriel_8float(const string&name,double second){
    FILE*r=fopen("particle_template/asriel_8float.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void asriel_8float_shadow(const string&name,double second){
    FILE*r=fopen("particle_template/asriel_8float_shadow.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}

void heartAll_appear(){
    for(int i=0;i<6;++i){
        FILE*r=fopen(("particle_template/heart"+to_string(i)+"_appear.particle.template").c_str(),"r");
        FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/heart"+to_string(i)+"_appear.particle.json").c_str(),"w");
        while(!feof(r)){
            char c=getc(r);
            fputc(c,w);
        }
        fclose(r);
        fclose(w);
    }
}
void heartAll_rotate(){
    for(int i=0;i<6;++i){
        FILE*r=fopen(("particle_template/heart"+to_string(i)+"_rotate.particle.template").c_str(),"r");
        FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/heart"+to_string(i)+"_rotate.particle.json").c_str(),"w");
        while(!feof(r)){
            char c=getc(r);
            fputc(c,w);
        }
        fclose(r);
        fclose(w);
    }
}

void background1(const string&name,double second){
    FILE*r=fopen("particle_template/background1.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void background2(const string&name,double second){
    FILE*r=fopen("particle_template/background2.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 't':{
                    fprintf(w,"%f",second);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}

void gun_outline(const string&name,int angle){
	FILE*r=fopen("particle_template/gun_outline.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 'a':{
                    fprintf(w,"%d",angle);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
void gun_rainbowray(const string&name,int angle){
	FILE*r=fopen("particle_template/gun_rainbowray.particle.template","r");
    FILE*w=fopen(("mcpack/hsyy_hopes_and_dreams/r/particles/"+name+".particle.json").c_str(),"w");
    while(!feof(r)){
        char c=getc(r);
        if(c=='@'){
            switch(c=getc(r)){
                case 'n':{
                    fprintf(w,"%s",name.c_str());
                    break;
                }
                case 'a':{
                    fprintf(w,"%d",angle);
                    break;
                }
                default:{
                    fprintf(w,"@%c",c);
                    break;
                }
            }
        }
        else{
            fputc(c,w);
        }
    }
    fclose(r);
    fclose(w);
}
