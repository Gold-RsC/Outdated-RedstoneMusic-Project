#ifndef MOREMIDIEVENT_HPP
#define MOREMIDIEVENT_HPP
#include"MidiEvent.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    More Midi Event
        ********************************/
        class Note:public BasicMidiEvent_Non{
            protected:
                ushort m_pitch;
                ushort m_velocity;
                ushort m_instrument;
                double m_bar;
                double m_beat;
            public:
                Note(ull _time=0,ushort _track=0,ushort _channel=0,ushort _pitch=0,ushort _velocity=0,ushort _instrument=0,double _bar=1.0,double _beat=1.0):
                    BasicMidiEvent_Non(_time,_track,_channel),m_pitch(_pitch),m_velocity(_velocity),m_instrument(_instrument),m_bar(_bar),m_beat(_beat){}
                Note(const Note&)=default;
                ~Note(void)=default;
            public:
                ushort&pitch(void){
                    return m_pitch;
                }
                const ushort&pitch(void)const{
                    return m_pitch;
                }
                ushort&velocity(void){
                    return m_velocity;
                }
                const ushort&velocity(void)const {
                    return m_velocity;
                }
                ushort&instrument(void){
                    return m_instrument;
                }
                const ushort&instrument(void)const{
                    return m_instrument;
                }
                double&bar(void){
                    return m_bar;
                }
                const double&bar(void)const{
                    return m_bar;
                }
                double&beat(void){
                    return m_beat;
                }
                const double&beat(void)const{
                    return m_beat;
                }
            public:
                bool is_empty(void)const {
                    return !(time())&&!(channel())&&!(track())&&!(pitch())&&!(velocity())&&(!instrument());
                }
        };
        bool operator==(const Note&a,const Note&b){
            return a.time()==b.time()&&a.channel()==b.channel()&&a.track()==b.track()&&a.pitch()==b.pitch()&&a.velocity()==b.velocity()&&a.instrument()==b.instrument();
        }
        bool operator!=(const Note&a,const Note&b){
            return !(a==b);
        }
        bool operator<(const Note&a,const Note&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.channel()!=b.channel()){
                return a.channel()<b.channel();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
            if(a.velocity()!=b.velocity()){
                return a.velocity()<b.velocity();
            }
            if(a.pitch()!=b.pitch()){
                return a.pitch()<b.pitch();
            }
            return a.instrument()<b.instrument();
        }
        bool operator>(const Note&a,const Note&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.channel()!=b.channel()){
                return a.channel()>b.channel();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
            if(a.pitch()!=b.pitch()){
                return a.pitch()>b.pitch();
            }
            if(a.velocity()!=b.velocity()){
                return a.velocity()>b.velocity();
            }
            return a.instrument()>b.instrument();
        }
        bool operator<=(const Note&a,const Note&b){
            return !(a>b);
        }
        bool operator>=(const Note&a,const Note&b){
            return !(a<b);
        }
        
        class Program:public BasicMidiEvent_Non{
            private:
                ushort m_instrument;
            public:
                Program(ull _time=0,ushort _track=0,ushort _channel=0,ushort _instrument=0):
                    BasicMidiEvent_Non(_time,_track,_channel),m_instrument(_instrument){}
                Program(const Program&)=default;
                ~Program(void)=default;
            public:
                ushort&instrument(void){
                    return m_instrument;
                }
                const ushort&instrument(void)const{
                    return m_instrument;
                }
        };
        bool operator==(const Program&a,const Program&b){
            return a.time()==b.time()&&a.channel()==b.channel()&&a.track()==b.track()&&a.instrument()==b.instrument();
        }
        bool operator!=(const Program&a,const Program&b){
            return !(a==b);
        }
        bool operator<(const Program&a,const Program&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.channel()!=b.channel()){
                return a.channel()<b.channel();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
            return a.instrument()<b.instrument();
        }
        bool operator>(const Program&a,const Program&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.channel()!=b.channel()){
                return a.channel()>b.channel();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
            return a.instrument()>b.instrument();
        }
        bool operator<=(const Program&a,const Program&b){
            return !(a>b);
        }
        bool operator>=(const Program&a,const Program&b){
            return !(a<b);
        }
        
		class Lyric:public BasicMidiEvent_Meta{
            private:
                std::string m_text;
            public:
                Lyric(ull _time=0,ushort _track=0,std::string&&_text=""):
                    BasicMidiEvent_Meta(_time,_track),m_text(std::forward<std::string>(_text)){}
                Lyric(const Lyric&)=default;
                ~Lyric(void)=default;
            public:
                std::string&text(void){
                    return m_text;
                }
                const std::string&text(void)const{
                    return m_text;
                }
        };
        bool operator==(const Lyric&a,const Lyric&b){
            return a.time()==b.time()&&a.track()==b.track()&&a.text()==b.text();
        }
        bool operator!=(const Lyric&a,const Lyric&b){
            return !(a==b);
        }
        bool operator<(const Lyric&a,const Lyric&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
        	return a.text()<b.text();
        }
        bool operator>(const Lyric&a,const Lyric&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
        	return a.text()>b.text();
        }
        bool operator<=(const Lyric&a,const Lyric&b){
            return !(a>b);
        }
        bool operator>=(const Lyric&a,const Lyric&b){
            return !(a<b);
        }
        
        class Tempo:public BasicMidiEvent_Meta{
            private:
                uint m_mispqn;
                ull m_timeNode;
            public:
                Tempo(ull _time=0,ushort _track=0,uint _mispqn=0x07A120,ull _timeNode=0):
                    BasicMidiEvent_Meta(_time,_track),m_mispqn(_mispqn),m_timeNode(_timeNode){}
                Tempo(const Tempo&)=default;
                ~Tempo(void)=default;
            public:
                uint&mispqn(void){
                    return m_mispqn;
                }
                const uint&mispqn(void)const{
                    return m_mispqn;
                }
                double bpm(void)const{
                    return 6e7/m_mispqn;
                }
                ull&timeNode(void){
                    return m_timeNode;
                }
                const ull&timeNode(void)const {
                    return m_timeNode;
                }
        };
        bool operator==(const Tempo&a,const Tempo&b){
            return a.time()==b.time()&&a.track()==b.track()&&a.mispqn()==b.mispqn()&&a.timeNode()==b.timeNode();
        }
        bool operator!=(const Tempo&a,const Tempo&b){
            return !(a==b);
        }
        bool operator<(const Tempo&a,const Tempo&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
            if(a.mispqn()!=b.mispqn()){
            	return a.mispqn()<b.mispqn(); 
			}
        	return a.timeNode()<b.timeNode();
        }
        bool operator>(const Tempo&a,const Tempo&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
            if(a.mispqn()!=b.mispqn()){
            	return a.mispqn()>b.mispqn(); 
			}
        	return a.timeNode()>b.timeNode();
        }
        bool operator<=(const Tempo&a,const Tempo&b){
            return !(a>b);
        }
        bool operator>=(const Tempo&a,const Tempo&b){
            return !(a<b);
        }
        
		class TimeSignature:public BasicMidiEvent_Meta{
            private:
                ushort m_numerator;
                ushort m_denominator;
                ushort m_tickPerMidiclock;
                ushort m_num32ndNotePer24Midiclock;
            public:
                TimeSignature(ull _time=0,ushort _track=0,ushort _numerator=4,ushort _denominator=4,ushort _tickPerMidiclock=24,ushort _num32ndNotePer24Midiclock=8):
                    BasicMidiEvent_Meta(_time,_track),m_numerator(_numerator),m_denominator(_denominator),m_tickPerMidiclock(_tickPerMidiclock),m_num32ndNotePer24Midiclock(_num32ndNotePer24Midiclock){}
                TimeSignature(const TimeSignature&)=default;
                ~TimeSignature(void)=default;
            public:
                ushort&numerator(void){
                    return m_numerator;
                }
                const ushort&numerator(void)const{
                    return m_numerator;
                }
                ushort&denominator(void){
                    return m_denominator;
                }
                const ushort&denominator(void)const{
                    return m_denominator;
                }
                ushort&tickPerMidiclock(void){
                    return m_tickPerMidiclock;
                }
                const ushort&tickPerMidiclock(void)const{
                    return m_tickPerMidiclock;
                }
                ushort&num32ndNotePer24Midiclock(void){
                    return m_num32ndNotePer24Midiclock;
                }
                const ushort&num32ndNotePer24Midiclock(void)const{
                    return m_num32ndNotePer24Midiclock;
                }
        };
        bool operator==(const TimeSignature&a,const TimeSignature&b){
            return a.time()==b.time()&&a.track()==b.track()&&a.numerator()==b.numerator()&&a.denominator()==b.denominator()&&a.tickPerMidiclock()==b.tickPerMidiclock()&&a.num32ndNotePer24Midiclock()==b.num32ndNotePer24Midiclock();
        }
        bool operator!=(const TimeSignature&a,const TimeSignature&b){
            return !(a==b);
        }
        bool operator<(const TimeSignature&a,const TimeSignature&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
            if(a.numerator()!=b.numerator()){
            	return a.numerator()<b.numerator(); 
			}
			if(a.denominator()!=b.denominator()){
            	return a.denominator()<b.denominator(); 
			}
			if(a.tickPerMidiclock()!=b.tickPerMidiclock()){
            	return a.tickPerMidiclock()<b.tickPerMidiclock(); 
			}
        	return a.num32ndNotePer24Midiclock()<b.num32ndNotePer24Midiclock();
        }
        bool operator>(const TimeSignature&a,const TimeSignature&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
            if(a.numerator()!=b.numerator()){
            	return a.numerator()>b.numerator(); 
			}
			if(a.denominator()!=b.denominator()){
            	return a.denominator()>b.denominator(); 
			}
			if(a.tickPerMidiclock()!=b.tickPerMidiclock()){
            	return a.tickPerMidiclock()>b.tickPerMidiclock(); 
			}
        	return a.num32ndNotePer24Midiclock()>b.num32ndNotePer24Midiclock();
        }
        bool operator<=(const TimeSignature&a,const TimeSignature&b){
            return !(a>b);
        }
        bool operator>=(const TimeSignature&a,const TimeSignature&b){
            return !(a<b);
        }
        
        class BarBeat:public BasicMidiEvent_Meta{
            private:
                double m_barNode;
                double m_beatNode;
                ushort m_numerator;
                ushort m_denominator;
            public:
                BarBeat(ull _time=0,ushort _track=0,double _barNode=0,double _beatNode=0,ushort _numerator=4,ushort _denominator=4):
                    BasicMidiEvent_Meta(_time,_track),m_barNode(_barNode),m_beatNode(_beatNode),m_numerator(_numerator),m_denominator(_denominator){}
                BarBeat(const BarBeat&)=default;
                ~BarBeat(void)=default;
            
		    public:
		        double&barNode(void){
		            return m_barNode;
                }
                const double&barNode(void)const {
		            return m_barNode;
                }
		        double&beatNode(void){
		            return m_beatNode;
                }
                const double&beatNode(void)const {
		            return m_beatNode;
                }
                ushort&numerator(void){
                    return m_numerator;
                }
                const ushort&numerator(void)const{
                    return m_numerator;
                }
                ushort&denominator(void){
                    return m_denominator;
                }
                const ushort&denominator(void)const{
                    return m_denominator;
                }
                
        };
        bool operator==(const BarBeat&a,const BarBeat&b){
            return a.time()==b.time()&&a.track()==b.track()&&a.barNode()==b.barNode()&&a.beatNode()==b.beatNode();
        }
        bool operator!=(const BarBeat&a,const BarBeat&b){
            return !(a==b);
        }
        bool operator<(const BarBeat&a,const BarBeat&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
            if(a.barNode()!=b.barNode()){
            	return a.barNode()<b.barNode(); 
			}
			if(a.beatNode()!=b.beatNode()){
            	return a.beatNode()<b.beatNode(); 
			}
			if(a.numerator()!=b.numerator()){
			    return a.numerator()<b.numerator();
            }
            return a.denominator()<b.denominator();
        }
        bool operator>(const BarBeat&a,const BarBeat&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
            if(a.barNode()!=b.barNode()){
            	return a.barNode()>b.barNode(); 
			}
            if(a.beatNode()!=b.beatNode()){
            	return a.beatNode()>b.beatNode(); 
			}
			if(a.numerator()!=b.numerator()){
			    return a.numerator()>b.numerator();
            }
            return a.denominator()>b.denominator();
        }
        bool operator<=(const BarBeat&a,const BarBeat&b){
            return !(a>b);
        }
        bool operator>=(const BarBeat&a,const BarBeat&b){
            return !(a<b);
        }
        
        
        enum class TimeState:uchar{
            //0000 0000
            tick=0x00,
            //0000 0001
            microSecond=0x01
        };
        template<typename _MidiEvent>
        class MidiEventList:public std::vector<_MidiEvent>{
            protected:
                TimeState m_timeState;
           public:
                MidiEventList(void):
                    m_timeState(TimeState::tick){
                }
                MidiEventList(const MidiEventList&)=default;
                
                TimeState&timeState(void){
                    return m_timeState;
                }
                const TimeState&timeState(void)const{
                    return m_timeState;
                }
        };
        
        typedef MidiEventList<Note> NoteList;
        typedef MidiEventList<Lyric> LyricList;
        typedef MidiEventList<Tempo> TempoList;
        typedef MidiEventList<TimeSignature> TsList;
        typedef MidiEventList<Program> ProgramList;
        typedef MidiEventList<BarBeat> BarBeatList;
        
        template<typename _MidiEvent>
        uint get_ntracks(const MidiEventList<_MidiEvent>&_list){
        	uint _num=0;
        	for(typename MidiEventList<_MidiEvent>::iterator it=_list.begin();it!=_list.end();++it){
        		_num=max(_num,it->track()+1);
			}
			return _num;
		}
        
        template<typename _MidiEvent>
        MidiEventList<_MidiEvent>*devide_track(const MidiEventList<_MidiEvent>&_list,uint&_num){
        	_num=get_ntracks(_list);
			MidiEventList<_MidiEvent>*ret=new MidiEventList<_MidiEvent>[_num];
			for(typename MidiEventList<_MidiEvent>::iterator it=_list.begin();it!=_list.end();++it){
        		ret[it->track()].push_back(*it);
			}
			return ret;
		}
		
		bool is_notePair(const Note&_begin,const Note&_end){
		    return _begin.channel()==_end.channel()&&_begin.track()==_end.track()&&_begin.pitch()==_end.pitch()&&_begin.instrument()==_end.instrument()&&_begin.velocity()&&!(_end.velocity())&&_begin.time()<_end.time();
        }
		class NotePair:public Note{
		    private:
		        ull m_duration;
		    public:
		        NotePair(ull _time=0,ull _duration=0,ushort _track=0,ushort _channel=0,ushort _pitch=0,ushort _velocity=0,ushort _instrument=0,double _bar=1.0,double _beat=1.0):
                    Note(_time,_track,_channel,_pitch,_velocity,_instrument,_bar,_beat),m_duration(_duration){}
                NotePair(const Note&_begin,const Note&_end){
                    if(is_notePair(_begin,_end)){
                        *this=NotePair(_begin.time(),_end.time()-_begin.time(),_begin.track(),_begin.channel(),_begin.pitch(),_begin.velocity(),_begin.instrument(),_begin.bar(),_begin.beat());
                    }
                    else{
                        *this=NotePair();
                    }
                }
                NotePair(const NotePair&)=default;
                ~NotePair(void)=default;
            public:
                ull&duration(void){
                    return m_duration;
                }
                const ull&duration(void)const {
                    return m_duration;
                }
                std::pair<Note,Note> devide(void)const{
                    return std::pair<Note,Note>(Note(m_time,m_track,m_channel,m_pitch,m_velocity,m_instrument),Note(m_time+m_duration,m_track,m_channel,m_pitch,0,m_instrument));
                }
        };
        bool operator==(const NotePair&a,const NotePair&b){
            return a.time()==b.time()&&a.channel()==b.channel()&&a.track()==b.track()&&a.pitch()==b.pitch()&&a.velocity()==b.velocity()&&a.instrument()==b.instrument();
        }
        bool operator!=(const NotePair&a,const NotePair&b){
            return !(a==b);
        }
        bool operator<(const NotePair&a,const NotePair&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            if(a.channel()!=b.channel()){
                return a.channel()<b.channel();
            }
            if(a.track()!=b.track()){
                return a.track()<b.track();
            }
            if(a.pitch()!=b.pitch()){
                return a.pitch()<b.pitch();
            }
            if(a.velocity()!=b.velocity()){
                return a.velocity()<b.velocity();
            }
            return a.instrument()<b.instrument();
        }
        bool operator>(const NotePair&a,const NotePair&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            if(a.channel()!=b.channel()){
                return a.channel()>b.channel();
            }
            if(a.track()!=b.track()){
                return a.track()>b.track();
            }
            if(a.pitch()!=b.pitch()){
                return a.pitch()>b.pitch();
            }
            if(a.velocity()!=b.velocity()){
                return a.velocity()>b.velocity();
            }
            return a.instrument()>b.instrument();
        }
        bool operator<=(const NotePair&a,const NotePair&b){
            return !(a>b);
        }
        bool operator>=(const NotePair&a,const NotePair&b){
            return !(a<b);
        }
        
        class NotePairList:public std::vector<NotePair>{
            public:
                NotePairList(void)=default;
                NotePairList(const NoteList&_noteList){
                    match(_noteList);
                }
                NotePairList(const NotePairList&)=default;
                ~NotePairList(void)=default;
            public:
                void match(const NoteList&_noteList){
                    Note map[2048];
                    for(NoteList::const_iterator it=_noteList.cbegin();it!=_noteList.cend();++it){
                        size_t idx=it->channel()<<7|it->pitch();
                        if(map[idx].is_empty()){
                            if(it->velocity()){
                                map[idx]=*it;
                            }
                            else{
                                printf("???[%d %2.2X %2.2X %2.2X %2.2X]",it-_noteList.cbegin(),it->pitch(),it->velocity(),map[idx].pitch(),map[idx].velocity());
                            }
                        }
                        else{
                            if(it->velocity()){
                                printf("^^^[%d %2.2X %2.2X %2.2X %d]",it-_noteList.cbegin(),it->pitch(),it->velocity(),map[idx].pitch(),map[idx].velocity());
                            }
                            else{
                                emplace_back(map[idx],*it);
                                map[idx]=Note();
                            }
                        }
                    }
                    for(size_t i=0;i<2048;++i){
                        if(!map[i].is_empty()){
                            printf("!!!");
                        }
                    }
                }
                NoteList devide(void)const{
                    NoteList _l;
                    for(NotePairList::const_iterator it=cbegin();it!=cend();++it){
                        std::pair<Note,Note> _n=it->devide();
                        _l.emplace_back(_n.first);
                        _l.emplace_back(_n.second);
                    }
                    return _l;
                }
        };
    }
}
#endif
