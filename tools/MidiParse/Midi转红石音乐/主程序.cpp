#include"headfile\\MidiFun.h"
using namespace std;
int ygg[128][1000];
int ygn[128];
int num;
MidiFun midi;
Note note[50000];
int notenum;
double speed;
int main()
{
	char fin[100];
	char fout[100];
	printf("midi文件（需加后缀）：\n");
	scanf("%[^\n]",fin);
	getchar();
	midi.parse(fin,0);
	notenum=midi.getNote(note);
	printf("导出的文件名（需加后缀）：\n");
	scanf("%[^\n]",fout);
	getchar();
	printf("speed:\n");
	scanf("%lf",&speed);
	FILE*w=fopen(fout,"w");
	for(int i=0;i<notenum;i++){ 
		if(note[i].volume==0)
			continue;
		ygg[note[i].pitch][++ygn[note[i].pitch]]=note[i].time/speed;
		ygg[note[i].pitch][0]=max(ygg[note[i].pitch][ygn[note[i].pitch]],ygg[note[i].pitch][0]);
	}
	for(int i=0;i<128;i++)
	{
		if(ygn[i]==0)continue;
		fprintf(w,"execute @a[scores={tick=0..%d",ygg[i][0]);
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
				fprintf(w,",tick=!%d..%d",l,r);
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
