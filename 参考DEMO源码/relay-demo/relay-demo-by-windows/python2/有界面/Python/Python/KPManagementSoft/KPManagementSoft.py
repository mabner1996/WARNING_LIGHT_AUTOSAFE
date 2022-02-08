#encoding=utf-8
__author__ = 'freedom'
#import Tkinter.font as tf 
from Tkinter import *
import serial
import ttk
import socket
from socket import *
import threading
import time
import sys
from datetime import datetime
import ConfigParser
import os



clientsockrev=socket() #=socket()#socket.AF_UNIX, socket.SOCK_STREAM
a = [0 for x in range(0, 1000)]
DOStatus = [0 for x in range(0, 16)]
DIStatus = [0 for x in range(0, 16)]
timer = [100000 for x in range(0, 16)]
connectNum=0
ServerIP="127.0.0.1"
portNum=6000
dictlbD0={}
dictlbDI={}
dictlaDlyDO={}
dictlabelDO={}
dictTbTimerDO={}
dictlaDO={}
dictlbWait={}
dictTxtWaitTime={}
dictlbSec={}
dictCbmode={}
index=0
def tcpListen(address,port):
    
    #监听自己的哪个端口
    s = socket(AF_INET,SOCK_STREAM)
    try: 
       s.bind((address,port))
       s.listen(100)     #最大连接数
    except: 
       global lbPort
       lbPort.config(fg="red")
       return
    while True:
        clientsock,clientaddress=s.accept()
        global connectNum
        
        connectNum=connectNum+1
        print('connect from:',clientaddress)
        #senddata='AT+STACH0=0\r\n'
        #clientsock.send(senddata.encode())
        
        #clientsock.send(senddata.encode())
        #传输数据都利用clientsock，和s无关
        t=threading.Thread(target=tcplink,args=(clientsock,clientaddress))  #t为新创建的线程
        t.start()
        tReadDO=threading.Thread(target=tcpReadDO,args=(clientsock,clientaddress))  #t为新创建的线程
        tReadDO.start()
        tReadDI=threading.Thread(target=tcpReadDI,args=(clientsock,clientaddress))  #t为新创建的线程
        tReadDI.start()
    s.close()    

def tcplink(sock,addr):
    global clientsockrev
    clientsockrev=sock

    while True:
        buffsize=1024          #接收从客户端发来的数据的缓存区大小
        global DIStatus 
        global connectNum
        try:
            recvdata=sock.recv(buffsize).decode('utf-8')
        except:
            ClearData()
        if recvdata=='exit' or not recvdata:
            ClearData()
            break 
        if len(recvdata)==0:
            ClearData()
        if recvdata.find('STACH')>0 or recvdata.find('OCCH')>0:
                       i= 0
                       count = 0
                       indexlast=recvdata.rindex('+')
                  
                       #and i!=indexlast+1
                       while  (recvdata.find('+',i))>=0:
                           i =recvdata.find("+", i) + 1
                           count=count+1
                       #if index>0
                       #print(count)
                       for num in range(0,count):
                        if recvdata.find("+STACH", 0)==0:
            
                            ch =recvdata[recvdata.find("STACH") + 5:recvdata.find(":")]
                      
                            if(recvdata.find(",") - recvdata.find(":") == 2):
                                indexfen=recvdata.find(":")+1 #分号的位置
                                intdexdou=recvdata.find(",") 
                                #print(recvdata)
                                intdexR=recvdata.find("\r")
                                
                                stastr = recvdata[indexfen:intdexdou]#继电器状态
                                timerstr =recvdata[intdexdou+1:intdexR]
                                #print(timerstr)
                                global timer
                                timer[int(ch)-1]=int(timerstr)
                            else:
                                stastr = recvdata[indexfen:intdexdou]#继电器状态
                            global DOStatus
                            DOStatus[int(ch) - 1] = int(stastr);
                        elif recvdata.find("+OCCH_ALL")==0:
                            print recvdata
                            ch =[0 for x in range(0, 16)]
                            indexALL=recvdata.find("+OCCH_ALL:")+10
                            indexN=recvdata.find("\n",0)+1
                            atStr = recvdata[indexALL:indexN].strip()
                            ch_index = 0
                            while True:
                                strch=atStr[0:1]
                                ch_index=ch_index+1
                                ch[ch_index] = int(strch)
                                #print strch
                                DIStatus[ch_index - 1] = int(strch)
                                lenInt=len(atStr.decode("utf-8"))
                                if lenInt>3:
                                    intdexDou=atStr.find(",") + 1
                                    #print intdexDou
                                    #print atStr
                                    atStr = atStr[intdexDou:]#
                                    #print("123456")
                                    #print(atStr)
                                else:
                                    break
                                if len(recvdata.decode("utf-8"))>3 and  ch_index<16:
                                    
                                    break 
                        elif recvdata.find("+OCCH")==0:
                            
                            #print recvdata
                            indexoch=recvdata.find("OCCH")+4
                            indexfen=recvdata.find(":")+1
                            intdexR=recvdata.find("\r")
                            ch=recvdata[indexoch:indexfen-1]
                            stastr=recvdata[indexfen:intdexR]
                            #print(stastr)     
                            if num<count-1:
                                index_n=recvdata.find("\n", 0);
                                recvdata=recvdata[index_n:]
                            
                            DIStatus[int(ch)-1] = int(stastr)
        
        #global root
                       #print(timer)     
        #reflashStatus(root)
