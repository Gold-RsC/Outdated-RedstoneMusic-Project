def msg_type(msg):
    return msg.split("{")[0].replace(":","")

def msg_value(msg,key):
    msgl = msg.split("{")[1].replace("}","").split(",")
    ld = {}
    for kv in msgl:
        try:
            ke = kv.split(":")[0]
            val = kv.split(":")[1]
        except BaseException:
            pass
        else:
            ld[ke] = val
    try:
        ld[key]
    except KeyError:
        return KeyError
    else:
        if ld[key] == "true":
            return True
        elif ld[key] == "false":
            return False
        else:
            try:
                int(ld[key])
            except BaseException:
                return ld[key]
            else:
                return int(ld[key])

def msg_edit(msg,key,newvalue):
    newvalue = str(newvalue)
    value = str(msg_value(msg,key))
    msg = msg.replace("%s:%s"%(key,value),"%s:%s"%(key,newvalue))
    return msg

def tn(dict):
    for i in dict:
        time = i
        note_list = dict[i]
    return (time,note_list)

def start(st,nl,c):
    pass

def mt_gt(mt,speed=20,ppqn=128,bpm=100):
    gt = round((speed*60*mt) / (bpm*ppqn))
    return gt
#def channel_split(events_dict)

def sort2(obj,list1):    #判断一个值在列表中的位置,没有则按顺序插入元素返回True,有则返回False和索引值
    list1.sort()
    if len(list1)==0:
        list1.append(obj)
        return (True,0)
    elif obj in list1:
        return (False,list1.index(obj))
    else:
        for i in range(len(list1)):
            if i==0 and obj<list1[i]:
                list1.insert(0,obj)
                return (True,0)
            elif list1[i-1]<obj and obj<list1[i]:
                list1.insert(i,obj)
                return (True,i)
            elif i==len(list1)-1 and obj>list1[-1]:
                list1.append(obj)
                return (True,len(list1)-1)
            else:
                continue
