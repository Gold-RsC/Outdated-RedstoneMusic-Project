#include<iostream>
#include"headfile\\MidiFun.h"
using namespace std;
MidiFun mid("midi\\´º½ÚÐòÇú.mid");
Note note[60000];
int size;
int main(){
	size=mid.getNote(note);
	double speed,errDegree=-1;
	for(double v=48;v<=52;v+=0.05){
		double degree=0;
		double sum=0;
		for(int i=0;i<size;i++){
			sum+=note[i].time-note[i].time/v;
		}
		sum/=size;
		for(int i=0;i<size;i++){
			degree+=(note[i].time-int(note[i].time/v)*v)*(note[i].time-int(note[i].time/v)*v);
		}
		degree/=size;
		if(degree<errDegree||errDegree<0){
			speed=v;
			errDegree=degree;
		}
	}
	printf("%.15f %.15f",speed,errDegree);
	return 0;
}
