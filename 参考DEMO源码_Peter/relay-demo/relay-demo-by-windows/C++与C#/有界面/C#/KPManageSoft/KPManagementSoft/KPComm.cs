using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;


namespace KPManagementSoft
{
    public enum KpCommStatus
    {
        /// <summary>
        /// 空闲
        /// </summary>
        Free = 0,
        /// <summary>
        /// 发送AT测试指令
        /// </summary>
        AT,
        /// <summary>
        /// 读设备名称
        /// </summary>
        ReadName,
        /// <summary>
        /// 读DO状态
        /// </summary>
        ReadDOStatus,
        /// <summary>
        /// 写DO状态
        /// </summary>
        WriteDOStatus,
        /// <summary>
        /// 读DI状态
        /// </summary>
        ReadDIStatus,
        /// <summary>
        /// 其它指令状态
        /// </summary>
        OtherCmdStatus,

    };

    public class CTimerOut
    {
        public int timerout;
        public CTimerOut()
        {
            this.timerout = 0;
        }
    }
    public class KPComm
    {
        /// <summary>
        /// 当前电脑的IP
        /// </summary>
        static public string hostIp = "127.0.0.1";
        /// <summary>
        /// 服务器端口号
        /// </summary>
        static public int serverPort = 6000;
        /// <summary>
        /// 用来记录连接的socket
        /// </summary>
        static public Dictionary<string, Socket> dicSocket = new Dictionary<string, Socket>();
        /// <summary>
        /// 5次检测不到时，即为超时======
        /// 在接收线程中清零
        /// </summary>
        static public Dictionary<string, CTimerOut> dicSocketTimer = new Dictionary<string, CTimerOut>();
        /// <summary>
        /// 所有连接设备的IP
        /// </summary>
        static public List<string> dicSocketIP = new List<string>();
        /// <summary>
        /// 设备名称
        /// </summary>
        static public Dictionary<string, string> _deviceName = new Dictionary<string, string>();

        /// <summary>
        /// 设备ID
        /// </summary>
        static public Dictionary<string, string> _deviceIDs = new Dictionary<string, string>();

        /// <summary>
        /// 当前选择的设备连接
        /// </summary>
        static public Socket currentSocket = null;
        /// <summary>
        /// KpCommStatus的全局字典
        /// </summary>
        static public Dictionary<string, KpCommStatus> gkcs = new Dictionary<string, KpCommStatus>();
        /// <summary>
        /// 心跳标志
        /// </summary>
        static public Dictionary<string, bool> heartFlag = new Dictionary<string, bool>();
        /// <summary>
        /// socket的监听变量
        /// </summary>
        static public Socket socListen = null;
        /// <summary>
        /// DO状态字
        /// </summary>
        //static public uint DOStatus = 0;
        static public Dictionary<string, uint> dicDOStatus = new Dictionary<string, uint>();
        /// <summary>
        /// DI状态字
        /// </summary>
        //static public uint DIStatus = 0;
        static public Dictionary<string, uint> dicDIStatus = new Dictionary<string, uint>();
        /// <summary>
        /// 读状态时间，
        /// 一个周期下来，
        /// 包括读DO状态，DI状态
        /// </summary>
        static public uint readStatusTimer = 100;
        /// <summary>
        /// 读DO状态时间
        /// </summary>
        static public int readDOStatusTime = 100;
        /// <summary>
        /// 读DI状态时间
        /// </summary>
        static public int readDIStatusTime = 100;
        /// <summary>
        /// AT定时发送时间
        /// </summary>
        static public int NetCheckTime = 30;
        /// <summary>
        /// 线程锁
        /// </summary>
        static object readlock = new object();
        public static object DOLock = new object();
        static object DILock = new object();
        /// <summary>
        /// 在线检测锁
        /// </summary>
        static public object INLineCheckLock = new object();
        /// <summary>
        /// 显示委托类型
        /// </summary>
        /// <param name="str"></param>
        public delegate void showMsgDelegate(string str);
        /// <summary>
        /// 将数据显示到控件
        /// </summary>
        static public showMsgDelegate ShowMsg = null;
        /// <summary>
        /// 时间显示
        /// </summary>
        /// <param name="ch">通道号</param>
        /// <param name="time">时间</param>
        public delegate void showTimerDelegate(int ch, string time);
        /// <summary>
        /// 用于将时间显示到时间控件上
        /// </summary>
        static public showTimerDelegate ShowTime = null;
        /// <summary>
        /// 
        /// </summary>
        public static Dictionary<string, Thread> _threadMana = new Dictionary<string, Thread>();

