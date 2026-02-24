#ifndef MIDIERROR_HPP
#define MIDIERROR_HPP
#include<stdio.h>
#include<string>
namespace GoldType{
    namespace MidiParse{
        typedef unsigned int uint;
        typedef unsigned short ushort;
        typedef unsigned long long ull;
        typedef unsigned char uchar;
        
        /********************************
                    Midi Error
        ********************************/
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
        
//        #ifdef MIDI_DEBUG
        class MidiError{
            protected:
                FILE*file;
                MidiErrorType _errtype;
            public:
                MidiError(void):
                    file(stderr),_errtype(MidiErrorType::noError){}
                MidiError(std::string name):
                    file(fopen(name.c_str(),"w")),_errtype(MidiErrorType::noError){}
                ~MidiError(void){
                    if(file!=stderr){
                        fclose(file);
                    }
                    file=NULL;
                }
                void replace(std::string name){
                    this->~MidiError();
                    file=fopen(name.c_str(),"w");
                }
                MidiErrorType write_type(MidiErrorType num){
                    _errtype=num;
                    fprintf(file,"MidiError:\n\t");
                    switch(num){
                        case MidiErrorType::noError:{
                            fprintf(file,"No error!\n");
                            break;
                        }
                        case MidiErrorType::filename:{
                            fprintf(file,"There is no file named this!\n");
                            break;
                        }
                        case MidiErrorType::head_identification:{
                            fprintf(file,"An error occurred in the head trunk!\n\t");
                            fprintf(file,"Head trunk identification error!\n");
                            break;
                        }
                        case MidiErrorType::head_length:{
                            fprintf(file,"An error occurred in the head trunk!\n\t");
                            fprintf(file,"Head trunk length error!\n");
                            break;
                        }
                        case MidiErrorType::head_format:{
                            fprintf(file,"An error occurred in the head trunk!\n\t");
                            fprintf(file,"Head trunk format error!\n");
                            break;
                        }
                        case MidiErrorType::head_ntracks:{
                            fprintf(file,"An error occurred in the head trunk!\n\t");
                            fprintf(file,"Head trunk track count error!\n");
                            break;
                        }
                        case MidiErrorType::head_division:{
                            fprintf(file,"An error occurred in the head trunk!\n\t");
                            fprintf(file,"Head trunk division error!\n");
                            break;
                        }
                        case MidiErrorType::track_identification:{
                            fprintf(file,"An error occurred in the track trunk!\n\t");
                            fprintf(file,"Track trunk identification error!\n");
                            break;
                        }
                        case MidiErrorType::track_length:{
                            fprintf(file,"An error occurred in the track trunk!\n\t");
                            fprintf(file,"Track trunk length error!\n");
                            break;
                        }
                        case MidiErrorType::track_deltaTime:{
                            fprintf(file,"An error occurred in the track trunk!\n\t");
                            fprintf(file,"Track trunk event delta time error!\n");
                            break;
                        }
                        case MidiErrorType::track_eventType:{
                            fprintf(file,"An error occurred in the track trunk!\n\t");
                            fprintf(file,"Track trunk event type error!\n");
                            break;
                        }
                        case MidiErrorType::track_eventData:{
                            fprintf(file,"An error occurred in the track trunk!\n\t");
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
                MidiErrorType write_text(std::string _text){
                    fprintf(file,_text.c_str());
                    return _errtype;
                }
                MidiErrorType get_type(void){
                    return _errtype;
				}
        };
        MidiError midierror;
//        #endif
    }
}
#endif