def tcpReadDO(sock,addr):
    while True:
        senddata='AT+STACH0=?\r\n'
        if sock!=None: #and sock.isConnected!=false
            try:
               sock.send(senddata.encode())
            except:
                break
                print("AAA")
        else:
            break        
        time.sleep(0.3)
 
def tcpReadDI(sock,addr):
    
    while True:
        senddata='AT+OCCH0=?\r\n'
        if sock!=None:#and sock.isConnected!=false
            try:
                sock.send(senddata.encode())
            except:
                break 
                print("bbbb") 
        else:
            break          
        time.sleep(0.3)

def RelayControl(ch,action,strTimer,waitTimer):
    strCmd = "AT+STACH"#声明开头
    strCmd=strCmd+str(ch)
    strOper = "="
    strCmd=strCmd+strOper
    strCmd=strCmd+str(action)
    strsignNew=","
    strSign=","
    if strTimer!="":
        strCmd+=strSign
        strCmd+=strTimer
    if waitTimer!="":
        strCmd=strCmd.strip()+strsignNew
        strCmd+=waitTimer
    strCmd+="\r\n"
    clientsockrev.send(strCmd.encode())

def get_host_ip():
    sip = socket(AF_INET, SOCK_DGRAM)
    try:
        sip.connect(('8.8.8.8', 80))
        ip = sip.getsockname()[0]
        #hostname = socket.gethostname()
        #ip = socket.gethostbyname(hostname)
    finally:
        sip.close()
        #ip="127.0.0.1"
    return ip
def gettime():
    # 获取当前时间并转为字符串
    timestr = time.strftime("%Y-%m-%d %H:%M:%S")
    dict = {0: "星期一", 1: "星期二", 2: "星期三", 3: "星期四", 4: "星期五", 5: "星期六", 6: "星期天"}
    strWeek=datetime.now().weekday() 
    
    week=dict.get(strWeek)
    current_time=timestr+week
    # 重新设置标签文本
    lb.configure(text=current_time)
    connectNumStr="当前连接数:"+str(connectNum)
    lbConectNum.configure(text=connectNumStr)
    # 每隔一秒调用函数gettime自身获取时间
    root.after(1000, gettime)
        
def handlerAdaptor(fun, **kwds):
    #'''事件处理函数的适配器，相当于中介，那个event是从那里来的呢，我也纳闷，这也许就是python的伟大之处吧'''
    return lambda event,fun=fun,kwds=kwds: fun(event, **kwds)

def Submit(cbname):
    context =dictCbmode[cbname].get()
    strTimer=""
    waitTimer=""
    index=-1
    if(context=="自锁关"):
        index=0
    elif(context=="自锁开"):
        index=1
    elif(context=="点动关"):
        index=2
    elif(context=="点动开"):
        index=3
    elif(context=="互锁延时"):
        index=4
    elif(context=="互锁点动"):                
        index=5
    elif(context=="循环"):
        index=6                
    elif(context=="翻转"):
        index=7 
    strTimer=dictTbTimerDO[cbname].get('0.0', END).lstrip()
    waitTimer=dictTxtWaitTime[cbname].get('0.0',END).lstrip()
    RelayControl(cbname,index,strTimer,waitTimer)
