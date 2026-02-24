import math, os
main_path="C:\\kami\\红石音乐2\\"
color_list=open(main_path+".system\\color.khm","r").read().splitlines()
linecount=0;linecount2=0
class File:
    def tsparticlefile(self,path,x):
        for i in range(99999):
            if os.path.exists(path+"particles\\%s_%s.json"%(x,i)):
                continue
            else:
                return i
    def tsfunctionfile(self,path,x):
        if os.path.exists(path+"all_functions\\f%s.mcfunction"%x)==True:
            return True
        else:
            return False
class Op:    #常规运算 
    def pos2dis(self,pos1,pos2):    #输入 两点:((x1,y1),(x2,y2))----返回 距离:int|float
        x1=pos1[0];y1=pos1[1];x2=pos2[0];y2=pos2[1]
        result=round(((x2-x1)**2+(y2-y1)**2)**0.5,4)
        return result
    def pos2mp(self,pos1,pos2):    #输入 两点:((x1,y1),(x2,y2))----返回 中点:(x,y)
        x1=pos1[0];y1=pos1[1];x2=pos2[0];y2=pos2[1]
        mx=round((x1+x2),4)/2;my=round((y1+y2)/2,4)
        result=(mx,my)
        return result
    def pos2abc(self,pos1,pos2):    #输入 两点:((x1,y1),(x2,y2))----返回 直线方程:(A,B,C)
        x1=pos1[0];y1=pos1[1];x2=pos2[0];y2=pos2[1];A=y1-y2;B=x2-x1;C=-A*x1-B*y1
        result=(A,B,C)
        return result
    def abc2pos(self,abc1,abc2):    #输入 两线:((A1,B1,C1),(A2,B2,C2))----返回 交点:(x,y)|None
        A1=abc1[0];B1=abc1[1];C1=abc1[2];A2=abc2[0];B2=abc2[1];C2=abc2[2]
        if A1==0 and A2!=0:    #--1平行于x,2不平行
            y=round(-C1/B1,4);x=round((-C2-B2*y)/A2,4)
            result=(x,y)
        elif B1==0 and B2!=0:    #--1平行于y,2不px
            x=round(-C1/A1,4);y=round((-C2-A2*x)/B2,4)
            result=(x,y)
        elif A1!=0 and B1!=0:    #--1是斜线
            k1=-A1/B1;b1=-C1/B1
            if B2!=0:
                k2=-A2/B2;b2=-C2/B2
                if k1==k2:    #平行
                    result=None
                else:
                    x=round((b1-b2)/(k2-k1),4);y=round(k1*x+b1,4)
                    result=(x,y)
            else:
                x=round(-C2/A2,4);y=round((-C1-A1*x)/B1,4)
                result=(x,y)
        else:    #两线平行无交点
            result=None
        return result
    def pos2vbabc(self,pos1,pos2):    #输入 两点:((x1,y1),(x2,y2))----返回 中垂线:(A,B,C)
        mx=self.pos2mp(pos1,pos2)[0];my=self.pos2mp(pos1,pos2)[1]
        abc=self.pos2abc(pos1,pos2)
        A=-abc[1];B=abc[0];C=-A*mx-B*my
        result=(A,B,C)
        return result
    def abcpos2vabc(self,abc,pos):    #输入 一线一p点:((A,B,C),(x,y))----返回 过点作线的垂线:(A,B,C)
        x=pos[0];y=pos[1]
        A=-abc[1];B=abc[0];C=-A*x-B*y
        result=(A,B,C)
        return result
    def pos2angle(self,pos1,pos2,vertex):    #输入 两端点和顶点:((x1,y1),(x2,y2),(x0,y0))----返回 角度:int|float
        x1=pos1[0];y1=pos1[1];x2=pos2[0];y2=pos2[1];xv=vertex[0];yv=vertex[1]
        try:
            result=round(math.acos(((x1-xv)*(x2-xv)+(y1-yv)*(y2-yv))/(self.pos2dis(pos1,vertex)*self.pos2dis(pos2,vertex)))*180/math.pi,3)
        except ValueError:
            result=90
        return result
    def angle2arclen(self,angle,r):    #输入 角度,半径:(int|float,int|float)----返回 弧长:int|floatint|float
        result=round(angle*math.pi*r/180,4)
        return result
    def arclen2angle(self,arclen,r):    #输入 弧长,半径:(int|float,int|float)----返回 角度:int|flo
        result=round(180*arclen/(math.pi*r),3)
        return result
    def tsposinabc(self,pos,abc):    #输入 点,线,判断是否在线上,返回bool
        x=pos[0];y=pos[1]
        if abc[1]==0:
            xts=-abc[2]/abc[0]
            if x==xts:
                return True
            else:
                return False
        else:
            yts=(-abc[2]-abc[0]*x)/abc[1]
            if yts==y:
                return True
            else:
                return False
