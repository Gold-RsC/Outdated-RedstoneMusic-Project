//#include"headfile\notepoint.h"
#include<iostream>
using namespace std;
//int c(const char*filename){
//		FILE*r=fopen(filename,"r");
//		if(r==NULL)return -2;
//		int tpqn=120;
//		long long tempo=1;
//		while(1){
//			int laji;
//			char*type=(char*)malloc(50),*strlj=(char*)malloc(50);
//			if(fscanf(r,"%s",type)==EOF)
//				return -1; 
//			if(!stricmp(type,"end"))
//				break;
//			else if(!stricmp(type,"endtrack"))
//				fscanf(r,"%[^\n]\n",strlj);
//			else if(!stricmp(type,"ts"))
//				fscanf(r,"%[^\n]\n",strlj);
//			else if(!stricmp(type,"tempo")){
//				fscanf(r,"%d%d%d\n",&notepoint::bpm[++notepoint::bpmidx].tick,&laji,&tempo);
//				notepoint::bpm[notepoint::bpmidx].value=6e7/tempo;
//			}
//			else if(!stricmp(type,"other"))
//				fscanf(r,"%[^\n]\n",strlj);
//			else if(!stricmp(type,"tpqn"))
//				fscanf(r,"%d\n",&tpqn);
//			else if(!stricmp(type,"note")){
//				int tk,td,yg,yl;
//				fscanf(r,"%d%d%d%d%d\n",
//					&tk,
//					&laji,
//					&td,
//					&yg,
//					&yl
//				);
////				if(yl==0)continue;
//				int gt=tk*1200/tpqn/150;
//				
////				printf("gt: %d\n",gt);
//				notepoint::note[++notepoint::noteidx].tick=gt;
//				notepoint::note[notepoint::noteidx].channel=td;
//				notepoint::note[notepoint::noteidx].pitch=yg;
//				notepoint::note[notepoint::noteidx].velocity=yl;
//				notepoint::GAME_TICK_MAX=(gt>notepoint::GAME_TICK_MAX?gt:notepoint::GAME_TICK_MAX);
//			}
//			else{
//				printf("error:%s\n",type);
//				return 0;
//			}
//		}
//		return 1;
//	}
//struct s
//{
//	int a,b,c;
//}aaa[100];
int main()
{
//	notepoint::getnote("gev\\¶Å¾éÔ²ÎèÇú.gev");
//	cout<<notepoint::GAME_TICK_MAX;
//	c("gev\\¶Å¾éÔ²ÎèÇú.gev");
//	cout<<" "<<notepoint::GAME_TICK_MAX;
//	cout<<aaa[1].a<<aaa[1].b<<aaa[1].c;
	FILE*r=fopen("midi\\Aaaaaaaaaaaaa.mid","wb");
	FILE*w=fopen("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.txt","r");
	while(!feof(w)){
		unsigned int a;
		fscanf(w,"%X",&a);
		fprintf(r,"%c",a);
	}
	fclose(r);
	fclose(w);
	return 0;
}
