#include<bits/stdc++.h>
using namespace std;
int main()
{
	cout<<"你的文件名（加后缀，且在同一目录下）:\n";
	char*name=(char*)malloc(500);
	cin>>name;
	FILE*r=fopen(name,"rb");
	if(r==NULL)
	{
		cout<<"文件路径错误或文件中无东西";
		return 1;
	}
	unsigned char a;
	for(int i=1;i<=12;i++)fgetc(r);
	int tpqn=0;
	for(int i=1;i<=2;i++)tpqn=tpqn*256+fgetc(r);
	cout<<"四分音符的miditick值是："<<tpqn;
	fclose(r);
	system("pause");
	return 0;
}
