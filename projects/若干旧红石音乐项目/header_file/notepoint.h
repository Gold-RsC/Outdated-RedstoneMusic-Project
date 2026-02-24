#ifndef NOTEPOINT_H
#define NOTEPOINT_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

namespace notepoint{
	struct Point{
		int tick,pitch,channel,velocity;
	}note[15000];
	int noteidx=0;
	
	int GAME_TICK_MAX=0;
	struct __BPM_VALUE__{
		int tick,value,gtsum;
	}bpm[1000]={0,1,0};
	int bpmidx=0;
	int bpmcheck(int idx,int tick)
	{
		if(bpm[idx].tick<=tick&&bpm[idx+1].tick>tick||idx==bpmidx)
			return 0;
		return (bpm[idx].tick>tick?1:-1);
	}
	int getnote(const char*filename){
		FILE*r=fopen(filename,"r");
		if(r==NULL)return -2;
		int tpqn=120;
		long long tempo=1;
		while(1){
			int laji;
			char*type=(char*)malloc(50),*strlj=(char*)malloc(50);
			if(fscanf(r,"%s",type)==EOF)
				return -1; 
			if(!stricmp(type,"end"))
				break;
			else if(!stricmp(type,"endtrack"))
				fscanf(r,"%[^\n]\n",strlj);
			else if(!stricmp(type,"ts"))
				fscanf(r,"%[^\n]\n",strlj);
			else if(!stricmp(type,"tempo")){
				fscanf(r,"%d%d%d\n",&bpm[++bpmidx].tick,&laji,&tempo);
				bpm[bpmidx].value=6e7/tempo;
				bpm[bpmidx].gtsum=bpm[bpmidx-1].gtsum+(bpm[bpmidx].tick-bpm[bpmidx-1].tick)*1200/tpqn/bpm[bpmidx-1].value;
//				printf("%d\n",bpm[bpmidx].gtsum);
			}
			else if(!stricmp(type,"other"))
				fscanf(r,"%[^\n]\n",strlj);
			else if(!stricmp(type,"tpqn"))
				fscanf(r,"%d\n",&tpqn);
			else if(!stricmp(type,"note")){
				int tk,td,yg,yl;
				fscanf(r,"%d%d%d%d%d\n",
					&tk,
					&laji,
					&td,
					&yg,
					&yl
				);
//				if(yl==0)continue;
				/*
				bpm:
					tick value
					0 150
					100 120
					200 100
					300 150
				note:
					tick
					0		
						0<=0<100==>150
						value=0*1200/tpqn/150
					20
						0<=20<100==>150
						value=20*1200/tpqn/150
					50
						0<=50<100==>150
						value=50*1200/tpqn/150
					100
						100<=100<200==>120
						value=(100-0)*1200/tpqn/150+(100-100)*1200/tpqn/120
					150
						100<=150<200==>120
						value=(100-0)*1200/tpqn/150+(150-100)*1200/tpqn/120
					250
						200<=250<300==>120
						value=(100-0)*1200/tpqn/150+(200-100)*1200/tpqn/120+(250-200)*1200/tpqn/100
				*/
//				int gt=0;
				
				int bpml=1,bpmr=bpmidx;
				
				while(bpml<=bpmr)
				{
					int mid=(bpml+bpmr)/2;
					int check=bpmcheck(mid,tk);
					if(!check)
					{
						note[++noteidx].tick=bpm[mid].gtsum+(tk-bpm[mid].tick)*1200/tpqn/bpm[mid].value;
						
						break;
					}
					else if(check==1)
					{
						bpmr=mid;
					}
					else if(check==-1)
//					else
					{
						bpml=mid+1;
					}
				}
//				if(!flag)printf("errorrrrr\n");
//				printf("gt: %d\n",gt);
				
				note[noteidx].channel=td;
				note[noteidx].pitch=yg;
				note[noteidx].velocity=yl;
				GAME_TICK_MAX=(note[noteidx].tick>GAME_TICK_MAX?note[noteidx].tick:GAME_TICK_MAX);
			}
			else{
				printf("error:%s\n",type);
				return 0;
			}
		}
		return 1;
	}
};
#endif