def Choice(event,indexTest):
    if event==None:
        context='自锁关' 
       #dictCbmode[indexTest].curren()0
    else:
        context=dictCbmode[indexTest].get()
    

    list = ['自锁关','自锁开','点动关','点动开','互锁延时','互锁点动','循环','翻转']
    if context in list:
        selectIndex = list.index(context)
    if selectIndex==0 or selectIndex==1 or selectIndex==7:
        dictlabelDO[indexTest].grid_remove()  #隐藏控件
        dictlbWait[indexTest].grid_remove()  #隐藏控件
        dictlaDO[indexTest].grid_remove() #隐藏控件
        
        dictTbTimerDO[indexTest].delete(0.0, END)
        dictTbTimerDO[indexTest].insert(INSERT,'')
        dictlbSec[indexTest].grid_remove() #隐藏控件   
                       
        dictTbTimerDO[indexTest].grid_remove()
        dictTxtWaitTime[indexTest].delete(0.0, END)
        dictTbTimerDO[indexTest].insert(INSERT,'')
        dictTxtWaitTime[indexTest].grid_remove()  #隐藏控件
        
    elif selectIndex==2 or selectIndex==3:
         
         dictlbWait[indexTest].grid_remove() #隐藏控件
         dictTxtWaitTime[indexTest].grid_remove()  #隐藏控件
         dictTxtWaitTime[indexTest].delete(0.0, END)
         dictTxtWaitTime[indexTest].insert(INSERT,'')
         dictlbSec[indexTest].grid_remove() #隐藏控件
        
         dictlabelDO[indexTest].grid() #隐藏控件
        
         
         dictTbTimerDO[indexTest].grid()
         dictTbTimerDO[indexTest].delete(0.0, END)
         dictTbTimerDO[indexTest].insert(INSERT,'99999')
        
         
         dictlaDO[indexTest].grid()  #隐藏控件
         
    elif selectIndex==4:#互锁延时
         dictlbWait[indexTest].grid() #隐藏控件
         dictTxtWaitTime[indexTest].grid()  #隐藏控件
         dictTxtWaitTime[indexTest].delete(0.0, END)
        
         dictTxtWaitTime[indexTest].insert(INSERT,'99999')
         dictlbSec[indexTest].grid() #隐藏控件
         
         dictlabelDO[indexTest].grid_remove() #隐藏控件
        
         
         dictTbTimerDO[indexTest].grid_remove()
         dictTbTimerDO[indexTest].delete(0.0, END)
         dictTbTimerDO[indexTest].insert(INSERT,'')
        
         dictlaDO[indexTest].grid_remove()  #隐藏控件
         
    elif selectIndex==5 or selectIndex==6:#循环或者互锁点动
         
         dictlabelDO[indexTest].grid()  
         dictlbWait[indexTest].grid()  
         dictlaDO[indexTest].grid() 
         dictlbSec[indexTest].grid()  
         dictTbTimerDO[indexTest].grid() 
         dictTbTimerDO[indexTest].delete(0.0, END)
         dictTbTimerDO[indexTest].insert(INSERT,'99999')
         
         dictTxtWaitTime[indexTest].grid()
         dictTxtWaitTime[indexTest].delete(0.0, END)
         dictTxtWaitTime[indexTest].insert(INSERT,'99999')
        

def addPanelTitle(master):
    frame = Frame(master)
    #current_time = self.get_current_time()#获取当前的系统时间
    lbOut = Label(frame,text = '输出(OUT)',font=("宋体", 20, "bold"))
    lbOut.grid(row =0,column =0,sticky =W+E+N+S,padx=260,pady=0,columnspan=3)
    
    #lbOut.place(x=30,y=10,anchor='nw')
    #lbInput = Label(frame,text='输入(INPUT)',fg="black")
    #lbInput.grid(row =0,column=4,sticky =W,padx=10)
    
    #lbOut.pack(side=LEFT)
    lbInput = Label(frame,text='输入(INPUT)',font=("宋体", 20, "bold"))
    lbInput.grid(row =0,column=3,sticky =W+E+N+S,padx=10,pady=0,columnspan=2)
    #lbInput.pack(side=RIGHT)
    #lbInput.place(x=100,y=10,anchor='nw')
    return frame
    
def addPanelBottom(master):
    frame = Frame(master)
    lbMode = Label(frame,text = '模式：TCP_Server',fg="black")
    lbMode.grid(row =0,column =0,sticky =W+E+N+S,padx=5)
    lbServerIPTest = Label(frame,text = '服务端IP：',fg="black")
    lbServerIPTest.grid(row =0,column =3,sticky = W+E+N+S,columnspan=2,padx=40)
    lbPortTest = Label(frame,text = '端口号：',fg="black")
    lbPortTest.grid(row =0,column =5,sticky = W+E+N+S,columnspan=2,padx=40)
    global lbServerIP
    lbServerIP=lbServerIPTest
    
    global lbPort
    lbPort=lbPortTest
    lbConectNumTest= Label(frame,text = '当前连接数',fg="black")
    lbConectNumTest.grid(row =0,column =7,sticky = W+E+N+S,columnspan=2,padx=40)
    global lbConectNum
    lbConectNum=lbConectNumTest
    lbTime = Label(frame,text = '时间',fg="black")
    lbTime.grid(row =0,column =9,sticky = W+E+N+S,columnspan=2,padx=40)
    global lb
    lb=lbTime
    return frame



