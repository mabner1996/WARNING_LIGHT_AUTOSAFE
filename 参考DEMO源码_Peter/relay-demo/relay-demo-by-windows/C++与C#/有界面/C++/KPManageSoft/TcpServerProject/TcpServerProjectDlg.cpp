
// TcpServerProjectDlg.cpp: 实现文件
//
#include "pch.h"
#include "framework.h"
#include "TcpServerProject.h"
#include "TcpServerProjectDlg.h"
#include "afxdialogex.h"
#include "string.h"
#include<windows.h>
#include<stdio.h>
#include <winbase.h>

#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "stdtostring.h"
#include "RrConfig.h" 
#endif
#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 int connectNum = 0;
 bool isRed = false;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	//CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif



	// 实现
protected:
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedButton11();
};



BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton11)
END_MESSAGE_MAP()


// CTcpServerProjectDlg 对话框
CTcpServerProjectDlg::CTcpServerProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPSERVERPROJECT_DIALOG, pParent)
	, m_Data(_T(""))
	, m_Data1(_T(""))
	, m_Data2(_T("●"))
	, m_bGreen(FALSE)
	, m_bGreen2(FALSE)
	, m_bGreen3(FALSE)
	, m_bGreen4(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CTcpServerProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cbMode);
	DDX_Control(pDX, IDC_STATIC6, labDO1);

	DDX_Text(pDX, lbD01, m_Data2);//===============

	DDX_Text(pDX, laDlyDO1, m_Data1);//====================
	DDX_Text(pDX, txtWaitTime1, m_Data);

	//DDX_Control(pDX, lbD02, Test2);//

	//DDX_Control(pDX, lbD01, m_staticTest);// 关联控件和变量
}

