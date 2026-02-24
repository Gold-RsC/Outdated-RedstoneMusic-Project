#include<stdio.h>
#include<windows.h>
#include<unistd.h>
class B{
	public:
		int b;
		B(void){
			b=0;
			printf("B");
		} 
		virtual ~B(void){
			printf("b");
		}
		virtual int m(){
			printf("lllllll");
			return 0;
		}
};
class C:public B{
	public:
		C(void){
			b=1;
			printf("C");
		} 
		~C(void){
			printf("c");
		}
		int m(){
			return 0;
		}
};
int main(){
    FILE*r=fopen("../8.mid","rb");
    FILE*w=fopen("w.txt","w");
    while(!feof(r)){
        unsigned char c;
        fscanf(r,"%c",&c);
        fprintf(w,"%2.2X ",c);
    }
	return 0;
}
