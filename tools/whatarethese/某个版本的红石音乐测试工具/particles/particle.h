#ifndef PARTICLE_H
#define PARTICLE_H
#include<fstream>
#include<sstream>
#include<vector>
namespace particle {
	template<typename...T>
	class String;
	template<>
	class String<> {};
	template<typename Va,typename...T>
	class String<Va,T...>:String<T...> {
			Va _value;
		public:
			String(Va arg,T...args)
				:_value(arg),String<T...>(args...) {}
			Va&value(void) {
				return _value;
			}
			String<T...>&next(void) {
				return *this;
			}
	};
	template<typename Va>
	std::ostream&operator<<(std::ostream&out,String<Va> a) {
		return out<<a.value();
	}
	template<typename Va,typename...T>
	std::ostream&operator<<(std::ostream&out,String<Va,T...> a) {
		return out<<a.value()<<a.next();
	}
	template<typename Va>
	std::string&operator<<(std::string&str,String<Va> a) {
		std::stringstream stream;
		stream<<a.value();
		std::string b;
		stream>>b;
		str+=b;
		return str;
	}
	template<typename Va>
	std::string&operator>>(String<Va> a,std::string&str) {
		std::stringstream stream;
		stream<<a.value();
		std::string b;
		stream>>b;
		str+=b;
		return str;
	}
	template<typename Va,typename...T>
	std::string&operator<<(std::string&str,String<Va,T...> a) {
		std::stringstream stream;
		stream<<a.value()<<a.next();
		std::string b;
		stream>>b;
		str+=b;
		return str;
	}
	template<typename Va,typename...T>
	std::string&operator>>(String<Va,T...> a,std::string&str) {
		std::stringstream stream;
		stream<<a.value()<<a.next();
		std::string b;
		stream>>b;
		str+=b;
		return str;
	}
	