class Application(ttk.Frame):
    def __init__(self, **kwargs):
        self.root = Tk()
        #Init master frame
        Tk.Frame.__init__(self,self.root)
        self.grid()
        self.frame1=Add_Frame(root)#,1
        self.frame1.grid(row=1,column=0)
        self.frame2=Add_Frame(root)
        self.frame2.grid(row=1, column=1)
        self.frame3=Add_Frame(root)
        self.frame3.grid(row=1, column=2)
        self.frame4=Add_Frame(root)
        self.frame4.grid(row=1, column=3)
        self.frame5=Add_Frame(root)
        self.frame5.grid(row=2, column=0)
        self.frame6=Add_Frame(root)
        self.frame6.grid(row=2, column=1)
        self.frame7=Add_Frame(root)
        self.frame7.grid(row=2, column=2)
        self.frame8=Add_Frame(root)
        self.frame8.grid(row=2, column=3)
        self.frame9=Addpanel(root)
        self.frame9.grid(row=3, column=0)
        self.frame10=Add_Frame(root)
        self.frame10.grid(row=3, column=1)
        self.frame11=Add_Frame(root)
        self.frame11.grid(row=3, column=2)
        self.frame12=Add_Frame(root)
        self.frame12.grid(row=3, column=3)
        self.frame13=Add_Frame(root)
        self.frame13.grid(row=4, column=0)
        self.frame14=Add_Frame(root)
        self.frame14.grid(row=4,column=1)
        self.frame15=Add_Frame(root)
        self.frame15.grid(row=4,column=2)
        self.frame16=Add_Frame(root)
        self.frame16.grid(row=4,column=3)
    #frame17=AddDIpanel(root)
    #frame17.grid(row=1, column=4,rowspan=4)
    #frame18=addPanelBottom(root)
     #frame18.grid(row=5, column=0,columnspan=5)
    
class Add_DIFrame(ttk.Frame):
    def __init__(self,parent, **kwargs):
        
        self.lbDI1 = SetDISize(frame) 
        self.lbDI1.grid(row =0,column = 0,sticky = W)
        global  dictlbDI
        dictlbDI[1]=self.lbDI1 
        self.lbDI2 = SetDISize(frame) 
        self.lbDI2.grid(row =0,column = 1,sticky = W) 
        dictlbDI[2]=self.lbDI2    
        self.lbDI3 = SetDISize(frame) 
        self.lbDI3.grid(row =0,column = 2,sticky = W)   
        dictlbDI[3]=self.lbDI3   
        self.lbDI4 =SetDISize(frame) 
        self.lbDI4.grid(row =0,column = 3,sticky = W)   
        dictlbDI[4]=self.lbDI4  
        self.lbNum1 =SetDINumSize(frame,"1")
        self.lbNum1.grid(row =1,column = 0,sticky = W,padx=10)   
        self.lbNum2 = SetDINumSize(frame,"2")
        self.lbNum2.grid(row =1,column = 1,sticky = W,padx=10)   
        self.lbNum3 = SetDINumSize(frame,"3")
        self.lbNum3.grid(row =1,column = 2,sticky = W,padx=10)
           
        self.lbNum4 =SetDINumSize(frame,"4")
        self.lbNum4.grid(row =1,column = 3,sticky = W,padx=10)
            
        self.lbDI5 =SetDISize(frame)          #Label(frame,text ='●',fg="green" )
        self.lbDI5.grid(row =2,column = 0,sticky = W)
        self.dictlbDI[5]=self.lbDI5
            
        self.lbDI6 =SetDISize(frame)
        self.lbDI6.grid(row =2,column = 1,sticky = W)
        dictlbDI[6]=self.lbDI6
            
        self.lbDI7 = SetDISize(frame)
        self.lbDI7.grid(row =2,column = 2,sticky = W)
        dictlbDI[7]=self.lbDI7
            
        self.lbDI8 =SetDISize(frame)
        self.lbDI8.grid(row =2,column = 3,sticky = W)
        dictlbDI[8]=self.lbDI8
        self.lbNum5 = SetDINumSize(frame,"5")
        self.lbNum5.grid(row =3,column = 0,sticky = W,padx=10) 
             
        self.lbNum6 =SetDINumSize(frame,"6")
        self.lbNum6.grid(row =3,column = 1,sticky = W,padx=10)
                
        self.lbNum7 = SetDINumSize(frame,"7")
        self.lbNum7.grid(row =3,column = 2,sticky = W,padx=10)
                
        self.lbNum8 =SetDINumSize(frame,"8")
        self.lbNum8.grid(row =3,column = 3,sticky = W,padx=10)
            
            
        self.lbDI9 = SetDISize(frame)
        self.lbDI9.grid(row =4,column = 0,sticky = W)
        dictlbDI[9]=self.lbDI9
        self.lbDI10 = SetDISize(frame)
        self.lbDI10.grid(row =4,column = 1,sticky = W)
        dictlbDI[10]=self.lbDI10
        self.lbDI11 =SetDISize(frame)
        self.lbDI11.grid(row =4,column =2,sticky = W)
        dictlbDI[11]=self.lbDI11
        self.lbDI12 =SetDISize(frame)
        self.lbDI12.grid(row =4,column = 3,sticky = W)
        self.dictlbDI[12]=self.lbDI12
        self.lbDI13 =SetDISize(frame)
        self.lbDI13.grid(row =6,column = 0,sticky = W)
        dictlbDI[13]=self.lbDI13
        self.lbDI14 = SetDISize(frame)
        self.lbDI14.grid(row =6,column = 1,sticky = W)
        self.dictlbDI[14]=self.lbDI14
        self.lbDI15 =SetDISize(frame)
        self.lbDI15.grid(row =6,column = 2,sticky = W)
        dictlbDI[15]=self.lbDI15
        self.lbDI16 = SetDISize(frame)
        self.lbDI16.grid(row =6,column = 3,sticky = W)
        dictlbDI[16]=self.lbDI16
        self.lbNum9 =SetDINumSize(frame,"9")
        self.lbNum9.grid(row =5,column = 0,sticky = W,padx=10)
            
        self.lbNum10 = SetDINumSize(frame,"10")
        self.lbNum10.grid(row =5,column = 1,sticky = W)
            
        self.lbNum11 =SetDINumSize(frame,"11")
        self.lbNum11.grid(row =5,column = 2,sticky = W)
            
        self.lbNum12 = SetDINumSize(frame,"12")
        self.lbNum12.grid(row =5,column = 3,sticky = W)
            
        self.lbNum13 =SetDINumSize(frame,"13")
        self.lbNum13.grid(row =7,column = 0,sticky = W)
            
        self.lbNum14 =SetDINumSize(frame,"14")
        self.lbNum14.grid(row =7,column = 1,sticky = W)
            
        self.lbNum15 =SetDINumSize(frame,"15")
        self.lbNum15.grid(row =7,column = 2,sticky = W)
            
        self.lbNum16 =SetDINumSize(frame,"16")
        self.lbNum16.grid(row =7,column = 3,sticky = W)
        
        



    
   
