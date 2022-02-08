#include "pch.h"
#include "CColorStatic.h"



HBRUSH CColorStatic::CtlColor(CDC*  pDC , UINT nCtlColor)
{
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkColor(m_crBackColor);
		if (m_bTran == true)
			pDC->SetBkMode(TRANSPARENT);
	}
	return m_hBrush;
}
BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
