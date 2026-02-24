/********************************************************
    Name: MidiFile.hpp
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

#ifndef MIDIFILE_HPP
#define MIDIFILE_HPP
#include"MidiHead.hpp"
#include"MidiTrack.hpp"

namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi File
        ********************************/
		class MidiFile {
            protected:
                std::string m_filename;
                MidiHead m_head;
                MidiTrack*m_track;
            protected:
                uint get_staticData(FILE*fin,size_t num){
                    uint ret=0;
                    for(size_t i=0;i<num;++i){
                        ret=(ret<<8)+fgetc(fin);
                    }
                    return ret;
                }
                uint get_dynamicData(FILE*FIN) {
                    uint ret=0;
                    for(size_t i=0;i<4; ++i) {
                        ret<<=7;
                        uchar a=fgetc(FIN);
                        ret|=(a&0x7F);
                        if(a<0x80) {
                            return ret;
                        }
                    }
                    return ret;
                }
                uint get_dynamicData(FILE*FIN,uint&varlen) {
                    uint ret=0;
                    for(size_t i=0; i<4; ++i) {
                        ret<<=7;
                        uchar a=fgetc(FIN);
                        ret|=(a&0x7F);
                        if(a<0x80) {
                            varlen+=i+1;
                            return ret;
                        }
                    }
                    varlen+=4;
                    return ret;
                }
                uint get_dynamicData(FILE*FIN,uint&varlen,MidiMessage&message) {
                    uint ret=0;
                    for(size_t i=0; i<4; ++i) {
                        ret<<=7;
                        uchar a=fgetc(FIN);
                        message.push_back(a);
                        ret|=(a&0x7F);
                        if(a<0x80) {
                            varlen+=i+1;
                            return ret;
                        }
                    }
                    varlen+=4;
                    return ret;
                }
                MidiErrorType get_midiData(FILE*fin,uchar&last_eventType,MidiEvent&event,uint&byte_read) {
                    uchar now_eventType=fgetc(fin);
                    ++byte_read;
                    if(now_eventType<0x80) {
                        if(last_eventType==(uchar)MidiEventType::null) {
                            return MidiErrorType::track_eventType;
                        } 
                        if((last_eventType&0xF0)==0xF0) {
                            return MidiErrorType::track_eventType;
                        }
                        now_eventType=last_eventType;
                        fseek(fin,-1,SEEK_CUR);
                        --byte_read;
                    }
                    event.message().clear();
                    switch(now_eventType&0xF0) {
                        case 0x80:        //note_off
                        case 0x90:        //note_on
                        case 0xA0:        //key_afterTouch
                        case 0xB0:        //controller
                        case 0xE0: {      //pitchWheel
                            uchar data1=fgetc(fin);
                            uchar data2=fgetc(fin);
                            event.message()={now_eventType,data1,data2};
                            byte_read+=2;
                            break;
                        }
                        case 0xC0:        //program
                        case 0xD0: {      //channel_afterTouch
                            uchar data1=fgetc(fin);
                            event.message()={now_eventType,data1};
                            ++byte_read;
                            break;
                        }
                        case 0xF0: {      //sysex or meta
                            switch(now_eventType) {
                                case 0xFF: {    //meta
                                    event.message().push_back(now_eventType);
                                    uchar metaType=fgetc(fin);
                                    event.message().push_back(metaType);
                                    ++byte_read;
                                    ull len=get_dynamicData(fin,byte_read,event.message());
                                    byte_read+=len;
                                    for(ull i=0; i<len; i++)
                                        event.message().push_back(fgetc(fin));
                                    break;
                                }
                                case 0xF0:        //sysex_begin
                                case 0xF7: {      //sysex_end
                                    event.message().push_back(now_eventType);
                                    ull len=get_dynamicData(fin,byte_read,event.message());
                                    byte_read+=len;
                                    for(ull i=0; i<len; i++)
                                        event.message().push_back(fgetc(fin));
                                    break;
                                }
                                default: {
                                    return MidiErrorType::track_eventData;
                                }
                            }
                            break;
                        }
                        default: {
                            return MidiErrorType::track_eventType;
                        }
                    }
                    last_eventType=now_eventType;
                    return MidiErrorType::noError;
                }
                MidiErrorType _read(FILE*fin) {
                    if(!fin) {
                        #ifdef MIDI_DEBUG
                        return midierror.write_type(MidiErrorType::filename);
                        #else
                        return MidiErrorType::filename;
                        #endif
                    }
                    if(!(fgetc(fin)=='M'&&fgetc(fin)=='T'&&fgetc(fin)=='h'&&fgetc(fin)=='d')){
                        #ifdef MIDI_DEBUG
                        return midierror.write_type(MidiErrorType::head_identification);
                        #else
                        return MidiErrorType::head_identification;
                        #endif
                    }
                    if(get_staticData(fin,4)!=6){
                        #ifdef MIDI_DEBUG
                        return midierror.write_type(MidiErrorType::head_length);
                        #else
                        return MidiErrorType::head_length;
                        #endif
                    }
                    m_head.format()=get_staticData(fin,2);
                    m_head.ntracks()=get_staticData(fin,2);
                    m_head.division()=get_staticData(fin,2);
                    MidiErrorType head_error=m_head.error();
                    if(head_error!=MidiErrorType::noError){
                        #ifdef MIDI_DEBUG
                        return midierror.write_type(head_error);
                        #else
                        return head_error;
                        #endif
                    }
                    m_track=new MidiTrack[m_head.ntracks()];
                    size_t event_count=0;
                    for(int trackIdx=0; trackIdx<m_head.ntracks(); trackIdx++) {
                        if(fgetc(fin)!='M'||fgetc(fin)!='T'||fgetc(fin)!='r'||fgetc(fin)!='k') {
                            #ifdef MIDI_DEBUG
                            return midierror.write_type(MidiErrorType::track_identification);
                            #else
                            return MidiErrorType::track_identification;
                            #endif
                        }
                        uint byte_count=get_staticData(fin,4);
                        uint byte_read=0;
                        uchar last_eventType=(uchar)MidiEventType::null;
                        while(!feof(fin)) {
                            MidiEvent event;
                            event.time()=get_dynamicData(fin,byte_read);
                            if(event.time()>0x0FFFFFFF){
                                #ifdef MIDI_DEBUG
                                return midierror.write_type(MidiErrorType::track_deltaTime);
                                #else
                                return MidiErrorType::track_deltaTime;
                                #endif
                            }
                            MidiErrorType track_error=get_midiData(fin,last_eventType,event,byte_read);
                            if(track_error!=MidiErrorType::noError) {
                                #ifdef MIDI_DEBUG
                                return midierror.write_type(track_error);
                                #else
                                return track_error;
                                #endif
                            }
                            m_track[trackIdx].push_back(event);
                            ++event_count;
                            if(event.is_meta()&&event[1]==0x2F) {
                                break;
                            }
                        }
                    }
                    return MidiErrorType::noError;
                }
            public:
                MidiFile(void):
                    m_filename(),m_head(),m_track(nullptr) {}
                MidiFile(std::string _filename):
                    m_filename(_filename),m_head(),m_track(nullptr) {}
                MidiFile(const MidiFile&)=delete;
                ~MidiFile(void){
                    if(m_track){
                        delete[] m_track;
                        m_track=nullptr;
                    }
                }
            public:
                MidiErrorType read(void) {
                    FILE*fin=fopen(m_filename.c_str(),"rb");
                    MidiErrorType ret=_read(fin);
                    fclose(fin);
                    if(ret!=MidiErrorType::noError){
                        m_head.format()=0;
                        m_head.ntracks()=0;
                        m_head.division()=0;
                        if(m_track){
                            delete[] m_track;
                        }
                    }
                    return ret;
                }
                MidiErrorType read(std::string _filename) {
                    m_filename=_filename;
                    FILE*fin=fopen(m_filename.c_str(),"rb");
                    MidiErrorType ret=_read(fin);
                    fclose(fin);
                    if(ret!=MidiErrorType::noError){
                        m_head.format()=0;
                        m_head.ntracks()=0;
                        m_head.division()=0;
                        if(m_track){
                            delete[] m_track;
                            m_track=nullptr;
                        }
                    }
                    return ret;
                }
                
                
                std::string&filename(void){
                    return m_filename;
                }
                const std::string&filename(void)const {
                    return m_filename;
                }
                ushort&format(void){
                    return m_head.format();
                }
                const ushort&format(void)const {
                    return m_head.format();
                }
                ushort&ntracks(void){
                    return m_head.ntracks();
                }
                const ushort&ntracks(void)const{
                    return m_head.ntracks();
                }
                ushort&division(void){
                    return m_head.division();
                }
                const ushort&division(void)const{
                    return m_head.division();
                }
                ushort tpqn(void)const{
                    return m_head.tpqn();
                }
                MidiHead&head(void){
                    return m_head;
                }
                const MidiHead&head(void)const {
                    return m_head;
                }
                MidiTrack*&track(void) {
                    return m_track;
                }
                const MidiTrack*track(void) const{
                    return m_track;
                }
            
            public:
                MidiFile&operator=(const MidiFile&)=delete;
                MidiTrack&operator[](int idx) {
                    return m_track[idx];
                }
                const MidiTrack&operator[](int idx) const{
                    return m_track[idx];
                }
        	public:
        		bool is_read(void){
        			return m_track;
				}
		};
    }
}

#endif
