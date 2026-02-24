//#include"header_file\\soma3.h"
//#include"header_file\\particle.h"
#include<iostream>
#include<algorithm>
#include<math.h>
using namespace std;
//using namespace geometry;
//using namespace soma3;
//#include"header_file\\MidiFun.h"
//#include<stdio.h>
struct A{
	long long tick;
	string function;
}a[200000];
int main(void) {
//	FILE*w=fopen("functions\\circle.mcfunction","w");
//	for(int i=0;i<360;i++){
//		fprintf(w,"execute @a ~~~ particle circle%d ~~~\n",i);
//	}
//	fclose(w);
//	struct A{
//		int a,b,c;
//	}a[]={{0,0,0},{0,0,1},{0,0,2},{0,0,3},{0,0,4},{0,0,5},{0,0,6},{0,0,7},{0,0,8},{0,0,9},{0,0,10},{0,1,0},{0,1,10},{0,2,0},{0,2,10},{0,3,0},{0,3,10},{0,4,0},{0,4,10},{0,5,0},{0,5,10},{0,6,0},{0,6,10},{0,7,0},{0,7,10},{0,8,0},{0,8,10},{0,9,0},{0,9,10},{0,10,0},{0,10,1},{0,10,2},{0,10,3},{0,10,4},{0,10,5},{0,10,6},{0,10,7},{0,10,8},{0,10,9},{0,10,10},{1,0,0},{1,0,10},{1,10,0},{1,10,10},{2,0,0},{2,0,10},{2,10,0},{2,10,10},{3,0,0},{3,0,10},{3,10,0},{3,10,10},{4,0,0},{4,0,10},{4,10,0},{4,10,10},{5,0,0},{5,0,10},{5,10,0},{5,10,10},{6,0,0},{6,0,10},{6,10,0},{6,10,10},{7,0,0},{7,0,10},{7,10,0},{7,10,10},{8,0,0},{8,0,10},{8,10,0},{8,10,10},{9,0,0},{9,0,10},{9,10,0},{9,10,10},{10,0,0},{10,0,1},{10,0,2},{10,0,3},{10,0,4},{10,0,5},{10,0,6},{10,0,7},{10,0,8},{10,0,9},{10,0,10},{10,1,0},{10,1,10},{10,2,0},{10,2,10},{10,3,0},{10,3,10},{10,4,0},{10,4,10},{10,5,0},{10,5,10},{10,6,0},{10,6,10},{10,7,0},{10,7,10},{10,8,0},{10,8,10},{10,9,0},{10,9,10},{10,10,0},{10,10,1},{10,10,2},{10,10,3},{10,10,4},{10,10,5},{10,10,6},{10,10,7},{10,10,8},{10,10,9},{10,10,10}};
//	long long a=-10;
//	cout<<a;
//	FILE*w=fopen("functions\\play.mcfunction","r");
//	FILE*f[16];
//	for(int i=0;i<16;i++){
//		char a[1000];
//		sprintf(a,"functions\\play%d.mcfunction",i);
//		f[i]=fopen(a,"w");
//	}
//	char a[1000];
//	int num=0;
//	while(!feof(w)){
//		fscanf(w,"%[^\n]\n",a);
//		num++;
//		fprintf(f[num%16],"%s\n",a);
//	}
//	fclose(w);
//	printf("%d",num);
//	for(int i=0;i<16;i++)
//		fclose(f[i]);
	FILE*r=fopen("functions\\play.mcfunction","r");
	int num=0;
	while(!feof(r)){
		char i[1000];
		fscanf(r,"%lld %[^\n]\n",&a[num].tick,i);
		a[num++].function=i;
	} 
	fclose(r);
	sort(a,a+num,[](A&x,A&y){
		return x.tick<=y.tick;
	});
	FILE*f[16];
	for(int i=0;i<16;i++){
		char a[1000];
		sprintf(a,"functions\\play%d.mcfunction",i);
		f[i]=fopen(a,"w");
		fprintf(f[i],"scoreboard players add @a tick 1\n");
	}
	for(int i=0;i<num;i++){
		fprintf(f[i/(num/16)==16?15:i/(num/16)],(a[i].function+"\n").c_str());
	}
	for(int i=1;i<16;i++){
		fprintf(f[i-1],"execute @a[scores={tick=%lld}] ~~~ setblock 0 %d 1 redstone_block\n",a[i*(num/16)].tick,200-i*2);
		fprintf(f[i-1],"execute @a[scores={tick=%lld}] ~~~ setblock 0 %d 1 air\n",a[i*(num/16)].tick,200-i*2+2);
	}
	for(int i=0;i<16;i++)
		fclose(f[i]);
	return 0;
}
