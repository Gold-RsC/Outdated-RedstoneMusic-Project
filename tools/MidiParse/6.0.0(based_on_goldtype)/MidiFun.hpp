/********************************************************
    Name: MidiFun.hpp
    Copyright: 2024 Csrua
    Author: Csrua
    website: https://space.bilibili.com/361846321
    contact: (QQ prioritized)
        bilibili: Csrua
        QQ:       310106329
        email:    310106329@qq.com
    Date: 2024-07-30
    Description: This head file provides functions which
        can parse midi files into various types of data.
        When using the file, you must CLEARLY indicate the
        AUTHOR(Csrua) and the ORIGIN of the file.
********************************************************/

#ifndef MIDIFUN_HPP
#define MIDIFUN_HPP
#include"MidiFile.hpp"
namespace GoldType{
    namespace MidiParse{
        class Note{
            private:
                ull m_time;
                ushort m_track;
                ushort m_channel;
                ushort m_pitch;
                ushort m_velocity;
                ushort m_instrument;
            public:
                Note(ull _time=0,ushort _track=0,ushort _channel=0,ushort _pitch=0,ushort _velocity=0,ushort _instrument=0):
                    m_time(_time),m_track(_track),m_channel(_channel),m_pitch(_pitch),m_velocity(_velocity),m_instrument(_instrument){}
                Note(const Note&)=default;
                ~Note(void){
                }
            public:
                ull&time(void){
                    return m_time;
                }
                const ull&time(void)const{
                    return m_time;
                }
                ushort&track(void){
                    return m_track;
                }
                const ushort&track(void)const{
                    return m_track;
                }
                ushort&channel(void){
                    return m_channel;
                }
                const ushort&channel(void)const{
                    return m_channel;
                }
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
            if(a.pitch()!=b.pitch()){
                return a.pitch()<b.pitch();
            }
            if(a.velocity()!=b.velocity()){
                return a.velocity()<b.velocity();
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
        
        class Program{
            private:
                ull m_time;
                ushort m_track;
                ushort m_channel;
                ushort m_instrument;
            public:
                Program(ull _time=0,ushort _track=0,ushort _channel=0,ushort _instrument=0):
                    m_time(_time),m_track(_track),m_channel(_channel),m_instrument(_instrument){}
                Program(const Program&)=default;
                ~Program(void){
                }
            public:
                ull&time(void){
                    return m_time;
                }
                const ull&time(void)const{
                    return m_time;
                }
                ushort&track(void){
                    return m_track;
                }
                const ushort&track(void)const{
                    return m_track;
                }
                ushort&channel(void){
                    return m_channel;
                }
                const ushort&channel(void)const{
                    return m_channel;
                }
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
        class Lyric{
            private:
                ull m_time;
                ushort m_track;
                GoldType::String m_text;
            public:
                Lyric(ull _time=0,ushort _track=0,GoldType::String&&_text=""):
                    m_time(_time),m_track(_track),m_text(GoldType::forward<GoldType::String>(_text)){}
                Lyric(const Lyric&)=default;
                ~Lyric(void){
                }
            public:
                ull&time(void){
                    return m_time;
                }
                const ull&time(void)const{
                    return m_time;
                }
                ushort&track(void){
                    return m_track;
                }
                const ushort&track(void)const{
                    return m_track;
                }
                GoldType::String&text(void){
                    return m_text;
                }
                const GoldType::String&text(void)const{
                    return m_text;
                }
        };
        class Tempo{
            private:
                ull m_time;
                ushort m_track;
                uint m_mispqn;
                ull m_timeBase;
            public:
                Tempo(ull _time=0,ushort _track=0,uint _mispqn=0x07A120,ull _timeBase=0):
                    m_time(_time),m_track(_track),m_mispqn(_mispqn),m_timeBase(_timeBase){}
                Tempo(const Tempo&)=default;
                ~Tempo(void){
                }
            public:
                ull&time(void){
                    return m_time;
                }
                const ull&time(void)const{
                    return m_time;
                }
                ushort&track(void){
                    return m_track;
                }
                const ushort&track(void)const{
                    return m_track;
                }
                uint&mispqn(void){
                    return m_mispqn;
                }
                const uint&mispqn(void)const{
                    return m_mispqn;
                }
                double bpm(void)const{
                    return 6e7/m_mispqn;
                }
                ull&timeBase(void){
                    return m_timeBase;
                }
                const ull&timeBase(void)const {
                    return m_timeBase;
                }
        };
        class TimeSignature{
            private:
                ull m_time;
                ushort m_track;
                ushort m_numerator;
                ushort m_denominator;
                ushort m_tickPerMidiclock;
                ushort m_num32ndNotePer24Midiclock;
            public:
                TimeSignature(ull _time=0,ushort _track=0,ushort _numerator=4,ushort _denominator=4,ushort _tickPerMidiclock=24,ushort _num32ndNotePer24Midiclock=8):
                    m_time(_time),m_track(_track),m_numerator(_numerator),m_denominator(_denominator),m_tickPerMidiclock(_tickPerMidiclock),m_num32ndNotePer24Midiclock(_num32ndNotePer24Midiclock){}
                TimeSignature(const TimeSignature&)=default;
                ~TimeSignature(void){}
            public:
                ull&time(void){
                    return m_time;
                }
                const ull&time(void)const{
                    return m_time;
                }
                ushort&track(void){
                    return m_track;
                }
                const ushort&track(void)const{
                    return m_track;
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
        typedef GoldType::List<Note> NoteList;
        typedef GoldType::List<Lyric> LyricList;
        typedef GoldType::List<Tempo> TempoList;
        typedef GoldType::List<TimeSignature> TsList;
        typedef GoldType::List<Program> ProgramList;
        
        class MidiFun{
            protected:
                MidiFile m_midi;
                TempoList m_tempoList;
            public:
                MidiFun(void)=delete;
                MidiFun(const MidiFun&)=delete;
                MidiFun(GoldType::String&&_filename):
                    m_midi(GoldType::forward<GoldType::String>(_filename)){
                    m_midi.read();
                    //tempo
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        ull time=0;
                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            time+=m_midi[trackIdx][eventIdx].time();
                            if(m_midi[trackIdx][eventIdx].is_meta()&&m_midi[trackIdx][eventIdx][1]==0x51){
                                m_tempoList.emplace_back(time,trackIdx,(m_midi[trackIdx][eventIdx][3]<<16)|(m_midi[trackIdx][eventIdx][4]<<8)|(m_midi[trackIdx][eventIdx][5]));
                            }
                        }
                    }
                    for(TempoList::Iterator it=m_tempoList.begin();it!=m_tempoList.end();++it){
                        if(it==m_tempoList.begin()){
                            it->timeBase()=it->time()*0x07A120/m_midi.tpqn();
                        }
                        else if(it->track()!=(it-1)->track()){
                            it->timeBase()=it->time()*0x07A120/m_midi.tpqn();
                        }
                        else{
                            it->timeBase()=(it->time()-(it-1)->time())*((it-1)->mispqn())/m_midi.tpqn();
                            it->timeBase()+=(it-1)->timeBase();
                        }
                    }
                }
                template<typename _FUN,typename...Args>
                void for_track(_FUN fun,Args...args){
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        fun(m_midi[trackIdx],args...);
                    }
                }
                template<typename _FUN,typename...Args>
                void for_event(_FUN fun,Args...args){
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            fun(m_midi[trackIdx][eventIdx],args...);
                        }
                    }
                }
                NoteList get_noteList(void){
                    NoteList ret;
                    uchar instrumentList[16];
                    for(size_t i=0;i<16;++i){
                        instrumentList[i]=0;
                    }
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        ull time=0;
                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            time+=m_midi[trackIdx][eventIdx].time();
                            if((m_midi[trackIdx][eventIdx].type()&0xF0)==(uchar)MidiEventType::program){
                                instrumentList[m_midi[trackIdx][eventIdx].type()&0x0F]=m_midi[trackIdx][eventIdx][1];
                            }
                            else if((m_midi[trackIdx][eventIdx].type()&0xF0)==(uchar)MidiEventType::note_on||(m_midi[trackIdx][eventIdx].type()&0xF0)==(uchar)MidiEventType::note_off){
                                ret.emplace_back(time,trackIdx,m_midi[trackIdx][eventIdx].type()&0x0F,m_midi[trackIdx][eventIdx][1],(m_midi[trackIdx][eventIdx].type()&0xF0)==(uchar)MidiEventType::note_off?0:m_midi[trackIdx][eventIdx][2],instrumentList[m_midi[trackIdx][eventIdx].type()&0x0F]);
                            }
                        }
                    }
                    return ret;
                }
                ProgramList get_programList(void){
                    ProgramList ret;
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        ull time=0;
                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            time+=m_midi[trackIdx][eventIdx].time();
                            if((m_midi[trackIdx][eventIdx].type()&0xF0)==(uchar)MidiEventType::program){
                                ret.emplace_back(time,trackIdx,m_midi[trackIdx][eventIdx].type()&0x0F,m_midi[trackIdx][eventIdx][1]);
                            }
                        }
                    }
                    return ret;
                }
                LyricList get_lyricList(void){
                    LyricList ret;
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        ull time=0;
                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            time+=m_midi[trackIdx][eventIdx].time();
                            if(m_midi[trackIdx][eventIdx].is_meta()&&m_midi[trackIdx][eventIdx][1]==0x05){
                                ret.emplace_back(time,trackIdx,GoldType::String(m_midi[trackIdx][eventIdx].message().begin()+m_midi[trackIdx][eventIdx].get_dataBeginIdx(),m_midi[trackIdx][eventIdx].message().end()));
                            }
                        }
                    }
                    return ret;
                }
                TempoList get_tempoList(void){
                    return m_tempoList;
                }
                TsList get_tsList(void){
                    TsList ret;
                    for(size_t trackIdx=0;trackIdx<m_midi.ntracks();++trackIdx){
                        ull time=0;
                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            time+=m_midi[trackIdx][eventIdx].time();
                            if(m_midi[trackIdx][eventIdx].is_meta()&&m_midi[trackIdx][eventIdx][1]==0x58){
                                ret.emplace_back(time,trackIdx,m_midi[trackIdx][eventIdx][3],1<<m_midi[trackIdx][eventIdx][4],m_midi[trackIdx][eventIdx][5],m_midi[trackIdx][eventIdx][6]);
                            }
                        }
                    }
                    return ret;
                }
                ull get_microSecond(ull tick,ushort track=0xFFFF){
                    if(m_midi.format()==0||m_midi.format()==1){
                        if(tick<m_tempoList[0].time()){
                            return tick*0x07A120/m_midi.tpqn();
                        }
                        if(tick>=m_tempoList[m_tempoList.size()-1].time()){
                            return m_tempoList[m_tempoList.size()-1].timeBase()+(tick-m_tempoList[m_tempoList.size()-1].time())*m_tempoList[m_tempoList.size()-1].mispqn()/m_midi.tpqn();
                        }
                        size_t left=0,right=m_tempoList.size()-1,mid=0;
                        while(left<right){
                            mid=(left+right)/2;
                            if(m_tempoList[mid].time()>tick){
                                right=mid;
                            }
                            else if(m_tempoList[mid+1].time()<=tick){
                                left=mid+1;
                            }
                            else{
                                return m_tempoList[mid].timeBase()+(tick-m_tempoList[mid].time())*m_tempoList[mid].mispqn()/m_midi.tpqn();
                            }
                        }
                    }
                    else if(m_midi.format()==2){
                        ushort trackBeginIdx=0,trackEndIdx=m_tempoList.size();
                        while(track!=m_tempoList[trackBeginIdx].track()){
                            ++trackBeginIdx;
                        }
                        while(track!=m_tempoList[trackEndIdx-1].track()){
                            --trackEndIdx;
                        }
                        
                        if(tick<m_tempoList[trackBeginIdx].time()){
                            return tick*0x07A120/m_midi.tpqn();
                        }
                        if(tick>=m_tempoList[trackEndIdx-1].time()){
                            return m_tempoList[trackEndIdx-1].timeBase()+(tick-m_tempoList[trackEndIdx-1].time())*m_tempoList[trackEndIdx-1].mispqn()/m_midi.tpqn();
                        }
                        size_t left=trackBeginIdx,right=trackEndIdx-1,mid=0;
                        while(left<right){
                            mid=(left+right)/2;
                            if(m_tempoList[mid].time()>tick){
                                right=mid;
                            }
                            else if(m_tempoList[mid+1].time()<=tick){
                                left=mid+1;
                            }
                            else{
                                return m_tempoList[mid].timeBase()+(tick-m_tempoList[mid].time())*m_tempoList[mid].mispqn()/m_midi.tpqn();
                            }
                        }
                    }
                    return 0xFFFFFFFFFFFFFFFF;
                }
                template<typename T>
                void change_timeStandard(GoldType::List<T>&li){
                    for(typename GoldType::List<T>::Iterator it=li.begin();it!=li.end();++it){
                        it->time()=get_microSecond(it->time(),it->track());
                    }
                }
        };
    }
}

#endif
