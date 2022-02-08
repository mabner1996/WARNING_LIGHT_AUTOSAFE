
// TcpServerProjectDlg.h: 头文件
//

#define WM_MY_MESSAGE (WM_USER+100) //声明一个
#pragma once

// CTcpServerProjectDlg 对话框
class CTcpServerProjectDlg : public CDialogEx
{
// 构造
public:
	CTcpServerProjectDlg(CWnd* pParent = nullptr);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPSERVERPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	//afx_msg void OnMyMessage(WPARAM wParam, LPARAM lParam);//增加刷新=====
	

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg
		
		//UINT CommThread(LPVOID pParam);
	LRESULT  OnDisplayChange(WPARAM wParam, LPARAM lParam);
public:
	
	CComboBox cbMode;
	//CColorStatic m_staticTest; // 声明变量,注意这里的 CColorStatic是上面自定义的类哦。
	
	//afx_msg void OnCbnSetfocusCombo1();//


	void   OnButtonClicked(UINT   uId);//增加处//


	void  OnCbnSetfocusComboxAny(UINT   uId);
	

	void  OnCbnSelchangeComboxAny(UINT   uId);//

	void SetDIStatus(UINT uid, CDC* pDC);

	void SetDOStatus(UINT uid, CDC* pDC);
	
	void SetDefaultCombox();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic labDO1;
	CBrush m_brush;
	
	afx_msg void OnClose();//

	CString m_Data;

	CString m_Data1;

	BOOL m_bGreen;
	BOOL m_bGreen2;
	BOOL m_bGreen3;
	BOOL m_bGreen4;

	CString m_Data2;
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnOnbtnclickthread(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnStnClickedlbserverip();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