        /// <summary>
        /// DI  DO  状态显示托管
        /// </summary>
        /// <param name="DOStatus"></param>
        /// <param name="DIStatus"></param>
        public delegate void DIOSSDelegate(int[] DOStatus, int[] DIStatus);
        /// <summary>
        /// DI  DO  状态显示托管变量
        /// </summary>
        static public DIOSSDelegate DIOShow = null;

        public delegate void InLineDelegate(bool enable);

        static public InLineDelegate InLine = null;
        /// <summary>
        /// 当前网卡
        /// </summary>
        public static CSimpleNetInfo csni = new CSimpleNetInfo();

        public delegate void CheckAndListenDelegate(string str = null);
        public static CheckAndListenDelegate checkAndListen = null;
        /// <summary>
        /// 发送AT指令线程
        /// </summary>
        public static Thread sendATThread = null;

        /// <summary>
        /// 接收线程运行管理字典
        /// </summary>
        public static Dictionary<string, bool> dicRecvThreadRun = new Dictionary<string, bool>();

        public delegate void ClearShowDelegate();
        public static ClearShowDelegate CleartvMachinesTree = null;

        /// <summary>
        /// 读取DO状态
        /// </summary>
        static Thread readDOThread = null;
        /// <summary>
        /// 读取DI状态
        /// </summary>
        static Thread readDIThread = null;

        /// <summary>
        /// 网卡集合
        /// </summary>
        public static List<string>lstComputerIPs = new List<string>();
        /// <summary>
        /// 关闭侦听
        /// </summary>
        static public void ListenClose()
        {
            if (socListen != null)
            {
                socListen.Close();
            }
        }

        /// <summary>
        /// 获取TCP连接数量
        /// </summary>
        /// <returns></returns>
        static public int GetTCPLinkNumber()
        {
            return dicSocket.Count;
        }

        /// <summary>
        /// 清除数据
        /// </summary>
        static public void ClearData()
        {
            //删除线程
            //删除连接
            foreach (string ip in dicSocketIP)
            {
                //删除线程
                if (_threadMana.Keys.Contains(ip) == true)
                {
                    Thread th = _threadMana[ip];//
                    th.Abort();
                    _threadMana.Remove(ip);
                }
                //关闭连接
                if (dicSocket.Keys.Contains(ip) == true)
                {
                    dicSocket[ip].Close();
                    dicSocket.Remove(ip);
                }
            }

            //清除列表与字典
            if (dicSocketIP.Count > 0)
            {
                dicSocketIP.Clear();
            }

            if (_deviceName.Count > 0) _deviceName.Clear();

            if (_threadMana.Count > 0)
            {
                _threadMana.Clear();
            }
        }


        #region 获取当前电脑IP
        /// <summary>
        /// 获取当前电脑IP
        /// </summary>
        static public void GetCurrentComputerIP()
        {
            //获取当前电脑IP
            IPHostEntry hostEntry = Dns.GetHostEntry("");
            foreach (IPAddress address in hostEntry.AddressList)
            {
                //==============   =========================
                lstComputerIPs.Add(address.ToString());
                if (address.AddressFamily == AddressFamily.InterNetwork)
                {
                    hostIp = address.ToString();
                    //break;
                }
            }
        }
        #endregion