BEGIN_MESSAGE_MAP(CTcpServerProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//ON_CBN_SETFOCUS(IDC_COMBO1, &CTcpServerProjectDlg::OnCbnSetfocusCombo1)


	ON_MESSAGE(WM_MY_MESSAGE, OnDisplayChange)//==================增加处
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	
	//
	ON_CONTROL_RANGE(BN_CLICKED, btnExcute1, btnExcute16, OnButtonClicked)//增加处 连号的buttton3 和 button4  //====================

	ON_CONTROL_RANGE(CBN_SETFOCUS, cbMode1, cbMode16, OnCbnSetfocusComboxAny)//增加处 连号的buttton3 和 button4=== 首先注册绑定事件  //====================

	ON_CONTROL_RANGE(CBN_SELCHANGE, cbMode1, cbMode16, OnCbnSelchangeComboxAny)//增加处 连号的buttton3 和 button4=== 首先注册绑定事件  //====================

	ON_BN_CLICKED(IDC_BUTTON1, &CTcpServerProjectDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_STN_CLICKED(lbServerIP, &CTcpServerProjectDlg::OnStnClickedlbserverip)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LPVOID hWnd;

// CTcpServerProjectDlg 消息处理程序
//CString GetServerIp();
char*  GetServerIp();
SOCKET sockServer;

void SetD0DISize(LPVOID pParam);


BOOL CTcpServerProjectDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	rr::RrConfig config;
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);//显示最大值  ====将窗体设置为最小值
	SetTimer(0, 1000, NULL);//设置定时器显示时间显示到标题栏
	//ShowWindow(SW_MINIMIZE);//显示最小值===初始化
	OnCbnSetfocusComboxAny(cbMode1);
	OnCbnSetfocusComboxAny(cbMode2);
	OnCbnSetfocusComboxAny(cbMode3);
	OnCbnSetfocusComboxAny(cbMode4);
	OnCbnSetfocusComboxAny(cbMode5);
	OnCbnSetfocusComboxAny(cbMode6);
	OnCbnSetfocusComboxAny(cbMode7);
	OnCbnSetfocusComboxAny(cbMode8);
	OnCbnSetfocusComboxAny(cbMode9);
	OnCbnSetfocusComboxAny(cbMode10);
	OnCbnSetfocusComboxAny(cbMode11);
	OnCbnSetfocusComboxAny(cbMode12);
	OnCbnSetfocusComboxAny(cbMode13);
	OnCbnSetfocusComboxAny(cbMode14);
	OnCbnSetfocusComboxAny(cbMode15);
	OnCbnSetfocusComboxAny(cbMode16);
	hWnd = this;
	//创建
	WSADATA wsaData;
	int nRet1;
	if ((nRet1 = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup failed\n");
		exit(0);
	}

	char*   txtIp;//获取到char*类型的IP //=============
	txtIp = GetServerIp();
	CStatic* lbServerIPText;
	std::string HostIP = "";
	int serverPort = 6000;
	char szModuleFilePath[MAX_PATH];
	char SaveResult[MAX_PATH];
	int n = GetModuleFileNameA(0, szModuleFilePath, MAX_PATH); //获得当前执行文件的路径
	szModuleFilePath[strrchr(szModuleFilePath, '\\') - szModuleFilePath + 1] = 0;//将最后一个"\\"后的字符置为0
	strcpy(SaveResult, szModuleFilePath);
	strcat(SaveResult, "\config.ini");//在当前路径后添加子路径===拼接
	//=============    =======================
	CString strChange(SaveResult);
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(strChange);//
	if (!ifFind)
	{
		//=================  ==================================
		CString   cHostIPstr = _T("127.0.0.1");
		if (txtIp != NULL)
		{
			cHostIPstr = txtIp;
		}
		WritePrivateProfileStringW(_T("MYConfig"), _T("HostIP"), cHostIPstr, strChange);//"192.168.1.269"
		WritePrivateProfileStringW(_T("MYConfig"), _T("Port"), _T("6000"), strChange);
	}
	else
	{
		CString strHostIP;
		GetPrivateProfileString(_T("MYConfig"), _T("HostIP"), CString(), strHostIP.GetBuffer(MAX_PATH), MAX_PATH, strChange);
		if (strHostIP == _T("127.0.0.1"))
		{
		  //AfxMessageBox("IP地址未输入，请退出软件重新进入");
		}
		HostIP = CT2A(strHostIP.GetBuffer());
		CString strPort;//
		GetPrivateProfileString(_T("MYConfig"), _T("Port"), CString("6000"), strPort.GetBuffer(MAX_PATH), MAX_PATH, strChange);
		std::string  portNumber = CT2A(strPort.GetBuffer());//
		serverPort = atoi(portNumber.c_str());
	}
	if (HostIP != "")//如果为空//
	{
		txtIp = new char[HostIP.length() + 1];
		strcpy(txtIp, HostIP.c_str());
	}
	lbServerIPText = (CStatic*)GetDlgItem(lbServerIP);
	CString str = _T("服务器IP:");
	//============        ===================
	CString str5(txtIp);//
	//=================   
	str = str + str5;
	std::string ss;

	std::string sb;
	//===============
	string str1(txtIp);
	ss.append(str1);

	connectNum = 0;

	LPCSTR  myCStr = "服务器IP:";
	CString str2(myCStr);
	

  lbServerIPText->SetWindowText(str);

  //=========设置端口号===========

   CString strPortHead = _T("端口号:");//端口号
   CString strPort;
   strPort.Format(_T("%d"), serverPort);
   strPort = strPortHead + strPort;
	CStatic* lbPortText;
	lbPortText = (CStatic*)GetDlgItem(lbPort);
	lbPortText->SetWindowText(strPort);
    //创建套接字
	sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockServer == INVALID_SOCKET) 
	{
		//创建服务器监听套接字IPPROTO_TCP 
		printf("socket创建失败");//
		exit(1);//====
	}
	

	//2.绑定端口
	sockaddr_in siServer;
	siServer.sin_family = AF_INET;
	siServer.sin_port = htons(serverPort);//端口
	//======================
	std::string serverIp = txtIp;                  //"192.168.1.27"; 设置服务器的IP

	siServer.sin_addr.S_un.S_addr = inet_addr(serverIp.c_str());

	int nRet = bind(sockServer, (sockaddr*)&siServer, sizeof(siServer));
	if (WSAGetLastError() == WSAEADDRINUSE)
	{
		//============端口已被占用===========//
		isRed = true;
		CString str;
		GetDlgItem(lbPort)->GetWindowText(str);
		CStatic* lbPortText;
		lbPortText = (CStatic*)GetDlgItem(lbPort);
		lbPortText->SetWindowText(str);
	}
	if (nRet == SOCKET_ERROR)
	{
		DWORD dwErr = GetLastError();
		printf("端口绑定失败\n");
		BOOL bDontLinger = FALSE;
		if (sockServer != INVALID_SOCKET)//防止关闭多次====
		{
			closesocket(sockServer);//关闭socket通讯
			sockServer = INVALID_SOCKET;//====
		}
		return 0;
	}
	//3.==================监听=======================
	nRet = listen(sockServer, SOMAXCONN);//监听最大值
	if (nRet == SOCKET_ERROR)
	{
		if (sockServer != INVALID_SOCKET)//防止关闭多次====
		{
			closesocket(sockServer);
			sockServer = INVALID_SOCKET;//======
		}
		printf("监听失败\n");//============
		return 0;
	}
	//======================   ===================
	//4.接受连接
	DWORD WINAPI Accept_Thread(LPVOID pParam);

   HANDLE Accept_ThreadTest= CreateThread(NULL, 0, Accept_Thread, (LPVOID)sockServer, 0, NULL);//用来收发数据的线程
   CloseHandle(Accept_ThreadTest);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


DWORD WINAPI Accept_Thread(LPVOID pParam)
{
	
	sockaddr_in siClient;
	int nSize = sizeof(siClient);//

	SOCKET sockServer = (SOCKET)(pParam);
	while (true)
	{
		SOCKET sockClient = accept(sockServer, (sockaddr*)&siClient, &nSize);

		if (sockClient == SOCKET_ERROR)
		{
			return false;
		}
		connectNum ++;
		printf("IP:%s port:%d 连接到服务器. \r\n",
			inet_ntoa(siClient.sin_addr),
			ntohs(siClient.sin_port));
		DWORD WINAPI HandleClientThread(LPVOID p);
		DWORD WINAPI ReadDOCmdThread(LPVOID p);
		DWORD WINAPI ReadDICmdThread(LPVOID p);
		HANDLE DOCmdTread = CreateThread(NULL, 0, ReadDOCmdThread, (LPVOID)sockClient, 0, NULL);//用来收发数据的线程
		CloseHandle(DOCmdTread);
		HANDLE DICmdTread = CreateThread(NULL, 0, ReadDICmdThread, (LPVOID)sockClient, 0, NULL);//用来收发数据的线程
		CloseHandle(DICmdTread);

		HANDLE hTread = CreateThread(NULL, 0, HandleClientThread, (LPVOID)sockClient, 0, NULL);//用来收发数据的线程
		CloseHandle(hTread);
	}
}
char* GetServerIp()
{
	char szText[256];
	//获取本机主机名称
	int iRet;
	iRet = gethostname(szText, 256);
	int a = WSAGetLastError();
	if (iRet != 0)
	{
		printf("gethostname() Failed!");
		return FALSE;
	}
	//通过主机名获取到地址信息
	HOSTENT *host = gethostbyname(szText);
	if (NULL == host)
	{
		printf("gethostbyname() Failed!");
		return false;
	}
	in_addr PcAddr;
	for (int i = 0;;i++)
	{
		char *p = host->h_addr_list[i];
		if (NULL == p)
		{
			break;
		}
		memcpy(&(PcAddr.S_un.S_addr), p, host->h_length);
		char*szIP = ::inet_ntoa(PcAddr);
		return szIP;
	}
	
}

SOCKET sockClient = NULL;

int DOStatus[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int  DIStatus[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int ArrDlyTime[16] = {100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000 };
//
UINT CommThread(LPVOID pParam);
void GetD0DIDlyStatus();
void ClearResource();
//5.创建一个线程用来收发数据
DWORD WINAPI HandleClientThread(LPVOID pParam)
{
	sockClient = (SOCKET)pParam;
	while (true)
	{
		//5) 收发数据
		char aryBuff[MAXWORD] = { 0 };
		struct timeval timeout = { 3,0 };
		fd_set rdfs;
		FD_ZERO(&rdfs);
		FD_SET(sockClient, &rdfs);
		int  recvlen;
		int res = select(sockClient + 1, &rdfs, NULL, NULL, &timeout);//select  模型
		if (res > 0)//进入此处
		{
			printf("socket connected\n");
			recvlen = recv(sockClient, aryBuff, sizeof(aryBuff), 0);
			if (recvlen > 0)
			{
				//连接
			}
			else if (recvlen < 0)//
			{
				if (errno == EINTR)
				{
					printf("socket connected\n");
				}
				else 
				{
					connectNum--;
					ClearResource();
					sockClient = NULL;//
					return 0;
				}
			}
			else if (recvlen == 0) {
				connectNum--;
				ClearResource();
				sockClient = NULL;
				return 0;
			}
		}
		else if (res == 0) 
		{
		
			
		}
		else if (res < 0)
		{
			if (errno == EINTR)
			{
				//return 1;//连接
			}
			else {
				ClearResource();
				connectNum--;
				sockClient = NULL;
				return 0;
			}
		}
			printf("收到数据: %s \r\n", aryBuff);
			string atCmd;
			stringstream stream;
			stream << aryBuff;
			atCmd = stream.str();

			string::size_type idxa;
			string::size_type idxb;
			string a = "STACH";
			string b = "OCCH";

			idxa = atCmd.find(a);//在a中查找b.
			idxb = atCmd.find(b);//在a中查找b.

			if (idxa != string::npos || idxb != string::npos)//不等于空
			{
				//计算‘+’出现次数
				int i = 0;
				int count = 0;

				string c = "+";//赋值
				//string::size_type idxc;//下标
				//idxb = str.find(b);//在a中查找b.
				int index = atCmd.find_last_of(c);

				//求 + 的出现次数
				while (atCmd.find(c, i) >= 0 && i != index + 1)//防止死循环
				{
					i = atCmd.find(c, i) + 1;
					count++; //
				};
				//AfxMessageBox("成功！");
				TRACE("进来操作函数");//
				//trace("进来操作函数");
				for (int index = 0; index < count; index++)
				{
					if (atCmd.find("+STACH", 0) == 0)
					{
						string ch = atCmd.substr(atCmd.find("STACH") + 5,
							atCmd.find(":") - (atCmd.find("STACH") + 5));

						string stastr = "";
						string timerstr = "";
						//===========================
						if (atCmd.find(",") - atCmd.find(":") == 2)//检查是否带时间
						{
							stastr = atCmd.substr(atCmd.find(":") + 1,
								atCmd.find(",") - (atCmd.find(":") + 1));

							timerstr = atCmd.substr(atCmd.find(",") + 1,
								atCmd.find("\r") - (atCmd.find(",") + 1));
							//================================================
							int int_temp;
							stringstream stream(ch);
							stream >> int_temp;
							//==================================================
							int int_timerstr;
							stringstream streamSta(timerstr);
							streamSta >> int_timerstr;//========================
							ArrDlyTime[int_temp - 1] = int_timerstr;

						}
						else
						{
							stastr = atCmd.substr(atCmd.find(":") + 1,
								atCmd.find("\r") - (atCmd.find(":") + 1));
						}
						//=================  ==============================
						int int_temp;
						stringstream stream(ch);
						stream >> int_temp;

						int int_stastr;
						stringstream streamSta(stastr);
						streamSta >> int_stastr;
						DOStatus[int_temp - 1] = int_stastr;              //Convert.ToByte(stastr);
					}
					else if (atCmd.find("+OCCH_ALL") == 0)
					{


						int* ch = new int[16];
						int ch_index = 0;

						string atStr = atCmd.substr(atCmd.find("+OCCH_ALL:") + 10,
							atCmd.find("\n", 0) + 1 - (atCmd.find("+OCCH_ALL:") + 10));

						do
						{
							string strch = atStr.substr(0, 1);

							int int_temp = -1;
							stringstream stream(strch);
							stream >> int_temp;//

							ch[ch_index++] = int_temp;
							int int_strch;
							stringstream streamSta(strch);
							streamSta >> int_strch;
							DIStatus[ch_index - 1] = int_strch;//赋值


							if (atStr.length() > 3)
							{
								atStr = atStr.substr(atStr.find(",") + 1,
									atStr.length() - (atStr.find(",") + 1));
							}
							else
							{
								break;
							}
						} while ((atCmd.length() > 3) && (ch_index < 16));
					}
					else if (atCmd.find("+OCCH", 0) == 0)
					{
						string ch = atCmd.substr(atCmd.find("OCCH") + 4,
							atCmd.find(":") - (atCmd.find("OCCH") + 4));

						string stastr = atCmd.substr(atCmd.find(":") + 1,
							atCmd.find("\r") - (atCmd.find(":") + 1));


						int int_ch;
						stringstream stream(ch);
						stream >> int_ch;

						int int_stastr;
						stringstream streamSta(stastr);
						streamSta >> int_stastr;

						int chNum = int_ch;

						DIStatus[chNum - 1] = int_stastr;
					}
					//每次要减去已使用的部分
					if (index < (count - 1))
					{
						int index_n = atCmd.find("\n", 0) + 1;
						int len = atCmd.length() - index_n;//=========
						atCmd = atCmd.substr(index_n,
							atCmd.length() - index_n);
					}
				}
				GetD0DIDlyStatus();

			}
		
	}
	return 0;
}

//============================== 清除资源==========================
void ClearResource() 
{
	for (size_t i = 0; i < 16; i++)
	{
		DOStatus[i] = 0;
		DIStatus[i] = 0;
		ArrDlyTime[i] = 100000;
	}
	GetD0DIDlyStatus();
	sockClient = NULL;
}

 void GetD0DIDlyStatus()
{
    //============   ==========
	CTcpServerProjectDlg *pDlg = (CTcpServerProjectDlg*)hWnd;//===========指针==========
	for (int k = 0; k < 16; k++)
	{
		CStatic* laDlyDO;
		CStatic* lbD0;//输出
		CStatic* lbDI;//输入
		int a1 = ArrDlyTime[k];
		int dO = DOStatus[k];
		int dI = DIStatus[k];
		CString str;
		str.Format(_T("%d"), a1);
		switch (k)
		{
		case 0:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO1);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD01);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI1);
			break;//
		}
		case 1:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO2);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD02);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI2);
			break;
		}
		case 2:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO3);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD03);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI3);
			break;
		}
		case 3:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO4);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD04);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI4);
			break;
		}
		case 4:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO5);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD05);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI5);
			break;
		}
		case 5:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO6);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD06);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI6);
			break;
		}
		case 6:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO7);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD07);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI7);
			break;
		}
		case 7:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO8);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD08);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI8);
			break;
		}
		case 8:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO9);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD09);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI9);
			break;
		}
		case 9:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO10);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD010);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI10);
			break;
		}
		case 10:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO11);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD011);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI11);
			break;
		}
		case 11:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO12);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD012);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI12);
			break;
		}
		case 12:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO13);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD013);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI13);
			break;
		}
		case 13:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO14);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD014);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI14);
			break;
		}
		case 14:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO15);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD015);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI15);
			break;
		}
		case 15:
		{
			laDlyDO = (CStatic*)pDlg->GetDlgItem(laDlyDO16);
			lbD0 = (CStatic*)pDlg->GetDlgItem(lbD016);
			lbDI = (CStatic*)pDlg->GetDlgItem(lbDI15);
			break;
		}
		}
		lbDI->SetWindowText(_T("●"));
		lbD0->SetWindowText(_T("●"));
		if (a1 == 100000)
		{
			laDlyDO->ShowWindow(FALSE);//=====不显示延时时间===============
		}
		else
		{
			laDlyDO->ShowWindow(TRUE);
			laDlyDO->SetWindowText(str);//====设置文字显示====
			//====================
		}
	}
	//========================
	//CWinThread *hThread = AfxBeginThread(CommThread, hWnd);
}

