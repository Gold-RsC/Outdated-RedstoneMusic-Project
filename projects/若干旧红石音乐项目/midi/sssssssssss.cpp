#include<bits/stdc++.h>
using namespace std;
int main()
{
	FILE*r=fopen("¶Å¾éÔ²ÎèÇú.mid","rb");
	FILE*w=fopen("aaaaa.txt","w");
	unsigned char s;
	while(fscanf(r,"%c",&s)!=EOF)
	{
		fprintf(w,"%X ",s);
	}
	fclose(r);
	fclose(w);
	return 0;
}
