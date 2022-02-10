#pragma once
#include <afxwin.h>

//继承类
class CColorStatic:
	public CStatic
{
public:
	COLORREF m_crText;          // 字体颜色  
	COLORREF m_crBackColor;     // 背景颜色  
	HBRUSH   m_hBrush;          // 画刷  
	LOGFONT  m_lf;              // 字体大小  
	CFont    m_font;            // 字体  
	bool    m_bTran;            // 是否透明  


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