int GetWndFromPid(char * proName)
{
	return 0;
	//return int(GetWndHwnd(proName));
}
//===========2020-09-26===========================================CTcpServerProjectDlg::==================
UINT  CommThread(LPVOID pParam)
{
	CTcpServerProjectDlg *pDlg = (CTcpServerProjectDlg*)pParam;//===========指针==========
	while (true)
	{
		//============测试==================================================
		::PostMessage(pDlg->m_hWnd, WM_MY_MESSAGE, 0, 0);//线程向对话框发送自定义消息
      //::PostMessage(pDlg->m_hWnd, WM_CTLCOLOR, 0, 0);//线程向对话框发送自定义消息
		Sleep(600);

	}
	return 0;
}








//===============================2020-09-26===============================
LRESULT CTcpServerProjectDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	//更新控件
	//SetDlgItemText(lbD01, m_Data2);//用于更新界面中的控件
	return 0;
}


//=====控制代码=========================
void RelayControl(int ch, int action, const std::string  strTimer, const std::string waitTimer)
{
	//============
	while (sockClient == NULL);
	//================测试发送命令==================
	std::string strCmd = "AT+STACH";//==============
	int nRet = 0;
	strCmd.append(std::to_string(ch));//=============

	//============ 拼接符号 ==============================
	std::string strOperator = "=";//====
	strCmd.append(std::to_string(strOperator));

	//=============拼接动作 ==========================
	strCmd.append(std::to_string(action));
	std::string strSign = ",";
	//判断时候为空
	if (!strTimer.empty())
	{
		strCmd.append(strSign);
		strCmd.append(strTimer);
	}
	//==========添加等待时间==============================
	if (!waitTimer.empty())
	{
		strCmd.append(strSign);
		strCmd.append(waitTimer);
	}
	//=================命令符结尾 ================
	std::string strEnd = "\r\n";//=========
	strCmd.append(std::to_string(strEnd));//拼接动作

	//std::cout << strCmd << std::endl;
	const char * sendData;
	//================  ==============================
	sendData = strCmd.c_str();
	nRet = send(sockClient, sendData, strlen(sendData), 0);//接收====
	if (nRet == SOCKET_ERROR)
	{
		printf("数据发送失败 \r\n");
	}
}
//=======创建另外的一条线程来读取========================

