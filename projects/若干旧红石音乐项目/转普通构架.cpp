#include<bits/stdc++.h>
using namespace std;
int tpqn;
int tk,td,yg,yl;
int ltk=-1;
int nn=1;
int num=3;
int xn=0,zn;
int main(){
	FILE*in=fopen("e:\\程序文件\\程序文件\\gev\\windmill.gev","r");
	FILE*out=fopen("e:\\程序文件\\程序文件\\mcfunction\\wdm.mcfunction","w");
	while(1){
		int laji;
		char*type=(char*)malloc(50),*strlj=(char*)malloc(50);
		fscanf(in,"%s",type);
		if(!strcmp(type,"end"))break;
		if(!strcmp(type,"ts")||!strcmp(type,"tempo")||!strcmp(type,"endtrack")||!strcmp(type,"other")){
			fscanf(in,"%[^\n]\n",strlj);
			continue;
		}
		if(!strcmp(type,"tpqn"))fscanf(in,"%d%d%d\n",&laji,&laji,&tpqn);
		else if(!strcmp(type,"note")){
			fscanf(in,"%d%d%d%d%d\n",&tk,&laji,&td,&yg,&yl);
			if(yl==0)continue;
			xn+=(tk-ltk)*1200/tpqn/150/2;
			if(ltk!=-1){
				if(ltk==tk)nn++;
				else{
					nn=1;
					xn++;
				}
			}
			zn=nn/2*((nn-1)%2?1:-1);
			fprintf(out,"clone -100 4 %d -100 4 %d %d 4 %d\n",yg,yg,xn,zn);
			fprintf(out,"setblock %d 5 %d redstone_wire\n",xn,zn);
			ltk=tk;
			num+=2;
		}
		else printf("error");
	}
	int li=0;
	for(int i=1;i<=xn;i+=60){
		fprintf(out,"fill %d 4 0 %d 4 0 unpowered_repeater 1 replace air\n",li,i);
		li=i;
	}
	fprintf(out,"fill %d 4 0 %d 4 0 unpowered_repeater 1 replace air\n",li,xn);
	cout<<"ok";
	fclose(out);
	fclose(in);
	return 0;
}
