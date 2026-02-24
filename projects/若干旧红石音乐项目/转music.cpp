#include<bits/stdc++.h>
using namespace std;
int tpqn;
int tk,td,yg,yl;
int ltk=-1;
int nn=1;
int num=3;
int xn=0,zn;
int main(){
	FILE*r=fopen("e:\\程序文件\\程序文件\\gev\\windmill.gev","r");;
	FILE*w=fopen("e:\\程序文件\\程序文件\\mcfunction\\wdmm.mcfunction","w");
	int tpqn;
	while(1){
		int laji;
		char*type=(char*)malloc(50),*strlj=(char*)malloc(50);
		if(fscanf(r,"%s",type)==EOF)
			break;
		if(!strcmp(type,"end"))
			break;
		else if(!strcmp(type,"endtrack"))
			fscanf(r,"%[^\n]\n",strlj);
		else if(
				!strcmp(type,"ts")||
				!strcmp(type,"tempo")
			)fscanf(r,"%[^\n]\n",strlj);
		else if(!strcmp(type,"other"))
			fscanf(r,"\n");
		else if(!strcmp(type,"tpqn"))
			fscanf(r,"%d%d%d\n",&laji,&laji,&tpqn);
		else if(!strcmp(type,"note")){
			int tk,td,yg,yl;
			fscanf(r,"%d%d%d%d%d\n",
				&tk,
				&laji,
				&td,
				&yg,
				&yl
			);
			if(yl==0)continue;
			if(td==9)td=0;
			else td=1;
			int gt=tk*1200/tpqn/150;
			fprintf(w,"execute @a[scores={tick=%d}] ~~~ playsound %da.%d @s ~~~ %lf\n",gt,td,yg,(yl+1)*100.0000/128);
		}
		else
			printf("error:%s\n",type);
	}
	cout<<"ok";
	fclose(r);
	fclose(w);
	return 0;
}
