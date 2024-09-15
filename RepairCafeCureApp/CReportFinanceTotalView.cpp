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
* File: CReportFinanceTotalView.h, implements class CReportFinanceTotalView
*
* This class is the view for generating a period report with the total finance flow.
* With this view, the user can select a period and generate a report.
* The report is displayed in a list control and can be printed.
*
* Target: Windows 10/11 64bit
* Version: 1.0.2.5 (beta)
* Created: 25-08-2024, (dd-mm-yyyy)
* Updated: 14-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CReportFinanceTotalView.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::sql;
using namespace artvabas::database::tables::reportfinancetotal;

IMPLEMENT_DYNCREATE(CReportFinanceTotalView, CFormView)

CReportFinanceTotalView::CReportFinanceTotalView()
	: CFormView(IDD_REPORT_FINANCE_TOTAL_FORM)
	, m_dtStart(COleDateTime::GetCurrentTime())
	, m_dtEnd(COleDateTime::GetCurrentTime())
	, m_strTotalInvoiceCash(_T(""))
	, m_strTotalInvoicePin(_T(""))
	, m_strTotalInvoice(_T(""))
	, m_strTotalContributionCash(_T(""))
	, m_strTotalContributionPin(_T(""))
	, m_strTotalContribution(_T(""))
	, m_ePrinterOrientation(PORTRAIT)
{}

CReportFinanceTotalView::~CReportFinanceTotalView()
{}

// CReportFinanceTotalView Message Handlers bindings
BEGIN_MESSAGE_MAP(CReportFinanceTotalView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CReportFinanceTotalView::OnFilePrintPreview)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FINANCE_TOTAL_FLOW_START_DATE, &CReportFinanceTotalView::OnDtnDateTimeChangeFinanceTotalFlowStartDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FINANCE_TOTAL_FLOW_END_DATE, &CReportFinanceTotalView::OnDtnDateTimeChangeFinanceTotalFlowStartDate)
	ON_BN_CLICKED(IDC_FINANCE_TOTAL_FLOW_CREATE, &CReportFinanceTotalView::OnBnClickedFinanceTotalFlowCreate)
END_MESSAGE_MAP()

/* Override methods */

// DoDataExchange, is called by the framework to exchange and validate dialog data
void CReportFinanceTotalView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_FINANCE_TOTAL_FLOW_START_DATE, m_dtStart);
	DDX_DateTimeCtrl(pDX, IDC_FINANCE_TOTAL_FLOW_END_DATE, m_dtEnd);
	DDX_Text(pDX, IDC_FINANCE_TOTAL_FLOW_TOTAL_INVOICE_CASH, m_strTotalInvoiceCash);
	DDX_Text(pDX, IDC_FINANCE_TOTAL_FLOW_TOTAL_INVOICE_PIN, m_strTotalInvoicePin);
	DDX_Text(pDX, IDC_FINANCE_TOTAL_FLOW_TOTAL_INVOICE, m_strTotalInvoice);
	DDX_Text(pDX, IDC_FINANCE_TOTAL_FLOW_TOTAL_CONTRIBUTION_CASH, m_strTotalContributionCash);
	DDX_Text(pDX, IDC_FINANCE_TOTAL_FLOW_TOTAL_CONTRIBUTION_PIN, m_strTotalContributionPin);
	DDX_Text(pDX, IDC_FINANCE_TOTAL_FLOW_TOTAL_CONTRIBUTION, m_strTotalContribution);
	DDX_Control(pDX, IDC_FINANCE_TOTAL_FLOW_RESULT_LIST, m_lstTotalReportResults);
	DDX_Control(pDX, IDC_FINANCE_TOTAL_FLOW_CREATE, m_btnCreateReport);
}

// OninitialUpdate, is called after the view is first attached to the document
void CReportFinanceTotalView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_lstTotalReportResults.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstTotalReportResults.InsertColumn(0, theApp.ConvertIDToString(IDS_DBT_DATE_COMPLETE), LVCFMT_LEFT, 100);
	m_lstTotalReportResults.InsertColumn(1, theApp.ConvertIDToString(IDS_DBT_WORKORDER_ID), LVCFMT_LEFT, 100);
	m_lstTotalReportResults.InsertColumn(2, theApp.ConvertIDToString(IDS_DBT_INVOICE_ID), LVCFMT_LEFT, 100);
	m_lstTotalReportResults.InsertColumn(3, theApp.ConvertIDToString(IDS_DBT_TOTAL), LVCFMT_RIGHT, 60);
	m_lstTotalReportResults.InsertColumn(4, theApp.ConvertIDToString(IDS_DBT_PIN), LVCFMT_RIGHT, 50);
	m_lstTotalReportResults.InsertColumn(5, theApp.ConvertIDToString(IDS_DBT_CONTRIBUTION), LVCFMT_RIGHT, 100);
}