DWORD WINAPI ReadDOCmdThread(LPVOID pParam)
{
	//=============================================
	char atCmd[] = { "AT+STACH0=?\r\n" };//AT指令
	while (sockClient = NULL);
	sockClient = (SOCKET)pParam;
	while (true)
	{
		if(sockClient!=0)
		int nRet = send(sockClient, atCmd, sizeof(atCmd), 0);
		Sleep(300);
	}

}



//========================  ================================
DWORD WINAPI ReadDICmdThread(LPVOID pParam)
{
	//=============================================
	char atCmd[] = { "AT+OCCH0=?\r\n" };//AT指令
	while (sockClient == NULL);
	while (true)
	{
		int nRet = send(sockClient, atCmd, sizeof(atCmd), 0);
		Sleep(300);
	}
}




void CTcpServerProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{


	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。 
//  这将由框架自动完成。

void CTcpServerProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTcpServerProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//=======设置Combox========
void CTcpServerProjectDlg::SetDefaultCombox()
{
	CTcpServerProjectDlg *pDlg = (CTcpServerProjectDlg*)hWnd;
	CComboBox* cmbMode1 = (CComboBox*)GetDlgItem(cbMode1);
	cmbMode1->SetCurSel(0);//================

}


void  CTcpServerProjectDlg::OnButtonClicked(UINT  uId)
{
	int chNum = 0;
	int iPos = 0;//Action 动作
	CEdit* tbTimer;
	CEdit* txtWaitTime;
	switch (uId)

	{
	case btnExcute1:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode1))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO1);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime1);
		chNum = 1;
	}
	break;
	case btnExcute2:
	{

		iPos = ((CComboBox*)GetDlgItem(cbMode2))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO2);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime2);
		chNum = 2;
		break;
	}

	case btnExcute3:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode3))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO3);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime3);
		chNum = 3;
		break;
	}

	case btnExcute4:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode4))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO4);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime4);
		chNum = 4;
		break;
	}
	case btnExcute5:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode5))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO5);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime5);
		chNum = 5;
		break;
	}
	case btnExcute6:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode6))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO6);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime6);
		chNum = 6;
		break;
	}
	case btnExcute7:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode7))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO7);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime7);
		chNum = 7;
		break;
	}
	case btnExcute8:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode8))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO8);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime8);
		chNum = 8;
		break;
	}
	case btnExcute9:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode9))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO9);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime9);
		chNum = 9;
		break;
	}
	case btnExcute10:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode10))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO10);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime10);
		chNum = 10;
		break;
	}
	case btnExcute11:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode11))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO11);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime11);
		chNum = 11;
		break;
	}
	case btnExcute12:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode12))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO12);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime12);
		chNum = 12;
		break;
	}
	case btnExcute13:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode13))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO13);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime13);
		chNum = 13;
		break;
	}
	case btnExcute14:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode14))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO14);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime14);
		chNum = 14;
		break;
	}
	case btnExcute15:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode15))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO15);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime15);
		chNum = 15;
		break;
	}

	case btnExcute16:
	{
		iPos = ((CComboBox*)GetDlgItem(cbMode16))->GetCurSel();//当前选中的行
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO16);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime16);
		chNum = 16;
		break;
	}
	}
	CString str;//===================
	tbTimer->GetWindowText(str);
	std::string strTimer = CT2A(str.GetBuffer());
	CString str2;//===========================
	txtWaitTime->GetWindowText(str2);//获取到里面的值
	std::string waitTimer = CT2A(str2.GetBuffer());
	if (sockClient == NULL)
	{
		::MessageBox(NULL, _T("当前设备未连接"), _T("操作错误"), MB_OK);
		return;
	}
	RelayControl(chNum, iPos, strTimer, waitTimer);
}
//==============   =================================================
void  CTcpServerProjectDlg::OnCbnSelchangeComboxAny(UINT  uId)
{
	//======================               ======================
	CStatic* labelDO; //延时
	CEdit* tbTimer;
	CStatic* laDO;
	CStatic* lbWait;//等待
	CEdit* txtWaitTime; //等待文本框
	CStatic* lbSec; //秒

	CStatic* laDlyDO; //显示时间
	
	int iPos = ((CComboBox*)GetDlgItem(uId))->GetCurSel();//当前选中的行
	switch (uId)
	{
	case cbMode1:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO1);
		labelDO = (CStatic*)GetDlgItem(labelDO1);
		laDO = (CStatic*)GetDlgItem(laDO1);
		lbWait = (CStatic*)GetDlgItem(lbWait1);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime1);
		lbSec = (CStatic*)GetDlgItem(lbSec1);//
		laDlyDO= (CStatic*)GetDlgItem(laDlyDO1);//
	}
	break;
	case cbMode2:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO2);
		labelDO = (CStatic*)GetDlgItem(labelDO2);
		laDO = (CStatic*)GetDlgItem(laDO2);
		lbWait = (CStatic*)GetDlgItem(lbWait2);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime2);
		lbSec = (CStatic*)GetDlgItem(lbSec2);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO2);//
		break;
	}


	case cbMode3:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO3);
		labelDO = (CStatic*)GetDlgItem(labelDO3);
		laDO = (CStatic*)GetDlgItem(laDO3);
		lbWait = (CStatic*)GetDlgItem(lbWait3);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime3);
		lbSec = (CStatic*)GetDlgItem(lbSec3);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO3);//
		break;
	}

	case cbMode4:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO4);
		labelDO = (CStatic*)GetDlgItem(labelDO4);
		laDO = (CStatic*)GetDlgItem(laDO4);
		lbWait = (CStatic*)GetDlgItem(lbWait4);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime4);//======
		lbSec = (CStatic*)GetDlgItem(lbSec4);//=======
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO4);//

		break;
	}
	case cbMode5:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO5);
		labelDO = (CStatic*)GetDlgItem(labelDO5);
		laDO = (CStatic*)GetDlgItem(laDO5);
		lbWait = (CStatic*)GetDlgItem(lbWait5);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime5);
		lbSec = (CStatic*)GetDlgItem(lbSec51);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO5);//
		break;
	}
	case cbMode6:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO6);
		labelDO = (CStatic*)GetDlgItem(labelDO6);
		laDO = (CStatic*)GetDlgItem(laDO6);
		lbWait = (CStatic*)GetDlgItem(lbWait6);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime6);
		lbSec = (CStatic*)GetDlgItem(lbSec61);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO6);//
		break;
	}
	case cbMode7:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO7);
		labelDO = (CStatic*)GetDlgItem(labelDO7);
		laDO = (CStatic*)GetDlgItem(laDO7);
		lbWait = (CStatic*)GetDlgItem(lbWait7);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime7);
		lbSec = (CStatic*)GetDlgItem(lbSec71);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO7);//

		break;
	}
	case cbMode8:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO8);
		labelDO = (CStatic*)GetDlgItem(labelDO8);
		laDO = (CStatic*)GetDlgItem(laDO8);
		lbWait = (CStatic*)GetDlgItem(lbWait8);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime8);
		lbSec = (CStatic*)GetDlgItem(lbSec81);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO8);//
		break;
	}
	case cbMode9:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO9);
		labelDO = (CStatic*)GetDlgItem(labelDO9);
		laDO = (CStatic*)GetDlgItem(laDO9);
		lbWait = (CStatic*)GetDlgItem(lbWait9);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime9);
		lbSec = (CStatic*)GetDlgItem(lbSec91);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO9);//
		break;
	}
	case cbMode10:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO10);
		labelDO = (CStatic*)GetDlgItem(labelDO10);
		laDO = (CStatic*)GetDlgItem(laDO10);
		lbWait = (CStatic*)GetDlgItem(lbWait10);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime10);
		lbSec = (CStatic*)GetDlgItem(lbSec101);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO10);//
		break;
	}
	case cbMode11:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO11);
		labelDO = (CStatic*)GetDlgItem(labelDO11);
		laDO = (CStatic*)GetDlgItem(laDO11);
		lbWait = (CStatic*)GetDlgItem(lbWait11);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime11);
		lbSec = (CStatic*)GetDlgItem(lbSec111);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO11);//
		break;
	}
	case cbMode12:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO12);
		labelDO = (CStatic*)GetDlgItem(labelDO12);
		laDO = (CStatic*)GetDlgItem(laDO12);
		lbWait = (CStatic*)GetDlgItem(lbWait12);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime12);
		lbSec = (CStatic*)GetDlgItem(lbSec121);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO12);//
		break;
	}
	case cbMode13:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO13);
		labelDO = (CStatic*)GetDlgItem(labelDO13);
		laDO = (CStatic*)GetDlgItem(laDO13);
		lbWait = (CStatic*)GetDlgItem(lbWait13);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime13);
		lbSec = (CStatic*)GetDlgItem(lbSec131);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO13);//
		break;
	}
	case cbMode14:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO14);
		labelDO = (CStatic*)GetDlgItem(labelDO14);
		laDO = (CStatic*)GetDlgItem(laDO14);
		lbWait = (CStatic*)GetDlgItem(lbWait14);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime14);
		lbSec = (CStatic*)GetDlgItem(lbSec141);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO14);//
		break;
	}
	case cbMode15:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO15);
		labelDO = (CStatic*)GetDlgItem(labelDO15);
		laDO = (CStatic*)GetDlgItem(laDO15);
		lbWait = (CStatic*)GetDlgItem(lbWait15);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime15);
		lbSec = (CStatic*)GetDlgItem(lbSec151);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO15);
		break;
	}
	case cbMode16:
	{
		tbTimer = (CEdit*)GetDlgItem(tbTimerDO16);
		labelDO = (CStatic*)GetDlgItem(labelDO16);
		laDO = (CStatic*)GetDlgItem(laDO16);
		lbWait = (CStatic*)GetDlgItem(lbWait16);
		txtWaitTime = (CEdit*)GetDlgItem(txtWaitTime16);
		lbSec = (CStatic*)GetDlgItem(lbSec161);//
		laDlyDO = (CStatic*)GetDlgItem(laDlyDO16);
		break;
	}

	}
	if (iPos == 1 || iPos == 0 || iPos == 7)
	{

		//===================================================================
	
		  //设置隐藏
		tbTimer->SetWindowText(_T(""));
		tbTimer->ShowWindow(FALSE); //隐藏该控件
		txtWaitTime->ShowWindow(FALSE); //隐藏该控件
		txtWaitTime->SetWindowText(_T("")); //隐藏该控件
		labelDO->ShowWindow(FALSE); //隐藏该控件
		laDlyDO->ShowWindow(FALSE);//
		laDO->ShowWindow(FALSE);//

		lbWait->ShowWindow(FALSE);//设置===是否=====显示

		lbSec->ShowWindow(FALSE);


	}//=============
	else if (iPos == 2 || iPos == 3)//=====
	{
		tbTimer->SetWindowText(_T("99999"));
		tbTimer->ShowWindow(TRUE); 
		labelDO->ShowWindow(TRUE);
		laDO->ShowWindow(TRUE);//

		lbWait->ShowWindow(FALSE);//
		txtWaitTime->ShowWindow(FALSE); //隐藏该控件
		txtWaitTime->SetWindowText(_T("")); //隐藏该控件
		lbSec->ShowWindow(FALSE);


	}//============================
	else if (iPos == 5 || iPos == 6)
	{
		tbTimer->SetWindowText(_T("99999"));
		tbTimer->ShowWindow(TRUE); //隐藏该控件
		labelDO->ShowWindow(TRUE); //隐藏该控件
		laDO->ShowWindow(TRUE);//


		lbWait->ShowWindow(TRUE);
		txtWaitTime->ShowWindow(TRUE); 
		txtWaitTime->SetWindowText(_T("99999"));
		lbSec->ShowWindow(TRUE);

	}
	else
	{
		tbTimer->SetWindowText(_T(""));
		tbTimer->ShowWindow(FALSE); 
		labelDO->ShowWindow(FALSE); 
		laDO->ShowWindow(FALSE);//

		lbWait->ShowWindow(TRUE);//设置===是否=====显示
		txtWaitTime->ShowWindow(TRUE); //隐藏该控件
		txtWaitTime->SetWindowText(_T("99999")); //隐藏该控件
		lbSec->ShowWindow(TRUE);

	}

}

