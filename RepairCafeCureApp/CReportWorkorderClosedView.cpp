/*
	Copyright (C) 2023/24  artvabas

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>

	To see the license for this source code, please visit:
		<https://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit:
		<https://artvabas.com>
		<https://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at:
		<https://artvabas.com/contact>

*/

/*
* This file is part of RepairCafeCureApp.
* File: CReportTaxView.h, implements class CReportWorkorderClosedView
*
* This class is the view of the closed workorders report
* With this view, the user can print a list with closed orders and
* jump into the close workorder to see the details with a option to print them.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.3 (alpha)
* Created: 02-06-2023, (dd-mm-yyyy)
* Updated: 28s-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CReportWorkorderClosedView.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"
#include "CClosedWorkorderDetails.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::sql;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::database::tables::closedworkorders;

IMPLEMENT_DYNCREATE(CReportWorkorderClosedView, CFormView)

CReportWorkorderClosedView::CReportWorkorderClosedView()
	: CFormView(IDD_REPORT_WORKOREER_CLOSED_FORM)
	, m_ePrinterOrientation(PORTRAIT)
{}

CReportWorkorderClosedView::~CReportWorkorderClosedView()
{}

// For handlling application messages
BEGIN_MESSAGE_MAP(CReportWorkorderClosedView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CReportWorkorderClosedView::OnFilePrintPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDER_CLOSED_REPORT, &CReportWorkorderClosedView::OnNMDoubleClickWorkorderClosedReport)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/* Overrode methods */

// For handling data exchange between the controls and their variables
// - pDX: A pointer to a CDataExchange object
void CReportWorkorderClosedView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORKORDER_CLOSED_REPORT, m_lstWorkorderClosedReport);
}