class Add_Frame(ttk.Frame):#index==0
    def __init__(self,parent, **kwargs):
        global index
        index=index+1
        Tk.Frame.__init__(self,parent)
        self.lbNum1 = Label(frame,text =index,fg="black",font=("宋体", 10, "bold") )
        self.lbNum1.grid(row =0,column = 0,sticky = W)
        self.lbD01 = Label(frame,text ='●',fg="gray",font=("宋体", 20, "bold") )
        self.lbD01.grid(row =0,column = 1,sticky = W)
        global dictlbD0
        dictlbD0[index]=lbD01
        boxValue = StringVar()
        self.cbMode1 = ttk.Combobox(frame,textvariable =boxValue,state = 'readonly',width=10)
        comboxData=['自锁关','自锁开','点动关','点动开','互锁延时','互锁点动','循环','翻转']
        self.cbMode1['value'] =comboxData 
        self.cbMode1.set(comboxData[0])
        self.cbMode1.bind('<<ComboboxSelected>>',handlerAdaptor(Choice,indexTest=index))
        self.cbMode1.set(comboxData[1])
        context=self.cbMode1.get()
        #Choice(0,index)
        self.cbMode1.grid(row = 0,column =2,sticky = W+E+N+S,columnspan=2,pady=5)
        global dictCbmode
        dictCbmode[index]=self.cbMode1
        
        self.laDlyDO1 = Label(frame,text = '')#倒计时
        self.laDlyDO1.grid(row = 1,column = 1,sticky = W)
        global dictlaDlyDO
        dictlaDlyDO[index]=self.laDlyDO1
        
        self.btnExcute1 = Button(frame,text = "执行",width=8,height=1,command =lambda:Submit(index))# 
        self.btnExcute1.grid(row = 1,column = 2,sticky = W,pady=2)  
                #延时
        self.labelDO1 = Label(frame,text = '延时')
        self.labelDO1.grid(row = 2,column = 1,sticky =W)#等待
        global dictlabelDO
        dictlabelDO[index]=self.labelDO1
        self.tbTimerDO1 = Text(frame,width=8,height=1)
        self.tbTimerDO1.grid(row = 2,column = 2,sticky = W)
        self.tbTimerDO1.insert(INSERT,'99999')
        global dictTbTimerDO
        dictTbTimerDO[index]=self.tbTimerDO1

        global dictlaDO
        self.laDO1 = Label(frame,text = '秒')
        self.laDO1.grid(row = 2,column = 3,sticky = W)
        dictlaDO[index]=self.laDO1
                
        self.lbWait1 = Label(frame,text = '等待')
        self.lbWait1.grid(row = 3,column = 1,sticky = W)
        global dictlbWait
        dictlbWait[index]=self.lbWait1
        self.txtWaitTime1 = Text(frame,width=8,height=1)
        self.txtWaitTime1.grid(row = 3,column = 2,sticky = W)
        self.txtWaitTime1.insert(INSERT,'99999')
        global dictTxtWaitTime
        dictTxtWaitTime[index]=txtWaitTime1    
                
        self.lbSec1 = Label(frame,text = '秒')
        self.lbSec1.grid(row = 3,column = 3,sticky = W)
        global dictlbSec
        dictlbSec[index]=self.lbSec1       
        Choice(None,index)
         
