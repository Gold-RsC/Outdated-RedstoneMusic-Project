#ifndef MIDIHEAD_HPP
#define MIDIHEAD_HPP
#include"MidiError.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Head
        ********************************/
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
                                #ifdef MIDI_DEBUG
                                midierror.write_type(MidiErrorType::head_division);
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
        
    }
}
#endif