void  CTcpServerProjectDlg::OnCbnSetfocusComboxAny(UINT  uId)
{
	CComboBox* cbModeNew;//====
	cbModeNew = (CComboBox*)GetDlgItem(uId);
	int number = 0;
	number = cbModeNew->GetCount();
	if (number == 0) 
	{
		cbModeNew->ResetContent();//清除
		cbModeNew->AddString(_T("自锁关"));
		cbModeNew->AddString(_T("自锁开"));
		cbModeNew->AddString(_T("点动关"));
		cbModeNew->AddString(_T("点动开"));
		cbModeNew->AddString(_T("互锁延时"));
		cbModeNew->AddString(_T("互锁点动"));
		cbModeNew->AddString(_T("循环"));
		cbModeNew->AddString(_T("翻转"));
		cbModeNew->SetCurSel(0);
		OnCbnSelchangeComboxAny(uId);//触发
		UpdateData(true);

	}
	
}

HBRUSH CTcpServerProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush m_brush;
	CFont m_font;
	m_font.CreatePointFont(200, _T("宋体"));//代表15号字体，华文行楷

	CFont mlab_font;
	mlab_font.CreatePointFont(300, _T("宋体"));//代表15号字体，华文行楷

	if (pWnd->GetDlgCtrlID() == lbPort)
		{
		   if(isRed==true)
		   pDC->SetTextColor(RGB(255, 0, 0));//背景色为红色
		}
	if (pWnd->GetDlgCtrlID() == lbD01 || pWnd->GetDlgCtrlID() == lbD02 || pWnd->GetDlgCtrlID() == lbD03 || pWnd->GetDlgCtrlID() == lbD04 || pWnd->GetDlgCtrlID() == lbD05 || pWnd->GetDlgCtrlID() == lbD06 || pWnd->GetDlgCtrlID() == lbD07 || pWnd->GetDlgCtrlID() == lbD08 || pWnd->GetDlgCtrlID() == lbD09 || pWnd->GetDlgCtrlID() == lbD010 || pWnd->GetDlgCtrlID() == lbD011 || pWnd->GetDlgCtrlID() == lbD012 || pWnd->GetDlgCtrlID() == lbD013 || pWnd->GetDlgCtrlID() == lbD014 || pWnd->GetDlgCtrlID() == lbD015 || pWnd->GetDlgCtrlID() == lbD016)//设置颜色
	{
		
		UINT uid = pWnd->GetDlgCtrlID();
		SetDOStatus(uid, pDC);
		
		pDC->SelectObject(&m_font);//文字为15号字体，华文行楷
		
	}
	//========================================
	//统一设置字体的大小
	if (pWnd->GetDlgCtrlID() == lbDI1 || pWnd->GetDlgCtrlID() == lbDI2
		|| pWnd->GetDlgCtrlID() == lbDI3 || pWnd->GetDlgCtrlID() == lbDI4 || pWnd->GetDlgCtrlID() == lbDI5 || pWnd->GetDlgCtrlID() == lbDI6 || pWnd->GetDlgCtrlID() == lbDI7 || pWnd->GetDlgCtrlID() == lbDI8 || pWnd->GetDlgCtrlID() == lbDI9 || pWnd->GetDlgCtrlID() == lbDI10 || pWnd->GetDlgCtrlID() == lbDI11 || pWnd->GetDlgCtrlID() == lbDI12 || pWnd->GetDlgCtrlID() == lbDI13 || pWnd->GetDlgCtrlID() == lbDI14 || pWnd->GetDlgCtrlID() == lbDI15 || pWnd->GetDlgCtrlID() == lbDI16)//设置颜色
	{



		UINT uid = pWnd->GetDlgCtrlID();
		SetDIStatus(uid, pDC);//
		pDC->SelectObject(&mlab_font);//文字为15号字体，华文行楷

		
	}
	if (pWnd->GetDlgCtrlID() == lbOne || pWnd->GetDlgCtrlID() == lbTwo
		|| pWnd->GetDlgCtrlID() == lbThree || pWnd->GetDlgCtrlID() == lbFour || pWnd->GetDlgCtrlID() == lbFive || pWnd->GetDlgCtrlID() == lbSix || pWnd->GetDlgCtrlID() == lbSeven || pWnd->GetDlgCtrlID() == lbEight || pWnd->GetDlgCtrlID() == lbNine || pWnd->GetDlgCtrlID() == lbTen || pWnd->GetDlgCtrlID() == lbElevent || pWnd->GetDlgCtrlID() == lbTwelve || pWnd->GetDlgCtrlID() == lbThirteen || pWnd->GetDlgCtrlID() == lbFourteen || pWnd->GetDlgCtrlID() == lbFifteen || pWnd->GetDlgCtrlID() == lbSixteen)//设置颜色
	{

		pDC->SelectObject(&mlab_font);//文字为15号字体，华文行楷
		// TODO:  在此更改 DC 的任何特性
	}
	//=================
	if (pWnd->GetDlgCtrlID() == lbOut || pWnd->GetDlgCtrlID() == lbInput)
	{
		pDC->SelectObject(&m_font);//文字为15号字体，华文行楷
		// TODO:  在此更改 DC 的任何特性
		// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	}
	return hbr;
}