class Sm3:
    def findstartcenter(self,startpoint,pos1):    #输入 开始点和第一点:((x0,y0),(x1,y1))----返回 圆心(伪圆心)和模式(伪模式):((x,z),mode:int)int|float
        startline=(0,-1,startpoint[1])
        vbline=Op().pos2vbabc(startpoint,pos1)
        point=Op().abc2pos(startline,vbline)
        if point==None:
            point=(pos1[1],pos1[0]-1)
            mode=0
        else:
            if startpoint[0]<pos1[0]:
                mode=1
            else:
                mode=-1
        return (point,mode)
    def findcenter(self,pos1,pos2,old):    #输入两点,旧值:((x1,y1),(x2,y2),(oldpos(x,y),oldmode:int))----返回 圆心(伪圆心)和模式(伪模式):((x,y)mode:int)S
        old_center=old[0]
        exline=Op().pos2abc(old[0],pos1)#即将判断的线段
        vbline=Op().pos2vbabc(pos1,pos2)
        center=Op().abc2pos(exline,vbline)
        
        if center==None:    #找不到圆心返回伪圆心
            vline=Op().abcpos2vabc(Op().pos2abc(pos1,pos2),pos2)
            if vline[0]!=0:
                dummy_center=(round((-vline[2]-vline[1]*(pos2[1]-1))/vline[0],4),round(pos2[1]-1,4))
            else:
                dummy_center=(round(pos2[0]-1,4),round((-vline[2]-vline[0]*(pos2[0]-1))/vline[1],4))
            mode=0
            return (dummy_center,mode)
        else:
            if exline[0]==0:#水平线段:
                xts=pos1[0]
                if (old_center[0]<xts and xts<center[0]) or (old_center[0]>xts and xts>center[0]):
                    mode=-old[1]
                else:
                    mode=old[1]
            else:
                yts=pos1[1]
                if (old_center[1]<yts and yts<center[1]) or (old_center[1]>yts and yts>center[1]):
                    mode=-old[1]
                else:
                    mode=old[1]
            return (center,mode)
    
    def pos2angle(self,pos1,pos2,rot):
        r=Op().pos2dis(rot[0],pos1)    #半径
        f=pos2[1]-pos1[1]    #距离
        center=rot[0]; mode=rot[1]
        #以圆心为原点构建直角坐标系
        xc1=pos1[0]-center[0];yc1=pos1[1]-center[1]
        if xc1>0 and yc1>0:#在第一象限
            sag=Op().pos2angle(pos1,(center[0],center[1]+1),center)
        elif xc1<0 and yc1>0:#在第二象限
            sag=360-Op().pos2angle(pos1,(center[0],center[1]+1),center)
        elif xc1<0 and yc1<0:#在第三象限
            sag=180+Op().pos2angle(pos1,(center[0],center[1]-1),center)
        elif xc1>0 and yc1<0:#在第四象限
            sag=180-Op().pos2angle(pos1,(center[0],center[1]-1),center)
        elif xc1>0 and yc1==0:#在x正半轴
            sag=90
        elif xc1<0 and yc1==0:#在x负半轴
            sag=270
        elif xc1==0 and yc1>0:#在y正半轴
            sag=0
        elif xc1==0 and yc1<0:#在y负半轴
            sag=180
        else:
            return None

        xc2=pos2[0]-center[0];yc2=pos2[1]-center[1]
        if xc2>0 and yc2>0:#在第一象限
            eag=Op().pos2angle(pos2,(center[0],center[1]+1),center)
        elif xc2<0 and yc2>0:#在第二象限
            eag=360-Op().pos2angle(pos2,(center[0],center[1]+1),center)
        elif xc2<0 and yc2<0:#在第三象限
            eag=180+Op().pos2angle(pos2,(center[0],center[1]-1),center)
        elif xc2>0 and yc2<0:#在第四象限
            eag=180-Op().pos2angle(pos2,(center[0],center[1]-1),center)
        elif xc2>0 and yc2==0:#在x正半轴
            eag=90
        elif xc2<0 and yc2==0:#在x负半轴
            eag=270
        elif xc2==0 and yc2>0:#在y正半轴
            eag=0
        elif xc2==0 and yc2<0:#在y负半轴
            eag=180
        else:
            return None

        if mode==1:
            if eag>sag:
                rag=eag-sag
            elif eag<sag:
                rag=360-(sag-eag)
            else:
                return None
        elif mode==-1:
            if sag>eag:
                rag=-(sag-eag)
            elif sag<eag:
                rag=-(360-(eag-sag))
            else:
                return None
        else:
            return None
        tps = 10*f;particle_distance = 0.125
        first_num_particles = tps
        num_jsonfile = round(((Op().angle2arclen(abs(rag),r)/(tps-1))/particle_distance)*1.2)
        if num_jsonfile==0:
            num_jsonfile=1
        angle_distance = mode*Op().arclen2angle(particle_distance,r)*0.9
        return ((sag,rag,r),(num_jsonfile,first_num_particles,mode*angle_distance),rot)

    #def arcdraw(self,path,arcdata,filedata,pos):
