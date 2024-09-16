#include "pch.h"
#include "CPrintHelper.h"

using namespace artvabas::rcc::support;

CPrintHelper::CPrintHelper(CDC* pDC)
	: m_pDC{ pDC }
	, m_pFont{ nullptr }
	, m_pixMargin{ 100 } // 1 inch margin
	, m_pixFontHeightHeader{ 120 }
	, m_pixFontHeightBody{ 80 }
	, m_pxPos{ std::make_unique<long>(0) }
	, m_pyPos{ std::make_unique<long>(0) }
	, m_imgLogo{ CImage() }
{
	VERIFY(m_fontBoldHeader.CreateFont(
		m_pixFontHeightHeader,      // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_BOLD,				  // nWeight
		FALSE,                    // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Cascadia Mono")));    // lpszFacename

	VERIFY(m_fontPlainBody.CreateFont(
		m_pixFontHeightBody,        // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_NORMAL,                // nWeight
		FALSE,                    // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Cascadia Mono")));    // lpszFacename

	VERIFY(m_fontBoldBody.CreateFont(
		m_pixFontHeightBody,        // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_BOLD,		          // nWeight
		FALSE,                     // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Cascadia Mono")));    // lpszFacename
}

CPrintHelper::~CPrintHelper()
{
	m_pxPos.release();
	m_pyPos.release();
	m_imgLogo.Destroy();
	m_fontBoldHeader.DeleteObject();
	m_fontPlainBody.DeleteObject();
	m_fontBoldBody.DeleteObject();
}

void CPrintHelper::PrintLogo()
{
	//CImage imgLogo;
	m_imgLogo.Load(_T("logo.bmp"));

	// Get printer device resolutions
	const int nHorRes = m_pDC->GetDeviceCaps(HORZRES);	// get printable width
	const int nVerRes = m_pDC->GetDeviceCaps(VERTRES);	// get printable height
	const int nLogPixelsX = m_pDC->GetDeviceCaps(LOGPIXELSX);	// get device resolution along X
	const int nLogPixelsY = m_pDC->GetDeviceCaps(LOGPIXELSY);	// get device resolution along Y

	// Print border
	CRect rctBorder(0, 0, nHorRes, nVerRes);
	rctBorder.DeflateRect(nLogPixelsX / 2, nLogPixelsY / 2);
	m_pDC->Draw3dRect(rctBorder, RGB(0, 0, 0), RGB(0, 0, 0));

	// Set print start position
	*m_pxPos = rctBorder.TopLeft().x + 10;
	*m_pyPos = rctBorder.TopLeft().y + 10;

	// Print logo
	m_imgLogo.StretchBlt(m_pDC->m_hDC, *m_pxPos, *m_pyPos, static_cast<int>(m_imgLogo.GetWidth() * 6.8), static_cast<int>(m_imgLogo.GetHeight() * 6.8), 0, 0, m_imgLogo.GetWidth(), m_imgLogo.GetHeight(), SRCCOPY);

	// Calculate new start print position
	*m_pyPos += static_cast<int>(m_imgLogo.GetHeight() * 6.8) + 10;
}

void CPrintHelper::PrintHeader(const CString& strHeader)
{
	// Print header
	m_pFont = &m_fontBoldHeader;

	// Print Header rectangle
	CRect rctHeader(*m_pxPos + 60, *m_pyPos,*m_pxPos + static_cast<int>(m_imgLogo.GetWidth() * 6.8) - 60, *m_pyPos + HeaderTextLineDown(2));
	m_pDC->FillRect(rctHeader, &CBrush(RGB(0, 102, 255)));
	m_pDC->Draw3dRect(rctHeader, RGB(0, 102, 255), RGB(0, 102, 255));

	// Print header text
	m_pDC->SelectObject(m_pFont);
	m_pDC->SetTextColor(RGB(255, 255, 255));

	m_pDC->DrawText(strHeader, rctHeader, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// calculate new start print position
	*m_pyPos += HeaderTextLineDown(3);
}

void CPrintHelper::PrintFooter(const CString& strFooter)
{
	*m_pyPos += BodyTextLineDown(1);
	m_pFont = &m_fontPlainBody;
	m_pDC->SelectObject(m_pFont);

	if (strFooter.IsEmpty()) {
		// Print footer
		COleDateTime cdtNow = COleDateTime::GetCurrentTime();
		m_pDC->TextOutW(*m_pxPos, *m_pyPos, _T("Dit overzicht is gegenereerd door Repair Cafe Cure App op ") + cdtNow.Format(_T("%d-%m-%Y")));
	}
	else {
		m_pDC->TextOutW(*m_pxPos, *m_pyPos, strFooter);
	}
}
