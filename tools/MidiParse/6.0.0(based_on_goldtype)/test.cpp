#include<stdio.h>
#include<string.h>
int main(){
	FILE*r=fopen("testFileName.txt","r");
	while(!feof(r)){
		char name[1000];
		fscanf(r,"%[^\n]\n",name);
		FILE*mid=fopen(name,"rb");
//		for(int i=0;i<8;++i){
//			fgetc(mid);
//		}
//		fgetc(mid);
//		unsigned char c=fgetc(mid);
//		printf("%2.2X\t",c);
//		if(c==2){
//			printf("\n%s\n\n%d",name,c);
//			break;
//		}
		
		for(int i=0;i<12;++i){
			fgetc(mid);
		}
		unsigned char c=fgetc(mid);
		printf("%2.2X\t",c);
		if((c&0x80)==0x80){
			printf("\n%s\n\n%2.2X 2.2X",name,c,fgetc(mid));
			break;
		}
		fclose(mid);
	}
	fclose(r);
	return 0;
}
