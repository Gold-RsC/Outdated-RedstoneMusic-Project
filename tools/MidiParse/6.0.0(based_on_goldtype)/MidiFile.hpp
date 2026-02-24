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
#include"../../GoldType/2.0.0/List.hpp"
#include"../../GoldType/2.0.0/String.hpp"
#include<stdio.h>
namespace GoldType{
    namespace MidiParse{
        typedef unsigned int uint;
        typedef unsigned short ushort;
        typedef unsigned long long ull;
        typedef unsigned char uchar;
        
        enum class MidiErrorType:uchar{
            //0000 0000
            noError=0x00,
            //1111 1111
            filename=0xFF,
                //0000 1111
                head_identification=0x0F,
                    //0000 0001
                    head_length=0x01,
                    //0000 0010
                    head_format=0x02,
                    //0000 0100
                    head_ntracks=0x04,
                    //0000 1000
                    head_division=0x08,
                //1000 1111
                track_identification=0x8F,
                    //1000 0001
                    track_length=0x81,
                    //1000 0010
                    track_deltaTime=0x82,
                    //1000 0100
                    track_eventType=0x84,
                    //1000 1000
                    track_eventData=0x88,
        };
        
        #ifdef WRITE_MIDI_ERROR
        class MidiError{
            protected:
                FILE*file;
            public:
                MidiError(void):
                    file(stderr){}
                MidiError(GoldType::String&&name):
                    file(fopen(name.c_str(),"w")){}
                ~MidiError(void){
                    if(file!=stderr){
                        fclose(file);
                    }
                    file=NULL;
                }
                void resetFile(GoldType::String&&name){
                    this->~MidiError();
                    file=fopen(name.c_str(),"w");
                }
                MidiErrorType write(MidiErrorType num){
                    fprintf(file,"MidiErrorType:\n");
                    switch(num){
                        case MidiErrorType::noError:{
                            fprintf(file,"No error!\n");
                            break;
                        }
                        case MidiErrorType::filename:{
                            fprintf(file,"There is no file named like this!\n");
                            break;
                        }
                        case MidiErrorType::head_identification:{
                            fprintf(file,"An error occurred in the head trunk!\n");
                            fprintf(file,"Head trunk identification error!\n");
                            break;
                        }
                        case MidiErrorType::head_length:{
                            fprintf(file,"An error occurred in the head trunk!\n");
                            fprintf(file,"Head trunk length error!\n");
                            break;
                        }
                        case MidiErrorType::head_format:{
                            fprintf(file,"An error occurred in the head trunk!\n");
                            fprintf(file,"Head trunk format error!\n");
                            break;
                        }
                        case MidiErrorType::head_ntracks:{
                            fprintf(file,"An error occurred in the head trunk!\n");
                            fprintf(file,"Head trunk track count error!\n");
                            break;
                        }
                        case MidiErrorType::head_division:{
                            fprintf(file,"An error occurred in the head trunk!\n");
                            fprintf(file,"Head trunk division error!\n");
                            break;
                        }
                        case MidiErrorType::track_identification:{
                            fprintf(file,"An error occurred in the track trunk!\n");
                            fprintf(file,"Track trunk identification error!\n");
                            break;
                        }
                        case MidiErrorType::track_length:{
                            fprintf(file,"An error occurred in the track trunk!\n");
                            fprintf(file,"Track trunk length error!\n");
                            break;
                        }
                        case MidiErrorType::track_deltaTime:{
                            fprintf(file,"An error occurred in the track trunk!\n");
                            fprintf(file,"Track trunk event delta time error!\n");
                            break;
                        }
                        case MidiErrorType::track_eventType:{
                            fprintf(file,"An error occurred in the track trunk!\n");
                            fprintf(file,"Track trunk event type error!\n");
                            break;
                        }
                        case MidiErrorType::track_eventData:{
                            fprintf(file,"An error occurred in the track trunk!\n");
                            fprintf(file,"Track trunk event data error!\n");
                            break;
                        }
                        default:{
                            fprintf(file,"Unknown error!\n");
                            break;
                        }
                    }
                    return num;
                }
        };
        MidiError midierror;
        #endif
        