// OnPreparePrinting, is called by the framework tp prepare the printing job
// - pInfo: a pointer to a CPrintInfo structure that contains information about the print job
// Returns: TRUE if the print job is prepared successfully; otherwise FALSE
BOOL CReportFinanceTotalView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

// OnBeginPrinting, is called by the framework when the printing job is about to begin
// - pDC: a pointer to the device context for the printer
// - pInfo: a pointer to a CPrintInfo structure that contains information about the print job
void CReportFinanceTotalView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	SetPrinterOrientation(theApp.GetDeviceMode(), pDC);
	CFormView::OnBeginPrinting(pDC, pInfo);
}

// OnEndPrinting, is called by the framework when the printing job is about to end
// - pDC: a pointer to the device context for the printer
// - pInfo: a pointer to a CPrintInfo structure that contains information about the print job
void CReportFinanceTotalView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	pInfo->m_bPreview ? theApp.SetPrintPreview(true) : theApp.SetPrintPreview(false);
	CFormView::OnEndPrinting(pDC, pInfo);
}

// OnPrint, is called by the framework to print the view
// - pDC: a pointer to the device context for the printer
// - pInfo: a pointer to a CPrintInfo structure that contains information about the print job
void CReportFinanceTotalView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
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

	pDC->DrawText(_T("Totaal overzicht alle transacties, maand ") + static_cast<CString>(wzsMonth[m_dtStart.GetMonth()]) + _T(" van ") + m_dtStart.Format(_T("%Y")),
		rctHeader, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// calculate new start print position
	nPosY += HeaderTextLineDown(3);

	CRect rctDate(nPosX, nPosY, nPosX + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctDate, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorder(rctDate.right, nPosY, rctDate.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorder, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctInvoice(rctWorkorder.right, nPosY, rctWorkorder.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctInvoice, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctTotal(rctInvoice.right, nPosY, rctInvoice.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctTotal, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctPin(rctTotal.right, nPosY, rctTotal.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctPin, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctContribution(rctPin.right, nPosY, rctPin.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctContribution, RGB(255, 255, 255), RGB(255, 255, 255));

	// Print Table Header
	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	// print work comment area
	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(_T("Datum:"), rctDate, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Werkorder:"), rctWorkorder, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Factuur:"), rctInvoice, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Bedrag:"), rctTotal, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("PIN:"), rctPin, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Bijdrage:"), rctContribution, DT_RIGHT | DT_TABSTOP);

	// Print Table Body
	pFont = &fontPlainBody;
	pDC->SelectObject(pFont);

	// Print body text
	nPosY += BodyTextLineDown(1);

	for (int i = 0; i < m_lstTotalReportResults	.GetItemCount(); i++) {
		CString strDate = m_lstTotalReportResults.GetItemText(i, 0);
		CString strWorkorder = m_lstTotalReportResults.GetItemText(i, 1);
		CString strInvoice = m_lstTotalReportResults.GetItemText(i, 2);
		CString strTotal = m_lstTotalReportResults.GetItemText(i, 3);
		CString strPin = m_lstTotalReportResults.GetItemText(i, 4);
		CString strContribution = m_lstTotalReportResults.GetItemText(i, 5);

		CRect rctDate(nPosX, nPosY, nPosX + TotalTabInPixels(6), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strDate, rctDate, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorder(rctDate.right, nPosY, rctDate.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strWorkorder, rctWorkorder, DT_LEFT | DT_TABSTOP);

		CRect rctInvoice(rctWorkorder.right, nPosY, rctWorkorder.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strInvoice, rctInvoice, DT_LEFT | DT_TABSTOP);

		CRect rctTotal(rctInvoice.right, nPosY, rctInvoice.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strTotal, rctTotal, DT_LEFT | DT_TABSTOP);

		CRect rctPin(rctTotal.right, nPosY, rctTotal.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strPin, rctPin, DT_LEFT | DT_TABSTOP);

		CRect rctContribution(rctPin.right, nPosY, rctPin.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(1));
		pDC->DrawText(strContribution, rctContribution, DT_RIGHT | DT_TABSTOP);

		nPosY += BodyTextLineDown(1);
	}

	// Print footer
	nPosY += BodyTextLineDown(1);

	pFont = &fontBoldBody;
	pDC->SelectObject(pFont);
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));

	CRect rctTotalAll(rctTotal.right, nPosY, rctTotal.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctTotal, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctTotalAmount(rctTotalAll.right, nPosY, rctTotalAll.right + TotalTabInPixels(6), nPosY + BodyTextLineDown(20));
	pDC->Draw3dRect(rctTotalAmount, RGB(255, 255, 255), RGB(255, 255, 255));

	pDC->DrawText(_T("Factuur contant:"), rctTotalAll, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(m_strTotalInvoiceCash, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	//nPosY += BodyTextLineDown(1);
	rctTotalAll.OffsetRect(0, BodyTextLineDown(1));
	rctTotalAmount.OffsetRect(0, BodyTextLineDown(1));

	pDC->DrawText(_T("Factuur pin:"), rctTotalAll, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(m_strTotalInvoicePin, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	rctTotalAll.OffsetRect(0, BodyTextLineDown(1));
	rctTotalAmount.OffsetRect(0, BodyTextLineDown(1));

	pDC->DrawText(_T("Factuur total:"), rctTotalAll, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(m_strTotalInvoice, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	rctTotalAll.OffsetRect(0, BodyTextLineDown(2));
	rctTotalAmount.OffsetRect(0, BodyTextLineDown(2));

	pDC->DrawText(_T("Bijdrage contant:"), rctTotalAll, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(m_strTotalContributionCash, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	rctTotalAll.OffsetRect(0, BodyTextLineDown(1));
	rctTotalAmount.OffsetRect(0, BodyTextLineDown(1));

	pDC->DrawText(_T("Bijdrage pin:"), rctTotalAll, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(m_strTotalContributionPin, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	rctTotalAll.OffsetRect(0, BodyTextLineDown(1));
	rctTotalAmount.OffsetRect(0, BodyTextLineDown(1));

	pDC->DrawText(_T("Bijdrage totaal:"), rctTotalAll, DT_RIGHT | DT_TABSTOP);
	pDC->DrawText(m_strTotalContribution, rctTotalAmount, DT_RIGHT | DT_TABSTOP);

	nPosY += BodyTextLineDown(8);
	pFont = &fontPlainBody;
	pDC->SelectObject(pFont);
	COleDateTime cdtNow = COleDateTime::GetCurrentTime();

	pDC->TextOutW(nPosX, nPosY, _T("Dit overzicht is gegenereerd door Repair Cafe Cure App op ") + cdtNow.Format(_T("%d-%m-%Y")));

	// Destroy image
	imgLogo.Destroy();
	fontBoldHeader.DeleteObject();
	fontPlainBody.DeleteObject();
	fontBoldBody.DeleteObject();

	CFormView::OnPrint(pDC, pInfo);

	CFormView::OnPrint(pDC, pInfo);
}

#ifdef _DEBUG
void CReportFinanceTotalView::AssertValid() const
{
	CFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CReportFinanceTotalView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/* Message Handlers */ 

// OnFilePrintPreview, is called by the framework to show the print preview dialog
void CReportFinanceTotalView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

// OnDtnDateTimeChangeFinanceTotalFlowStartDate, is called when the date time picker control date changes
// Enenable the create report button if the start date is less than the end date
// - pNMHDR: a pointer to a NMHDR structure that contains information about a notification message
// - pResult: a pointer to an LRESULT variable that receives the result of the notification message
void CReportFinanceTotalView::OnDtnDateTimeChangeFinanceTotalFlowStartDate(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	if (pDTChange->dwFlags == GDT_VALID) {
		UpdateData(TRUE);
		if (m_dtStart >= m_dtEnd) {
			m_btnCreateReport.EnableWindow(FALSE);
		} else {
			m_btnCreateReport.EnableWindow(TRUE);
		}
	}
	*pResult = 0;
}

// OnBnClickedFinanceTotalFlowCreate, is called when the create report button is clicked
// Get the data from the database and display it in the list control
// And calculate the total invoice and contribution
void CReportFinanceTotalView::OnBnClickedFinanceTotalFlowCreate() noexcept
{
	UpdateData(TRUE);

	m_lstTotalReportResults.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	auto row(0);		// Row of the list control item.
	CString strSqlQuery;
	CString strTemp;
	double dTotalInvoiceCash{ 0.0 };
	double dTotalInvoicePin{ 0.0 };
	double dTotalInvoice{ 0.0 };
	double dTotalContributionCash{ 0.0 };
	double dTotalContributionPin{ 0.0 };
	double dTotalContribution{ 0.0 };
	double dTemp{ 0.0 };
	bool bIsPin{ false };

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	strSqlQuery.Format(_T("SELECT COALESCE([INVOICE_CREATE_DATE],[CONTRIBUTION_CREATEDATE]) AS CREATE_DATE, ")
		_T("COALESCE([INVOICE_WORKORDER_ID],[CONTRIBUITION_WORKORDER_ID]) AS WORKORDER_ID, ")
		_T("[INVOICE_ID], [INVOICE_TOTAL], ")
		_T("COALESCE([INVOICE_PAYMENT_PIN],[CONTRIBUTION_PIN]) AS TRANSACTION_PIN, ")
		_T("[CONTRIBUTION_AMOUNT] ")
		_T("FROM [dbo].[INVOICE] ")
		_T("FULL OUTER JOIN [dbo].[CONTRIBUTION] ON [INVOICE_WORKORDER_ID] = [CONTRIBUITION_WORKORDER_ID] ")
		_T("WHERE COALESCE(INVOICE_CREATE_DATE, CONTRIBUTION_CREATEDATE) BETWEEN N'%s' AND N'%s' ")
		_T("ORDER BY CREATE_DATE"),
		static_cast<LPCTSTR>(m_dtStart.Format(_T("%Y-%m-%d"))),
		static_cast<LPCTSTR>(m_dtEnd.Format(_T("%Y-%m-%d"))));

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		double dTotalAmount{ 0.0 };
		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strSqlQuery.GetBuffer() };
		strSqlQuery.ReleaseBuffer();

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

					auto CheckForNullDouble = [](SQLCHAR* szName, SQLLEN cbName) -> double {
						if (cbName == SQL_NULL_DATA) {
							return 0.0;
						}
						return _wtof(static_cast<CString>(szName));
					};

					SQLGetData(hstmt, REPORT_FINANCE_TOTAL.CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lstTotalReportResults.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_FINANCE_TOTAL.WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstTotalReportResults.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_FINANCE_TOTAL.INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstTotalReportResults.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_FINANCE_TOTAL.INVOICE_TOTAL, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					dTemp = CheckForNullDouble(szName, cbName);
					dTotalInvoice += dTemp;
					strTemp.Format(_T("%.2f"), dTemp);
					m_lstTotalReportResults.SetItemText(nIndex, 3, strTemp);

					SQLGetData(hstmt, REPORT_FINANCE_TOTAL.TRANSACTION_PIN, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					strTemp = CheckForNull(szName, cbName);
					if (strTemp.IsEmpty()) bIsPin = false;
					else bIsPin = static_cast<bool>(_wtoi(strTemp));
					m_lstTotalReportResults.SetItemText(nIndex, 4, strTemp);
					
					if (bIsPin) {
						dTotalInvoicePin += dTemp;
					}
					else {
						dTotalInvoiceCash += dTemp;
					}

					SQLGetData(hstmt, REPORT_FINANCE_TOTAL.CONTRIBUTION_AMOUNT, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					dTemp = CheckForNullDouble(szName, cbName);
					dTotalContribution += dTemp;
					strTemp.Format(_T("%.2f"), dTemp);
					m_lstTotalReportResults.SetItemText(nIndex, 5, strTemp);

					if (bIsPin) {
						dTotalContributionPin += dTemp;
					}
					else {
						dTotalContributionCash += dTemp;
					}
				}
				else
					break;
			}
		}
		m_strTotalContribution.Format(_T("%.2f"), dTotalContribution);
		m_strTotalContributionCash.Format(_T("%.2f"), dTotalContributionCash);
		m_strTotalContributionPin.Format(_T("%.2f"), dTotalContributionPin);
		m_strTotalInvoice.Format(_T("%.2f"), dTotalInvoice);
		m_strTotalInvoiceCash.Format(_T("%.2f"), dTotalInvoiceCash);
		m_strTotalInvoicePin.Format(_T("%.2f"), dTotalInvoicePin);
		UpdateData(FALSE);

		if (!sql.CheckReturnCodeForClosing(retcode)) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		}
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	m_btnCreateReport.EnableWindow(FALSE);
	theApp.EndWaitCursor();
}

/* class methods */

// SetPrinterOrientation, sets the printer orientation
// - h: a handle to the printer device
// - dc: a pointer to a CDC object
// Returns: TRUE if the printer orientation is set successfully; otherwise FALSE
bool CReportFinanceTotalView::SetPrinterOrientation(HANDLE h, CDC* dc) const noexcept
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