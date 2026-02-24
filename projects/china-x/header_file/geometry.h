#ifndef GEOMETRY_H
#define GEOMETRY_H
#include<stdio.h>
#include<fstream>
#include<math.h>
#include<windows.h>
namespace geometry {
	class Double {
		private:
			double value;
			static double __Degree_Of_Accuracy;
			template<typename T1,typename T2>
			static int __cmp(const T1&a,const T2&b) {
				if(a-b>__Degree_Of_Accuracy)
					return 1;
				if(b-a>__Degree_Of_Accuracy)
					return -1;
				return 0;
			}
		public:
			Double(void) {
				value=0;
			}
			template<typename T>
			Double(const T&a) {
				value=a;
			}
			Double(const Double&)=default;
			template<typename T>
			Double&operator()(const T&a) {
				value=a;
				return *this;
			}
			Double&operator()(const Double&another) {
				value=another.value;
				return *this;
			}
			Double&operator=(const Double&)=default;
		public:
			template<typename T>
			static void set_doa(const T&a) {
				__Degree_Of_Accuracy=a;
			}
			friend bool operator==(const Double&a,const Double&b) {
				return !__cmp(a.value,b.value);
			}
			template<typename T>
			friend bool operator==(const T&a,const Double&b) {
				return !__cmp(a,b.value);
			}
			template<typename T>
			friend bool operator==(const Double&a,const T&b) {
				return !__cmp(a.value,b);
			}
			friend bool operator!=(const Double&a,const Double&b) {
				return __cmp(a.value,b.value);
			}
			template<typename T>
			friend bool operator!=(const T&a,const Double&b) {
				return __cmp(a,b.value);
			}
			template<typename T>
			friend bool operator!=(const Double&a,const T&b) {
				return __cmp(a.value,b);
			}
			friend bool operator<(const Double&a,const Double&b) {
				return __cmp(a.value,b.value)==-1;
			}
			template<typename T>
			friend bool operator<(const T&a,const Double&b) {
				return __cmp(a,b.value)==-1;
			}
			template<typename T>
			friend bool operator<(const Double&a,const T&b) {
				return __cmp(a.value,b)==-1;
			}
			friend bool operator>(const Double&a,const Double&b) {
				return __cmp(a.value,b.value)==1;
			}
			template<typename T>
			friend bool operator>(const T&a,const Double&b) {
				return __cmp(a,b.value)==1;
			}
			template<typename T>
			friend bool operator>(const Double&a,const T&b) {
				return __cmp(a.value,b)==1;
			}
			friend bool operator<=(const Double&a,const Double&b) {
				return __cmp(a.value,b.value)!=1;
			}
			template<typename T>
			friend bool operator<=(const T&a,const Double&b) {
				return __cmp(a,b.value)!=1;
			}
			template<typename T>
			friend bool operator<=(const Double&a,const T&b) {
				return !__cmp(a.value,b)!=1;
			}
			friend bool operator>=(const Double&a,const Double&b) {
				return __cmp(a.value,b.value)!=-1;
			}
			template<typename T>
			friend bool operator>=(const T&a,const Double&b) {
				return __cmp(a,b.value)!=-1;
			}
			template<typename T>
			friend bool operator>=(const Double&a,const T&b) {
				return __cmp(a.value,b)!=-1;
			}
			friend Double operator+(const Double&a,const Double&b) {
				return a.value+b.value;
			}
			template<typename T>
			friend Double operator+(const T&a,const Double&b) {
				return a+b.value;
			}
			template<typename T>
			friend Double operator+(const Double&a,const T&b) {
				return a.value+b;
			}
			friend Double operator-(const Double&a,const Double&b) {
				return a.value-b.value;
			}
			template<typename T>
			friend Double operator-(const T&a,const Double&b) {
				return a-b.value;
			}
			template<typename T>
			friend Double operator-(const Double&a,const T&b) {
				return a.value-b;
			}
			friend Double operator*(const Double&a,const Double&b) {
				return a.value*b.value;
			}
			template<typename T>
			friend Double operator*(const T&a,const Double&b) {
				return a*b.value;
			}
			template<typename T>
			friend Double operator*(const Double&a,const T&b) {
				return a.value*b;
			}
			friend Double operator/(const Double&a,const Double&b) {
				return a.value/b.value;
			}
			template<typename T>
			friend Double operator/(const T&a,const Double&b) {
				return a/b.value;
			}
			template<typename T>
			friend Double operator/(const Double&a,const T&b) {
				return a.value/b;
			}
			Double operator+=(const Double&b) {
				return value+=b.value;
			}
			Double operator-=(const Double&b) {
				return value-=b.value;
			}
			Double operator*=(const Double&b) {
				return value*=b.value;
			}
			Double operator/=(const Double&b) {
				return value/=b.value;
			}
			template<typename T>
			Double operator+=(const T&b) {
				return value+=b;
			}
			template<typename T>
			Double operator-=(const T&b) {
				return value-=b;
			}
			template<typename T>
			Double operator*=(const T&b) {
				return value*=b;
			}
			template<typename T>
			Double operator/=(const T&b) {
				return value/=b;
			}
			friend Double operator-(const Double&a) {
				return -a.value;
			}
			friend bool operator!(const Double&a) {
				return !(a.value);
			}
			operator double(void) {
				return value;
			}
			operator float(void) {
				return value;
			}
			operator int(void) {
				return value;
			}
			friend std::istream&operator>>(std::istream&in,Double&a) {
				return in>>a.value;
			}
			friend std::ostream&operator<<(std::ostream&out,const Double&a) {
				return out<<(!__cmp(a.value,0)?0:a.value);
			}
			friend Double abs(const Double&a) {
				return (a<0?-a:a);
			}
			friend Double sin(const Double&x) {
				return sin(x.value);
			}
			friend Double cos(const Double&x) {
				return cos(x.value);
			}
			friend Double tan(const Double&x) {
				return tan(x.value);
			}
			friend Double asin(const Double&x) {
				return asin(x.value);
			}
			friend Double acos(const Double&x) {
				return acos(x.value);
			}
			friend Double atan(const Double&x) {
				return atan(x.value);
			}
			friend Double atan2(const Double&y,const Double&x) {
				return atan2(y.value,x.value);
			}
			friend Double sinh(const Double&x) {
				return sinh(x.value);
			}
			friend Double cosh(const Double&x) {
				return cosh(x.value);
			}
			friend Double tanh(const Double&x) {
				return tanh(x.value);
			}
			friend Double pow(const Double&x,const Double&y) {
				return pow(x.value,y.value);
			}
			friend Double sqrt(const Double&x) {
				return sqrt(x.value);
			}
			friend Double floor(const Double&x,int degree=0) {
				Double y=x;
				for(int i=0; i<degree; i++) {
					y*=10;
				}
				y.value=floor(y.value);
				for(int i=0; i<degree; i++) {
					y/=10;
				}
				return y;
			}
			friend Double ceil(const Double&x,int degree=0) {
				Double y=x;
				for(int i=0; i<degree; i++) {
					y*=10;
				}
				y.value=ceil(y.value);
				for(int i=0; i<degree; i++) {
					y/=10;
				}
				return y;
			}
			friend Double round(const Double&x,int degree=0) {
				Double y=x;
				for(int i=0; i<degree; i++) {
					y*=10;
				}
				y.value=round(y.value);
				for(int i=0; i<degree; i++) {
					y/=10;
				}
				return y;
			}
	};
	double Double::__Degree_Of_Accuracy;
	/********************************
	*	value	meaning
	*	0		null
	*	1		countless
	*	default	unknown
	********************************/
	namespace geoerr {
		struct __POINT {
			int f;
		};
		__POINT point(int f) {
			return {f};
		}
		struct __LINE {
			int f;
		};
		__LINE line(int f) {
			return {f};
		}
		struct __ANGLE {
			int f;
		};
		__ANGLE angle(int f) {
			return {f};
		}
		class __GEOERR {
			private:
				std::ofstream out;
				int _mode;
			public:
				__GEOERR(void) {
					_mode=0;
					out.open("CON");
				}
				__GEOERR(const __GEOERR&)=delete;
				__GEOERR(const char*filename) {
					_mode=0;
					out.open(filename);
				}
				__GEOERR(int mode) {
					_mode=mode;
					out.open("CON");
				}
				__GEOERR&operator()(const __GEOERR&)=delete;
				__GEOERR&operator()(const char*filename) {
					_mode=0;
					out.close();
					out.open(filename);
					return*this;
				}
				__GEOERR&operator()(int mode) {
					_mode=mode;
					return *this;
				}
				__GEOERR&operator<<(const __POINT&a) {
					switch(_mode) {
						case 0: {
							out<<"Error about the point:";
							switch(a.f) {
								case 0: {
									out<<"There's no point here!\n";
									break;
								}
								case 1: {
									out<<"There're countless points here!\n";
									break;
								}
								default: {
									out<<"Unknown number:"<<a.f<<"\n";
									break;
								}
							}
							break;
						}
						case 1: {
							char str[100];
							switch(a.f) {
								case 0: {
									sprintf(str,"There's no point here!");
									break;
								}
								case 1: {
									sprintf(str,"There're countless points here!");
									break;
								}
								default: {
									sprintf(str,"Unknown number:%d",a.f);
									break;
								}
							}
							MessageBox(NULL,str,"Error about the point",MB_OK|MB_ICONINFORMATION);
							break;
						}
						default: {
							break;
						}
					}
					return *this;
				}
				__GEOERR&operator<<(const __LINE&a) {
					switch(_mode) {
						case 0: {
							out<<"Error about the line:";
							switch(a.f) {
								case 0: {
									out<<"There's no line here!\n";
									break;
								}
								case 1: {
									out<<"There're countless lines here!\n";
									break;
								}
								default: {
									out<<"Unknown number:"<<a.f<<"\n";
									break;
								}
							}
							break;
						}
						case 1: {
							char str[100];
							switch(a.f) {
								case 0: {
									sprintf(str,"There's no line here!");
									break;
								}
								case 1: {
									sprintf(str,"There're countless lines here!");
									break;
								}
								default: {
									sprintf(str,"Unknown number:%d",a.f);
									break;
								}
							}
							MessageBox(NULL,str,"Error about the line",MB_OK|MB_ICONINFORMATION);
							break;
						}
						default: {
							break;
						}
					}
					return *this;
				}
				__GEOERR&operator<<(const __ANGLE&a) {
					switch(_mode) {
						case 0: {
							out<<"Error about the angle:";
							switch(a.f) {
								case 0: {
									out<<"It can't know which vertex is the angle's vertex!\n";
									break;
								}
								case 1: {
									out<<"There's no angle here!\n";
									break;
								}
								default: {
									out<<"Unknown number:"<<a.f<<"\n";
									break;
								}
							}
							break;
						}
						case 1: {
							char str[100];
							switch(a.f) {
								case 0: {
									sprintf(str,"It can't know which vertex is the angle's vertex!");
									break;
								}
								case 1: {
									sprintf(str,"There's no angle here!");
									break;
								}
								default: {
									sprintf(str,"Unknown number:%d",a.f);
									break;
								}
							}
							MessageBox(NULL,str,"Error about the angle",MB_OK|MB_ICONINFORMATION);
							break;
						}
						default: {
							break;
						}
					}
					return *this;
				}
		};
	}
	geoerr::__GEOERR GEOERR;
#define PI 3.1415926535897931
#define ROUND_ANGLE 360.0
#define ANGLE_PER_RADIAN 57.29577951308232
#define RADIAN_PER_ANGLE 0.01745329251994
	struct Point {
		Double x,y;
		bool isnone=0;
		Point(void)=default;
		Point(const Point&)=default;
		template<typename T1,typename T2>
		Point(T1 _x,T2 _y) {
			x=_x,y=_y;
		}
		Point&operator()(Point another) {
			*this=another;
			return *this;
		}
		template<typename T1,typename T2>
		Point&operator()(T1 _x,T2 _y) {
			x=_x,y=_y;
			return *this;
		}
		Point&operator=(const Point&)=default;
		friend bool operator==(Point p1,Point p2) {
			return (p1.x==p2.x)&&(p1.y==p2.y);
		}
		friend bool operator!=(Point p1,Point p2) {
			return (p1.x!=p2.x)||(p1.y!=p2.y);
		}
		friend bool operator<(Point p1,Point p2) {
			return (p1.y<p2.y)||((p1.y==p2.y)&&(p1.x<p2.x));
		}
		friend bool operator>(Point p1,Point p2) {
			return (p1.y>p2.y)||((p1.y==p2.y)&&(p1.x>p2.x));
		}
		friend bool operator<=(Point p1,Point p2) {
			return (p1<p2)||(p1==p2);
		}
		friend bool operator>=(Point p1,Point p2) {
			return (p1==p2)||(p1==p2);
		}
		Double distance(Point p2) {
			return sqrt((x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y));
		}
		friend distance(Point p1,Point p2) {
			return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
		}
		Double radian(Point p1,Point p2,int mode=0) {
			Double alpha=atan2(p1.y-y,p1.x-x),beta=atan2(p2.y-y,p2.x-x);
			if(mode==0)
				return abs(alpha-beta)>PI?2*PI-abs(alpha-beta):abs(alpha-beta);
			else if(mode>0)
				return alpha-beta;
			else if(mode<0)
				return beta-alpha;
		}
		friend radian(Point p0,Point p1,Point p2,int mode=0) {
			Double alpha=atan2(p1.y-p0.y,p1.x-p0.x),beta=atan2(p2.y-p0.y,p2.x-p0.x);
			if(mode==0)
				return abs(alpha-beta)>PI?2*PI-abs(alpha-beta):abs(alpha-beta);
			else if(mode>0)
				return alpha-beta;
			else if(mode<0)
				return beta-alpha;
		}
		friend std::ostream&operator<<(std::ostream&out,Point p) {
			return out<<"("<<p.x<<","<<p.y<<")";;
		}
	};
	struct Line {
		//ax+by+c=0
		Double a,b,c;
		bool isnone=0;
		Line(void)=default;
		Line(const Line&)=default;
		Line(Point p1,Point p2) {
			if(p1==p2) {
				GEOERR<<geoerr::line(1);
				isnone=1;
				return;
			}
			a=p1.y-p2.y,b=p2.x-p1.x,c=(p2.y-p1.y)*p1.x+(p1.x-p2.x)*p1.y;
		}
		Line&operator()(Line another) {
			*this=another;
			return *this;
		}
		Line&operator()(Point p1,Point p2) {
			if(p1==p2) {
				GEOERR<<geoerr::line(1);
				isnone=1;
				return *this;
			}
			a=p1.y-p2.y,b=p2.x-p1.x,c=(p2.y-p1.y)*p1.x+(p1.x-p2.x)*p1.y;
			return *this;
		}
		Line&operator=(const Line&)=default;
		Line vertical_line(Point p) {
			Line l;
			l.a=b,l.b=-a,l.c=-l.a*p.x-l.b*p.y;
			return l;
		}
		Line parallel_line(Point p) {
			Line l;
			l.a=a,l.b=b,l.c=-a*p.x-b*p.y;
			return l;
		}
		Point intersection_point(Line l2) {
			Point ans;
			//a1/b1==a2/b2
			if(a*l2.b==l2.a*b) {
				ans.x=0,ans.y=0;
				GEOERR<<geoerr::point(0);
				ans.isnone=1;
			} else
				ans.x=(l2.c*b-c*l2.b)/(a*l2.b-l2.a*b),ans.y=(l2.c*a-c*l2.a)/(b*l2.a-l2.b*a);
			return ans;
		}
		friend Point intersection_point(Line l1,Line l2) {
			Point ans;
			//a1/b1==a2/b2
			if(l1.a*l2.b==l2.a*l1.b) {
				ans.x=0,ans.y=0;
				GEOERR<<geoerr::point(0);
				ans.isnone=1;
			} else
				ans.x=(l2.c*l1.b-l1.c*l2.b)/(l1.a*l2.b-l2.a*l1.b),ans.y=(l2.c*l1.a-l1.c*l2.a)/(l1.b*l2.a-l2.b*l1.a);
			return ans;
		}
		Double distance(Line l2) {
			if(a*l2.b!=l2.a*b)
				return 0;
			else
				return abs(c*l2.b-l2.c*b)/sqrt((a*l2.b+l2.a*b)*(a*l2.b+l2.a*b)/4+(b*l2.b)*(b*l2.b));
		}
		friend Double distance(Line l1,Line l2) {
			if(l1.a*l2.b!=l2.a*l1.b)
				return 0;
			else
				return abs(l1.c*l2.b-l2.c*l1.b)/sqrt((l1.a*l2.b+l2.a*l1.b)*(l1.a*l2.b+l2.a*l1.b)/4+(l1.b*l2.b)*(l1.b*l2.b));
		}
		Double distance(Point point) {
			return abs(a*point.x+b*point.y+c)/sqrt(a*a+b*b);
		}
		friend distance(Line l,Point p) {
			return abs(l.a*p.x+l.b*p.y+l.c)/sqrt(l.a*l.a+l.b*l.b);
		}
		friend distance(Point p,Line l) {
			return abs(l.a*p.x+l.b*p.y+l.c)/sqrt(l.a*l.a+l.b*l.b);
		}
		bool has_point(Point p) {
			return !(p.x*a+p.y*b+c);
		}
		friend std::ostream&operator<<(std::ostream&out,Line line) {
			return out<<line.a<<((line.b<0)?"x":"x+")<<line.b<<((line.c<0)?"y":"y+")<<line.c<<"=0 {type=line}";
		}
	};
	struct Ray {
		//ax+by+c=0
		Double a,b,c;
		Point p0;
		bool direction;
		bool isnone=0;
		Ray(void)=default;
		Ray(const Ray&)=default;
		Ray(Point p1,Point p2) {
			if(p1==p2) {
				GEOERR<<geoerr::line(1);
				isnone=1;
				return;
			}
			a=p1.y-p2.y,b=p2.x-p1.x,c=(p2.y-p1.y)*p1.x+(p1.x-p2.x)*p1.y,p0=p1,direction=p1<p2;
		}
		Ray&operator()(Ray another) {
			*this=another;
			return *this;
		}
		Ray&operator()(Point p1,Point p2) {
			if(p1==p2) {
				GEOERR<<geoerr::line(1);
				isnone=1;
				return *this;
			}
			a=p1.y-p2.y,b=p2.x-p1.x,c=(p2.y-p1.y)*p1.x+(p1.x-p2.x)*p1.y,p0=p1,direction=p1<p2;
			return *this;
		}
		Ray&operator=(const Ray&)=default;
		operator Line(void) {
			Line l;
			l.a=a,l.b=b,l.c=c;
			return l;
		}
		Point intersection_point(Line l2) {
			//a1/b1==a2/b2
			if(a*l2.b==l2.a*b) {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
			Point ans((l2.c*b-c*l2.b)/(a*l2.b-l2.a*b),(l2.c*a-c*l2.a)/(b*l2.a-l2.b*a));
			if(has_point(ans))
				return ans;
			else {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
		}
		Point intersection_point(Ray l2) {
			if(a*l2.b==l2.a*b) {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
			Point ans((l2.c*b-c*l2.b)/(a*l2.b-l2.a*b),(l2.c*a-c*l2.a)/(b*l2.a-l2.b*a));
			if(has_point(ans)&&l2.has_point(ans))
				return ans;
			else {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
		}
		friend Point intersection_point(Ray l1,Line l2) {
			return l1.intersection_point(l2);
		}
		friend Point intersection_point(Line l1,Ray l2) {
			return l2.intersection_point(l1);
		}
		friend Point intersection_point(Ray l1,Ray l2) {
			if(l1.a*l2.b==l2.a*l1.b) {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
			Point ans((l2.c*l1.b-l1.c*l2.b)/(l1.a*l2.b-l2.a*l1.b),(l2.c*l1.a-l1.c*l2.a)/(l1.b*l2.a-l2.b*l1.a));
			if(l1.has_point(ans)&&l2.has_point(ans))
				return ans;
			else {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
		}
		Line vertical_line(Point p) {
			Line l;
			l.a=b,l.b=-a,l.c=-l.a*p.x-l.b*p.y;
			return l;
		}
		Line parallel_line(Point p) {
			Line l;
			l.a=a,l.b=b,l.c=-a*p.x-b*p.y;
			return l;
		}
		Double radian(Ray another,int mode=0) {
			if(p0!=another.p0) {
				GEOERR<<geoerr::angle(0);
				return 0;
			}
			Double alpha=(direction?atan2(-a,b):atan2(a,-b)+PI),beta=(another.direction?atan2(-another.a,another.b):atan2(another.a,-another.b)+PI);
			if(mode==0)
				return abs(alpha-beta)>PI?2*PI-abs(alpha-beta):abs(alpha-beta);
			else if(mode>0)
				return alpha-beta;
			else if(mode<0)
				return beta-alpha;
		}
		friend Double radian(Ray r1,Ray r2,int mode=0) {
			if(r1.p0!=r2.p0) {
				GEOERR<<geoerr::angle(0);
				return 0;
			}
			Double alpha=(r1.direction?atan2(-r1.a,r1.b):atan2(r1.a,-r1.b)+PI),beta=(r2.direction?atan2(-r2.a,r2.b):atan2(r2.a,-r2.b)+PI);
			if(mode==0)
				return abs(alpha-beta)>PI?2*PI-abs(alpha-beta):abs(alpha-beta);
			else if(mode>0)
				return alpha-beta;
			else if(mode<0)
				return beta-alpha;
		}
		Line angular_bisector(Ray another) {
			Line ret;
			if(p0!=another.p0) {
				GEOERR<<geoerr::line(0);
				ret.a=0,ret.b=0,ret.c=0,ret.isnone=1;
				return ret;
			}
			Double alpha=(direction?atan2(-a,b):atan2(a,-b)+PI),beta=(another.direction?atan2(-another.a,another.b):atan2(another.a,-another.b)+PI);
			Double theta=(alpha+beta)/2;
			if(theta==PI/2||theta==PI/2*3) {
				ret(p0,Point(p0.x,p0.y+1));
			} else if(theta==0||theta==PI) {
				ret(p0,Point(p0.x+1,p0.y));
			} else {
				ret.a=-tan(theta),ret.b=1,ret.c=-ret.a*p0.x-ret.b*p0.y;
			}
			return ret;
		}
		friend Line angular_bisector(Ray r1,Ray r2) {
			Ray ret;
			if(r1.p0!=r2.p0) {
				GEOERR<<geoerr::line(0);
				ret.isnone=1;
				return ret;
			}
			Double alpha=(r1.direction?atan2(-r1.a,r1.b):atan2(r1.a,-r1.b)+PI),beta=(r2.direction?atan2(-r2.a,r2.b):atan2(r2.a,-r2.b)+PI);
			Double theta=(alpha+beta)/2;
			if(theta==PI/2||theta==PI/2*3) {
				ret(r1.p0,Point(r1.p0.x,r1.p0.y+1));
			} else if(theta==0||theta==PI) {
				ret(r1.p0,Point(r1.p0.x+1,r1.p0.y));
			} else {
				ret.a=-tan(theta),ret.b=1,ret.c=-ret.a*r1.p0.x-ret.b*r1.p0.y;
			}
			return ret;
		}
		bool has_point(Point p) {
			return Line(*this).has_point(p)&&(direction^(p0>p));
		}
		friend std::ostream&operator<<(std::ostream&out,Ray line) {
			return out<<line.a<<((line.b<0)?"x":"x+")<<line.b<<((line.c<0)?"y":"y+")<<line.c<<"=0 {type=ray,endpoint="<<line.p0<<",direction="<<line.direction<<"}";
		}
	};
	struct Segment {
		Point p1,p2;
		bool isnone=0;
		Segment(void)=default;
		Segment(const Segment&)=default;
		Segment(Point P1,Point P2) {
			if(P1==P2) {
				GEOERR<<geoerr::line(1);
				isnone=1;
				return;
			}
			p1=P1,p2=P2;
		}
		Segment&operator()(Segment another) {
			*this=another;
			return *this;
		}
		Segment&operator()(Point P1,Point P2) {
			if(P1==P2) {
				GEOERR<<geoerr::line(1);
				isnone=1;
				return *this;
			}
			p1=P1,p2=P2;
			return *this;
		}
		Segment&operator=(const Segment&)=default;
		operator Line(void) {
			return Line(p1,p2);
		}
		Point intersection_point(Line l2) {
			//a1/b1==a2/b2
			Line l1(*this);
			if(l1.a*l2.b==l2.a*l1.b) {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
			Point ans((l2.c*l1.b-l1.c*l2.b)/(l1.a*l2.b-l2.a*l1.b),(l2.c*l1.a-l1.c*l2.a)/(l1.b*l2.a-l2.b*l1.a));
			if(has_point(ans))
				return ans;
			else {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
		}
		Point intersection_point(Ray l2) {
			Line l1(*this);
			if(l1.a*l2.b==l2.a*l1.b) {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
			Point ans((l2.c*l1.b-l1.c*l2.b)/(l1.a*l2.b-l2.a*l1.b),(l2.c*l1.a-l1.c*l2.a)/(l1.b*l2.a-l2.b*l1.a));
			if(has_point(ans)&&l2.has_point(ans))
				return ans;
			else {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
		}
		Point intersection_point(Segment l) {
			Line l1(*this);
			Line l2(l);
			if(l1.a*l2.b==l2.a*l1.b) {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
			Point ans((l2.c*l1.b-l1.c*l2.b)/(l1.a*l2.b-l2.a*l1.b),(l2.c*l1.a-l1.c*l2.a)/(l1.b*l2.a-l2.b*l1.a));
			if(has_point(ans)&&l2.has_point(ans))
				return ans;
			else {
				GEOERR<<geoerr::point(0);
				Point p;
				p.isnone=1;
				return p;
			}
		}
		friend Point intersection_point(Segment l1,Line l2) {
			return l1.intersection_point(l2);
		}
		friend Point intersection_point(Line l1,Segment l2) {
			return l2.intersection_point(l1);
		}
		friend Point intersection_point(Segment l1,Ray l2) {
			return l1.intersection_point(l2);
		}
		friend Point intersection_point(Ray l1,Segment l2) {
			return l2.intersection_point(l1);
		}
		friend Point intersection_point(Segment l1,Segment l2) {
			return l1.intersection_point(l2);
		}
		Line vertical_line(Point p) {
			Line l,self(*this);
			l.a=self.b,l.b=-self.a,l.c=-l.a*p.x-l.b*p.y;
			return l;
		}
		Line parallel_line(Point p) {
			Line l,self(*this);
			l.a=self.a,l.b=self.b,l.c=-self.a*p.x-self.b*p.y;
			return l;
		}
		Point mid_point(void) {
			return Point((p1.x+p2.x)/2,(p1.y+p2.y)/2);
		}
		Line vertical_bisector_line(void) {
			return vertical_line(mid_point());
		}
		bool has_point(Point p) {
			return Ray(p1,p2).has_point(p)&&Ray(p2,p1).has_point(p);
		}
		friend std::ostream&operator<<(std::ostream&out,Segment line) {
			out<<Line(line).a<<((Line(line).b<0)?"x":"x+")<<Line(line).b<<((Line(line).c<0)?"y":"y+")<<Line(line).c<<"=0 {type=segment,endpoint[2]={"<<line.p1<<","<<line.p2<<"}}";
		}
	};
	struct Angle {
		Ray r1,r2;
		int mode;
		bool isnone=0;
		Angle(void)=default;
		Angle(const Angle&)=default;
		Angle(Point p0,Point p1,Point p2,int mod=0) {
			r1(p0,p1),r2(p0,p2),mode=mod;
		}
		Angle(Ray R1,Ray R2,int mod=0) {
			if(r1.p0!=r2.p0) {
				GEOERR<<geoerr::angle(0);
				isnone=1;
				return;
			}
			r1=R1,r2=R2,mode=mod;
		}
		Angle&operator()(const Angle&another) {
			*this=another;
			return *this;
		}
		Angle&operator()(Point p0,Point p1,Point p2,int mod=0) {
			r1(p0,p1),r2(p0,p2),mode=mod;
			return *this;
		}
		Angle&operator()(Ray R1,Ray R2,int mod=0) {
			if(r1.p0!=r2.p0) {
				GEOERR<<geoerr::angle(0);
				isnone=1;
				return *this;
			}
			r1=R1,r2=R2,mode=mod;
			return *this;
		}
		Angle&operator=(const Angle&)=default;
		operator Double(void) {
			return radian(r1,r2,mode);
		}
		Line angular_bisector(void) {
			return r1.angular_bisector(r2);
		}
		Point vertex_point(void) {
			return r1.p0;
		}
		friend std::ostream&operator<<(std::ostream&out,Angle line) {
			out<<line.r1<<","<<line.r2<<" {type=angle,vertex="<<line.r1.p0<<",mode="<<line.mode<<"}";
		}
		friend Double abs(Angle a) {
			Double alpha=radian(a.r1,a.r2,a.mode);
			return alpha<0?2*PI-alpha:alpha;
		}
	};
	struct Arc {
		Angle angle;
		Double len;
		bool isnone=0;
		Arc(void)=default;
		Arc(const Arc&)=default;
		Arc(Angle a,Double l) {
			angle(a),len(l);
		}
		Arc(Ray R1,Ray R2,int mode,Double l) {
			angle(R1,R2,mode),len(l);
		}
		Arc&operator()(const Arc&another) {
			*this=another;
			return *this;
		}
		Arc&operator()(Angle a,Double l) {
			angle(a),len(l);
			return *this;
		}
		Arc&operator()(Ray R1,Ray R2,int mode,Double l) {
			angle(R1,R2,mode),len(l);
			return *this;
		}
		Arc&operator=(const Arc&)=default;
		operator Angle(void) {
			return angle;
		}
		operator Double(void) {
			return len*abs(angle);
		}
		Double radian(void) {
			return angle;
		}
		Point vertex_point(void) {
			return angle.vertex_point();
		}
		Line angular_bisector(void) {
			return angle.r1.angular_bisector(angle.r2);
		}
		Double radius(void) {
			return len;
		}
		friend std::ostream&operator<<(std::ostream&out,Arc arc) {
			out<<arc.angle.r1<<","<<arc.angle.r2<<" {type=arc,vertex="<<arc.vertex_point()<<",mode="<<arc.angle.mode<<",len="<<arc.len<<"}";
		}
	};
}

#endif

