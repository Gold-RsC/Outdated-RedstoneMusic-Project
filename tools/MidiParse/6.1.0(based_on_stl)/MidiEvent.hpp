#ifndef MIDIEVENT_HPP
#define MIDIEVENT_HPP
#include"MidiMessage.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Event
        ********************************/
        enum class MidiEventType:uchar{
            null=0x00,
            /******************
            Part1:type = 0xNx (N = 8~E ,x = 0~F)
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
        
        
        
        class BasicMidiEvent{
        	protected:
        		ull m_time;
                ushort m_track;
            public:
            	BasicMidiEvent(ull _time=0,ushort _track=0):
                    m_time(_time),m_track(_track){}
                BasicMidiEvent(const BasicMidiEvent&)=default;
                virtual ~BasicMidiEvent(void)=default;
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
		};
        
        class BasicMidiEvent_Non:public BasicMidiEvent{
        	protected:
        		ushort m_channel;
            public:
            	BasicMidiEvent_Non(ull _time=0,ushort _track=0,ushort _channel=0):
                    BasicMidiEvent(_time,_track),m_channel(_channel){}
                BasicMidiEvent_Non(const BasicMidiEvent_Non&)=default;
                virtual ~BasicMidiEvent_Non(void)=default;
            public:
        		ushort&channel(void){
                    return m_channel;
                }
                const ushort&channel(void)const{
                    return m_channel;
                }
		};
		class BasicMidiEvent_Meta:public BasicMidiEvent{
            public:
            	BasicMidiEvent_Meta(ull _time=0,ushort _track=0):
				    BasicMidiEvent(_time,_track){}
                BasicMidiEvent_Meta(const BasicMidiEvent_Meta&)=default;
                virtual ~BasicMidiEvent_Meta(void)=default;
		};
		
        class MidiEvent:public BasicMidiEvent {
            protected:
                MidiMessage m_message;
            public:
                MidiEvent(void):
                    m_message() {}
                MidiEvent(const MidiEvent&another)=default;
                MidiEvent(const MidiMessage&_message):
                    m_message(_message) {}
                ~MidiEvent(void){}
            public:
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
                const uchar&operator[](int idx) const{
                    return m_message[idx];
                }
                MidiEvent&operator=(const MidiEvent&another)=default;
        };
        
    }
}
#endif
