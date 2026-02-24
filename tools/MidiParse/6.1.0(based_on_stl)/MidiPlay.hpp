#ifndef MIDIPLAY_HPP
#define MIDIPLAY_HPP
#include<windows.h>
#include<unistd.h>
#include<algorithm>
#pragma comment(lib,"winmm.lib")
#include"MidiFun.hpp"
namespace GoldType{
    namespace MidiParse{
        class ShortMessage{
            ull m_time;
            uint m_message;
            public:
                ShortMessage(ull _time=0,uint _message=0):
                    m_time(_time),m_message(_message){}
                ShortMessage(const ShortMessage&)=default;
                ull&time(void){
                    return m_time;
                }
                const ull&time(void)const {
                    return m_time;
                }
                uint&message(void){
                    return m_message;
                }
                const uint&message(void)const{
                    return m_message;
                }
        }; 
        bool operator==(const ShortMessage&a,const ShortMessage&b){
            return a.time()==b.time()&&a.message()==b.message();
        }
        bool operator!=(const ShortMessage&a,const ShortMessage&b){
            return !(a==b);
        }
        bool operator<(const ShortMessage&a,const ShortMessage&b){
            if(a.time()!=b.time()){
                return a.time()<b.time();
            }
            return a.message()<b.message();
        }
        bool operator>(const ShortMessage&a,const ShortMessage&b){
            if(a.time()!=b.time()){
                return a.time()>b.time();
            }
            return a.message()>b.message();
        }
        bool operator<=(const ShortMessage&a,const ShortMessage&b){
            return !(a.message()>b.message());
        }
        bool operator>=(const ShortMessage&a,const ShortMessage&b){
            return !(a.message()<b.message());
        }
        class ShortMessageList:public std::vector<ShortMessage>{
            public:
                ShortMessageList(void){}
                ShortMessageList(const NoteList&_noteList){
                    read(_noteList);
                }
                ShortMessageList(const NotePairList&_noteList){
                    read(_noteList.devide());
                }
                ShortMessageList(const MidiFun&_mf){
                    read(_mf);
                }
                void read(const NoteList&_noteList){
                    NoteList _n=_noteList;
                    std::sort(_n.begin(),_n.end());
                    uchar instrument[16]={0};
                    for(NoteList::const_iterator it=_noteList.cbegin();it!=_noteList.cend();++it){
                        if(it->instrument()!=instrument[it->channel()]){
                            instrument[it->channel()]=it->instrument();
                            emplace_back(it->time(),(it->instrument()<<8)|0xC0|it->channel());
                        }
                        emplace_back(it->time(),(it->velocity()<<16)|(it->pitch()<<8)|0x90|it->channel());
                    }
                }
                void read(const NotePairList&_noteList){
                    read(_noteList.devide());
                }
                void read(const MidiFun&_mf){
                    for(int trackIdx=0;trackIdx<_mf.m_midi.ntracks();++trackIdx){
						ull tm=0;
						for(int eventIdx=0;eventIdx<_mf.m_midi.track()[trackIdx].size();++eventIdx){
							tm+=_mf.m_midi.track()[trackIdx][eventIdx].time();
							switch(_mf.m_midi.track()[trackIdx][eventIdx].type()&0xF0) {
		                        case 0x80:        //note_off
		                        case 0x90:        //note_on
		                        case 0xA0:        //key_afterTouch
		                        case 0xB0:        //controller
                                case 0xE0: {      //pitchWheel
		                            emplace_back(_mf.match_microSecond(tm,trackIdx),(_mf.m_midi[trackIdx][eventIdx][2]<<16)|(_mf.m_midi[trackIdx][eventIdx][1]<<8)|_mf.m_midi[trackIdx][eventIdx][0]);
		                            break;
		                        }
		                        case 0xC0:        //program
		                        case 0xD0: {      //channel_afterTouch
		                            emplace_back(_mf.match_microSecond(tm,trackIdx),(_mf.m_midi[trackIdx][eventIdx][1]<<8)|_mf.m_midi[trackIdx][eventIdx][0]);
		                            break;
		                        }
		                        case 0xF0:{
                                    break;
                                }
		                    }
						}
					}
                }
                void sort(void){
                    std::sort(begin(),end());
                }
                void play(void){
                    HMIDIOUT handle;
					midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
					if(size())
					for(int i=0,j=0;i<size()-1;++i){
						midiOutShortMsg(handle,operator[](i).message());
						ull _delta=operator[](i+1).time()-operator[](i).time();
						ull _n=_delta%999999,_d=_delta/999999;
						while(_d--){
						    usleep(999999);
                        }
						if(_n){
							usleep(_n);
						}
					}
					if(size()){
					   midiOutShortMsg(handle,operator[](size()-1).message());
                    }
					sleep(5);
					midiOutClose(handle);
                }
                template<typename _Function,typename..._Args>
                void play_if(_Function _fun,_Args..._args){
                    ShortMessageList a;
                    for(ShortMessageList::iterator i=begin();i!=end();++i){
                        if(_fun(*i,_args...)){
                            a.emplace_back(*i);
                        }
                    }
                    a.play();
                }
                template<typename _Function,typename..._Args>
                void play_while(_Function _fun,_Args..._args){
                    HMIDIOUT handle;
					midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
					for(int i=0;i<size()-1;++i){
					    _fun(operator[](i),_args...);
				        midiOutShortMsg(handle,operator[](i).message());
						ull _delta=operator[](i+1).time()-operator[](i).time();
						ull _n=_delta%999999,_d=_delta/999999;
						while(_d--){
						    usleep(999999);
                        }
						if(_n){
							usleep(_n);
						}
					}
					    
					if(size()){
					    _fun(operator[](size()-1),_args...);
    					midiOutShortMsg(handle,operator[](size()-1).message());
                    }
					sleep(5);
					midiOutClose(handle);
                } 
                