def  Addpanel(master,index):#,parent=None,**kwargs
        frame = Frame(master)
 
        lbNum1 = Label(frame,text =index,fg="black",font=("宋体", 10, "bold") )
        lbNum1.grid(row =0,column = 0,sticky = W)

        lbD01 = Label(frame,text ='●',fg="gray",font=("宋体", 20, "bold") )
        lbD01.grid(row =0,column = 1,sticky = W)
        global dictlbD0
        dictlbD0[index]=lbD01
        #串口号选择下拉菜单
        boxValue = StringVar()
        cbMode1 = ttk.Combobox(frame,textvariable =boxValue,state = 'readonly',width=10)
        comboxData=['自锁关','自锁开','点动关','点动开','互锁延时','互锁点动','循环','翻转']
        cbMode1['value'] =comboxData 
        cbMode1.set(comboxData[0])
        cbMode1.bind('<<ComboboxSelected>>',handlerAdaptor(Choice,indexTest=index))
        cbMode1.set(comboxData[1])
        context=cbMode1.get()
        #Choice(0,index)
        cbMode1.grid(row = 0,column =2,sticky = W+E+N+S,columnspan=2,pady=5)
        global dictCbmode
        dictCbmode[index]=cbMode1
        
        
        laDlyDO1 = Label(frame,text = '')#倒计时
        laDlyDO1.grid(row = 1,column = 1,sticky = W)
        global dictlaDlyDO
        dictlaDlyDO[index]=laDlyDO1
        
    
        #等待 
        btnExcute1 = Button(frame,text = "执行",width=8,height=1,command =lambda:Submit(index))# 
        btnExcute1.grid(row = 1,column = 2,sticky = W,pady=2)  
        #延时
        labelDO1 = Label(frame,text = '延时')
        labelDO1.grid(row = 2,column = 1,sticky =W)#等待
        global dictlabelDO
        dictlabelDO[index]=labelDO1
       
       
        
        tbTimerDO1 = Text(frame,width=8,height=1)
        tbTimerDO1.grid(row = 2,column = 2,sticky = W)
        tbTimerDO1.insert(INSERT,'99999')
        global dictTbTimerDO
        dictTbTimerDO[index]=tbTimerDO1
        
        global dictlaDO
        laDO1 = Label(frame,text = '秒')
        laDO1.grid(row = 2,column = 3,sticky = W)
        dictlaDO[index]=laDO1
        
        lbWait1 = Label(frame,text = '等待')
        lbWait1.grid(row = 3,column = 1,sticky = W)
        global dictlbWait
        dictlbWait[index]=lbWait1
               
        txtWaitTime1 = Text(frame,width=8,height=1)
        txtWaitTime1.grid(row = 3,column = 2,sticky = W)
        txtWaitTime1.insert(INSERT,'99999')
        global dictTxtWaitTime
        dictTxtWaitTime[index]=txtWaitTime1    
        
        lbSec1 = Label(frame,text = '秒')
        lbSec1.grid(row = 3,column = 3,sticky = W)
        global dictlbSec
        dictlbSec[index]=lbSec1
        
        Choice(None,index)
        
        return frame


def SetDISize(master):
    lbReturn = Label(master,text ='●',fg="Gray",justify = "left",font=("宋体", 30, "bold")) 
    return lbReturn

def SetDINumSize(master,textNum):
    lbReturn = Label(master,text =textNum,fg="black",justify = "center",font=("宋体", 30, "bold"),pady=10) 
    return lbReturn