class Draw:
    def __init__(self):
        lib_path=main_path+"particle_lib\\"
        self.json_ef_1=open(lib_path+"ef_1.json","r").read()    #直线
        self.json_ef_2=open(lib_path+"ef_2.json","r").read()    #抛物线
        self.json_ef_3=open(lib_path+"ef_3.json","r").read()    #立半圆
        self.json_ef_4=open(lib_path+"ef_4.json","r").read()    #v状线
        self.json_ef_5=open(lib_path+"ef_5.json","r").read()    #soma_3
        self.index=0
        self.high=60.5
    def replacevar(self,text):
        replacedict={
            "kami.color":self.color, "kami.xdistance":self.xd, "kami.zdistance":self.zd,
            "kami.xzdistance":self.xzd, "kami.updownmode":self.updownmode, "kami.startangle":self.sag,
            "kami.rotationangle":self.rag, "kami.radius":self.radius, "kami.timepoint":self.timepoint, "kami.notin":self.notin, "kami.soma3notin":self.soma_3_notin}
        for key in replacedict:
            text=text.replace(key,str(replacedict[key]))
        return text
    def timerange(self,time,mintime,maxtime):
        if time>=mintime and time<=maxtime:
            return True
        else:
            return False
    def ef_1(self,pos1,pos2,channel):    #直线
        print("ef_1",channel,pos1,pos2)
        pf=open(self.path+"particles\\%s_%s.json"%(pos1[1],self.notin),"w")
        pf.write(self.replacevar(self.json_ef_1))
        pf.close()
        ff=open(self.path+"all_functions\\f%s.mcfunction"%pos1[1],"a")
        ff.write("particle %s_%s %s %s %s\n"%(pos1[1],self.notin,pos1[1],self.high,pos1[0]))
        print("写入",pos1[1],self.notin)
        ff.close()

    def ef_2(self,pos1,pos2,channel):    #抛物线
        global linecount
        linecount+=1
        pf=open(self.path+"particles\\%s_%s.json"%(pos1[1],self.notin),"w")
        pf.write(self.replacevar(self.json_ef_2))
        pf.close()
        ff=open(self.path+"all_functions\\f%s.mcfunction"%pos1[1],"a")
        ff.write("particle %s_%s %s %s %s\n"%(pos1[1],self.notin,pos1[1],self.high,pos1[0]))
        ff.close()

    def ef_3(self,pos1,pos2,channel):    #立体半圆
        pf=open(self.path+"particles\\%s_%s.json"%(pos1[1],self.notin),"w")
        pf.write(self.replacevar(self.json_ef_3))

    def ef_4(self,pos1,pos2,channel):    #v线
        pf=open(self.path+"particles\\%s_%s.json"%(pos1[1],self.notin),"w")
        pf.write(self.replacevar(self.json_ef_4))

    def ef_5(self,pos1,pos2,channel):    #soma3
        global linecount,linecount2
        ff=open(self.path+"all_functions\\f%s.mcfunction"%pos1[1],"a")
        print("中心:"+str(self.center),"起始点:"+str(pos1),"起转角:"+str(self.sag),"旋转角:"+str(self.rag),"半径:"+str(self.radius))
        linecount2+=self.arc_data[1][0]
        for i in range(self.notin,self.notin+self.arc_data[1][0]):
            linecount+=1
            self.soma_3_notin=i
            if i==self.notin:
                self.par_num=self.arc_data[1][1]
            else:
                self.sag+=self.angle_dis
                self.par_num=self.arc_data[1][1]-1
            pf=open(self.path+"particles\\%s_%s.json"%(pos1[1],i),"w")
            pf.write(self.replacevar(self.json_ef_5))
            pf.close()          
            ff.write("particle %s_%s %s %s %s\n"%(pos1[1],i,self.center[1],self.high,self.center[0]))
        ff.close()

    def setline(self,path,pos1,pos2,channel,arc_data=None):
        if pos1[1]>pos2[1]:
            return False
        global linecount
        self.soma_3_notin=None
        self.index+=1
        self.notin=File().tsparticlefile(path,pos1[1])
        self.timepoint=pos1[1]
        self.zd=pos2[0]-pos1[0]    #音符偏差
        self.xd=pos2[1]-pos1[1]    #时间轴
        self.xzd=Op().pos2dis(pos1,pos2)
        self.path=path
        self.updownmode=(lambda indexint:1 if indexint%2==1 else -1)(self.index)
        if arc_data!=None:
            self.arc_data=arc_data
            self.sag=arc_data[0][0];self.rag=arc_data[0][1];self.radius=arc_data[0][2];self.angle_dis=arc_data[1][2];self.center=arc_data[2][0]
        if channel in [0,1,2,3,4,5]:
            if self.timerange(pos1[1],-100,99999):
                self.color=color_list[channel]    #由于溢出缘故颜色定义被移到了这里
                self.ef_5(pos1,pos2,channel)
        elif channel == 404:
            if self.timerange(pos1[1],-100,99999):
                self.color=color_list[0]
                self.ef_2(pos1,pos2,0)


pos1=(2,0); pos2=(0,2); pos3=(0,0)
abc1=(2,2,2);abc2=(3,5,7)
Sm3().pos2angle(pos1,pos2,(pos3,1))
print(Sm3().findcenter((6,4),(10,8),((3,3),1)))
print("--------import_effect_suc--------")
