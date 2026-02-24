import os, shutil, json, pickle, easygui as ui, effect as ef, kevfile as kf
print("--------load success--------")
draw=ef.Draw()
sm3=ef.Sm3()
op=ef.Op()
filets=ef.File()
format_version = [3,8,0]
while True:
    try:
        cr = ui.fileopenbox(msg="kev",title="选择json文件",default="/*.json",filetypes=["*.json","*.kev"],multiple=False)
        if cr == None:
            os._exit(1)
        else: 
            fn = os.path.basename(cr).replace("kev","").replace("json","");file_type = os.path.basename(cr).split(".")[-1]
            mainpath = os.path.dirname(cr)
            if mainpath.count("\\"):
                pass
            else:
               mainpath.replace("/","\\")
            mainpath = mainpath + "\\" + fn + "\\"
            if os.path.exists(mainpath):
                shutil.rmtree(mainpath)
            os.mkdir(mainpath)
            os.mkdir(mainpath+"particles")
            os.mkdir(mainpath+"all_functions")
            os.mkdir(mainpath+"meta_functions")
            init = open(mainpath+"meta_functions\\init.mcfunction","w",encoding="UTF-8")
            mother = open(mainpath+"meta_functions\\event.mcfunction","w",encoding="UTF-8")
            
            if file_type == "json":
                events_dict = json.load(open(cr,"r"))
            elif file_type == "kev":
                pickle_file = open(cr,"rb")
                events_dict = pickle.load(pickle_file)
    except BaseException:
            ui.msgbox(msg="出现未知错误，请重试",title="错误",ok_button="确定")
    else:
        break
time_list = []; channel_note=[]; channel_program={}; note_count1=0
for msg in events_dict["events"]:
    if msg["type"] == "ticks_per_beat":
        ppqn = msg["value"]
    elif msg["type"] == "program_change":
        key=msg["channel"]; value=msg["program"]
        channel_program[key]=value
    elif msg["type"] == "note_on":
        if msg["channel"]==9:
            channel_program[9]=-1
        if msg["velocity"] > 0:
            note_count1+=1
            #print(msg["time"],msg["channel"],msg["note"],msg["velocity"])
            time = kf.mt_gt(msg["time"],20,ppqn,100) ; channel = msg["channel"] ; note = msg["note"]
            if filets.tsfunctionfile(mainpath,time) == False:
                mother.write("execute @p[scores={tick=%s}] ~ ~ ~ function tick/f%s\n"%(time,time))
            tickfun=open(mainpath+"\\all_functions\\f%s.mcfunction"%time,"a")
            tickfun.write("execute @p %s %s %s function channel_%s\n"%(time,60.5,note,channel))
            tickfun.write("setblock %s %s %s air\n"%(time,60,note))
            
            
            init.write("setblock %s %s %s light_%s\n"%(time,60,note,channel))
            tickfun.close()
            sort_result = kf.sort2(time,time_list)
            if sort_result[0] == True:
                channel_note.insert(sort_result[1],{})
                try:
                    channel_note[sort_result[1]][channel]
                except KeyError:
                    channel_note[sort_result[1]][channel]=[]
                    channel_note[sort_result[1]][channel].append(note)
                else:
                    channel_note[sort_result[1]][channel].append(note)
            else:
                try:
                    channel_note[sort_result[1]][channel]
                except KeyError:
                    channel_note[sort_result[1]][channel]=[]
                    channel_note[sort_result[1]][channel].append(note)
                else:
                    channel_note[sort_result[1]][channel].append(note)

    #print(time_list[i],channel_note[i])
print("通道对应乐器:"+str(channel_program))

#特效计算部分
#全部统一输入(z,x)
note_count=0
start_point=[64,-30]
mother.write("execute @p[scores={tick=%s}] ~ ~ ~ function tick/f%s"%(start_point[1],start_point[1]))

for channel_chose in range(len(channel_program)):
    temp_time=[]; temp_note=[]
    for time,note_list in zip(time_list,channel_note):
        try:
            note_list[channel_chose]
        except KeyError:
            continue
        else:
            temp_time.append(time)
            temp_note.append(note_list[channel_chose])
    i=-1
    arc_info_temp=[]
    while i <= len(temp_time)-2:
        #if channel_chose==0:
            #print(channel_chose,temp_time[i],str(temp_note[i])+"\n")
        note_count+=len(temp_note[i])
        #print(str(round(note_count/note_count1,2)*100)+"%")
        if i == -1:
            for each_note in temp_note[0]:
                start_center=sm3.findstartcenter(start_point,(each_note,temp_time[0]))[0]
                start_mode=sm3.findstartcenter(start_point,(each_note,temp_time[0]))[1]
                arc_data=sm3.pos2angle(start_point,(each_note,temp_time[0]),(start_center,start_mode))
                if arc_data == None or arc_data[1][0]>30:
                    draw.setline(mainpath,start_point,(each_note,temp_time[0]),404)
                else:
                    draw.setline(mainpath,start_point,(each_note,temp_time[0]),channel_chose,arc_data)
                arc_info_temp.append((start_center,start_mode))    #添加继承数据
                
        else:    #正常连线
            arc_son=arc_info_temp    #继承数据
            arc_info_temp=[]    #临时列表清空
            #print(temp_time[i],arc_son)
            al=temp_note[i];bl=temp_note[i+1]
            at=temp_time[i];bt=temp_time[i+1]
            #print(at,al)
            if len(al)>=len(bl):    #多对少;对等
                bi=0
                while bi<=len(bl)-1:
                    center=sm3.findcenter((al[bi],at),(bl[bi],bt),arc_son[bi])[0]
                    mode=sm3.findcenter((al[bi],at),(bl[bi],bt),arc_son[bi])[1]
                    arc_data=sm3.pos2angle((al[bi],at),(bl[bi],bt),(center,mode))
                    if arc_data==None or arc_data[1][0]>30:
                        draw.setline(mainpath,(al[bi],at),(bl[bi],bt),404)
                    else:
                        draw.setline(mainpath,(al[bi],at),(bl[bi],bt),channel_chose,arc_data)
                    arc_info_temp.append((center,mode))
                    bi+=1
            elif len(al)<len(bl):   #少对多
                ai=0; bi=0; yu=len(bl)%len(al);shang=(len(bl)-yu)/len(al)
                while bi<=len(bl)-1:
                    center=sm3.findcenter((al[ai],at),(bl[bi],bt),arc_son[ai])[0]
                    mode=sm3.findcenter((al[ai],at),(bl[bi],bt),arc_son[ai])[1]
                    arc_data=sm3.pos2angle((al[ai],at),(bl[bi],bt),(center,mode))
                    if arc_data==None or arc_data[1][0]>30:
                        draw.setline(mainpath,(al[ai],at),(bl[bi],bt),404)
                    else:
                        draw.setline(mainpath,(al[ai],at),(bl[bi],bt),channel_chose,arc_data)
                    arc_info_temp.append((center,mode))
                    bi+=1
                    if bi+1%shang==0 and ai<=len(al)-2:
                        ai+=1
        i+=1
print("线条数:"+str(ef.linecount),ef.linecount2)

init.close()
mother.close()

print("--------finished--------")