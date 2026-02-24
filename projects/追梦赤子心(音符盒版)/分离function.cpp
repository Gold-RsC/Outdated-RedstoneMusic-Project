#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;
struct A{
	char cmd[100];
	int tick;
}a[30000];
int main(void){
	[]{
		FILE*r=fopen("functions\\play.mcfunction","r");
		FILE*w[10];
		char filename[100];
		for(int i=0;i<10;i++){
			sprintf(filename,"functions\\play%d.mcfunction",i);
			w[i]=fopen(filename,"w");
		}
		int b=0,last_b=-1;
		int i;
		for(i=0;!feof(r);i++){
			fscanf(r,"%d %[^\n]\n",&a[i].tick,a[i].cmd);
		}
		sort(a,a+i,[](A a,A b){
			return a.tick<b.tick;
		});
		for(int j=0,c=0;j<i;j++){
			b=a[j].tick;
			if(last_b!=b&&(b==1307||b==1881||b==2213||b==2533||b==2933||b==3954||b==4477||b==4821||b==5261)){
				fprintf(w[c],"execute @a[scores={tick=%d}] ~~~ setblock 1 %d 0 redstone_block\n",b-3,101+c);
				fprintf(w[c],"execute @a[scores={tick=%d}] ~~~ setblock 1 %d 0 air\n",b+3,100+c);
				c++;
			}
			fprintf(w[c],"%s\n",a[j].cmd);
			last_b=b;
		}
		fclose(r);
		for(int i=0;i<10;i++){
			fclose(w[i]);
		}
		
	}();
	[]{
		FILE*r=fopen("functions\\set.mcfunction","r");
		FILE*w[4];
		w[0]=fopen("functions\\set0.mcfunction","w");
		w[1]=fopen("functions\\set1.mcfunction","w");
		w[2]=fopen("functions\\set2.mcfunction","w");
		w[3]=fopen("functions\\set3.mcfunction","w");
		char a[1000];
		for(int i=0;!feof(r);i++){
			fscanf(r,"%[^\n]\n",a);
			fprintf(w[i%4],"%s\n",a);
		}
		fclose(r);
		fclose(w[0]);
		fclose(w[1]);
		fclose(w[2]);
		fclose(w[3]);
	}();
	[]{
		FILE*r=fopen("functions\\music.mcfunction","r");
		FILE*w[10];
		char a[1000];
		for(int i=0;i<10;i++){
			sprintf(a,"functions\\music%d.mcfunction",i);
			w[i]=fopen(a,"w");
		}
		int b=0,last_b=-1;
		for(int i=0;!feof(r);){
			fscanf(r,"%d %[^\n]\n",&b,a);
			if(last_b!=b&&(b==1307||b==1881||b==2213||b==2533||b==2933||b==3954||b==4477||b==4821||b==5261)){
				fprintf(w[i],"execute @a[scores={tick=%d}] ~~~ setblock 1 %d 0 redstone_block\n",b-3,5+i);
				fprintf(w[i],"execute @a[scores={tick=%d}] ~~~ setblock 1 %d 0 air\n",b+3,4+i);
				i++;
			}
			fprintf(w[i],"%s\n",a);
			last_b=b;
		}
		fclose(r);
		for(int i=0;i<10;i++){
			fclose(w[i]);
		}
	}();
	return 0;
}

