import os, sys ,json, pickle, mido as md, easygui as ui, kevfile as kf; format_version = [3,8,0]
while True:    #打开midi
    try:
        cr = ui.fileopenbox(msg="mid",title="选择midi文件",default="\\*.mid",filetypes=["*.mid"],multiple=False)
        if cr == None:
            os._exit(1)
        else:
            midi_file = md.MidiFile(cr); fn = os.path.basename(cr).replace(".mid","");print(str(midi_file))
    except BaseException:
        result = ui.indexbox(msg='你选择的不是标准midi文件', title='错误', choices=('重新选择', '退出'), image=None, default_choice='重新选择', cancel_choice='退出')
        if result == 0:
            continue
        else:
            os._exit(1)
    else:
        del cr; break
while True:    #保存文件类型
    try:
        cr = ui.filesavebox(msg="导出",title="将输出文件保存为",default="\\"+fn+".json",filetypes=["*.kev","*.json"])
        if cr == None:
            os._exit()
        else:
            kev_file = open(cr,"w",encoding="UTF-8"); file_type = cr.split(".")[-1]
            pickle_file = open(cr.replace(".json",".kev"),"wb")
    except BaseException:
        ui.msgbox(msg="出现未知错误，请重试",title="错误",ok_button="确定");continue
    else:
        del cr; break
ppqn = int(midi_file.ticks_per_beat); events_list = []; events_list.append({'type':'ticks_per_beat', 'value':ppqn, 'time':0, 'is_meta': True}); global_time = 0
for i, track in enumerate(midi_file.tracks):
    for msg in track:
        msg_dict = md.Message.dict(msg); global_time += msg_dict['time']; msg_dict['time'] = global_time
        if msg_dict['type'] == 'end_of_track':
            global_time = 0
        if msg.is_meta:
            is_metal = True
        else:   
            is_metal = False
        msg_dict["is_meta"] = is_metal
        events_list.append(msg_dict)
kev = {'description':{'type':'kev_json_file','name':fn, 'version':format_version},'events':events_list}
pickle.dump(kev,pickle_file)
r = json.dumps(kev,indent=4); kev_file.write(r); kev_file.close()
#<midi file 'C:\\kami\\红石音乐\\midi\\sincerely.mid' type 1, 2 tracks, 1633 messages>