        /// <summary>
        /// 移除指定的IP 就是指定为设备ID
        /// </summary>
        static public void RemoveAccptIP(string acceptIp)
        {
            //=================================================================
            if (_threadMana.Keys.Contains(acceptIp) == true)
            {
                Thread thr = _threadMana[acceptIp];
                thr.Abort();
                _threadMana.Remove(acceptIp);
                dicRecvThreadRun[acceptIp] = false;
                Thread.Sleep(5);
            }
            if (dicSocketTimer.Keys.Contains(acceptIp) == true)
            {
                dicSocketTimer.Remove(acceptIp);
            }
            //检查是否存在残留连接，如果有需要删除
            if (dicSocket.Keys.Contains(acceptIp) == true)
            {
                dicSocket.Remove(acceptIp);
            }
            //记录新的连接
            //记录连接的IP
            for (int i = 0; i < dicSocketIP.Count; i++)
            {
                if (dicSocketIP[i] == acceptIp)
                {
                    dicSocketIP.RemoveAt(i);
                }
            }
            //心跳标志处理
            if (heartFlag.Keys.Contains(acceptIp) == true)
            {
                heartFlag.Remove(acceptIp);
            }
            if (gkcs.Keys.Contains(acceptIp) == true)
            {
                gkcs.Remove(acceptIp);
            }

            if (dicDOStatus.Keys.Contains(acceptIp) == true)
            {
                dicDOStatus.Remove(acceptIp);
            }

            if (dicDIStatus.Keys.Contains(acceptIp) == true)
            {
                dicDIStatus.Remove(acceptIp);
            }
            if (dicRecvThreadRun.Keys.Contains(acceptIp) == true)
            {
                dicRecvThreadRun.Remove(acceptIp);
            }


        }
        #region 监听线程
        static public void ListeningThread()
        {
            #region
            string acceptIp = null;
            #endregion
            try
            {
                //使用指定的地址族，套接字类型和协议实例化新的套接字
                socListen = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                //将字符串地址转化为网际协议地址
                IPAddress ipAddress = IPAddress.Parse(hostIp);
                //使用指定地址和端口号实例化新的网络端点
                IPEndPoint endPoint = new IPEndPoint(ipAddress, serverPort);//服务器端口，这里需要修改一下

                //socket与本地终结点相关联
                socListen.Bind(endPoint);

                //设置最大侦听长度
                socListen.Listen(1000);
            }
            catch { return; }


            while (true)
            {
                try
                {
                    Socket socketAccept = socListen.Accept();
                    //块内的代码与在线检测代码是互斥的，同一时刻只能执行之一
                    lock (INLineCheckLock)
                    {
                        CTimerOut timerout = new CTimerOut();
                        acceptIp = socketAccept.RemoteEndPoint.ToString();
                        CFileManager.WriteLog("设备：" + acceptIp + "连接！");
                        //检查此IP是否已连接过了，
                        //如果连接过，即需要删除上次连接产生的线程，
                        //如果还没连接过，即直接产生通讯线程
                        if (_threadMana.Keys.Contains(acceptIp) == true)
                        {
                            Thread thr = _threadMana[acceptIp];
                            thr.Abort();
                            _threadMana.Remove(acceptIp);
                            dicRecvThreadRun[acceptIp] = false;
                            Thread.Sleep(5);
                        }
                        if (dicSocketTimer.Keys.Contains(acceptIp) == true)
                        {
                            dicSocketTimer.Remove(acceptIp);
                        }
                        dicSocketTimer.Add(acceptIp, timerout);

                        //检查是否存在残留连接，如果有需要删除
                        if (dicSocket.Keys.Contains(acceptIp) == true)
                        {
                            dicSocket.Remove(acceptIp);
                        }
                        //记录新的连接
                        dicSocket.Add(acceptIp, socketAccept);

                        //记录连接的IP
                        for (int i = 0; i < dicSocketIP.Count; i++)
                        {
                            if (dicSocketIP[i] == acceptIp)
                            {
                                dicSocketIP.RemoveAt(i);
                            }
                        }
                        dicSocketIP.Add(acceptIp);
                        //心跳标志处理
                        if (heartFlag.Keys.Contains(acceptIp) == true)
                        {
                            heartFlag.Remove(acceptIp);
                        }
                        heartFlag.Add(acceptIp, false);

                        if (gkcs.Keys.Contains(acceptIp) == true)
                        {
                            gkcs.Remove(acceptIp);
                        }
                        if (dicDOStatus.Keys.Contains(acceptIp) == true)
                        {
                            dicDOStatus.Remove(acceptIp);
                        }
                        if (dicDIStatus.Keys.Contains(acceptIp) == true)
                        {
                            dicDIStatus.Remove(acceptIp);
                        }
                        if (dicRecvThreadRun.Keys.Contains(acceptIp) == true)
                        {
                            dicRecvThreadRun.Remove(acceptIp);
                        }
                        dicRecvThreadRun.Add(acceptIp, true);
                        GC.Collect();//强制内存回收
                        Thread th = new Thread(ReceiveCmdThread);
                        _threadMana.Add(acceptIp, th);
                        th.IsBackground = true;
                        th.Start(socketAccept);
                    }
                }
                catch (Exception e)
                {
                    CFileManager.WriteLog("ListeningThread:" + e.Message);
                }
            }
        }
        #endregion
        /// <summary>
        /// 发送 AT+NAME=?\r\n 指令===暂时打开===2020-09-03
        /// </summary>
        /// <param name="ip"></param>
        static public void SendATName(string ip)
        {
            string atCmd = "AT+NAME=?\r\n";
            byte[] sendByte = MyConvert.StringToAsciiByte(atCmd);
            try { dicSocket[ip].Send(sendByte); } //使用ascll发送
            catch { }
        }