def AddDIpanel(master):#,parent=None,**kwargs
    frame = Frame(master)      
    lbDI1 = SetDISize(frame) 
    lbDI1.grid(row =0,column = 0,sticky = W)
    global  dictlbDI
    dictlbDI[1]=lbDI1
    
    lbDI2 = SetDISize(frame) 
    lbDI2.grid(row =0,column = 1,sticky = W)
    
    dictlbDI[2]=lbDI2
    
    lbDI3 = SetDISize(frame) 
    lbDI3.grid(row =0,column = 2,sticky = W)
    
    dictlbDI[3]=lbDI3
    
    lbDI4 =SetDISize(frame) 
    lbDI4.grid(row =0,column = 3,sticky = W)
    
    dictlbDI[4]=lbDI4
    
    lbNum1 =SetDINumSize(frame,"1")
    lbNum1.grid(row =1,column = 0,sticky = W,padx=10)
    
    lbNum2 = SetDINumSize(frame,"2")
    lbNum2.grid(row =1,column = 1,sticky = W,padx=10)
    
    lbNum3 = SetDINumSize(frame,"3")
    lbNum3.grid(row =1,column = 2,sticky = W,padx=10)
    
    lbNum4 =SetDINumSize(frame,"4")
    lbNum4.grid(row =1,column = 3,sticky = W,padx=10)
    
    lbDI5 =SetDISize(frame)          #Label(frame,text ='●',fg="green" )
    lbDI5.grid(row =2,column = 0,sticky = W)
    dictlbDI[5]=lbDI5
    
    lbDI6 =SetDISize(frame)
    lbDI6.grid(row =2,column = 1,sticky = W)
    dictlbDI[6]=lbDI6
    
    lbDI7 = SetDISize(frame)
    lbDI7.grid(row =2,column = 2,sticky = W)
    dictlbDI[7]=lbDI7
    
    lbDI8 =SetDISize(frame)
    lbDI8.grid(row =2,column = 3,sticky = W)
    dictlbDI[8]=lbDI8
    lbNum5 = SetDINumSize(frame,"5")
    lbNum5.grid(row =3,column = 0,sticky = W,padx=10) 
     
    lbNum6 =SetDINumSize(frame,"6")
    lbNum6.grid(row =3,column = 1,sticky = W,padx=10)
        
    lbNum7 = SetDINumSize(frame,"7")
    lbNum7.grid(row =3,column = 2,sticky = W,padx=10)
        
    lbNum8 =SetDINumSize(frame,"8")
    lbNum8.grid(row =3,column = 3,sticky = W,padx=10)
    
    
    lbDI9 = SetDISize(frame)
    lbDI9.grid(row =4,column = 0,sticky = W)
    dictlbDI[9]=lbDI9
    lbDI10 = SetDISize(frame)
    lbDI10.grid(row =4,column = 1,sticky = W)
    dictlbDI[10]=lbDI10
    lbDI11 =SetDISize(frame)
    lbDI11.grid(row =4,column =2,sticky = W)
    dictlbDI[11]=lbDI11
    lbDI12 =SetDISize(frame)
    lbDI12.grid(row =4,column = 3,sticky = W)
    dictlbDI[12]=lbDI12
    lbDI13 =SetDISize(frame)
    lbDI13.grid(row =6,column = 0,sticky = W)
    dictlbDI[13]=lbDI13
    lbDI14 = SetDISize(frame)
    lbDI14.grid(row =6,column = 1,sticky = W)
    dictlbDI[14]=lbDI14
    lbDI15 =SetDISize(frame)
    lbDI15.grid(row =6,column = 2,sticky = W)
    dictlbDI[15]=lbDI15
    lbDI16 = SetDISize(frame)
    lbDI16.grid(row =6,column = 3,sticky = W)
    dictlbDI[16]=lbDI16
    lbNum9 =SetDINumSize(frame,"9")
    lbNum9.grid(row =5,column = 0,sticky = W,padx=10)
    
    lbNum10 = SetDINumSize(frame,"10")
    lbNum10.grid(row =5,column = 1,sticky = W)
    
    lbNum11 =SetDINumSize(frame,"11")
    lbNum11.grid(row =5,column = 2,sticky = W)
    
    lbNum12 = SetDINumSize(frame,"12")
    lbNum12.grid(row =5,column = 3,sticky = W)
    
    lbNum13 =SetDINumSize(frame,"13")
    lbNum13.grid(row =7,column = 0,sticky = W)
    
    lbNum14 =SetDINumSize(frame,"14")
    lbNum14.grid(row =7,column = 1,sticky = W)
    
    lbNum15 =SetDINumSize(frame,"15")
    lbNum15.grid(row =7,column = 2,sticky = W)
    
    lbNum16 =SetDINumSize(frame,"16")
    lbNum16.grid(row =7,column = 3,sticky = W)
    return frame

def refresh_dataNew():

    for num in range(0,16):
        global DOStatus
        global DIStatus
        global timer
            
        if DOStatus[num]==0:
            dictlbD0[num+1].config(fg="gray") 
        if DOStatus[num]==1:
            dictlbD0[num+1].config(fg="green")    
        if DIStatus[num]==1:
            dictlbDI[num+1].config(fg="green")
        if DIStatus[num]==0:
            dictlbDI[num+1].config(fg="gray")
        if timer[num]==100000:
            dictlaDlyDO[num+1].configure(text ='')#grid_remove()#
        else:
            dictlaDlyDO[num+1].configure(text =timer[num])       
        
    root.after(1000, refresh_dataNew)
    
 