	class Color {
		private:
			int r,g,b,a;
		public:
			Color(int _r,int _g,int _b,int _a) {
				r=_r,
				g=_g,
				b=_b,
				a=_a;
			}
			friend std::ostream&operator<<(std::ostream&out,Color c) {
				return out<<"\"color\": ["<<(c.r/255.0)<<", "<<(c.g/255.0)<<", "<<(c.b/255.0)<<", "<<(c.a/255.0)<<"]";
			}
	};
	class VariousColor {
		public:
			struct __Pair {
				double time;
				std::string color;
				friend std::ostream&operator<<(std::ostream&out,__Pair p) {
					out<<"\""<<p.time<<"\": \""<<p.color<<"\"";
				}
			};
			VariousColor(std::initializer_list<__Pair>_list) {
				list=_list;
			}
			friend std::ostream&operator<<(std::ostream&out,VariousColor c) {
				out<<"\"color\": {\n                    \"interpolant\": \"variable.particle_age/variable.particle_lifetime\",\n                    \"gradient\": {\n";
				for(int p=0; p<c.list.size(); p++) {
					out<<"                        "<<c.list[p]<<(p!=c.list.size()-1?",\n":"\n");
				}
				out<<"                    }\n                }";

			}
		private:
			std::vector<__Pair>list;
	};
	class Offset {
			std::string x,y,z;
			void change(std::string&in,std::string&out) {
				for(int i=0; i<in.length(); i++) {
					if(in[i]=='T') {
						out+="(variable.emitter_age/variable.emitter_lifetime)";
					} else {
						out+=in[i];
					}
				}
			}
		public:
			template<typename Vax,typename...Tx,typename Vay,typename...Ty,typename Vaz,typename...Tz>
			Offset(String<Vax,Tx...> _x,String<Vay,Ty...> _y,String<Vaz,Tz...> _z) {
				std::string __x,__y,__z;
				__x<<_x;
				__y<<_y;
				__z<<_z;
				change(__x,x);
				change(__y,y);
				change(__z,z);
			}
			friend std::ostream&operator<<(std::ostream&out,Offset a) {
				return out<<"\""<<a.x<<"\", \""<<a.y<<"\", \""<<a.z<<"\"";
			}
	};
	class Motion {
			std::string x,y,z,drag;
			void change(std::string&in,std::string&out) {
				for(int i=0; i<in.length(); i++) {
					if(in[i]=='T') {
						out+="(variable.particle_age/variable.particle_lifetime)";
					} else {
						out+=in[i];
					}
				}
			}
		public:
			template<typename Vax,typename...Tx,typename Vay,typename...Ty,typename Vaz,typename...Tz,typename Vad,typename...Td>
			Motion(String<Vax,Tx...> _x,String<Vay,Ty...> _y,String<Vaz,Tz...> _z,String<Vad,Td...>_drag) {
				std::string __x,__y,__z,__drag;
				__x<<_x;
				__y<<_y;
				__z<<_z;
				__drag<<_drag;
				change(__x,x);
				change(__y,y);
				change(__z,z);
				change(__drag,drag);
			}
			friend std::ostream&operator<<(std::ostream&out,Motion a) {
				return out<<"\"linear_acceleration\": [\n					\""<<a.x<<"\", \""<<a.y<<"\", \""<<a.z<<"\"\n				],\n				\"linear_drag_coefficient\": "<<a.drag;
			}
	};
	class Size {
			std::string x,y;
			void change(std::string&in,std::string&out) {
				for(int i=0; i<in.length(); i++) {
					if(in[i]=='T') {
						out+="(variable.particle_age/variable.particle_lifetime)";
					} else {
						out+=in[i];
					}
				}
			}
		public:
			template<typename Vax,typename...Tx,typename Vay,typename...Ty>
			Size(String<Vax,Tx...> _x,String<Vay,Ty...> _y) {
				std::string __x,__y;
				__x<<_x;
				__y<<_y;
				change(__x,x);
				change(__y,y);
			}
			friend std::ostream&operator<<(std::ostream&out,Size a) {
				return out<<"\""<<a.x<<"\", \""<<a.y<<"\"";
			}
	};
	template<typename T1,typename...T>
	void useTemplateEach(std::ifstream&r,std::ofstream&w,T1 arg) {
		char a='#';
		while(!r.eof()&&a!='@') {
			a=r.get();
			if(a=='%') {
				a=r.get();
				switch(a) {
					case 'N': {
						w<<arg;
						break;
					}
					case 'T': {
						w<<arg;
						break;
					}
					case 'L': {
						w<<arg;
						break;
					}
					case 'M':{
						w<<arg;
						break;
					}
					case 'S': {
						w<<arg;
						break;
					}
					case 'C': {
						w<<arg;
						break;
					}
				}
			} else if(a=='@'||a==-1) {
				return;
			} else {
				w<<a;
			}
		}
	}
	template<typename T1,typename...T>
	void useTemplateEach(std::ifstream&r,std::ofstream&w,T1 arg,T...args) {
		char a='#';
		while(!r.eof()) {
			a=r.get();
			if(a=='%') {
				a=r.get();
				switch(a) {
					case 'N': {
						w<<arg;
						useTemplateEach(r,w,args...);
						return;
					}
					case 'T': {
						w<<arg;
						useTemplateEach(r,w,args...);
						return;
					}
					case 'L': {
						w<<arg;
						useTemplateEach(r,w,args...);
						return;
					}
					case 'M': {
						w<<arg;
						useTemplateEach(r,w,args...);
						return;
					}
					case 'S': {
						w<<arg;
						useTemplateEach(r,w,args...);
						return;
					}
					case 'C': {
						w<<arg;
						useTemplateEach(r,w,args...);
						return;
					}
				}
			} else if(a=='@'||a==-1) {
				return;
			} else {
				w<<a;
			}
		}
	}
	template<typename Va1,typename...T1,typename Va2,typename...T2,typename...T>
	void useTemplate(String<Va1,T1...>inName,String<Va2,T2...>outName,T...args) {
		std::string in,out;
		in<<inName;
		out<<outName;
		std::ifstream r(in.c_str(),std::ios::in);
		std::ofstream w(out.c_str(),std::ios::out);
		char a='#';
		while(!r.eof()&&a!='@') {
			a=r.get();
		}
		useTemplateEach(r,w,args...);
		r.close();
		w.close();
	}
}
#endif

