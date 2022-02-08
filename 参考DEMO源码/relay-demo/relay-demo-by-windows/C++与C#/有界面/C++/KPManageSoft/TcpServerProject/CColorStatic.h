#pragma once
#include <afxwin.h>

//�̳���
class CColorStatic:
	public CStatic
{
public:
	COLORREF m_crText;          // ������ɫ  
	COLORREF m_crBackColor;     // ������ɫ  
	HBRUSH   m_hBrush;          // ��ˢ  
	LOGFONT  m_lf;              // �����С  
	CFont    m_font;            // ����  
	bool    m_bTran;            // �Ƿ�͸��  


public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	void CColorStatic::SetTextColor(COLORREF crText)
	{
		m_crText = crText;
		RedrawWindow();
	}
	
	void CColorStatic::SetFontSize(int nSize)
	{
		nSize *= -1;
		m_lf.lfHeight = nSize;
		m_font.DeleteObject();
		m_font.CreateFontIndirect(&m_lf);
		RedrawWindow();
	}

	
	void CColorStatic::SetBackColor(COLORREF crBackColor)
	{
		m_crBackColor = crBackColor;
		RedrawWindow();
	}

	void CColorStatic::SetTransparent(bool bTran)
	{
		m_bTran = bTran;
		RedrawWindow();
	}
	


};