// For handling the view's initial update
// - This method is called after the view is first attached to the document
void CReportWorkorderClosedView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_lstWorkorderClosedReport.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstWorkorderClosedReport.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(1, _T("WORKORDER DESCRIPTION"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(2, _T("EMPLOYEE RESPONSIBLE"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(3, _T("WORKORDER CLOSED DATE"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(4, _T("WORKORDER STATUS"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(5, _T("ASSET ID"), LVCFMT_LEFT, 0);
	m_lstWorkorderClosedReport.InsertColumn(6, _T("ASSET DESCRIPTION"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(7, _T("CUSTOMER ID"), LVCFMT_LEFT, 0);
	m_lstWorkorderClosedReport.InsertColumn(8, _T("INVOICE ID"), LVCFMT_LEFT, 0);
}

// Is called before printing, for preparing the print job
BOOL CReportWorkorderClosedView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

// Is called just before printing for setting the printer settings
void CReportWorkorderClosedView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	SetPrinterOrientation(theApp.GetDeviceMode(), pDC);
	CFormView::OnBeginPrinting(pDC, pInfo);
}

// Is called after printing for cleaning up
void CReportWorkorderClosedView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

// Is called for printing the view
void CReportWorkorderClosedView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	typedef unsigned int pixel;
	const pixel pixMargin = 100; // 1 inch margin
	const pixel pixFontHeightHeader = 120;
	const pixel pixFontHeightBody = 80;

	const wchar_t* wzsMonth[] = { L"januari", L"februari", L"maart", L"april", L"mei",  L"juni",
								 L"juli", L"augustus", L"september", L"oktober", L"november", L"december" };

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
	auto TotalTabInPixels = [pixMargin](unsigned int nTotalTabs) -> pixel {
		return pixMargin * nTotalTabs;
		};

	auto HeaderTextLineDown = [pixFontHeightHeader](unsigned int nTotalLines) -> pixel {
		return pixFontHeightHeader * nTotalLines;
		};

	auto BodyTextLineDown = [pixFontHeightBody](unsigned int nTotalLines) -> pixel {
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


	pDC->DrawText(_T("Overzicht gesloten werkorders"),	rctHeader, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	// calculate new start print position
	nPosY += HeaderTextLineDown(3);

	CRect rctWorkorderID(nPosX, nPosY, nPosX + TotalTabInPixels(4), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderID, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderDescription(rctWorkorderID.right, nPosY, rctWorkorderID.right + TotalTabInPixels(14), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderDescription, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderResponsible(rctWorkorderDescription.right, nPosY, rctWorkorderDescription.right + TotalTabInPixels(5), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderResponsible, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderCloseDate(rctWorkorderResponsible.right, nPosY, rctWorkorderResponsible.right + TotalTabInPixels(5), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderCloseDate, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderStatus(rctWorkorderCloseDate.right, nPosY, rctWorkorderCloseDate.right + TotalTabInPixels(4), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderStatus, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctAssetDescription(rctWorkorderStatus.right, nPosY, rctWorkorderStatus.right + TotalTabInPixels(10), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctAssetDescription, RGB(255, 255, 255), RGB(255, 255, 255));

	// Print Table Header
	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	// print work comment area
	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(_T("Werkorder:"), rctWorkorderID, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Defect:"), rctWorkorderDescription, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Technicus:"), rctWorkorderResponsible, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Datum:"), rctWorkorderCloseDate, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Status:"), rctWorkorderStatus, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Apparaat:"), rctAssetDescription, DT_LEFT | DT_TABSTOP);

	// Print Table Body
	pFont = &fontPlainBody;
	pDC->SelectObject(pFont);

	// Print body text
	nPosY += BodyTextLineDown(1);

	// Print table body
	for (int i = 0; i < m_lstWorkorderClosedReport.GetItemCount(); i++) {
		CString strWorkorderID = m_lstWorkorderClosedReport.GetItemText(i, 0);
		CString strWorkorderDescription = m_lstWorkorderClosedReport.GetItemText(i, 1);
		CString strWorkorderResponsible = m_lstWorkorderClosedReport.GetItemText(i, 2);
		CString strWorkorderCloseDate = m_lstWorkorderClosedReport.GetItemText(i, 3);
		CString strWorkorderStatus = m_lstWorkorderClosedReport.GetItemText(i, 4);
		CString strAssetDescription = m_lstWorkorderClosedReport.GetItemText(i, 6);

		CRect rctWorkorderID(nPosX, nPosY, nPosX + TotalTabInPixels(4), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strWorkorderID, rctWorkorderID, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderDescription(rctWorkorderID.right, nPosY, rctWorkorderID.right + TotalTabInPixels(14), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strWorkorderDescription, rctWorkorderDescription, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderResponsible(rctWorkorderDescription.right, nPosY, rctWorkorderDescription.right + TotalTabInPixels(5), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strWorkorderResponsible, rctWorkorderResponsible, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderCloseDate(rctWorkorderResponsible.right, nPosY, rctWorkorderResponsible.right + TotalTabInPixels(5), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strWorkorderCloseDate, rctWorkorderCloseDate, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderStatus(rctWorkorderCloseDate.right, nPosY, rctWorkorderCloseDate.right + TotalTabInPixels(4), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strWorkorderStatus, rctWorkorderStatus, DT_LEFT | DT_TABSTOP);

		CRect rctAssetDescription(rctWorkorderStatus.right, nPosY, rctWorkorderStatus.right + TotalTabInPixels(10), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strAssetDescription, rctAssetDescription, DT_LEFT | DT_TABSTOP);

		nPosY += BodyTextLineDown(1);
	}

	// Print footer
	nPosY += BodyTextLineDown(1);
	pFont = &fontPlainBody;
	pDC->SelectObject(pFont);
	COleDateTime cdtNow = COleDateTime::GetCurrentTime();

	pDC->TextOutW(nPosX, nPosY, _T("Dit overzicht is gegenereerd door Repair Cafe CureApp op ") + cdtNow.Format(_T("%d-%m-%Y")));

	// Destroy image
	imgLogo.Destroy();
	fontBoldHeader.DeleteObject();
	fontPlainBody.DeleteObject();
	fontBoldBody.DeleteObject();

	CFormView::OnPrint(pDC, pInfo);
}

// For validating the view
#ifdef _DEBUG
void CReportWorkorderClosedView::AssertValid() const
{
	CFormView::AssertValid();
}
// For dumping the view
#ifndef _WIN32_WCE
void CReportWorkorderClosedView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/* Message map methods */

// For showing the print preview
void CReportWorkorderClosedView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

// Is called when the view is shown, for loading the data and showing it in the list control
// - bShow: A flag that indicates if the view is shown
// - nStatus: The status of the view
void CReportWorkorderClosedView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	if ( bShow ) {
		int nIndex{ 0 };			// Index of the list control item.
		auto row{ 0 };		// Row of the list control item.
		CString strBuildQuery{ _T("SELECT [WORKORDER_ID], [WORKORDER_DESCRIPTION], [WORKORDER_RESPONSIBLE], [WORKORDER_CLOSED_DATE], [WORKORDER_STATUS], ")
		_T("[WORKORDER_ASSET_ID], [ASSET_DESCRIPTION], [WORKORDER_CUSTOMER_ID], [WORKORDER_INVOICE_ID] ")
		_T("FROM [dbo].[WORKORDER], [dbo].[ASSET] WHERE ([WORKORDER_STATUS] = N'Closed' AND [ASSET_CUSTOMER_ID] = [WORKORDER_CUSTOMER_ID] ")
		_T("AND [WORKORDER_ASSET_ID] = [ASSET_ID])") };

		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
		theApp.BeginWaitCursor();

		m_lstWorkorderClosedReport.DeleteAllItems();

		CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

		if (sql.CreateSQLConnection()) {

			SQLCHAR szName[SQLCHARVSMALL]{};
			SQLLEN cbName{};
			SQLRETURN retcode{};
			SQLHSTMT hstmt{ sql.GetStatementHandle() };
			SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
			strBuildQuery.ReleaseBuffer();

			retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

			if (retcode == SQL_SUCCESS) {
				while (TRUE) {
					retcode = SQLFetch(hstmt);
					if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
						AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

						auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
							if (cbName == SQL_NULL_DATA) {
								return _T("");
							}
							return static_cast<CString>(szName);
							};

						// Get data for columns 1, employee names
						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						nIndex = m_lstWorkorderClosedReport.InsertItem(row++, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_CLOSED_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_STATUS, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.ASSET_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 6, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 7, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 8, CheckForNull(szName, cbName));
					}
					else
						break;
				}
			}
			if (!sql.CheckReturnCodeForClosing(retcode))
				theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
			else
				theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
		}
		sql.CloseConnection();
		theApp.EndWaitCursor();
		UpdateData(FALSE);
	}
}

// Is called when the user double clicks on a list control item, for showing the details of the closed workorder
// - pNMHDR: A pointer to a NMHDR structure
// - pResult: A pointer to a LRESULT structure
void CReportWorkorderClosedView::OnNMDoubleClickWorkorderClosedReport(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	auto pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1) {
		unsigned int nWorkorderID = _ttoi(m_lstWorkorderClosedReport.GetItemText(pNMItemActivate->iItem, 0));
		unsigned int nAssetID = _ttoi(m_lstWorkorderClosedReport.GetItemText(pNMItemActivate->iItem, 5));
		unsigned int nCustomerID = _ttoi(m_lstWorkorderClosedReport.GetItemText(pNMItemActivate->iItem, 7));
		unsigned int nInvoiceID = _ttoi(m_lstWorkorderClosedReport.GetItemText(pNMItemActivate->iItem, 8));
		CClosedWorkorderDetails dlg(nCustomerID, nAssetID, nWorkorderID, nInvoiceID);
		dlg.DoModal();
	}
	*pResult = 0;
}
/* Member methods */

// For setting the printer orientation
bool CReportWorkorderClosedView::SetPrinterOrientation(HANDLE h, CDC* dc) const noexcept
{
	DEVMODE* devMode;
	if (!h) return false;
	devMode = (DEVMODE*)::GlobalLock(h);    if (!devMode) return false;

	switch (m_ePrinterOrientation) {
	case PORTRAIT:
		devMode->dmOrientation = DMORIENT_PORTRAIT;  // portrait mode
		devMode->dmFields |= DM_ORIENTATION;
		break;

	case LANDSCAPE:
		devMode->dmOrientation = DMORIENT_LANDSCAPE; // landscape mode
		devMode->dmFields |= DM_ORIENTATION;
		break;
	}

	if (dc) dc->ResetDC(devMode);
	::GlobalUnlock(h);
	return true;
}