"MidiFile.h":
	header_file:
		stdio.h
		stdlib.h
		vector
		windows.h
	define:
		MIDIERR		-1
		NULL_TICK	-2 
		NULL_CHNL	-2 
		NULL_TRK	-2 
		NULL_PITCH	-2 
		NULL_VELO	-2 
		NULL_INSTRU	-2 
		NULL_BPM	-2
	variable:
		NOTEev null_of_note_event;
		NOTEev error_of_note_event(-1,-1,-1,-1,-1,-1);
		#define NULL_Nev null_of_note_event
		#define ERR_Nev error_of_note_event
		
		BPMev null_of_bpm_event;
		BPMev error_of_bpm_event(-1,-1);
		#define NULL_Bev null_of_bpm_event
		#define ERR_Bev error_of_bpm_event
	class(type):
		NOTEev
		BPMev
		NOTE//NOTEev数组.用vector实现 
		BPM//BPMev数组.用vector实现
		MidiFile
		
	members:
		NOTEev:
			int tick,
				channel,track,
				pitch,velocity,
				instrument;//乐器
			//构造函数 
			NOTEev(int tick=-2,int channel=-2,int track=-2,int pitch=-2,int velocity=-2,int instrument=-2);
			//拷贝构造函数 
			NOTEev(const NOTEev&note);
			//设置参数 
			void set(int tick=-2,int channel=-2,int track=-2,int pitch=-2,int velocity=-2,int instrument=-2);
			//清空参数 
			void clear();
			//拷贝参数 
			void copy(const NOTEev&note);
			//剪切参数 
			void move(NOTEev&note);
			//比较 
			bool operator==(NOTEev&note);
			bool operator!=(NOTEev&note);
		BPMev: //同上 
			int tick,
				value;
			BPMev(int tick=-2,int value=-2);
			BPMev(const BPMev&bpm);
			void set(int tick=-2,int value=-2);
			void clear();
			void copy(BPMev&bpm);
			void move(BPMev&bpm);
			bool operator==(BPMev&bpm);
			bool operator!=(BPMev&bpm);
		NOTE:
			//这些雨女无瓜 
			int tick,channel,track,pitch,velocity,instrument;
			//构造函数 
			NOTE();
			//拷贝构造函数 
			NOTE(const NOTE&sth);
			//在末尾添加参数 
			void push(NOTEev&sth);
			//弹出末尾的参数 
			void pop();
			//拷贝整个数组 
			void copy(NOTE&sth);
			//剪切整个数组 
			void move(NOTE&sth);
			//清空整个数组 
			void clear();
			//数组是否为空 
			bool empty();
			//数组参数数量 
			int size();
			//排序 
			void sort();
			//访问某下标对应参数 
			NOTEev&operator[](int idx);
		BPM://同上 
			int tick,value;
			BPM();
			BPM(const BPM&sth);
			void push(BPMev&sth);
			void pop();
			void copy(BPM&sth);
			void move(BPM&sth);
			void clear();
			bool empty();
			int size();
			void sort();
			BPMev&operator[](int idx);
		MidiFile:
			//构造函数 
			MidiFile();
			MidiFile(FILE*FIN);
			MidiFile(const char*filename);
			MidiFile(const MidiFile&other);
			//读取 
			int read(FILE*FIN);
			int read(const char*filename);
			//是否读取 
			bool is_read();
			//获取事件信息,
			//返回值为事件信息或事件数量 
			int get_tpqn();
			int get_fmt();
			int get_ntrk();
			int get_note(NOTE&sth);
			int get_bpm(BPM&sth);
			int get_ts_n();
			int get_ts_d();
			//析构函数 
			~MidiFile();
"MidiFun.h" 
	header_file:
		MidiFile.h
	class(type):
		MidiFun
	member:
		MidiFun:
			//构造函数 
			MidiFun();
			MidiFun(const MidiFun&other);
			MidiFun(const MidiFile&other);
			MidiFun(const char*filename);
			MidiFun(FILE*FIN);
			//初始化(构造函数李有参数时可不用) 
			void init(const char*filename);
			void init(FILE*FIN);
			void init(const MidiFile&other);
			//获取事件 
			int get_tpqn();
			int get_fmt();
			int get_ntrk();
			int get_note(NOTE&sth);
			int get_bpm(BPM&sth);
			int get_ts_n();
			int get_ts_d();
			/***********************************************
			*连线函数
			*	参数:pinit(NOTEev&),writecmd(<template>__FUNCTION); 
			*	参数详解：
			*		pinit:
			*			类型:(NOTE&),variable
			*		writecmd:
			*			类型:(<template>__FUNCTION),function
			*			参数:NOTEev,NOTEev(均可加"&")
			*			返回值:void 
			*	返回值:void
			***********************************************/
			template<typename __FUNCTION>
			void makeline(NOTEev&pinit,__FUNCTION writecmd);
			//析构函数 
			~MidiFun();
