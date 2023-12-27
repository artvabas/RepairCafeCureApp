#include "pch.h"
#include "CPrintWorkorder.h"

CPrintWorkorder::CPrintWorkorder(WorkorderData* workData)
	
{
	m_pStructWorkorderData = workData;
}

CPrintWorkorder::~CPrintWorkorder()
{
}

void CPrintWorkorder::PrintCombi(CDC* pDC) const
{
	typedef unsigned int pixel;
	const pixel pixMargin = 100; // 1 inch margin
	const pixel pixFontHeightHeader = 110;
	const pixel pixFontHeightBody = 70;

	// Create fonts
	CFont fontPlain;
	VERIFY(fontPlain.CreateFont(
		pixFontHeightHeader, 	  // nHeight
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

	CFont fontBold;
	VERIFY(fontBold.CreateFont(
		pixFontHeightHeader,      // nHeight
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

	CFont fontItalic;
	VERIFY(fontItalic.CreateFont(
		pixFontHeightBody,        // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_NORMAL,                // nWeight
		TRUE,                     // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Cascadia Mono")));    // lpszFacename

	// Lambda functions for calculating pixels movements
	auto TotalTabInPixels = [pixMargin](unsigned int nTotalTabs) -> pixel
		{
			return pixMargin * nTotalTabs;
		};

	auto HeaderTextLineDown = [pixFontHeightHeader](unsigned int nTotalLines) -> pixel
		{
			return pixFontHeightHeader * nTotalLines;
		};

	auto BodyTextLineDown = [pixFontHeightBody](unsigned int nTotalLines) -> pixel
		{
			return pixFontHeightBody * nTotalLines;
		};

	// Get printer device resolutions
	const int nHorRes = pDC->GetDeviceCaps(HORZRES);	// get printable width
	const int nVerRes = pDC->GetDeviceCaps(VERTRES);	// get printable height
	const int nLogPixelsX = pDC->GetDeviceCaps(LOGPIXELSX);	// get device resolution along X
	const int nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);	// get device resolution along Y

	CImage imgLogo;
	imgLogo.Load(_T("logo.bmp"));
	CFont* pFont = nullptr;
	unsigned long middleDocBody = 0UL;

	pDC->m_bPrinting = TRUE;
	pDC->StartPage();

	// Print border
	CRect rctBorder(0, 0, nHorRes, nVerRes);
	rctBorder.DeflateRect(nLogPixelsX / 2, nLogPixelsY / 2);
	pDC->Draw3dRect(rctBorder, RGB(0, 0, 0), RGB(0, 0, 0));

	/* Common print jobs */
	auto PrintLogo = [pDC, imgLogo](int nPosX, int nPosY)
		{
			imgLogo.StretchBlt(pDC->m_hDC, nPosX, nPosY, imgLogo.GetWidth() * 5, imgLogo.GetHeight() * 5, 0, 0, imgLogo.GetWidth(), imgLogo.GetHeight(), SRCCOPY);
		};

	auto PrintHeader = [pDC, HeaderTextLineDown, TotalTabInPixels, imgLogo](CFont* font, CString& strAssetDescription, CString& strWorkorderDescription,
		CString& strAssetModelNumber, CString& strAssetBrand, CString& strLastWOID, int nPosX, int nPosY) -> CRect
		{
			// Print Header rectangle
			CRect rctHeader(nPosX + 60, nPosY, nPosX + (imgLogo.GetWidth() * 5) - 60, nPosY + HeaderTextLineDown(2));
			pDC->FillRect(rctHeader, &CBrush(RGB(0, 102, 255)));
			pDC->Draw3dRect(rctHeader, RGB(0, 102, 255), RGB(0, 102, 255));

			// Print header text
			pDC->SelectObject(font);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(_T(" ") + strAssetDescription + _T(": ") + strWorkorderDescription + _T("\n") +
				_T("Workorder: ") + strLastWOID + _T(" | Model: ") + strAssetModelNumber + _T(" | Brand: ") + strAssetBrand
				, rctHeader, DT_CENTER | DT_TABSTOP);

			return rctHeader;
		};

	auto PrintDetails = [pDC, HeaderTextLineDown, &fontPlain](CFont* font, CString& strWorkorderCreationDate, CString& strCustomerSurname, CString& strCustomerName,
		CString& strCustomerCellPhone, CString& strCustomerPhone, CString& strCustomerEmail, CString& strEmployee, unsigned long ulMiddle,
		int nPosX, int nPosY) -> int
		{
			int nLineDown = 0;
			// Set details values
			CString strTextLine1a = _T("Date: ");// +COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));
			CString strTextLine1b = _T("Customer: ");// +m_strCustomerSurname + _T(" ") + m_strCustomerName;
			CString strTextLine2a = _T("Employee: ");// +theApp.GetSelectedEmployeeName();
			CString strTextLine2b = _T("Mobile: ");// 06 - 3456789");
			CString strTextLine3 = _T("Phone: ");// 070 - 5647864");
			CString strTextLine4 = _T("Email: ");// zuurtje@zuiker.com");

			// Set font color
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));

			// Print details text
			// Line 1 Date and Custemer
			pDC->TextOut(nPosX, nPosY, strTextLine1a);
			pDC->TextOut(nPosX + ulMiddle, nPosY, strTextLine1b);
			//Line 2 Employee and mobile
			pDC->TextOut(nPosX, nPosY + HeaderTextLineDown(nLineDown + 1), strTextLine2a);
			pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 1), strTextLine2b);
			//Line 3 Phone
			pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 2), strTextLine3);
			//Line 4 Email
			pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 3), strTextLine4);

			font = &fontPlain;
			pDC->SelectObject(font);

			CSize textSizeCustomer = pDC->GetTextExtent(strTextLine1b);
			CSize TextSizeEmployee = pDC->GetTextExtent(strTextLine2a);

			// Print detals value text
			pDC->TextOut(nPosX + TextSizeEmployee.cx, nPosY, strWorkorderCreationDate);
			pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY, strCustomerSurname + _T(" ") + strCustomerName);
			pDC->TextOut(nPosX + TextSizeEmployee.cx, nPosY + HeaderTextLineDown(++nLineDown), strEmployee);
			pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(nLineDown), strCustomerCellPhone);
			pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(++nLineDown), strCustomerPhone);
			pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(++nLineDown), strCustomerEmail);

			return nLineDown;
		};

	/***** print left half (A5) on A4 - Customer receipt ******/

					// Set print start position
	int nPosX1 = rctBorder.TopLeft().x + 10;
	int nPosY1 = rctBorder.TopLeft().y + 10;

	// Print logo
	PrintLogo(nPosX1, nPosY1);

	// Calculate new start print position
	nPosY1 += imgLogo.GetHeight() * 5 + 10;

	// Print header
	pFont = &fontBold;
	auto rctHeaderLeft = PrintHeader(pFont, m_pStructWorkorderData->strAssetDescription, m_pStructWorkorderData->strWorkorderDescription, 
		m_pStructWorkorderData->strAssetModelNumber, m_pStructWorkorderData->strAssetBrand, m_pStructWorkorderData->strWorkorderID, nPosX1, nPosY1);

	middleDocBody = (((rctHeaderLeft.BottomRight().x - rctHeaderLeft.TopLeft().x) + (nPosX1 + 60)) / 2) - TotalTabInPixels(2);

	// Calculate new start print position
	nPosX1 += TotalTabInPixels(1);
	nPosY1 = rctHeaderLeft.BottomRight().y + HeaderTextLineDown(1);

	// Print customer workorder details
	auto nLinesDown = PrintDetails(pFont, m_pStructWorkorderData->strWorkorderCreatedDate, m_pStructWorkorderData->strCustomerSurname,
		m_pStructWorkorderData->strCustomerName, m_pStructWorkorderData->strCustomerCellPhone, m_pStructWorkorderData->strCustomerPhone,
		m_pStructWorkorderData->strCustomerEmail, m_pStructWorkorderData->strEmployeeResponsible, middleDocBody, nPosX1, nPosY1);

	nPosY1 += HeaderTextLineDown(nLinesDown + 2);

	pFont = &fontItalic;
	pDC->SelectObject(pFont);

	// Print body text (receipt)
	pDC->TextOutW(nPosX1, nPosY1, _T("HUISREGELS"));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De activiteiten van NME Repair worden gratis en op vrijwillige basis uitgevoerd door de"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aanwezige reparatiedeskundigen."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Noch de organisatie van het NME Repair, noch de reparateurs zijn aansprakelijk voor schade"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("als gevolg van verstrekte reparatieadviezen of instructies, voor schade aan de ter reparatie"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aangeboden voorwerpen, voor gevolgdchade of voor andere schade die het gevolg is van"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("activiteiten van het NME Repair."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Voor het gebruik van nieuwe materialen zoals; snoeren, stekkers, zekeringen, onderdelen, etc,"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("dient appart te worden betaald."));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("Bezoekers die defecte voorwerpen aanbieden ter reparatie, doen dat op eigen risico."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De reparateurs van het NME Repair behouden zich het recht voor om voorwerpen te weigeren en/of"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("niet te repareren."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De reparateurs van het NME Repair zijn niet verplicht om gedomonteerde voorwerpen die niet"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("gerepareerd kunnen worden, weer te monteren."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De bezoekers van het MNE Repair zijn zelfs verantwoorderdelijk voor ht netjes afvoeren van"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("kapotte voorwerpen die niet konden worden gerepareerd."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Een vrijwilge bijdrage wordt op prijs gesteld."));

	/***** print right half (A5) on A4 - Workorder  ****/

					// Set print start position
	nPosX1 = rctBorder.TopLeft().x + (rctBorder.Width() / 2) + 10;
	nPosY1 = rctBorder.TopLeft().y + 10;

	PrintLogo(nPosX1, nPosY1);

	// Calculate new start print position
	nPosY1 += imgLogo.GetHeight() * 5 + 10;

	// Print header
	pFont = &fontBold;
	auto rctHeaderRight = PrintHeader(pFont, m_pStructWorkorderData->strAssetDescription, m_pStructWorkorderData->strWorkorderDescription, 
		m_pStructWorkorderData->strAssetModelNumber, m_pStructWorkorderData->strAssetBrand,	m_pStructWorkorderData->strWorkorderID, nPosX1, nPosY1);

	// Calculate new start print position
	nPosX1 += TotalTabInPixels(1);
	nPosY1 = rctHeaderRight.BottomRight().y + HeaderTextLineDown(1);

	// Print customer workorder details
	nLinesDown = PrintDetails(pFont, m_pStructWorkorderData->strWorkorderCreatedDate, m_pStructWorkorderData->strCustomerSurname,
		m_pStructWorkorderData->strCustomerName, m_pStructWorkorderData->strCustomerCellPhone, m_pStructWorkorderData->strCustomerPhone,
		m_pStructWorkorderData->strCustomerEmail, m_pStructWorkorderData->strEmployeeResponsible, middleDocBody, nPosX1, nPosY1);

	// Calculate new start print position
	nPosY1 += HeaderTextLineDown(nLinesDown + 2);

	// Print body text (workorder)
	pDC->TextOut(nPosX1 + TotalTabInPixels(2), nPosY1, _T("Status: In Progress"));

	// calculate new start print position
	nPosY1 += HeaderTextLineDown(2);

	// print work comment area
	pDC->TextOut(nPosX1, nPosY1, _T("Werkzaamheden"));
	nPosY1 += HeaderTextLineDown(1);
	CRect rctWork(nPosX1, nPosY1, rctHeaderRight.right, nPosY1 + HeaderTextLineDown(8));
	pDC->Draw3dRect(rctWork, RGB(0, 102, 255), RGB(0, 102, 255));

	// calculate new start print position
	nPosY1 += HeaderTextLineDown(10);

	// Set font color
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));

	// print general comment area
	pDC->TextOut(nPosX1, nPosY1, _T("Diversen"));
	nPosY1 += HeaderTextLineDown(1);
	CRect rctComment(nPosX1, nPosY1, rctHeaderRight.right, nPosY1 + HeaderTextLineDown(4));
	pDC->Draw3dRect(rctComment, RGB(0, 102, 255), RGB(0, 102, 255));

	// Destroy image
	imgLogo.Destroy();
	fontPlain.DeleteObject();
	fontBold.DeleteObject();
	fontItalic.DeleteObject();

}
