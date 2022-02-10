using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Threading;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace KPManagementSoft
{
    public partial class FrmMain : Form
    {
        /// <summary>
        /// 主线程
        /// </summary>
        Thread mainThread = null;
        /// <summary>
        /// 发送命令的线程
        /// </summary>
        Thread sendThread = null;
        /// <summary>
        /// 读取DO状态
        /// </summary>
        Thread readDOThread = null;
        /// <summary>
        /// 读取DI状态
        /// </summary>
        Thread readDIThread = null;

        /// <summary>
        /// 用于显示的线程
        /// </summary>
        Thread disThread = null;
        /// <summary>
        /// 在线检测线程
        /// </summary>
        Thread INLineThread = null;
        /// <summary>
        /// DO显示列表
        /// </summary>
        List<Label> DOList = new List<Label>();
        /// <summary>
        /// DI显示列表
        /// </summary>
        List<Label> DIList = new List<Label>();
        /// <summary>
        /// 时间列表
        /// </summary>
        List<TextBox> tbListTimer = new List<TextBox>();

        /// <summary>
        ///字体延时
        /// </summary>
        List<Label> lstdlyTime = new List<Label>();
        
        /// <summary>
        /// 等待时间列表
        /// </summary>
        List<TextBox> txtListWaitTime = new List<TextBox>();

        /// <summary>
        /// 模式列表
        /// </summary>
        List<ComboBox> lstComboBox = new List<ComboBox>();

        /// <summary>
        /// 时间列表每个项对应的使能标志
        /// </summary>
        List<CheckBox> cbListCheck = new List<CheckBox>();
        /// <summary>
        /// 秒的显示
        /// </summary>
        List<Label> laDOList = new List<Label>();

        /// <summary>
        /// 等待时间秒的显示
        /// </summary>
        List<Label> lstSec = new List<Label>();

        //====================
        /// <summary>
        /// 等待字体
        /// </summary>
        List<Label> lstWait = new List<Label>();

        /// <summary>
        /// 等待时间复选框
        /// </summary>
        List<CheckBox> lstcbWait = new List<CheckBox>();
        /// <summary>
        /// DO延时时间显示
        /// </summary>
        List<Label> laDlyDOList = new List<Label>();
        /// <summary>
        /// 列表显示锁，作用于自动检测及显示
        /// </summary>
        object disTreeLock = new object();

        int NetNoAckNum = 0;

        public FrmMain()
        {
            InitializeComponent();
            BindAllCombox();
        }

        /// <summary>
        /// 显示设置
        /// </summary>
        public void BindAllCombox()
        {
            
            bindCbox(cbMode1);
            bindCbox(cbMode2);
            bindCbox(cbMode3);
            bindCbox(cbMode4);
            bindCbox(cbMode5);
            bindCbox(cbMode6);
            bindCbox(cbMode7);
            bindCbox(cbMode8);
            bindCbox(cbMode9);
            bindCbox(cbMode10);
            bindCbox(cbMode11);
            bindCbox(cbMode12);
            bindCbox(cbMode13);
            bindCbox(cbMode14);
            bindCbox(cbMode15);
            bindCbox(cbMode16);
        }
        /// <summary>
        /// 获取设备管理参数
        /// </summary>
        private void GetManaParam()
        {
            KPComm.readDOStatusTime = Convert.ToInt32(ConfigHelper.GetAppConfig("OUTQueryTime"));
            KPComm.readDIStatusTime = Convert.ToInt32(ConfigHelper.GetAppConfig("INPUTQueryTime"));
            KPComm.NetCheckTime = Convert.ToInt32(ConfigHelper.GetAppConfig("NetCheckTime"));
            NetNoAckNum = Convert.ToInt32(ConfigHelper.GetAppConfig("NetNoAckNum"));
        }

        /// <summary>
        /// 检查服务器端口
        /// </summary>
        /// <returns>返回true为占用，false为末占用</returns>
        private bool CheckServerPort()
        {

            bool useFlag = CPublic.CheckPort(KPComm.serverPort.ToString());
            if (useFlag == true)
            {
                LocalPort.ForeColor = Color.Red;
            }
            else
            {
                LocalPort.ForeColor = Color.Black;
            }
            return useFlag;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="comboBox"></param>
        private void bindCbox(ComboBox comboBox)
        {
            IList<CbInfo> infoList = new List<CbInfo>();
            CbInfo info0 = new CbInfo() { Id = 0, Name = "自锁关" };
            CbInfo info1 = new CbInfo() { Id = 1, Name = "自锁开" };
            CbInfo info2 = new CbInfo() { Id = 2, Name = "点动关" };
            CbInfo info3 = new CbInfo() { Id = 3, Name = "点动开" };
            CbInfo info4 = new CbInfo() { Id = 4, Name = "互锁延时" };
            CbInfo info5 = new CbInfo() { Id = 5, Name = "互锁点动" };
            CbInfo info6 = new CbInfo() { Id = 6, Name = "循环" };
            CbInfo info7 = new CbInfo() { Id = 7, Name = "翻转" };
            infoList.Add(info0);
            infoList.Add(info1);
            infoList.Add(info2);
            infoList.Add(info3);
            infoList.Add(info4);
            infoList.Add(info5);
            infoList.Add(info6);
            infoList.Add(info7);
            comboBox.DataSource = infoList;
            comboBox.ValueMember = "Id";
            comboBox.DisplayMember = "Name";
        }
        /// <summary>
        /// 解析所选网卡的描述名及IP
        /// </summary>
        private void ParsingNetCardInfo()
        {
            string nciStr = ConfigHelper.GetAppConfig("NetCardDescription");
            if (nciStr == "abcdefg")
            {
                KPComm.GetCurrentComputerIP();
            }
            else if (nciStr != "")
            {
                KPComm.csni.Description = nciStr.Substring(0, nciStr.IndexOf(':'));
                KPComm.csni.IPStr = nciStr.Substring(nciStr.IndexOf(':') + 1);
                KPComm.hostIp = KPComm.csni.IPStr;
            }
        }

        private void FrmMain2_Load(object sender, EventArgs e)
        {
            CFileManager.WriteLog("系统启动。。。");
            ParsingNetCardInfo();
            GetManaParam();
            InitControls();
            CFileManager.AppPath = Application.StartupPath;
            KPComm.ShowTime = new KPComm.showTimerDelegate(ShowTime);
            KPComm.DIOShow = new KPComm.DIOSSDelegate(DIOShow);

            KPComm.checkAndListen = new KPComm.CheckAndListenDelegate(CheckAndListen);
            ShowTxt();
            Thread t = new Thread(ShowTimeThread);
            t.IsBackground = true;
            t.Start();
            CheckAndListen();
            //检测设置是否连接上，如果连接上，即进行显示
            Thread th = new Thread(CheckDeviceThread);
            th.IsBackground = true;
            th.Start();
        }
        /// <summary>
        /// 检查端口是否被占用
        /// 监听端口
        /// 在线检测
        /// </summary>
        public void CheckAndListen(string nsi = null)
        {
            this.Invoke(new EventHandler(delegate
            {
                ClearResources();
                string oldnsi = ConfigHelper.GetAppConfig("NetCardDescription");
                if (nsi != null) ConfigHelper.UpdateAppConfig("NetCardDescription", nsi);
                KPComm.serverPort = Convert.ToInt32(ConfigHelper.GetAppConfig("LocalPort"));
                if (((nsi != null) && (oldnsi != nsi))
                || (CheckServerPort() == false))
                {
                    if (mainThread != null)//========
                    {
                        mainThread.Abort();
                        mainThread = null;
                        KPComm.socListen.Close();//================
                        KPComm.socListen = null;
                    }
                    //启动监听线程
                    mainThread = new Thread(KPComm.ListeningThread);
                    mainThread.IsBackground = true;
                    mainThread.Start();
                    #region 
                    if (INLineThread != null)
                    {
                        INLineThread.Abort();
                        INLineThread = null;
                    }
                    INLineThread = new Thread(INLineCheck);
                    INLineThread.IsBackground = true;
                    INLineThread.Start();
                    #endregion
                }
            }));
        }

        private void ShowTxt()
        {
        
                LocalIP.Text = "IP地址：" + KPComm.hostIp;
                LocalPort.Text = "端口号：" + KPComm.serverPort.ToString();
        }
        private void ShowTimeThread()
        {
            while (true)
            {
                try
                {
                    DateTime dt = DateTime.Now;
                    DayOfWeek week = DateTime.Now.DayOfWeek;
                    this.Invoke(new EventHandler(delegate
                    {
                            DateTimeWeek.Text = dt.ToString("yyyy/MM/dd hh:mm:ss") + "   " + GetWeekString((int)week);
                            currLinkNum.Text = "当前连接数：" + KPComm.dicSocket.Count.ToString();
                    }));
                    Thread.Sleep(1000);
                }
                catch
                {

                }
            }
        }
        private string GetWeekString(int week)
        {
            string sWeek = "星期日";
            switch (week)
            {
                case 1: sWeek = "星期一"; break;
                case 2: sWeek = "星期二"; break;
                case 3: sWeek = "星期三"; break;
                case 4: sWeek = "星期四"; break;
                case 5: sWeek = "星期五"; break;
                case 6: sWeek = "星期六"; break;
                default: sWeek = "星期日"; break;
            }
            return sWeek;
        }

        private void DIOShow(int[] DOStatus, int[] DIStatus)
        {
            this.Invoke(new EventHandler(delegate
            {
                for (int i = 0; i < 16; i++)
                {
                   if(DOStatus[i] == 1)
                    {
                        DOList[i].ForeColor = Color.LimeGreen;

                    }
                    else
                    {
                        DOList[i].ForeColor = Color.LightGray;

                    }
                    if (DIStatus[i] == 1)
                    {
                        DIList[i].ForeColor = Color.LimeGreen;

                    }
                    else
                    {
                        DIList[i].ForeColor = Color.LightGray;
                    }
                }
            }));
        }

        private void ShowTime(int ch, string timer)
        {
            try
            {
                this.Invoke(new EventHandler(delegate
                {
        
                    int iTime = Convert.ToInt32(timer);
                    if ((iTime != 0) && (iTime != 100000))
                    {
                        laDlyDOList[ch - 1].Visible = true;
                        laDlyDOList[ch - 1].Text = timer;
                    }
                    else laDlyDOList[ch - 1].Visible = false;
                }));
            }
            catch
            { }
        }
        private void InitControls()
        {
            DOList.Add(lbD01);
            DOList.Add(lbD02);
            DOList.Add(lbD03);
            DOList.Add(lbD04);

            DOList.Add(lbD05);
            DOList.Add(lbD06);
            DOList.Add(lbD07);
            DOList.Add(lbD08);

            DOList.Add(lbD09);
            DOList.Add(lbD10);
            DOList.Add(lbD011);
            DOList.Add(lbD012);

            DOList.Add(lbD013);
            DOList.Add(lbD014);
            DOList.Add(lbD015);
            DOList.Add(lbD016);

         
            cbListCheck.Add(cbTimerDO1);
            tbListTimer.Add(tbTimerDO1);
            lstComboBox.Add(cbMode1);
            txtListWaitTime.Add(txtWaitTime1);
            lstSec.Add(lbSec1);
            lstWait.Add(lbWait1);
            laDOList.Add(laDO1);
            lstcbWait.Add(cbWait1);
            lstdlyTime.Add(labelDO1);

            cbListCheck.Add(cbTimerDO2);
            tbListTimer.Add(tbTimerDO2);
            lstComboBox.Add(cbMode2);
            txtListWaitTime.Add(txtWaitTime2);
            lstcbWait.Add(cbWait2);
            lstSec.Add(lbSec2);
            lstWait.Add(lbWait2);
            lstdlyTime.Add(labelDO2);
            laDOList.Add(laDO2);

            cbListCheck.Add(cbTimerDO3);
            tbListTimer.Add(tbTimerDO3);
            lstComboBox.Add(cbMode3);
            lstcbWait.Add(cbWait3);
            txtListWaitTime.Add(txtWaitTime3);
            laDOList.Add(laDO3);
            lstdlyTime.Add(labelDO3);
            lstSec.Add(lbSec3);
            lstWait.Add(lbWait3);

            lstcbWait.Add(cbWait4);
            cbListCheck.Add(cbTimerDO4);
            lstComboBox.Add(cbMode4);
            tbListTimer.Add(tbTimerDO4);
            txtListWaitTime.Add(txtWaitTime4);
            laDOList.Add(laDO4);
            lstdlyTime.Add(labelDO4);

            lstSec.Add(lbSec4);
            lstWait.Add(lbWait4);

            lstcbWait.Add(cbWait5);
            lstdlyTime.Add(labelDO5);
            cbListCheck.Add(cbTimerDO5);
            lstComboBox.Add(cbMode5); ;
            txtListWaitTime.Add(txtWaitTime5);
            tbListTimer.Add(tbTimerDO5);
            laDOList.Add(laDO5);

            lstSec.Add(lbSec5);
            lstWait.Add(lbWait5);

            lstcbWait.Add(cbWait6);
            cbListCheck.Add(cbTimerDO6);
            lstdlyTime.Add(labelDO6);
            lstComboBox.Add(cbMode6);
            txtListWaitTime.Add(txtWaitTime6);
            tbListTimer.Add(tbTimerDO6);
            laDOList.Add(laDO6);

            lstSec.Add(lbSec6);
            lstWait.Add(lbWait6);

            lstcbWait.Add(cbWait7);
            lstdlyTime.Add(labelDO7);
            cbListCheck.Add(cbTimerDO7);
            lstComboBox.Add(cbMode7);
            txtListWaitTime.Add(txtWaitTime7);
            tbListTimer.Add(tbTimerDO7);
            laDOList.Add(laDO7);
            lstSec.Add(lbSec7);
            lstWait.Add(lbWait7);

            lstcbWait.Add(cbWait8);
            cbListCheck.Add(cbTimerDO8);
            lstComboBox.Add(cbMode8);
            txtListWaitTime.Add(txtWaitTime8);
            tbListTimer.Add(tbTimerDO8);
            laDOList.Add(laDO8);
            lstSec.Add(lbSec8);
            lstWait.Add(lbWait8);
            lstdlyTime.Add(labelDO8);

            lstcbWait.Add(cbWait9);
            cbListCheck.Add(cbTimerDO9);
            lstComboBox.Add(cbMode9);
            txtListWaitTime.Add(txtWaitTime9);
            tbListTimer.Add(tbTimerDO9);
            laDOList.Add(laDO9);
            lstSec.Add(lbSec9);
            lstWait.Add(lbWait9);
            lstdlyTime.Add(labelDO9);

            lstcbWait.Add(cbWait10);
            cbListCheck.Add(cbTimerDO10);
            lstComboBox.Add(cbMode10);
            txtListWaitTime.Add(txtWaitTime10);
            tbListTimer.Add(tbTimerDO10);
            laDOList.Add(laDO10);
            lstSec.Add(lbSec10);
            lstWait.Add(lbWait10);
            lstdlyTime.Add(labelDO10);

            lstcbWait.Add(cbWait11);
            cbListCheck.Add(cbTimerDO11);
            lstComboBox.Add(cbMode11);
            txtListWaitTime.Add(txtWaitTime11);
            tbListTimer.Add(tbTimerDO11);
            laDOList.Add(laDO11);
            lstSec.Add(lbSec11);
            lstWait.Add(lbWait11);
            lstdlyTime.Add(labelDO11);

            lstcbWait.Add(cbWait12);
            cbListCheck.Add(cbTimerDO12);
            lstComboBox.Add(cbMode12);
            txtListWaitTime.Add(txtWaitTime12);
            tbListTimer.Add(tbTimerDO12);
            laDOList.Add(laDO12);
            lstSec.Add(lbSec12);
            lstWait.Add(lbWait12);
            lstdlyTime.Add(labelDO12);

            lstcbWait.Add(cbWait13);
            cbListCheck.Add(cbTimerDO13);
            txtListWaitTime.Add(txtWaitTime13);
            lstComboBox.Add(cbMode13);
            tbListTimer.Add(tbTimerDO13);
            laDOList.Add(laDO13);
            lstSec.Add(lbSec13);
            lstWait.Add(lbWait13);
            lstdlyTime.Add(labelDO13);

            lstcbWait.Add(cbWait14);
            cbListCheck.Add(cbTimerDO14);
            txtListWaitTime.Add(txtWaitTime14);
            lstComboBox.Add(cbMode14);
            tbListTimer.Add(tbTimerDO14);
            laDOList.Add(laDO14);
            lstdlyTime.Add(labelDO14);

            lstSec.Add(lbSec14);
            lstWait.Add(lbWait14);

            lstcbWait.Add(cbWait15);
            cbListCheck.Add(cbTimerDO15);
            lstComboBox.Add(cbMode15);
            txtListWaitTime.Add(txtWaitTime15);
            tbListTimer.Add(tbTimerDO15);
            laDOList.Add(laDO15);
            lstSec.Add(lbSec15);
            lstWait.Add(lbWait15);
            lstdlyTime.Add(labelDO15);

            lstcbWait.Add(cbWait16);
            cbListCheck.Add(cbTimerDO16);
            txtListWaitTime.Add(txtWaitTime16);
            lstComboBox.Add(cbMode16);
            tbListTimer.Add(tbTimerDO16);
            laDOList.Add(laDO16);
            lstSec.Add(lbSec16);
            lstWait.Add(lbWait16);
            lstdlyTime.Add(labelDO16);

            //不显示===
            foreach (TextBox tb in tbListTimer) tb.Visible = false;
            foreach (Label la in laDOList) la.Visible = false;
            //不打勾
            foreach (CheckBox cb in cbListCheck) { cb.Visible = false;cb.Checked = false;  }
            foreach (CheckBox item in lstcbWait)
            {
                //
                item.Visible = false;
            }
            foreach (var item in txtListWaitTime)
            {
                item.Visible = false;
            }
            foreach (var item in lstWait)
            {
                item.Visible = false;
            }
            foreach (var item in lstdlyTime)
            {
                item.Visible = false;
            }
            DIList.Add(lbDI1);
            DIList.Add(lbDI2);
            DIList.Add(lbDI3);
            DIList.Add(lbDI4);

            DIList.Add(lbDI5);
            DIList.Add(lbDI6);
            DIList.Add(lbDI7);
            DIList.Add(lbDI8);

            DIList.Add(lbDI9);
            DIList.Add(lbDI10);
            DIList.Add(lbDI11);
            DIList.Add(lbDI12);

            DIList.Add(lbDI13);
            DIList.Add(lbDI14);
            DIList.Add(lbDI15);
            DIList.Add(lbDI16);

            laDlyDOList.Add(laDlyDO1);
            laDlyDOList.Add(laDlyDO2);
            laDlyDOList.Add(laDlyDO3);
            laDlyDOList.Add(laDlyDO4);
            laDlyDOList.Add(laDlyDO5);
            laDlyDOList.Add(laDlyDO6);
            laDlyDOList.Add(laDlyDO7);
            laDlyDOList.Add(laDlyDO8);
            laDlyDOList.Add(laDlyDO9);
            laDlyDOList.Add(laDlyDO10);
            laDlyDOList.Add(laDlyDO11);
            laDlyDOList.Add(laDlyDO12);
            laDlyDOList.Add(laDlyDO13);
            laDlyDOList.Add(laDlyDO14);
            laDlyDOList.Add(laDlyDO15);
            laDlyDOList.Add(laDlyDO16);

            int i = 0;
            laDlyDO1.Visible = false;
            for (i = 1; i < laDlyDOList.Count; i++)
            {
                laDlyDOList[i].AutoSize = laDlyDO1.AutoSize;
                laDlyDOList[i].Visible = laDlyDO1.Visible;
                laDlyDOList[i].TextAlign = laDlyDO1.TextAlign;
                laDlyDOList[i].Location = laDlyDO1.Location;
                laDlyDOList[i].Size = laDlyDO1.Size;
            }
        }

        private void CheckDeviceThread()
        {
            int i = 0;
            int count = 0;

            while (KPComm.dicSocketIP.Count == 0) ;//还没有设备连接上时，即停留在此

            while (true)
            {
                try
                {
                    do
                    {
                        Thread.Sleep(100);
                        count = KPComm.dicSocketIP.Count;

                        Thread.Sleep(200);
                        if (count == KPComm._deviceName.Count)//如果设备的socket与设备名数量不一致，即需要再发设备名
                        {
                          
                        }
                        else
                        {
                            #region ===发送名称=======
                            lock (KPComm.INLineCheckLock)
                            {
                                count = KPComm.dicSocketIP.Count;
                                for (int index = 0; index < count; index++)
                                {
                                    if (KPComm._deviceName.Keys.Contains(KPComm.dicSocketIP[index]) == false)
                                    {
                                        KPComm.SendATName(KPComm.dicSocketIP[index]);//如果有设备没回设备名时，即不会显示
                                    }
                                }
                            }
                            #endregion
                        }

                        //当前选择线程为空时，即将显示清除
                        if (KPComm.currentSocket == null)
                        {
                            int[] DOStatus = new int[16];
                            int[] DIStatus = new int[16];
                            DIOShow(DOStatus, DIStatus);//初始化
                            for (int k =0; i < 16; k++)
                            {
                                ShowTime(k + 1,"100000" );
                            }
                        }
                    } while (count != KPComm._deviceName.Count);//如果条件成立，即一直循环

                    Thread.Sleep(100);
                }
                catch (Exception e)
                {
                    CFileManager.WriteLog("CheckDeviceThread:" + e.Message);
                }
            }
        }

        private void FrmMain2_FormClosing(object sender, FormClosingEventArgs e)
        {
            KPComm.ListenClose();
            //KPComm.ClearData();
            //关闭所有资源
            try
            {
                if (disThread == null)
                {
                    disThread.Abort();
                    CFileManager.WriteLog("退出：disThread");
                }

                if (sendThread == null)
                {
                    sendThread.Abort();
                    CFileManager.WriteLog("退出：sendThread");
                }

                if (readDOThread == null)
                {
                    readDOThread.Abort();
                    CFileManager.WriteLog("退出：readDOThread");
                }

                if (readDIThread == null)
                {
                    readDIThread.Abort();
                    CFileManager.WriteLog("退出：readDIThread");
                }

                if (mainThread == null)
                {
                    mainThread.Abort();
                    CFileManager.WriteLog("退出：mainThread");
                }
            }
            catch { }
            CFileManager.WriteLog("系统关闭。。。");
        }
        /// <summary>
        /// 清除资源
        /// </summary>
        private void ClearResources()
        {
            KPComm.currentSocket = null;
            KPComm.ClearData();
            KPComm.ListenClose();
        }
        private void RelayControl(int ch, int action)
        {
            if (KPComm.currentSocket == null)
            {
                MessageBox.Show("设备未连接成功");
                return;
            }
            int index = ch - 1;//取下标
            if (cbListCheck[index].Checked == true)
            {
                if (this.txtListWaitTime[index].Visible == true)
                {
                    KPComm.RelayControl(ch, action, tbListTimer[index].Text, this.txtListWaitTime[index].Text);//这个cbListCheck[ch].Checked = false屏蔽
                }
                else
                {
                    KPComm.RelayControl(ch, action, tbListTimer[index].Text);
              
                }
            } ////
            else if (this.txtListWaitTime[index].Visible == true)
            {
                KPComm.RelayControl(ch, action, tbListTimer[index].Text, this.txtListWaitTime[index].Text);
                cbListCheck[index].Checked = false;
            }
            else
            {
                KPComm.RelayControl(ch, action);
            }
        }
        
        private void btnExcute1_Click(object sender, EventArgs e)
        {
            int action = this.cbMode1.SelectedIndex;
            RelayControl(1, action);
        }

        private void btnExcute2_Click(object sender, EventArgs e)
        {
            int action = this.cbMode2.SelectedIndex;
            RelayControl(2, action);
        }

        private void btnExcute3_Click(object sender, EventArgs e)
        {
            int action = this.cbMode3.SelectedIndex;
            RelayControl(3, action);
        }

        private void btnExcute4_Click(object sender, EventArgs e)
        {
            int action = this.cbMode4.SelectedIndex;
            RelayControl(4, action);
        }

        private void btnExcute5_Click(object sender, EventArgs e)
        {
            int action = this.cbMode5.SelectedIndex;
            RelayControl(5, action);
        }

        private void btnExcute6_Click(object sender, EventArgs e)
        {
            int action = this.cbMode6.SelectedIndex;
            RelayControl(6, action);
        }

        private void btnExcute7_Click(object sender, EventArgs e)
        {
            int action = this.cbMode7.SelectedIndex;
            RelayControl(7, action);
        }

        private void btnExcute8_Click(object sender, EventArgs e)
        {
            int action = this.cbMode8.SelectedIndex;
            RelayControl(8, action);
        }

        private void btnExcute9_Click(object sender, EventArgs e)
        {
            int action = this.cbMode9.SelectedIndex;
            RelayControl(9, action);
        }

        private void btnExcute10_Click(object sender, EventArgs e)
        {
            int action = this.cbMode10.SelectedIndex;
            RelayControl(10, action);
        }

        private void btnExcute11_Click(object sender, EventArgs e)
        {
            int action = this.cbMode11.SelectedIndex;

            RelayControl(11, action);
        }

        private void btnExcute12_Click(object sender, EventArgs e)
        {
            //=======
            int action = this.cbMode12.SelectedIndex;
            RelayControl(12, 0);
        }

        private void btnExcute13_Click(object sender, EventArgs e)
        {
            int action = this.cbMode13.SelectedIndex;
            RelayControl(13, action);
        }

        private void btnExcute14_Click(object sender, EventArgs e)
        {
            int action = this.cbMode14.SelectedIndex;
            RelayControl(14, action);
        }

        private void btnExcute15_Click(object sender, EventArgs e)
        {
            int action = this.cbMode15.SelectedIndex;
            RelayControl(15, action);
        }

        private void btnExcute16_Click(object sender, EventArgs e)
        {
            int action = this.cbMode16.SelectedIndex;
            RelayControl(16, action);
        }

        public void tbTimerDO0_TextChanged(object sender, EventArgs e)
        {
            TextBox textBox = sender as TextBox;
            if (textBox.Text == "") return;
            if (CPublic.IsNumberic(textBox.Text) == false)
            {
                textBox.Text = "1";
                return;
            }

            if ((Convert.ToInt32(textBox.Text) > 99999) || (Convert.ToInt32(textBox.Text) < 1))
            {
                textBox.Text = "1";
            }

        }

        private void INLineCheck()
        {
            while (true)
            {
                try
                {
                    int sleep = KPComm.NetCheckTime;
                    if (sleep < 3) sleep = 3;
                    if(sleep>10) sleep = 3;
                    Thread.Sleep(sleep * 1000);//等待
                    int index = 0;
                    int count = KPComm.dicSocketIP.Count;
                    //在检索的时候，要给侦听上锁，列表更新也要上锁
                    lock (KPComm.INLineCheckLock)
                    {
                        for (index = count; index > 0; index--)
                        {
                            string ip = KPComm.dicSocketIP[index - 1];
                            KPComm.dicSocketTimer[ip].timerout++;//接收中清零
                            if (KPComm.dicSocketTimer[ip].timerout < 3) continue;//
                            CFileManager.WriteLog(ip + "连接断开");
                            //======================  jjj== ==============================
                            KPComm.dicSocket.Remove(ip);//减去数量

                            //==========================================================




                            if ((KPComm.currentSocket != null)
                                && (KPComm.currentSocket.RemoteEndPoint.ToString() == ip))
                            {
                                KPComm.currentSocket = null;
                            }
                            //达到5次即为断线，将与该连接相关的所有内容均要释放
                            KPComm._deviceName.Remove(KPComm.dicSocketIP[index - 1]);
                            KPComm.dicSocketIP.RemoveAt(index - 1);

                            if (KPComm.dicSocketTimer.Keys.Contains(ip) == true)
                            {
                                KPComm.dicSocketTimer.Remove(ip);
                            }
                            if (KPComm.dicSocket.Keys.Contains(ip) == true)
                            {
                                //先关闭线程
                                if (KPComm._threadMana.Keys.Contains(ip) == true)
                                {
                                    Thread thr = KPComm._threadMana[ip];
                                    thr.Abort();
                                    KPComm._threadMana.Remove(ip);
                                }
                                string addThName = ip + "D0";
                                if (KPComm._threadMana.Keys.Contains(addThName) == true)
                                {
                                    Thread thr = KPComm._threadMana[addThName];
                                    thr.Abort();
                                    KPComm._threadMana.Remove(addThName);

                                }
                                addThName = ip + "DI";
                                if (KPComm._threadMana.Keys.Contains(addThName) == true)
                                {
                                    Thread thr = KPComm._threadMana[addThName];
                                    thr.Abort();
                                    KPComm._threadMana.Remove(addThName);

                                }
                                //再关闭连接
                                KPComm.dicSocket[ip].Close();
                                KPComm.dicSocket.Remove(ip);
                            }

                            //============只有一个实例，刷新界面==========
                            KPComm.ClearData();

                            if (KPComm.gkcs.Keys.Contains(ip) == true)
                            {
                                KPComm.gkcs.Remove(ip);
                            }

                            if (KPComm.heartFlag.Keys.Contains(ip) == true)
                            {
                                KPComm.heartFlag.Remove(ip);
                            }
                        }
                    }
                }
                catch { }
            }
        }
        private void cbMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox comboBox = sender as ComboBox;
            string cbName = comboBox.Name;
            int index = -1;
            if (cbName.Length > 6)
            {
                cbName = cbName.Substring(6);
                bool isNum = int.TryParse(cbName, out index);
            }
            if (index>0)
            {
                index = index - 1;//取下标
            }
            if ( comboBox.SelectedIndex == 6|| comboBox.SelectedIndex == 5)//循环模式或者互锁点动
            {
                if (index>=0)
                {
                    this.txtListWaitTime[index].Visible = true;
                }
                txtListWaitTime[index].Text = "99999";

                cbListCheck[index].Enabled = false;//可以选择
                cbListCheck[index].Visible = true;
                cbListCheck[index].Checked = true;//可以选择
                //================增加等待字样============
                tbListTimer[index].Visible = true;
                tbListTimer[index].Text = "99999";
                lstSec[index].Visible = true;//单位
                lstcbWait[index].Visible = true;
                lstcbWait[index].Checked = true;
                lstcbWait[index].Enabled = false;
                lstWait[index].Visible = true;
                laDOList[index].Visible = true;
                lstdlyTime[index].Visible = true;

            }
            else if (comboBox.SelectedIndex == 4)//互锁延时
            {
                tbListTimer[index].Text = "100000";
                cbListCheck[index].Checked = false;
                cbListCheck[index].Visible = false;
                cbListCheck[index].Enabled = false;//可以选择
                if (index >=0)
                {
                    this.txtListWaitTime[index].Visible = true;
                }
                lstdlyTime[index].Visible = false;
                lstWait[index].Visible = true;
                lstcbWait[index].Visible = true;
                lstcbWait[index].Checked = true;
                lstcbWait[index].Enabled = false;
                lstSec[index].Visible = true;
                laDOList[index].Visible = false;
                txtListWaitTime[index].Text = "99999";
                tbListTimer[index].Visible = false;
            }
            else if (comboBox.SelectedIndex == 0 || comboBox.SelectedIndex == 1|| comboBox.SelectedIndex == 7)
            {
                cbListCheck[index].Checked = false;
                cbListCheck[index].Visible = false;
                cbListCheck[index].Enabled = false;//可以选择
                this.txtListWaitTime[index].Visible = false;
                this.tbListTimer[index].Visible = false;
                lstWait[index].Visible = false ;
                lstcbWait[index].Visible = false;
                laDOList[index].Visible = false;
                lstdlyTime[index].Visible = false;

                lstSec[index].Visible = false;
            }
            else //点动开，点动关  //23
            {
                if (index >= 0)
                {
                    this.txtListWaitTime[index].Visible = false;
                    cbListCheck[index].Checked = true;
                    cbListCheck[index].Visible = true;
                    cbListCheck[index].Enabled = false;//可以选择
                }
                //if (cbListCheck[index].Checked) { tbListTimer[index].Enabled = true; tbListTimer[index].Visible = true; }
                cbListCheck[index].Visible = true;
                txtListWaitTime[index].Text = "99999";
                lstSec[index].Visible = false;
                lstWait[index].Visible = false;
                lstcbWait[index].Visible = false;
                laDOList[index].Visible = true;
                lstdlyTime[index].Visible = true;
                tbListTimer[index].Visible = true;
                tbListTimer[index].Text = "99999";
            }
        }
        private void txtWaitTime1_MouseMove(object sender, MouseEventArgs e)
        {
            TextBox textbox=sender as TextBox;
            ToolTip tip = new ToolTip();
            tip.ShowAlways = true;
            tip.IsBalloon = true;
            tip.SetToolTip(textbox, "取值范围为：1-99999");
        }
        //====================在这里修改IP=================
        private void LocalIP_Click(object sender, EventArgs e)
        {
            int index = this.LocalIP.Text.IndexOf("：") +1;
            string oldStart= this.LocalIP.Text.Substring(0, index);
            string oldStr = this.LocalIP.Text.Substring(index);
            string str = Interaction.InputBox("请输入当前的电脑IP", "标题提示信息", oldStr, -1, -1);

            if(str!= oldStr&& KPComm.lstComputerIPs.Contains(str.Trim()))//============
            {
                this.LocalIP.Text = oldStart+str;
                ClearResources();//清除资源
                KPComm.hostIp = str;
                CheckAndListen();//str
            }
            //================================================
            if (!KPComm.lstComputerIPs.Contains(str.Trim()))
            {
                MessageBox.Show("您输入的电脑IP不存在当前电脑中，请重新操作");
            }

        }
     
        
    }
}