                void write(const std::string&_path){
                    FILE*w=fopen(_path.c_str(),"w");
                    fprintf(w,
                    "#include<windows.h>\n"
                    "#include<unistd.h>\n"
                    "#pragma comment(lib,\"winmm.lib\")\n"
                    "HMIDIOUT handle;\n"
                    "int main(void){\n"
                    "\tmidiOutOpen(&handle,0,0,0,CALLBACK_NULL);\n"
                    );
                    for(int i=0;i<size()-1;++i){
                    	fprintf(w,"\tmidiOutShortMsg(handle,0x%X);\n",operator[](i).message());
                    	ull _delta=operator[](i+1).time()-operator[](i).time();
                    	ull _n=_delta%999999,_d=_delta/999999;
						while(_d--){
						    fprintf(w,"\tusleep(999999);\n");
                        }
						if(_n){
						    fprintf(w,"\tusleep(%llu);\n",_n);
						}
                    }
					if(size()){
					    fprintf(w,"\tmidiOutShortMsg(handle,0x%X);\n",operator[](size()-1).message());
                    }
                    fprintf(w,
                    "\tmidiOutClose(handle);\n"
                    "\tsleep(5);\n"
                    "\treturn 0;\n"
                    "}"
                    );
                    fclose(w);
                }
        };
        
		MidiErrorType play(ShortMessageList _l){
            _l.sort();
            _l.play();
            return MidiErrorType::noError;
        }
    	MidiErrorType play(std::string _filename){
    	    MidiFun _mf(_filename);
			if(!_mf.m_midi.track()){
//        				#ifdef MIDI_DEBUG
                return midierror.write_text("The function play() cannot run susseecefully!\n");
//                        #else
//                        return midierror.get_type();
//                        #endif
			}
			//input
			ShortMessageList command(_mf);
			
			//sort
			return play(command);
		}
//                MidiErrorType play_lyrics(FILE*_lyricFile=stdout){
//        			if(!_mf.m_midi.track()){
//        				#ifdef MIDI_DEBUG
//                        return write_error("The function play() cannot run susseecefully!\n");
//                        #else
//                        return midierror.get_type();
//                        #endif
//					}
//					//input
//					std::vector<std::pair<ull,uint>> command;
//					LyricList ly;
//					
//					for(int trackIdx=0;trackIdx<_mf.m_midi.ntracks();++trackIdx){
//						ull tm=0;
//						for(int eventIdx=0;eventIdx<_mf.m_midi.track()[trackIdx].size();++eventIdx){
//							tm+=_mf.m_midi.track()[trackIdx][eventIdx].time();
//							switch(_mf.m_midi.track()[trackIdx][eventIdx].type()&0xF0) {
//		                        case 0x90:        //note_on
//		                        case 0xA0:        //key_afterTouch
//		                        case 0xB0: {      //controller
//		                            command.emplace_back(match_microSecond(tm,trackIdx),(_mf.m_midi.track()[trackIdx][eventIdx][2]<<16)|(_mf.m_midi.track()[trackIdx][eventIdx][1]<<8)|_mf.m_midi.track()[trackIdx][eventIdx][0]);
//		                            break;
//		                        }
//		                        case 0xE0: {      //pitchWheel
//		                            command.emplace_back(match_microSecond(tm,trackIdx),(_mf.m_midi.track()[trackIdx][eventIdx][2]<<16)|(_mf.m_midi.track()[trackIdx][eventIdx][1]<<8)|_mf.m_midi.track()[trackIdx][eventIdx][0]);
//		                            break;
//		                        }
//		                        case 0x80:        //note_off
//		                        case 0xC0:        //program
//		                        case 0xD0: {      //channel_afterTouch
//		                            command.emplace_back(match_microSecond(tm,trackIdx),(_mf.m_midi.track()[trackIdx][eventIdx][1]<<8)|_mf.m_midi.track()[trackIdx][eventIdx][0]);
//		                            break;
//		                        }
//		                        case 0xF0:{
//		                            switch(_mf.m_midi[trackIdx][eventIdx].type()){
//										case 0xFF:{
//											switch(_mf.m_midi[trackIdx][eventIdx][1]){
//												case 0x05:{
//													ly.emplace_back(match_microSecond(tm,trackIdx),trackIdx,std::string(_mf.m_midi[trackIdx][eventIdx].message().begin()+_mf.m_midi[trackIdx][eventIdx].get_dataBeginIdx(),_mf.m_midi[trackIdx][eventIdx].message().end()));
//													break;
//												}
//											}
//											break;
//										}
//									}
//                                    break;
//                                }
//		                    }
//						}
//					}
//					//sort
//					std::sort(command.begin(),command.end());
//					std::sort(ly.begin(),ly.end());
//					//play
//					HMIDIOUT handle;
//					midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
//					for(int i=0,j=0;i<command.size()-1;++i){
//						midiOutShortMsg(handle,command[i].second);
//						while(j<ly.size()){
//						    if(ly[j].time()>command[i].first){
//							    break;
//                            }
//							fprintf(_lyricFile,ly[j].text().c_str());
//							fprintf(_lyricFile," ");
//							++j;
//							
//						}
//						ull _delta=command[i+1].first-command[i].first;
//						ull _n=_delta%999999,_d=_delta/999999;
//						while(_d--){
//						    usleep(999999);
//                        }
//						if(_n){
//							usleep(_n);
//						}
//					}
//					midiOutShortMsg(handle,command[command.size()-1].second);
//					sleep(5);
//					midiOutClose(handle);
//        			return MidiErrorType::noError;
//				}
			
    }
}			
#endif