        #region 发送DO读取命令线程

        /// <summary>
        /// 读DO线程
        /// </summary>
        static public void ReadDOCmdThread()
        {
            //如果没有选择设备，不能往下执行
            while (currentSocket == null) ;
            CFileManager.WriteLog("当前设备为：" + currentSocket.RemoteEndPoint.ToString() + "！");
            CFileManager.WriteLog("创建设备：" + currentSocket.RemoteEndPoint.ToString() + "发送线程！");
            Thread.Sleep(1000);
            string atCmd = "AT+STACH0=?\r\n";//读取所有
            byte[] sendByte = MyConvert.StringToAsciiByte(atCmd);
            while (true)
            {
                try
                {
                    if (currentSocket!=null&& currentSocket.Connected==true)
                    {
                        currentSocket.Send(sendByte);
                    }
                    //30ms发送一次==每次发送
                    int timems = (int)(readDOStatusTime);
                    Thread.Sleep(timems);
                }
                catch (Exception e)
                {
                    CFileManager.WriteLog("ReadDICmdThread:" + e.Message);
                   //发送D0 读取错误====================================
                }
            }
        }
        #endregion
        #region DI读取
        /// <summary>
        /// 读DI线程
        /// </summary>
        static public void ReadDICmdThread()
        {
            //如果没有选择设备，不能往下执行
            while (currentSocket == null) ;

            CFileManager.WriteLog("当前设备为：" + currentSocket.RemoteEndPoint.ToString() + "！");
            CFileManager.WriteLog("创建设备：" + currentSocket.RemoteEndPoint.ToString() + "发送线程！");
            Thread.Sleep(1000);
            string atCmd = "AT+OCCH0=?\r\n";//读取所有
            byte[] sendByte = MyConvert.StringToAsciiByte(atCmd);
            while (true)
            {
                try
                {
                    while (currentSocket == null) ;
                    currentSocket.Send(sendByte);

                    //30ms发送一次
                    int timems = (int)(readDIStatusTime);
                    Thread.Sleep(timems);
                }
                catch (Exception e)
                {
                    CFileManager.WriteLog("ReadDICmdThread:" + e.Message);
                }
            }
        }
        #endregion
        #region 信息接收线程
        /// <summary>
        /// 信息接收线程
        /// </summary>
        /// <param name="socketObj"></param>
        static public void ReceiveCmdThread(object socketObj)
        {
            Socket deviceSocket = socketObj as Socket;
            byte[] buffer = new byte[1024];
            int dataLen = 0;
            KpCommStatus kcs = KpCommStatus.ReadName;
            int timerout = 0;
             int[] DOStatus = new int[16];
            int[] DIStatus = new int[16];
            string devIP = deviceSocket.RemoteEndPoint.ToString();
            CFileManager.WriteLog("创建设备：" + deviceSocket.RemoteEndPoint.ToString() + "接收线程！");
            //设置当前的线程
            currentSocket = deviceSocket;//直接选定为当前的线程

            if (readDOThread == null)
            {
                readDOThread = new Thread(KPComm.ReadDOCmdThread);
                readDOThread.IsBackground = true;
                readDOThread.Start();
            
                string addThName = devIP + "D0";
                _threadMana.Add(addThName, readDOThread);
            };
            if (readDIThread == null)
            {
                readDIThread = new Thread(KPComm.ReadDICmdThread);
                readDIThread.IsBackground = true;
                readDIThread.Start();
                string addThName = devIP + "DI";
                _threadMana.Add(addThName, readDIThread);
            };
            string atCmd = "AT+NAME=?\r\n";
            byte[] sendByte = MyConvert.StringToAsciiByte(atCmd);
            try
            {
                deviceSocket.Send(sendByte);
            }
            catch
            {
            }
            while (dicRecvThreadRun[devIP] == true)
            {
                try
                {
                    dataLen = deviceSocket.Receive(buffer);
                    //将字节转为字符串
                    atCmd = MyConvert.AsciiByteToString(buffer, dataLen);
                    string deviceIP = deviceSocket.RemoteEndPoint.ToString();
                    KPComm.dicSocketTimer[deviceIP].timerout = 0;
                    heartFlag[deviceIP] = true; //这个暂时不用
                    CFileManager.WriteLog(deviceSocket.RemoteEndPoint.ToString() + "接收数据：" + atCmd);
                    if (atCmd.Contains("AT") == true)//发上来的是心跳包
                    {
                        kcs = KpCommStatus.Free;
                    }
                    else if (atCmd.Contains("NAME") == true)
                    {
                        kcs = KpCommStatus.ReadName;
                    }
                    else if (atCmd.Contains("STACH") == true)
                    {
                        kcs = KpCommStatus.ReadDOStatus;
                    }
                    else if (atCmd.Contains("OCCH") == true)
                    {
                        kcs = KpCommStatus.ReadDIStatus;
                    }
                    else
                    {//处理自定义指令
                        kcs = KpCommStatus.OtherCmdStatus;
                    }
                    switch (kcs)
                    {
                        case KpCommStatus.Free:
                            if (atCmd.Contains("AT") == true)//响应心跳
                            {
                                byte[] sendbuf = MyConvert.StringToAsciiByte("AT+ACK\r\n");
                                deviceSocket.Send(sendbuf);
                            }
                            timerout = 0;
                            break;
                        case KpCommStatus.AT:
                            if (atCmd.Contains("OK") == true)
                            {
                                timerout = 0;
                                kcs = KpCommStatus.Free;
                            }
                            else
                            {
                               
                            }
                            break;
                        case KpCommStatus.ReadName:
                            if (atCmd.Contains("NAME:") == true)
                            {
                                atCmd = MyConvert.ByteArrayToString(buffer);//转为unicode编码
                                timerout = 0;
                                kcs = KpCommStatus.Free;
                                atCmd = atCmd.Substring(atCmd.IndexOf("NAME:"), atCmd.Length - atCmd.IndexOf("NAME:"));
                                string str = atCmd.Substring(atCmd.IndexOf(":") + 1, atCmd.IndexOf("\r") - atCmd.IndexOf(":") - 1);
                                if (_deviceName.Keys.Contains(deviceIP) == false)
                                {
                                    _deviceName.Add(deviceIP, str);
                                }
                            }
                            else
                            {
                                
                            }
                            break;
                        case KpCommStatus.ReadDIStatus:
                        case KpCommStatus.ReadDOStatus:
                            {
                               
                                //计算‘+’出现次数
                                int i = 0;
                                int count = 0;
                                //求 + 的出现次数
                                while (atCmd.IndexOf("+", i) >= 0)
                                {
                                    i = atCmd.IndexOf("+", i) + 1;
                                    count++;
                                }
                                for (int index = 0; index < count; index++)
                                {
                                    if (atCmd.IndexOf("+STACH", 0) == 0)
                                    {
                                        string ch = atCmd.Substring(atCmd.IndexOf("STACH") + 5,
                                         atCmd.IndexOf(":") - (atCmd.IndexOf("STACH") + 5));
                                        //=====================================================
                                        string stastr = "";
                                        string timerstr = "";

                                        if ((atCmd.IndexOf(",") - atCmd.IndexOf(":")) == 2)//检查是否带时间
                                        {
                                            stastr = atCmd.Substring(atCmd.IndexOf(":") + 1,
                                            atCmd.IndexOf(",") - (atCmd.IndexOf(":") + 1));

                                            timerstr = atCmd.Substring(atCmd.IndexOf(",") + 1,
                                                atCmd.IndexOf("\r") - (atCmd.IndexOf(",") + 1));

                                            if (currentSocket == deviceSocket)//=============
                                            {
                                                ShowTime(Convert.ToInt32(ch), timerstr);
                                            }
                                        }
                                        else
                                        {
                                                stastr = atCmd.Substring(atCmd.IndexOf(":") + 1,
                                                atCmd.IndexOf("\r") - (atCmd.IndexOf(":") + 1));
                                        }
                                   
                                        DOStatus[Convert.ToByte(ch) - 1] = Convert.ToByte(stastr);
                                       
                                    }
                                    else if (atCmd.IndexOf("+OCCH_ALL") == 0)
                                    {
                                        byte[] ch = new byte[16];
                                        int ch_index = 0;

                                        string atStr = atCmd.Substring(atCmd.IndexOf("+OCCH_ALL:") + 10,
                                                atCmd.IndexOf("\n", 0) + 1 - (atCmd.IndexOf("+OCCH_ALL:") + 10));

                                        do
                                        {
                                            string strch = atStr.Substring(0, 1);
                                            ch[ch_index++] = Convert.ToByte(strch);

                                            DIStatus[ch_index - 1] = Convert.ToByte(strch);


                                            if (atStr.Length > 3)
                                            {
                                                atStr = atStr.Substring(atStr.IndexOf(",") + 1,
                                                    atStr.Length - (atStr.IndexOf(",") + 1));
                                            }
                                            else
                                            { break; }
                                        } while ((atCmd.Length > 3) && (ch_index < 16));
                                    }
                                    else if (atCmd.IndexOf("+OCCH", 0) == 0)
                                    {
                                        string ch = atCmd.Substring(atCmd.IndexOf("OCCH") + 4,
                                                        atCmd.IndexOf(":") - (atCmd.IndexOf("OCCH") + 4));

                                        string stastr = atCmd.Substring(atCmd.IndexOf(":") + 1,
                                            atCmd.IndexOf("\r") - (atCmd.IndexOf(":") + 1));

                                        int chNum = Convert.ToInt32(ch);
                                        //
                                        DIStatus[chNum-1] = Convert.ToInt32(stastr);//=====
                                    }
                                    //每次要减去已使用的部分
                                    if (index < (count - 1))
                                    {
                                        int index_n = atCmd.IndexOf("\n", 0) + 1;
                                        int len = atCmd.Length - index_n;
                                        atCmd = atCmd.Substring(index_n,
                                        atCmd.Length - index_n);
                                    }
                                }
                                //显示处理
                                lock (DOLock)
                                {
                                    if ((currentSocket != null) && (deviceIP == currentSocket.RemoteEndPoint.ToString()))
                                    {
                                        DIOShow(DOStatus, DIStatus);//
                                    }
                                }
                            }
                            break;

                        case KpCommStatus.WriteDOStatus:
                            {
                                if (atCmd.Contains("OK") == true)
                                {

                                }
                                kcs = KpCommStatus.Free;
                            }
                            break;
                        case KpCommStatus.OtherCmdStatus:
                            {
                                kcs = KpCommStatus.Free;
                                timerout = 0;
                            }
                            break;

                        default:
                            kcs = KpCommStatus.Free;
                            timerout = 0;
                            break;
                    }
                   
                    //gkcs[deviceIP] = kcs;
                }
                catch (Exception e)
                {
                    //连接断开
                   if(dicSocket.Keys.Contains(devIP)) dicSocket.Remove(devIP); 
                    DOStatus = new int[16];
                    DIStatus = new int[16];
                    DIOShow(DOStatus, DIStatus);//
                    for (int i = 1; i < 17; i++)
                    {
                        ShowTime(i, "100000");//清除显示
                    }
                    ClearData();//刷新完界面然后关闭线程然后
                    currentSocket = null;
                    CFileManager.WriteLog("ReceiveCmdThread:" + e.Message);
                }
            }
        }
        #endregion
        #region 继电器控制
        /// <summary>
        /// 继电器控制函数
        /// </summary>
        /// <param name="ch">0 - 所有通道 1...16 - 指定某个通道，不同型号的设备所支持的通道数量不一样</param>
        /// <param name="action">自锁关模式，常开接口断开，常闭接口吸合，状态长期保持 1 - 自锁开模式，常开接口吸合，常闭接口断开，状态长期保持 2 - 点动关模式，常开接口点动关(断开后延时吸合)，常闭接口点动开(吸合后延时断开) 3 - 点动开模式，常开接口点动开(吸合后延时断开)，常闭接口点动关(断开后延时吸合) 4 - 互锁延时模式，常开接口吸合或延时吸合，常闭接口断开或延时断开，互锁通道的常开接口断开，常闭接口 吸合5 - 互锁点动模式，常开接口点动开或延时点动开，常闭接口点动关或延时点动关，互锁通道的常开接口断开， 常闭接口吸合 6 - 循环模式，常开接口吸后延时断开，断开后再延时吸合，一直循环下去，直到收到断开指令或触发停止事件 7 - 翻转模式，常开接口状态翻转（取反），吸合变断开，断开变吸合 注：互锁的 2 个继电器通道固定为 1 和 2，3 和 4，5 和 6……</param>
        /// <param name="strTimer">时间单位为秒或 100 毫秒，可通过 AT+DLYUNIT 指令设置 1...99999 - 点动关模式、点动开模式和互锁点动模式下，继电器状态切换(取反)延时时间；循环模式下，继电 器吸合的时间 100000 - 继电器状态锁定，不执行延时切换，自锁关模式、自锁开模式、互锁延时模式和翻转模式固定为此值</param>
        /// <param name="strTimer">时间单位为秒或 100 毫秒，可通过 AT+DLYUNIT 指令设置 0...99999 - 互锁延时模式和互锁点动模式下，继电器执行延时吸合或延时点动开的等待时间 1...99999 - 循环模式下，继电器断开的时间 100000 - 继电器状态锁定，不执行延时切换，自锁关模式、自锁开模式、点动关模式、点动开模式和翻转模式 固定为此值</param>
        static public void RelayControl(int ch, int action, string strTimer = null, string waitTimer = null)
        {
            //==========================         =========================
            string atCmd = "AT+STACH" + ch.ToString() + "="
                         + action.ToString();
            if (strTimer != null)
            {
                atCmd += "," + strTimer;
            }
            if (waitTimer != null)
            {
                atCmd += "," + waitTimer;
            }
            //=======================================================================  
            atCmd += "\r\n";
            byte[] buf = MyConvert.StringToAsciiByte(atCmd);//===================
            try
            {
                gkcs[currentSocket.RemoteEndPoint.ToString()] = KpCommStatus.WriteDOStatus;
                currentSocket.Send(buf);
            }
            catch (Exception ex)
           {

            }

        }
        #endregion
    }
}
