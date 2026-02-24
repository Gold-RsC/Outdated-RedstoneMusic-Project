#include<bits/stdc++.h>
using namespace std;
int ygg[128][1000];
int ygn[128];
int num;
int main()
{
	char*fin=(char*)malloc(500);
	char*fo=(char*)malloc(500);
	char*fout=(char*)malloc(500);
	int bpm,tpqn;
	printf("调用的文件名（需加后缀）：\n");
	cin>>fin;
	printf("导出的文件名（不加后缀）：\n");
	cin>>fo;
	printf("BPM:\n");
	cin>>bpm;
	printf("一个四分音符的miditick值:\n");
	cin>>tpqn;
	sprintf(fout,"%s.mcfunction",fo);
	FILE*r=fopen(fin,"r");
	FILE*w=fopen(fout,"w");
	if(r==NULL)
	{
		printf("请检查您的文件名是否正确或文件中是否有数据\n");
		return 1;
	}
	int tk,td,yg,yl;
	char ljstr[100];
	while(fscanf(r,"%d %d %s %d %d\n",&tk,&td,ljstr,&yg,&yl)!=EOF)
	{
		if(yl==0)continue;
		ygg[yg][++ygn[yg]]=tk*1200/tpqn/bpm;
		ygg[yg][0]=max(ygg[yg][ygn[yg]],ygg[yg][0]);
	}
	for(int i=0;i<128;i++)
	{
		if(ygn[i]==0)continue;
		fprintf(w,"execute @a[scores={tick=0..%d",ygg[i][0]*10);
		bool a[100000]={0};
		for(int j=2;j<=ygn[i];j++)
		{
			a[ygg[i][j]]=1;
		}
		int l=0,r=0;
		for(int j=0;j<=ygg[i][0];j++)
		{
			if(a[j])
			{
				r=j-1;
				fprintf(w,",tick=!%d..%d",l*10,r*10);
				int idx=j;
				while(a[++idx]);
				l=idx;
				j=idx;
				continue;
			}
		}
		fprintf(w,"}] ~~~ playsound note.harp @s ~~~ 1 %lf 1\n",pow(2,(i-66)*1.0/12));
		num++;
	}
	printf("共生成%d个指令",num);
	return 0;
}
