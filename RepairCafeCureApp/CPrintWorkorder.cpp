#include "pch.h"
/*
Copyright(C) 2023/24  artvabas

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>

To see the license for this source code, please visit :
< https ://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit :
< https ://artvabas.com>
	< https ://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at :
< https ://artvabas.com/contact>

	*/

	/*
	* This file is part of RepairCafeCureApp.
	* File: CPrintWorkorder.h, implements the class CPrintWorkorder.
	*
	* This class is used to print a workorder in a combi or invoice format.
	*
	* Target: Windows 10/11 64bit
	* Version: 1.0.0.1 (Alpha)
	* Created: 18-10-2023, (dd-mm-yyyy)
	* Updated: 25-05-2024, (dd-mm-yyyy)
	* Creator: artvabasDev / artvabas
	*
	* License: GPLv3
	*/
#include "CPrintWorkorder.h"

using namespace artvabas::rcc::support;

CPrintWorkorder::CPrintWorkorder(WorkorderData* workData) noexcept
{
	m_pStructWorkorderData = workData;
}

CPrintWorkorder::~CPrintWorkorder()
{}

// Print workorder and customer receipt (combi format)
// The combi format is a A4 paper that is divided into two A5 papers.
// The left half is the customer receipt and the right half is the workorder.
// - pDC: A pointer to the printer device context.
void CPrintWorkorder::PrintCombi(CDC* pDC) const noexcept
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
	auto b = imgLogo.Load(_T("logo.bmp"));
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
			CString strTextLine4 = _T("Email: ");// zuurtje@suiker.com");

			// Set font color
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));

			// Print details text
			// Line 1 Date and Customer
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

			// Print details value text
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
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aangeboden voorwerpen, voor gevolgschade of voor andere schade die het gevolg is van"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("activiteiten van het NME Repair."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Voor het gebruik van nieuwe materialen zoals; snoeren, stekkers, zekeringen, onderdelen, etc,"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("dient apart te worden betaald."));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("Bezoekers die defecte voorwerpen aanbieden ter reparatie, doen dat op eigen risico."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De reparateurs van het NME Repair behouden zich het recht voor om voorwerpen te weigeren en/of"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("niet te repareren."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De reparateurs van het NME Repair zijn niet verplicht om gedemonteerde voorwerpen die niet"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("gerepareerd kunnen worden, weer te monteren."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De bezoekers van het MNE Repair zijn zelfs verantwoordelijk voor het netjes afvoeren van"));
	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("kapotte voorwerpen die niet konden worden gerepareerd."));

	pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Een vrijwillige bijdrage wordt op prijs gesteld."));

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

// Print workorder invoice
// The invoice format is a A4 paper that is used to print the workorder invoice.
// - pDC: A pointer to the printer device context.
void CPrintWorkorder::PrintInvoice(CDC* pDC) const noexcept
{
	typedef unsigned int pixel;
	const pixel pixMargin = 100; // 1 inch margin
	const pixel pixFontHeightHeader = 120;
	const pixel pixFontHeightBody = 80;

	// Create fonts
	CFont fontPlainHeader;
	VERIFY(fontPlainHeader.CreateFont(
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

	CFont fontBoldHeader;
	VERIFY(fontBoldHeader.CreateFont(
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

	CFont fontPlainBody;
	VERIFY(fontPlainBody.CreateFont(
		pixFontHeightBody,        // nHeight
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

	CFont fontBoldBody;
	VERIFY(fontBoldBody.CreateFont(
		pixFontHeightBody,        // nHeight
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

	// Set print start position
	int nPosX = rctBorder.TopLeft().x + 10;
	int nPosY = rctBorder.TopLeft().y + 10;
	
	// Print logo
	imgLogo.StretchBlt(pDC->m_hDC, nPosX, nPosY, static_cast<int>(imgLogo.GetWidth() * 6.8), static_cast<int>(imgLogo.GetHeight() * 6.8), 0, 0, imgLogo.GetWidth(), imgLogo.GetHeight(), SRCCOPY);

	// Calculate new start print position
	nPosY += static_cast<int>(imgLogo.GetHeight() * 6.8) + 10;

	// Print header
	pFont = &fontBoldHeader;

	// Print Header rectangle
	CRect rctHeader(nPosX + 60, nPosY, nPosX + static_cast<int>(imgLogo.GetWidth() * 6.8) - 60, nPosY + HeaderTextLineDown(2));
	pDC->FillRect(rctHeader, &CBrush(RGB(0, 102, 255)));
	pDC->Draw3dRect(rctHeader, RGB(0, 102, 255), RGB(0, 102, 255));

	// Print header text
	pDC->SelectObject(pFont);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(_T(" ") + m_pStructWorkorderData->strAssetDescription + _T(": ") + m_pStructWorkorderData->strWorkorderDescription + _T("\n") +
		_T("Workorder: ") + m_pStructWorkorderData->strWorkorderID + _T(" | Model: ") + m_pStructWorkorderData->strAssetModelNumber + 
		_T(" | Brand: ") + m_pStructWorkorderData->strAssetBrand, rctHeader, DT_CENTER | DT_TABSTOP);

	// Calculate new start print position
	nPosX += TotalTabInPixels(1);
	nPosY = rctHeader.BottomRight().y + HeaderTextLineDown(1);
	auto ulMiddle = (((rctHeader.BottomRight().x - rctHeader.TopLeft().x) + (nPosX + 60)) / 2) - TotalTabInPixels(2);

	// Print customer workorder details
	int nLineDown = 0;
	// Set details values
	CString strTextLine1a = _T("Date: ");// +COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));
	CString strTextLine1b = _T("Customer: ");// +m_strCustomerSurname + _T(" ") + m_strCustomerName;
	CString strTextLine2a = _T("Employee: ");// +theApp.GetSelectedEmployeeName();
	CString strTextLine2b = _T("Mobile: ");// 06 - 3456789");
	CString strTextLine3 = _T("Phone: ");// 070 - 5647864");
	CString strTextLine4 = _T("Email: ");// zuurtje@suiker.com");

	// Set font color
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));

	// Print details text
	// Line 1 Date and Customer
	pDC->TextOut(nPosX, nPosY, strTextLine1a);
	pDC->TextOut(nPosX + ulMiddle, nPosY, strTextLine1b);
	//Line 2 Employee and mobile
	pDC->TextOut(nPosX, nPosY + HeaderTextLineDown(nLineDown + 1), strTextLine2a);
	pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 1), strTextLine2b);
	//Line 3 Phone
	pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 2), strTextLine3);
	//Line 4 Email
	pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 3), strTextLine4);

	pFont = &fontPlainHeader;
	pDC->SelectObject(pFont);

	CSize textSizeCustomer = pDC->GetTextExtent(strTextLine1b);
	CSize TextSizeEmployee = pDC->GetTextExtent(strTextLine2a);

	// Print details value text
	pDC->TextOut(nPosX + TextSizeEmployee.cx, nPosY, m_pStructWorkorderData->strWorkorderCreatedDate);
	pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY, m_pStructWorkorderData->strCustomerSurname + _T(" ") + 
		m_pStructWorkorderData->strCustomerName);
	pDC->TextOut(nPosX + TextSizeEmployee.cx, nPosY + HeaderTextLineDown(++nLineDown), m_pStructWorkorderData->strEmployeeResponsible);
	pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(nLineDown), m_pStructWorkorderData->strCustomerCellPhone);
	pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(++nLineDown), m_pStructWorkorderData->strCustomerPhone);
	pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(++nLineDown), m_pStructWorkorderData->strCustomerEmail);

	// Calculate new start print position
	nPosY += HeaderTextLineDown(nLineDown + 2);

	// Print body text (workorder)
	pDC->TextOut(nPosX, nPosY, _T("Date Finished: ") + m_pStructWorkorderData->m_structWorkorderLog.strWorkorderRepairedDate);

	if (m_pStructWorkorderData->m_bIsPinTransfer){
		pDC->TextOut(nPosX + ulMiddle, nPosY, _T("PIN Transaction"));
	} else {
		pDC->TextOut(nPosX + ulMiddle, nPosY, _T("Cash Transaction"));
	}

	// calculate new start print position
	nPosY += HeaderTextLineDown(2);

	CRect rctDescriptionHeader(nPosX , nPosY, nPosX + static_cast<int>(imgLogo.GetWidth() * 6.8) - TotalTabInPixels(2), nPosY + BodyTextLineDown(1));
	pDC->FillRect(rctDescriptionHeader, &CBrush(RGB(0, 102, 255)));
	pDC->Draw3dRect(rctDescriptionHeader, RGB(0, 102, 255), RGB(0, 102, 255));

	// print work comment area
	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(_T("Werkzaamheden"), rctDescriptionHeader, DT_LEFT | DT_TABSTOP);

	nPosY += BodyTextLineDown(2);

	CRect rctDescription(nPosX, nPosY, nPosX + static_cast<int>(imgLogo.GetWidth() * 6.8) - TotalTabInPixels(2), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctDescription, RGB(255, 255, 255), RGB(255, 255, 255));

	pFont = &fontPlainBody;
	pDC->SelectObject(pFont);

	// Set font color
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));

	pDC->DrawText(m_pStructWorkorderData->m_structWorkorderLog.strWorkorderLog, rctDescription, DT_LEFT | DT_TABSTOP);

	// calculate new start print position
	nPosY += BodyTextLineDown(21);

	CRect rctInvoiceHeader(nPosX, nPosY, nPosX + static_cast<int>(imgLogo.GetWidth() * 6.8) - TotalTabInPixels(2), nPosY + BodyTextLineDown(1));
	pDC->FillRect(rctInvoiceHeader, &CBrush(RGB(0, 102, 255)));
	pDC->Draw3dRect(rctInvoiceHeader, RGB(0, 102, 255), RGB(0, 102, 255));

	// print work comment area
	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(_T("Factuur"), rctInvoiceHeader, DT_LEFT | DT_TABSTOP);

	nPosY += BodyTextLineDown(1);

	CRect rctPartDescription(nPosX, nPosY, nPosX + TotalTabInPixels(20), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctPartDescription, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctPartAmount(rctPartDescription.right, nPosY, rctPartDescription.right + TotalTabInPixels(3), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctPartAmount, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctPartUnitPrice(rctPartAmount.right, nPosY, rctPartAmount.right + TotalTabInPixels(3), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctPartUnitPrice, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctPartSubtotal(rctPartUnitPrice.right + TotalTabInPixels(1), nPosY, rctPartUnitPrice.right + TotalTabInPixels(5), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctPartSubtotal, RGB(255, 255, 255), RGB(255, 255, 255));

	CString strPartDescription(_T("Omschrijving:\n"));
	CString strPartAmount(_T("Aantal:\n"));
	CString strPartUnitPrice(_T("Prijs:\n"));
	CString strPartSubtotal(_T("Subtotaal:\n"));


	if (m_pStructWorkorderData->m_vecSpareParts.size() > 0)
	{
		for (int i = 0; i < m_pStructWorkorderData->m_vecSpareParts.size(); i++)
		{
			strPartDescription += _T("\n") + m_pStructWorkorderData->m_vecSpareParts[i].strSparePartDescription;
			strPartAmount += _T("\n") + m_pStructWorkorderData->m_vecSpareParts[i].strSparePartQuantity;
			strPartUnitPrice += _T("\n€") + m_pStructWorkorderData->m_vecSpareParts[i].strSparePartPrice;
			strPartSubtotal += _T("\n€") + m_pStructWorkorderData->m_vecSpareParts[i].strSparePartSubTotalPrice;
		}
	}

	nPosY += BodyTextLineDown(1);
	pFont = &fontPlainBody;
	pDC->SelectObject(pFont);

	// Set font color
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));

	pDC->DrawText(strPartDescription, rctPartDescription, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(strPartAmount, rctPartAmount, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(strPartUnitPrice, rctPartUnitPrice, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(strPartSubtotal, rctPartSubtotal, DT_RIGHT | DT_TABSTOP);

	// calculate new start print position
	nPosY += BodyTextLineDown(20);

	CRect rctTotal(rctPartAmount.right, nPosY, rctPartAmount.right + TotalTabInPixels(3), nPosY + BodyTextLineDown(1));
	pDC->Draw3dRect(rctTotal, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctTotalAmount(rctPartUnitPrice.right + TotalTabInPixels(1), nPosY, rctPartUnitPrice.right + TotalTabInPixels(5), nPosY + BodyTextLineDown(1));
	pDC->Draw3dRect(rctTotalAmount, RGB(255, 255, 255), RGB(255, 255, 255));

	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	// Set font color
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));

	pDC->DrawText(_T("Totaal:"), rctTotal, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(_T("€") + m_pStructWorkorderData->strWorkorderTotalPartsPrice, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	// Destroy image
	imgLogo.Destroy();
	fontPlainHeader.DeleteObject();
	fontBoldHeader.DeleteObject();
	fontPlainBody.DeleteObject();
	fontBoldBody.DeleteObject();
}