void CTcpServerProjectDlg::SetDIStatus(UINT uid, CDC* pDC)
{

	CStatic* pLableTest;
	BOOL m_bIsGreen = FALSE;
	if (hWnd == NULL)return;
	CTcpServerProjectDlg *pDlg = (CTcpServerProjectDlg*)hWnd;//===========指针==========
	switch (uid)
	{
	case lbDI1:
	{
		m_bIsGreen = DIStatus[0];
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI1);
		break;
	}
	case lbDI2:
	{
		m_bIsGreen = DIStatus[1];
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI2);
		break;
	}
	case lbDI3:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI3);
		m_bIsGreen = DIStatus[2];        //m_bGreen3;
		break;
	}
	case lbDI4:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI4);
		m_bIsGreen = DIStatus[3];
		break;
	}
	case lbDI5:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI5);
		m_bIsGreen = DIStatus[4];
		break;
	}
	case lbDI6:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI6);
		m_bIsGreen = DIStatus[5];
		break;
	}
	case lbDI7:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI7);
		m_bIsGreen = DIStatus[6];
		break;
	}
	case lbDI8:
	{
		m_bIsGreen = DIStatus[7];
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI8);
		break;
	}
	case lbDI9:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI9);
		m_bIsGreen = DIStatus[8];
		break;
	}
	case lbDI10:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI10);
		m_bIsGreen = DIStatus[9];
		break;
	}
	case lbDI11:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI11);
		m_bIsGreen = DIStatus[10];
		break;
	}
	case lbDI12:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI12);
		m_bIsGreen = DIStatus[11];
		break;
	}
	case lbDI13:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI13);
		m_bIsGreen = DIStatus[12];
		break;
	}
	case lbDI14:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI14);
		m_bIsGreen = DIStatus[13];
		break;
	}
	case lbDI15:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI15);
		m_bIsGreen = DIStatus[14];
		break;
	}
	case lbDI16:
	{
		pLableTest = (CStatic*)pDlg->GetDlgItem(lbDI16);
		m_bIsGreen = DIStatus[15];
		break;
	}
	}
  //测试是否为画布的这里出现问题
	COLORREF clrText = RGB(211, 211, 211);
	
	CDC* pDCTest = pLableTest->GetDC();
	if (pDCTest != NULL)
	{
		//COLORREF clrText1234= pDCTest->GetTextColor();//文字颜色

	}
	if (m_bIsGreen == FALSE)
	{
		
		//if (clrText!= RGB(211, 211, 211))
		//可以考虑获取到当前的颜色，然后再设置颜色
		pDC->SetTextColor(RGB(211, 211, 211));//背景色为灰色
		pDC->SetBkMode(TRANSPARENT);//
	}
	else
	{
		//if (clrText != RGB(0, 255, 0))
		pDC->SetTextColor(RGB(0, 255, 0));//背景色为绿色
		pDC->SetBkMode(TRANSPARENT);//设备背景透明
	}
	
}