        typedef GoldType::List<uchar> MidiMessage;
        
        enum class MidiEventType:uchar{
            null=0x00,
            /******************
            Part1:type = 0xNx (N => 8~E ,x => 0~F)
            type & 0x7F => 0xN0
            ******************/
            note_off=0x80,
            note_on=0x90,
            key_afterTouch=0xA0,
            controller=0xB0,
            program=0xC0,
            channel_afterTouch=0xD0,
            pitchWheel=0xE0,
            
            /*******************
            Part2:type = 0xF0 or 0xF7
            *******************/
            sysex_begin=0xF0,sysex_end=0xF7,
            
            /*******************
            Part3:type = 0xFF
            *******************/
            meta=0xFF
        };
        
        class MidiEvent {
            protected:
                uint m_time;
                MidiMessage m_message;
            public:
                MidiEvent(void):
                    m_time(),m_message() {}
                MidiEvent(const MidiEvent&another)=default;
                MidiEvent(const ull&_time,const MidiMessage&_message):
                    m_time(_time),m_message(_message) {}
                ~MidiEvent(void){}
            public:
                uint&time(void) {
                    return m_time;
                }
                const uint&time(void)const {
                    return m_time;
                }
                MidiMessage&message(void) {
                    return m_message;
                }
                const MidiMessage&message(void)const {
                    return m_message;
                }
            public:
                uchar type(void)const {
                    return m_message[0];
                }
                uchar metaType(void)const{
                    return is_meta()?m_message[1]:m_message[0];
                }
                bool is_normal(void)const{
                    return !(is_meta());
                }
                bool is_meta(void)const {
                    return type()==(uchar)MidiEventType::meta;
                }
                bool is_sysex(void)const{
                    return type()==(uchar)MidiEventType::sysex_begin||type()==(uchar)MidiEventType::sysex_end;
                }
                
                size_t get_dataBeginIdx(void){
                    if(is_sysex()){
                        for(size_t i=1;i<m_message.size();++i){
                            if(m_message[i]<0x80){
                                return i+1;
                            }
                        }
                    }
                    else if(is_meta()){
                        for(size_t i=2;i<m_message.size();++i){
                            if(m_message[i]<0x80){
                                return i+1;
                            }
                        }
                    }
                    return 1;
                }
            public:
                uchar&operator[](int idx) {
                    return m_message[idx];
                }
                MidiEvent&operator=(const MidiEvent&another)=default;
        };
        
        typedef GoldType::List<MidiEvent> MidiTrack;
        
