#pragma once


// Server 对话框

class Server : public CDialog
{
	DECLARE_DYNAMIC(Server)

public:
	Server(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Server();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPSERVERPROJECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	CStatic m_static1;
	CStatic Test1;
	
	CStatic Test2;
	
	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnBnClickedButton1();

	
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
