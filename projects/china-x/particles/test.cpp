#include"particle.h"
using namespace particle;
int main(void) {
	useTemplate("template\\[线]通用.template","azhe.particle.json",
	            "aa",3,
	            Offset(
	                "5*T",
	                "-12*T*(T-1)",
	                "5*T"
	            ),
	            Size(
	                "(T>0.5?0.2*(1-T):0.1)",
	                "(T>0.5?0.2*(1-T):0.1)"
	            ),
	VariousColor({
		{0,"#FF0000"},
		{0.17,"#FF6D00"},
		{0.33,"#FFF500"},
		{0.5,"#40FF00"},
		{0.67,"#00DBFF"},
		{0.83,"#9B00FF"},
		{1,"#FF0000"}
	}));
//	useTemplate("template\\[点]扩散球.template","azhe.particle.json",
//	           "aa",3,
//	           VariousColor({
//	            	{0,"#FF0000"},
//					{0.17,"#FF6D00"},
//					{0.33,"#FFF500"},
//					{0.5,"#40FF00"},
//					{0.67,"#00DBFF"},
//					{0.83,"#9B00FF"},
//					{1,"#FF0000"}
//				}));
	return 0;
}