def ClearData():
    global DOStatus
    global DIStatus
    global timer
    global clientsockrev
    global connectNum
    for num in range(0,16):
        DOStatus[num]=0
        DIStatus[num]=0
        timer[num]=100000
    if connectNum>0:
        connectNum=connectNum-1
    global t
    t.clear()
    global t2
    t2.clear()   
    clientsockrev.close()    
    


def setAddreAndPort(address,port):
    lbServerIP.configure(text="服务端IP:"+address)
    lbPort.configure(text="端口号:"+str(port))
def callbackClose():
    os._exit(0)
    global clientsockrev
    clientsockrev.close()
    clientsockrev=None
    global root
    root.destroy()
    sys.exit(0)
    
reload(sys)
sys.setdefaultencoding('utf8 ')#
root = Tk()
root.title("网络继电器管理软件 V1.0.7")
root.resizable(0,0) #防止用户调整尺寸

lb=Label(root, text="")

lbConectNum=Label(root, text="")
lbServerIP=Label(root, text="")
lbPort=Label(root, text="")
frme0=addPanelTitle(root)
frme0.grid(row=0,column=0,columnspan=5)

frame1=Addpanel(root,1)
frame1.grid(row=1,column=0)
frame2=Addpanel(root,2)
frame2.grid(row=1, column=1)
frame3=Addpanel(root,3)
frame3.grid(row=1, column=2)
frame4=Addpanel(root,4)
frame4.grid(row=1, column=3)
frame5=Addpanel(root,5)
frame5.grid(row=2, column=0)
frame6=Addpanel(root,6)
frame6.grid(row=2, column=1)
frame7=Addpanel(root,7)
frame7.grid(row=2, column=2)
frame8=Addpanel(root,8)
frame8.grid(row=2, column=3)
frame9=Addpanel(root,9)
frame9.grid(row=3, column=0)
frame10=Addpanel(root,10)
frame10.grid(row=3, column=1)
frame11=Addpanel(root,11)
frame11.grid(row=3, column=2)
frame12=Addpanel(root,12)
frame12.grid(row=3, column=3)
frame13=Addpanel(root,13)
frame13.grid(row=4, column=0)
frame14=Addpanel(root,14)
frame14.grid(row=4,column=1)
frame15=Addpanel(root,15)
frame15.grid(row=4,column=2)
frame16=Addpanel(root,16)
frame16.grid(row=4,column=3)
frame17=AddDIpanel(root)
frame17.grid(row=1, column=4,rowspan=4)
frame18=addPanelBottom(root)
frame18.grid(row=5, column=0,columnspan=5)

address=get_host_ip()
# 获取本机ip
port=6000
try:
    config = ConfigParser.ConfigParser()
    #
    content=None
    #if os.path.exists(cfgFile):    
    with open('config.ini', mode='rb') as f:
        content = f.read()
    if content.startswith(b'\xef\xbb\xbf'):
        content = content[3:]
    config.read('config.ini')    
except:
    print("888888")
    config = ConfigParser.ConfigParser() 
    info= {
            'HostIP':address,
            'Port':str(port) 
           }
    config.add_section('MYConfig')    
    for key in info.keys():
         config.set('MYConfig', key, info[key])
    with open('config.ini', 'w') as configfile:
         config.write(configfile)
    with open('config.ini', mode='rb') as f:
         content = f.read()
    if content.startswith(b'\xef\xbb\xbf'):
        content = content[3:]
    config.read('config.ini')                      

    
contentStr= content.decode('utf8') 
#print(contentStr) 
config.read(contentStr)#_string 改为此写法                
config.sections()#获取所有的章节
address=config.get("MYConfig","hostip")

port=int(config.get("MYConfig","port"))
 
root.protocol("WM_DELETE_WINDOW",callbackClose)# 
              
setAddreAndPort(address,port)
t=threading.Thread(target=tcpListen,args=(address,port))  #t为新创建的线程
t.start()

gettime()

root.update() # update window ,must do
curWidth = root.winfo_reqwidth() # get current width
curHeight = root.winfo_height() # get current height
scnWidth,scnHeight = root.maxsize() # get screen width and height
 # now generate configuration information
tmpcnf = '%dx%d+%d+%d'%(curWidth,curHeight,
(scnWidth-curWidth)/2,(scnHeight-curHeight)/2)
root.geometry(tmpcnf)

try:
   root.iconbitmap('./logo.ico')
except:
    print("出错啦")

refresh_dataNew()
root.mainloop()