        class MidiHead{
            protected:
                ushort m_format;
                ushort m_ntracks;
                ushort m_division;
            public:
                MidiHead(ushort fmt=0,ushort ntrcks=1,ushort dvs=120):
                    m_format(fmt),m_ntracks(ntrcks),m_division(dvs){}
                ushort&format(void){
                    return m_format;
                }
                const ushort&format(void)const {
                    return m_format;
                }
                ushort&ntracks(void){
                    return m_ntracks;
                }
                const ushort&ntracks(void)const{
                    return m_ntracks;
                }
                ushort&division(void){
                    return m_division;
                }
                const ushort&division(void)const{
                    return m_division;
                }
                ushort tpqn(void)const{
                    //15th
                    //1xxx xxxx xxxx xxxx
                    if(m_division&0x8000){
                        //8th~14th
                        //1fff ffff xxxx xxxx
                        ushort fps=0x0100-(m_division>>8)&0xFF;
                        //0th~7th
                        //1xxx xxxx tttt tttt
                        ushort tpf=m_division&0xFF;
                        switch(fps){
                            case 24:break;
                            case 25:break;
                            case 29:break;
                            case 30:break;
                            default:{
                                #ifdef WRITE_MIDI_ERROR
                                midierror.write(MidiErrorType::head_division);
                                #endif
                                return 120;
                            }
                        }
                        return fps*tpf;
                    }
                    //0xxx xxxx xxxx xxxx
                    else{
                        return m_division;
                    }
                }
                MidiErrorType error(void)const{
                    if(!(m_format==0||m_format==1||m_format==2)) {
                        return MidiErrorType::head_format;
                    }
                    if(m_format==0&&m_ntracks!=1) {
                        return MidiErrorType::head_ntracks;
                    }
                    if(m_division&0x8000){
                        ushort fps=0x0100-(m_division>>8)&0xFF;
                        ushort tpf=m_division&0xFF;
                        switch(fps){
                            case 24:break;
                            case 25:break;
                            case 29:break;
                            case 30:break;
                            default:{
                                return MidiErrorType::head_division;
                            }
                        }
                    }
                    return MidiErrorType::noError;
                }
        };
        class MidiFile {
            protected:
                GoldType::String m_filename;
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
                MidiErrorType getMidiData(FILE*fin,uchar&last_eventType,MidiEvent&event,uint&byte_read) {
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
                MidiErrorType read(FILE*fin) {
                    if(!fin) {
                        #ifdef WRITE_MIDI_ERROR
                        return midierror.write(MidiErrorType::filename);
                        #else
                        return MidiErrorType::filename;
                        #endif
                    }
                    if(!(fgetc(fin)=='M'&&fgetc(fin)=='T'&&fgetc(fin)=='h'&&fgetc(fin)=='d')){
                        #ifdef WRITE_MIDI_ERROR
                        return midierror.write(MidiErrorType::head_identification);
                        #else
                        return MidiErrorType::head_identification;
                        #endif
                    }
                    if(get_staticData(fin,4)!=6){
                        #ifdef WRITE_MIDI_ERROR
                        return midierror.write(MidiErrorType::head_length);
                        #else
                        return MidiErrorType::head_length;
                        #endif
                    }
                    m_head.format()=get_staticData(fin,2);
                    m_head.ntracks()=get_staticData(fin,2);
                    m_head.division()=get_staticData(fin,2);
                    MidiErrorType head_error=m_head.error();
                    if(head_error!=MidiErrorType::noError){
                        #ifdef WRITE_MIDI_ERROR
                        return midierror.write(head_error);
                        #else
                        return head_error;
                        #endif
                    }
                    m_track=new MidiTrack[m_head.ntracks()];
                    size_t event_count=0;
                    for(int trackIdx=0; trackIdx<m_head.ntracks(); trackIdx++) {
                        if(fgetc(fin)!='M'||fgetc(fin)!='T'||fgetc(fin)!='r'||fgetc(fin)!='k') {
                            #ifdef WRITE_MIDI_ERROR
                            return midierror.write(MidiErrorType::track_identification);
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
                                #ifdef WRITE_MIDI_ERROR
                                return midierror.write(MidiErrorType::track_deltaTime);
                                #else
                                return MidiErrorType::track_deltaTime;
                                #endif
                            }
                            MidiErrorType track_error=getMidiData(fin,last_eventType,event,byte_read);
                            if(track_error!=MidiErrorType::noError) {
                                #ifdef WRITE_MIDI_ERROR
                                return midierror.write(track_error);
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
                MidiFile(GoldType::String&&_filename):
                    m_filename(GoldType::forward<GoldType::String>(_filename)),m_head(),m_track(nullptr) {}
                MidiFile(const MidiFile&)=delete;
            public:
                MidiErrorType read(void) {
                    FILE*fin=fopen(m_filename.c_str(),"rb");
                    MidiErrorType ret=read(fin);
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
                MidiErrorType read(GoldType::String&&_filename) {
                    m_filename=GoldType::forward<GoldType::String>(_filename);
                    FILE*fin=fopen(m_filename.c_str(),"rb");
                    MidiErrorType ret=read(fin);
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
                ~MidiFile(void){
                    if(m_track){
                        delete[] m_track;
                        m_track=nullptr;
                    }
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
        };
    }
}

#endif
