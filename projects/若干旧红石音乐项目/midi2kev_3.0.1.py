import os,sys,datetime,uuid,locale
main_path = sys.path[0]
format_version = "3.0.0"

#Get system language
sys_lang = locale.getdefaultlocale()[0]    #System language

#Load language file
if os.path.exists(main_path + "\setting\lang\%s.lang"%sys_lang):
    lang_file = open(main_path + "\setting\lang\%s.lang"%sys_lang,"r")
else:
    lang_file = open(main_path + "\setting\lang\en.US.lang","r")
lang_dict = {}
for i in lang_file:
    if i.count("="):
        phrase = i.split("=")
        index_word = phrase[0]; answer_word = phrase[1]
        lang_dict[index_word] = answer_word

#Test for mido
try:
    import mido
except ImportError:
    print(lang_dict["text.importmido.error"])
    os.system("python -m pip install mido")
    print(lang_dict["text.installmido.success"])
    import mido

#Test for mid file and oepn files
midfn = input(lang_dict["text.inputmidifilename.request"])
while os.path.exists(main_path + "\midi\%s.mid"%midfn) == False:
    midfn = input(lang_dict["text.inputmidifilename.requestagain"])
midi_file = mido.MidiFile(main_path + "\midi\%s.mid"%midfn)
print(midi_file)
kev = open(main_path + "\kev\%s.kev"%midfn,"w",encoding='utf8')
kev.write("identity:{name:%s,version%s,date:%s,time:0}\n"%(midfn,format_version,str(datetime.datetime.now())))

#print midi msg
ppqn = int(midi_file.ticks_per_beat)    #midi tick 4
kev.write("ticks_per_beat:{value:%s,time:0}\n"%ppqn)
for i, track in enumerate(midi_file.tracks):
    for msg in track:
        msg = str(msg)
        if msg.count("meta message"):
            is_meta = "true"
            msg = msg.replace("<","").replace(">","").split()
            type = msg[2]
            value_list = []
            for v in range(3,len(msg)):
                value_list.append(msg[v].replace("=",":"))
            value_str = ("%s:{ismeta:true,%s}\n"%(type,str(value_list).replace("[","").replace("]","").replace("'",""))).replace(" ","")
        else:
            is_meta = "false"
            msg = msg.split()
            type = msg[0]
            value_list = []
            for v in range(1,len(msg)):
                value_list.append(msg[v].replace("=",":"))
            value_str = ("%s:{ismeta:false,%s}\n"%(type,str(value_list).replace("[","").replace("]","").replace("'",""))).replace(" ","")
        kev.write(value_str)
print(lang_dict["text.done.return"])
kev.close()