void CTcpServerProjectDlg::SetDOStatus(UINT uid, CDC * pDC)
{
	BOOL m_bIsGreen;
	CStatic* pLable;       
	if (hWnd == NULL)return;
	CTcpServerProjectDlg *pDlg = (CTcpServerProjectDlg*)hWnd;//===========指针==========
	switch (uid)
	{
	case lbD01:
	{
		m_bIsGreen = DOStatus[0];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD01);//======
		break;
	}
	case lbD02:
	{
		m_bIsGreen = DOStatus[1];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD02);
		break;
	}
	case lbD03:
	{
		m_bIsGreen = DOStatus[2];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD03);
		break;
	}
	case lbD04:
	{
		m_bIsGreen = DOStatus[3];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD04);
		break;
	}
	case lbD05:
	{
		m_bIsGreen = DOStatus[4];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD05);
		break;
	}
	case lbD06:
	{
		m_bIsGreen = DOStatus[5];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD06);
		break;
	}
	case lbD07:
	{
		m_bIsGreen = DOStatus[6];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD07);
		break;
	}
	case lbD08:
	{
		m_bIsGreen = DOStatus[7];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD08);
		break;
	}
	case lbD09:
	{
		m_bIsGreen = DOStatus[8];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD09);
		break;
	}
	case lbD010:
	{
		m_bIsGreen = DOStatus[9];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD010);
		break;
	}
	case lbD011:
	{
		m_bIsGreen = DOStatus[10];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD011);
		break;
	}
	case lbD012:
	{
		m_bIsGreen = DOStatus[11];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD012);
		break;
	}
	case lbD013:
	{
		m_bIsGreen = DOStatus[12];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD013);
		break;
	}
	case lbD014:
	{
		m_bIsGreen = DOStatus[13];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD014);
		break;
	}
	case lbD015:
	{
		m_bIsGreen = DOStatus[14];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD015);
		break;
	}
	case lbD016:
	{
		m_bIsGreen = DOStatus[15];
		pLable = (CStatic*)pDlg->GetDlgItem(lbD016);
		break;
	}
	}
	COLORREF clrText;
	if (m_bIsGreen == FALSE)
	{
		   

		   //if (clrText != RGB(211, 211, 211))
			pDC->SetTextColor(RGB(211, 211, 211));//背景色为灰色
	}
	else
	{
		//if (clrText != RGB(0, 255, 0))
			pDC->SetTextColor(RGB(0, 255, 0));//背景色为绿色
	}

}
//==================关闭的事件==================================
void CTcpServerProjectDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//closesocket(sockServer);
	CDialogEx::OnClose();//
}


