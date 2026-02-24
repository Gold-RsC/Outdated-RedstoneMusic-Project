#ifndef __MIDIFUN_H__
#define __MIDIFUN_H__
#include"MidiFile.h"
class MidiFun {
	private:
		MidiFile mid;
		NOTE note;
		BPM bpm;
		int Main();
		int findbpmidx(int tick);
		void change();
		template<typename __FUNCTION>
		void drawline(NOTE&lp,NOTE&np,__FUNCTION writecmd);
	public:
		MidiFun();
		MidiFun(const MidiFun&other);
		MidiFun(const MidiFile&other);
		MidiFun(const char*filename);
		MidiFun(FILE*FIN);
		void init(const char*filename);
		void init(FILE*FIN);
		void init(const MidiFile&other);
		int get_tpqn();
		int get_fmt();
		int get_ntrk();
		int get_note(NOTE&sth);
		int get_bpm(BPM&sth);
		int get_ts_n();
		int get_ts_d();
		template<typename __FUNCTION>
		void makeline(NOTEev&pinit,__FUNCTION writecmd);
		~MidiFun();
};
MidiFun::MidiFun() {
}
MidiFun::MidiFun(const MidiFun&other) {
	*this=other;
	Main();
}
MidiFun::MidiFun(const MidiFile&other) {
	this->mid=other;
	Main();
}
MidiFun::MidiFun(const char*filename) {
	mid.read(filename);
	Main();
}
MidiFun::MidiFun(FILE*FIN) {
	mid.read(FIN);
	Main();
}
void MidiFun::init(const char*filename) {
	mid.read(filename);
	Main();
}
void MidiFun::init(FILE*FIN) {
	mid.read(FIN);
	Main();
}
void MidiFun::init(const MidiFile&other) {
	mid=other;
}
int MidiFun::Main() {
	if(!mid.is_read())
		return -2;
	change();
	return 1;
}
int MidiFun::get_tpqn() {
	return mid.get_tpqn();
}
int MidiFun::get_fmt() {
	return mid.get_fmt();
}
int MidiFun::get_ntrk() {
	return mid.get_ntrk();
}
int MidiFun::get_ts_d() {
	return mid.get_ts_d();
}
int MidiFun::get_ts_n() {
	return mid.get_ts_n();
}
int MidiFun::get_note(NOTE&sth) {
	if(!mid.is_read())
		return -2;
	sth=note;
	return note.size();
}
int MidiFun::get_bpm(BPM&sth) {
	if(!mid.is_read())
		return -2;
	sth=bpm;
	return bpm.size();
}
int MidiFun::findbpmidx(int tick) {
	int l=0,r=bpm.size()-1;
	if(tick>=bpm[r].tick)
		return r;
	if(tick<bpm[l].tick)
		return -1;
	while(l<=r) {
		int mid=(l+r)/2;
		if(tick<bpm[mid].tick)
			r=mid;
		else if(tick>=bpm[mid+1].tick)
			l=mid+1;
		else
			return mid;
	}
	return -1;
}
void MidiFun::change() {
	mid.get_note(note);
	mid.get_bpm(bpm);
	int bpm_size=bpm.size();
	int*sum=new int[bpm_size+1];
	int TPQN=mid.get_tpqn();
	int ts_d=mid.get_ts_d(),ts_n=mid.get_ts_n();
	for(int i=0; i<bpm_size; i++) {
		sum[i]=
		    (i==0?0:sum[i-1])
		    +(bpm[i].tick-(i==0?0:bpm[i-1].tick))
		    *1200*ts_d/TPQN
		    /(i==0?1:bpm[i-1].value)/ts_n;
	}
	int note_size=note.size();
	for(int i=0; i<note_size; i++) {
		int bpmidx=findbpmidx(note[i].tick);
		if(bpmidx==-1)
			return;
		int tick=
		    sum[bpmidx]
		    +(note[i].tick-bpm[bpmidx].tick)
		    *1200*ts_d
		    /TPQN/bpm[bpmidx].value/ts_n;
		note[i].tick=tick;
	}
	for(int i=0; i<bpm_size; i++)
		bpm[i].tick=sum[i];
	delete[] sum;
}
template<typename __FUNCTION>
void MidiFun::drawline(NOTE&lp,NOTE&np,__FUNCTION writecmd) {
	int lpn=lp.size(),npn=np.size();
	if(lpn*1.0/npn>1) {
		int k=lpn/npn;
		if(k*npn==lpn)
			for(int i=1; i<=npn; i++)
				for(int j=k*(i-1)+1; j<=k*i; j++)
					writecmd(lp[j-1],np[i-1]);
		else {
			for(int i=1; i<=lpn-k*npn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					writecmd(lp[j-1],np[i-1]);
			for(int i=lpn-k*npn+1; i<=npn; i++)
				for(int j=k*(i-1-npn)+lpn+1; j<=k*(i-npn)+lpn; j++)
					writecmd(lp[j-1],np[i-1]);
		}
	} else if(npn*1.0/lpn>1) { //npnÎªlpnµÄk±¶(k>1)
		int k=npn/lpn;
		if(k*lpn==npn)
			for(int i=1; i<=lpn; i++)
				for(int j=k*i-k+1; j<=k*i; j++)
					writecmd(lp[i-1],np[j-1]);
		else {
			for(int i=1; i<=npn-k*lpn; i++)
				for(int j=(k+1)*(i-1)+1; j<=(k+1)*i; j++)
					writecmd(lp[i-1],np[j-1]);
			for(int i=npn-k*lpn+1; i<=lpn; i++)
				for(int j=k*(i-1-lpn)+npn+1; j<=k*(i-lpn)+npn; j++)
					writecmd(lp[i-1],np[j-1]);
		}
	} else //lpnµÈÓÚnpn 
		for(int i=1; i<=npn; i++)
			writecmd(lp[i-1],np[i-1]);
}
template<typename __FUNCTION>
void MidiFun::makeline(NOTEev&pinit,__FUNCTION writecmd) {
	NOTE lp,np;
	lp.push(pinit);
	np.push(note[0]);
	int size=note.size();
	for(int i=1; i<size; i++) {
		if(!note[i].velocity)
			continue;
		if(
		    np[0].channel!=note[i].channel||
		    np[0].track!=note[i].track
		) {
			drawline(lp,np,writecmd);
			lp.clear();
			np.clear();
			lp.push(pinit);
			np.push(note[i]);
		} else if(
		    np[0].channel==note[i].channel&&
		    np[0].track==note[i].track&&
		    np[0].tick!=note[i].tick
		) {
			drawline(lp,np,writecmd);
			lp.clear();
			lp.move(np);
			np.push(note[i]);
		} else
			np.push(note[i]);
	}
	drawline(lp,np,writecmd);
}
MidiFun::~MidiFun() {
}
#endif
