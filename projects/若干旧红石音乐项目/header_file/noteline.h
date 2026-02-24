#ifndef NOTELINE_H
#define NOTELINE_H
#include"notepoint.h"
namespace notepoint{
	void drawline(Point lp[],Point np[],int lpn,int npn,void (*writecmd)(Point,Point)){
	//for一个ample，方便梳理思路 
	//**********************************
	//两种情况，左和右 
	// npn||lpn k					|  npn||lpn k
	//	5   10  2					|	5   13  3(2)
	//	1    2    3    4    5		|	1      2      3      4     5
	//	1 2  3 4  5 6  7 8  9 10	|	1 2 3  4 5 6  7 8 9  10 11 12 13
	//**********************************
		if(lpn*1.0/npn>1){//lpn为npn的k倍(k>1) 
			int k=lpn/npn;
			if(k*npn==lpn)
				for(int i=1;i<=npn;i++)
					for(int j=k*(i-1)+1;j<=k*i;j++)
						writecmd(lp[j],np[i]);
			else{
				for(int i=1;i<=lpn-k*npn;i++)
					for(int j=(k+1)*(i-1)+1;j<=(k+1)*i;j++)
						writecmd(lp[j],np[i]);
				for(int i=lpn-k*npn+1;i<=npn;i++)
					for(int j=k*(i-1-npn)+lpn+1;j<=k*(i-npn)+lpn;j++)
						writecmd(lp[j],np[i]);
			}
		}
		else if(npn*1.0/lpn>1){//npn为lpn的k倍(k>1)
			int k=npn/lpn;
			if(k*lpn==npn)
				for(int i=1;i<=lpn;i++)
					for(int j=k*i-k+1;j<=k*i;j++)
						writecmd(lp[i],np[j]);
			else{
				for(int i=1;i<=npn-k*lpn;i++)
					for(int j=(k+1)*(i-1)+1;j<=(k+1)*i;j++)
						writecmd(lp[i],np[j]);
				for(int i=npn-k*lpn+1;i<=lpn;i++)
					for(int j=k*(i-1-lpn)+npn+1;j<=k*(i-lpn)+npn;j++)
						writecmd(lp[i],np[j]);
			}
		}
		else//lpn等于npn 
			for(int i=1;i<=npn;i++)
				writecmd(lp[i],np[i]);
	} 
	void makeline(Point pinit,void (*writecmd)(Point,Point)){
	//	初始化:
	//	1.lpn,npn为1
	//	2.将lp第一项赋值为初始点，提前获取npn第一项 
		Point lp[128],np[128];
		memset(lp,0,sizeof(struct Point)*128);
		memset(np,0,sizeof(struct Point)*128);
		int lpn=1,npn=1;
		lp[1]=pinit;
		int beginidx=0;
		while(!note[++beginidx].velocity){
			np[1]=note[beginidx];
			break;
		}
		for(int i=beginidx;i<=noteidx;i++){
			if(!note[i].velocity)continue;
//			只要 np的channel与此时note的channel不同 
//			或者 np的tick大于note的tick 
//			则说明 这个channel或此音轨的线已经造完 
//  	    就 造线 并 重置lp和np
//			重置方法:
//			1.lp,np清空 
//			2.lp的第一项为初始点
//			3.np的第一项为note[i]
//			4.lpn,npn设为1
			if(
				np[1].channel!=note[i].channel||
				np[1].tick>note[i].tick
			){
				drawline(lp,np,lpn,npn,writecmd);
				memset(lp,0,sizeof(struct Point)*128);
				memset(np,0,sizeof(struct Point)*128);
				lp[1]=pinit;
				np[1]=note[i];
				lpn=1;
				npn=1;
			}
	//		channel相同的前提下
	//		则说明 此channel未结束 
	//		如果 tk不同
	//		则 造线 并 重置lp,np 
	//		重置方法:
	//		1.lp清空 
	//		2.np移植给lp
	//		3.np清空
	//		4.np的第一项被note[i]赋值
	//		5.lpn为npn，之后npn设为1
			else if(
				np[1].channel==note[i].channel&&
				np[1].tick!=note[i].tick
			){
				drawline(lp,np,lpn,npn,writecmd);
				memset(lp,0,sizeof(struct Point)*128);
				for(int j=1;j<=npn;j++)
					lp[j]=np[j];
				memset(np,0,sizeof(struct Point)*128);
				np[1]=note[i];
				lpn=npn;
				npn=1;
			}
	//		channel相同的前提下 
	//		如果tk相同 
	//		即 else（其他情况） 
	//		则 在np中添加note[i]
			else
				np[++npn]=note[i];
		}
	//	处理剩下的点 
		drawline(lp,np,lpn,npn,writecmd);
	}
};
#endif