afx_msg LRESULT CTcpServerProjectDlg::OnOnbtnclickthread(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
void CAboutDlg::OnBnClickedButton11()
{

}


void CTcpServerProjectDlg::OnBnClickedButton2()
{
	m_bGreen = TRUE;
	CStatic *lbDO;

	lbDO = (CStatic*)GetDlgItem(lbD01);
}
void CTcpServerProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 0:
	{
		CTime cTime = CTime::GetCurrentTime();//
		CString strTime;    //字符串格式化时间  
		strTime = cTime.Format("%Y-%m-%d   %X");

		CString strWeek;
		int time= cTime.GetDayOfWeek();
		switch (time)
		{

		
		case 1:
		{
			strWeek = _T("星期日");
			break;
		}
		case 2:
		{
			strWeek = _T("星期一");
			break;
		}
		case 3:
		{
			strWeek = _T("星期二");
			break;
		}
		case 4:
		{
			strWeek = _T("星期三");
			break;
		}
		
		case 5:
		{
			strWeek = _T("星期四");
			break;
		}
		case 6:
		{
			strWeek = _T("星期五");
			break;
		}
		case 7:
		{
			strWeek = _T("星期六");
			break;
		}
		case 0: 
		{

			strWeek = _T(" ");
			break;
		}

		}
		strTime = strTime +_T("  ") +strWeek;
		CStatic* timeEdit;
		timeEdit = (CStatic*)GetDlgItem(lbTimeShow);
		timeEdit->SetWindowText(strTime);//

		CString conectNumHead= _T("当前连接数:");
		CStatic* conecControl;
		conecControl = (CStatic*)GetDlgItem(lbConectNum);

		CString conectNumStr;
		conectNumStr.Format(_T("%d"), connectNum);
		conectNumStr = conectNumHead + conectNumStr;//=======
		conecControl->SetWindowText(conectNumStr);//

		CDialogEx::OnTimer(nIDEvent);
		break;
	}
	}
}
LRESULT CTcpServerProjectDlg::OnNcHitTest(CPoint point)
{
	int ret = CDialog::OnNcHitTest(point);

	//if语句的前两行是用来禁止改变大小的，最后一行是用来禁止移动的
	if (HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret
		|| HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret || HTTOPRIGHT == ret || HTCAPTION == ret)
		return HTCLIENT;

	return ret;
}
//================C++=====如何更改到======
void CTcpServerProjectDlg::OnStnClickedlbserverip()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CTcpServerProjectDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnEraseBkgnd(pDC);
	//return false;
}
