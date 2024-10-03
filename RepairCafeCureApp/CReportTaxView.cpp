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
* File: CReportTaxView.h, implement class CReportTaxView
*
* This class is the view for generating contribution and PIN transaction reports for tax uses.
* With this view, the user can select a period and generate a report.
* The report is displayed in a list control and can be printed.
*
* Target: Windows 10/11 64bit
* Version: 1.0.4.5 (beta)
* Created: 02-06-2023, (dd-mm-yyyy)
* Updated: 21-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CReportTaxView.h"
#include "DatabaseTables.h"
#include "CPrintHelper.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::sql;
using namespace artvabas::database::tables::reportcontributiontax;
using namespace artvabas::rcc::support;

IMPLEMENT_DYNCREATE(CReportTaxView, CFormView)

CReportTaxView::CReportTaxView()
	: CFormView(IDD_REPORT_FINANCE_TAX_FORM)
	, m_cdtStartDate(COleDateTime::GetCurrentTime())
	, m_cdtEndDate(COleDateTime::GetCurrentTime())
	, m_strTotalAmountContribution(_T(""))
	, m_strReportTaxKind(_T(""))
	, m_ePrinterOrientation(PORTRAIT)
{}

CReportTaxView::~CReportTaxView()
{}

// CReportTaxView Message Handlers bindings
BEGIN_MESSAGE_MAP(CReportTaxView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CReportTaxView::OnFilePrintPreview)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_REPORT_TAX_PERIOD_START, &CReportTaxView::OnDtnDateTimeChangeReportTaxPeriod)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_REPORT_TAX_PERIOD_END, &CReportTaxView::OnDtnDateTimeChangeReportTaxPeriod)
	ON_BN_CLICKED(IDC_REPORT_TAX_PERIOD_CREATE, &CReportTaxView::OnBnClickedReportTaxPeriodCreate)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/* Override methods */

// DoDataExchange: Exchange and validate dialog data values with controls
// @param pDX: A pointer to a CDataExchange object
void CReportTaxView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_REPORT_TAX_PERIOD_START, m_cdtStartDate);
	DDX_DateTimeCtrl(pDX, IDC_REPORT_TAX_PERIOD_END, m_cdtEndDate);
	DDX_Control(pDX, IDC_REPORT_TAX_PERIOD_CREATE, m_btnCreateReport);
	DDX_Control(pDX, IDC_REPORT_TAX_PERIOD_RESULT, m_lstReportResultTax);
	DDX_Text(pDX, IDC_REPORT_TAX_PERIOD_TOTAL_AMOUNT, m_strTotalAmountContribution);
	DDX_Text(pDX, IDC_REPORT_TAX_PERIOD_KIND, m_strReportTaxKind);
}

// OnInitialUpdate: Called after the view is first time created
// Initialize the list control and set the extended style
void CReportTaxView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_lstReportResultTax.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstReportResultTax.InsertColumn(0, theApp.ConvertIDToString(IDS_DBT_DATE_COMPLETE), LVCFMT_LEFT, 100);
	m_lstReportResultTax.InsertColumn(1, theApp.ConvertIDToString(IDS_DBT_WORKORDER_ID), LVCFMT_LEFT, 100);
	m_lstReportResultTax.InsertColumn(2, theApp.ConvertIDToString(IDS_DBT_SURNAME), LVCFMT_LEFT, 100);
	m_lstReportResultTax.InsertColumn(3, theApp.ConvertIDToString(IDS_DBT_SURNAME), LVCFMT_LEFT, 100);
	m_lstReportResultTax.InsertColumn(4, theApp.ConvertIDToString(IDS_DBT_CELL_PHONE), LVCFMT_LEFT, 100);
	m_lstReportResultTax.InsertColumn(5, theApp.ConvertIDToString(IDS_DBT_AMOUNT_MONEY), LVCFMT_RIGHT, 100);
	
	m_btnCreateReport.SetTextColor(RGB(255, 0, 0));
}

// PreTranslateMessage: Called before the window message is translated
// Capture the Enter key and call the OnBnClickedReportTaxPeriodCreate method
// when the button is enabled.
// @param pMsg: A pointer to a MSG structure
BOOL CReportTaxView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			if (m_btnCreateReport.IsWindowEnabled()) {
				OnBnClickedReportTaxPeriodCreate();
				return TRUE;
			}
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

// OnPreparePrinting: Called before the print job is started
// @param pInfo: A pointer to a CPrintInfo structure
BOOL CReportTaxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Calculate total pages to print, 55 items each page
	int nPages = m_lstReportResultTax.GetItemCount() / 55;
	if (m_lstReportResultTax.GetItemCount() % 55 > 0) nPages++;
	pInfo->SetMaxPage(nPages);

	return DoPreparePrinting(pInfo);
}

// OnBeginPrinting: Called before the print job is started
// Set the printer orientation
// @param pDC: A pointer to a CDC object
// @param pInfo: A pointer to a CPrintInfo structure
void CReportTaxView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	SetPrinterOrientation(theApp.GetDeviceMode(), pDC);
	CFormView::OnBeginPrinting(pDC, pInfo);
}

// OnEndPrinting: Called after the print job is finished
// @param pDC: A pointer to a CDC object
void CReportTaxView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	pInfo->m_bPreview ? theApp.SetPrintPreview(true) : theApp.SetPrintPreview(false);
	CFormView::OnEndPrinting(pDC, pInfo);
}

// OnPrint: Called to print the report or to print preview
// @param pDC: A pointer to a CDC object
// @param pInfo: A pointer to a CPrintInfo structure
void CReportTaxView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	const wchar_t* wzsMonth[] = { L"januari", L"februari", L"maart", L"april", L"mei",  L"juni", 
								 L"juli", L"augustus", L"september", L"oktober", L"november", L"december"};

	CPrintHelper PH{ pDC };
	pDC->m_bPrinting = TRUE;
	pDC->StartPage();

	PH.PrintLogo();

	if (theApp.GetFinanceTaxViewType() == VIEW_CONTRIBUTON_REPORT) {
		PH.PrintHeader(_T("Overzicht vrijwillige bijdrage, maand ") + static_cast<CString>(wzsMonth[m_cdtStartDate.GetMonth() - 1]) + _T(" van ") + m_cdtStartDate.Format(_T("%Y")));
	}
	else if (theApp.GetFinanceTaxViewType() == VIEW_PIN_TRANSACTION_REPORT) {
		PH.PrintHeader(_T("Overzicht PIN transacties, maand ") + static_cast<CString>(wzsMonth[m_cdtStartDate.GetMonth() -1 ]) + _T(" van ") + m_cdtStartDate.Format(_T("%Y")));
	}
	else if (theApp.GetFinanceTaxViewType() == VIEW_CASH_TRANSACTION_REPORT) {
		PH.PrintHeader(_T("Overzicht contante transacties, maand ") + static_cast<CString>(wzsMonth[m_cdtStartDate.GetMonth() -1 ]) + _T(" van ") + m_cdtStartDate.Format(_T("%Y")));
	}
	
	CRect rctDate(*PH.m_pxPos, *PH.m_pyPos, *PH.m_pxPos + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctDate, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorder(rctDate.right, *PH.m_pyPos, rctDate.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorder, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctSurName(rctWorkorder.right, *PH.m_pyPos, rctWorkorder.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctSurName, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctHomePhone(rctSurName.right, *PH.m_pyPos, rctSurName.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctHomePhone, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctMobilePhone(rctHomePhone.right, *PH.m_pyPos, rctHomePhone.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctMobilePhone, RGB(255, 255, 255), RGB(255, 255, 255));
	
	CRect rctContribution(rctMobilePhone.right, *PH.m_pyPos, rctMobilePhone.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctContribution, RGB(255, 255, 255), RGB(255, 255, 255));

	// Print Table Header
	PH.m_pFont = &PH.m_fontBoldBody;
	pDC->SelectObject(PH.m_pFont);
	
	// print work comment area
	PH.m_pFont = &PH.m_fontBoldBody;
	pDC->SelectObject(PH.m_pFont);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(_T("Datum:"), rctDate, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Werkorder:"), rctWorkorder, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Naam:"), rctSurName, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Telefoon:"), rctHomePhone, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Mobiel:"), rctMobilePhone, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Bijdrage:"), rctContribution, DT_RIGHT | DT_TABSTOP);

	// Print Table Body
	PH.m_pFont = &PH.m_fontPlainBody;
	pDC->SelectObject(PH.m_pFont);

	// Print body text
	*PH.m_pyPos += PH.BodyTextLineDown(1);

	// 55 items each page
	int nPage = pInfo->m_nCurPage;
	int i = (nPage - 1) * 55;
	if (i > 0) i += (nPage - 1);

	for ( i; i < m_lstReportResultTax.GetItemCount(); i++) {
		CString strDate = m_lstReportResultTax.GetItemText(i, 0);
		CString strWorkorder = m_lstReportResultTax.GetItemText(i, 1);
		CString strSurName = m_lstReportResultTax.GetItemText(i, 2);
		CString strHomePhone = m_lstReportResultTax.GetItemText(i, 3);
		CString strMobilePhone = m_lstReportResultTax.GetItemText(i, 4);
		CString strContribution = m_lstReportResultTax.GetItemText(i, 5);

		CRect rctDate(*PH.m_pxPos, *PH.m_pyPos, *PH.m_pxPos + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strDate, rctDate, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorder(rctDate.right, *PH.m_pyPos, rctDate.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strWorkorder, rctWorkorder, DT_LEFT | DT_TABSTOP);

		CRect rctSurName(rctWorkorder.right, *PH.m_pyPos, rctWorkorder.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strSurName, rctSurName, DT_LEFT | DT_TABSTOP);

		CRect rctHomePhone(rctSurName.right, *PH.m_pyPos, rctSurName.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strHomePhone, rctHomePhone, DT_LEFT | DT_TABSTOP);

		CRect rctMobilePhone(rctHomePhone.right, *PH.m_pyPos, rctHomePhone.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strMobilePhone, rctMobilePhone, DT_LEFT | DT_TABSTOP);

		CRect rctContribution(rctMobilePhone.right, *PH.m_pyPos, rctMobilePhone.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strContribution, rctContribution, DT_RIGHT | DT_TABSTOP);

		*PH.m_pyPos += PH.BodyTextLineDown(1);

		// reach 55 items per page
		if (*PH.m_pyPos == PH.m_pixEndPage) {
			break;
		}
	}

	// If last page, then print totals
	if (pInfo->m_nCurPage == pInfo->GetMaxPage()) {
		
		// Print report footer
		*PH.m_pyPos += PH.BodyTextLineDown(1);

		PH.m_pFont = &PH.m_fontBoldHeader;
		pDC->SelectObject(PH.m_pFont);
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));

		CRect rctTotal(rctHomePhone.right, *PH.m_pyPos, rctHomePhone.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
		pDC->Draw3dRect(rctTotal, RGB(255, 255, 255), RGB(255, 255, 255));

		CRect rctTotalAmount(rctTotal.right, *PH.m_pyPos, rctTotal.right + PH.TotalTabInPixels(6), *PH.m_pyPos + PH.BodyTextLineDown(20));
		pDC->Draw3dRect(rctTotalAmount, RGB(255, 255, 255), RGB(255, 255, 255));

		pDC->DrawText(_T("Totaal:"), rctTotal, DT_RIGHT | DT_TABSTOP);
		pDC->DrawText(m_strTotalAmountContribution, rctTotalAmount, DT_RIGHT | DT_TABSTOP);
	}

	// Print footer
	PH.PrintFooter(pInfo->m_nCurPage);
	
	CFormView::OnPrint(pDC, pInfo);
}

#ifdef _DEBUG
// AssertValid: Asserts the validity of the view
void CReportTaxView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
// Dump: Dumps the view
void CReportTaxView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/* message handlers */

// OnFilePrintPreview: Called to show the print preview dialog
void CReportTaxView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

// OnDtnDateTimeChangeReportTaxPeriod: Called when the date time control is changed
// Enable the create report button when the start date is less than the end date
// @param pNMHDR: A pointer to a NMHDR structure
// @param pResult: A pointer to a LRESULT structure
void CReportTaxView::OnDtnDateTimeChangeReportTaxPeriod(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(TRUE);
	if (m_cdtStartDate < m_cdtEndDate)
		m_btnCreateReport.EnableWindow(TRUE);
	else
		m_btnCreateReport.EnableWindow(FALSE);
	*pResult = 0;
}

// OnShowWindow: Called when the window is shown
// Initialize the list control and set the extended styl.
// Because the view is shared, the list control is cleared and the total amount is set to empty.
// It's shared because the view is used to generate a contribution report or a PIN transaction report.
// @param bShow: A boolean value to show the window
// @param nStatus: An unsigned integer value to show the status
void CReportTaxView::OnShowWindow(BOOL bShow, UINT nStatus) noexcept
{
	CFormView::OnShowWindow(bShow, nStatus);

	if ( bShow && !theApp.IsPrintPreview() ) {
		m_lstReportResultTax.DeleteAllItems();
		m_strTotalAmountContribution.Empty();
		m_cdtEndDate = COleDateTime::GetCurrentTime();
		m_cdtStartDate = COleDateTime::GetCurrentTime();
		m_btnCreateReport.EnableWindow(FALSE);
		if (theApp.GetFinanceTaxViewType() == VIEW_CONTRIBUTON_REPORT) {
			m_strReportTaxKind = _T("Contribution Report");
		}
		else if (theApp.GetFinanceTaxViewType() == VIEW_PIN_TRANSACTION_REPORT) {
			m_strReportTaxKind = _T("PIN Transaction Report");
		}
		else if (theApp.GetFinanceTaxViewType() == VIEW_CASH_TRANSACTION_REPORT) {
			m_strReportTaxKind = _T("Cash Transaction Report");
		}
		UpdateData(FALSE);
	}
}

// OnBnClickedReportTaxPeriodCreate: Called when the create report button is clicked
// Create a report of the contributions or PIN transaction in the selected period from the database
// Display the report in the list control
void CReportTaxView::OnBnClickedReportTaxPeriodCreate() noexcept
{
	UpdateData(TRUE);

	m_lstReportResultTax.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	auto row(0);		// Row of the list control item.
	CString strSqlQuery;

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	if (theApp.GetFinanceTaxViewType() == VIEW_CONTRIBUTON_REPORT) {
		strSqlQuery.Format(_T("SELECT [CONTRIBUTION_CREATEDATE], [CONTRIBUITION_WORKORDER_ID], [CUSTOMER_SURNAME], [CUSTOMER_PHONE], ")
			_T("[CUSTOMER_CELL_PHONE], [CONTRIBUTION_AMOUNT] FROM [dbo].[CONTRIBUTION], [dbo].[CUSTOMER] ")
			_T("WHERE ([CONTRIBUTION_CREATEDATE] >= N\'%s\' AND [CONTRIBUTION_CREATEDATE] <= N\'%s\' AND [CUSTOMER_ID] = [CONTRIBUTION_CUSTOMER_ID] AND [CUSTOMER_PARTIAL_INVOICE] = 0)"),
			static_cast<LPCTSTR>(m_cdtStartDate.Format(_T("%Y-%m-%d"))),
			static_cast<LPCTSTR>(m_cdtEndDate.Format(_T("%Y-%m-%d"))));
	}
	else if (theApp.GetFinanceTaxViewType() == VIEW_PIN_TRANSACTION_REPORT) {
		strSqlQuery.Format(_T("SELECT [INVOICE_CREATE_DATE], [INVOICE_WORKORDER_ID], [CUSTOMER_SURNAME], [CUSTOMER_PHONE], ")
			_T("[CUSTOMER_CELL_PHONE], [INVOICE_TOTAL] FROM [dbo].[INVOICE], [dbo].[CUSTOMER] ")
			_T("WHERE ([INVOICE_CREATE_DATE] >= N\'%s\' AND [INVOICE_CREATE_DATE] <= N\'%s\' AND [CUSTOMER_ID] = [INVOICE_CUSTOMER_ID] ")
			_T("AND [CUSTOMER_PARTIAL_INVOICE] = 0 AND [INVOICE_PAYMENT_PIN] = 1)"),
			static_cast<LPCTSTR>(m_cdtStartDate.Format(_T("%Y-%m-%d"))),
			static_cast<LPCTSTR>(m_cdtEndDate.Format(_T("%Y-%m-%d"))));
	}
	else if (theApp.GetFinanceTaxViewType() == VIEW_CASH_TRANSACTION_REPORT) {
		strSqlQuery.Format(_T("SELECT [INVOICE_CREATE_DATE], [INVOICE_WORKORDER_ID], [CUSTOMER_SURNAME], [CUSTOMER_PHONE], ")
			_T("[CUSTOMER_CELL_PHONE], [INVOICE_TOTAL] FROM [dbo].[INVOICE], [dbo].[CUSTOMER] ")
			_T("WHERE ([INVOICE_CREATE_DATE] >= N\'%s\' AND [INVOICE_CREATE_DATE] <= N\'%s\' AND [CUSTOMER_ID] = [INVOICE_CUSTOMER_ID] ")
			_T("AND [CUSTOMER_PARTIAL_INVOICE] = 0 AND [INVOICE_PAYMENT_PIN] = 0)"),
			static_cast<LPCTSTR>(m_cdtStartDate.Format(_T("%Y-%m-%d"))),
			static_cast<LPCTSTR>(m_cdtEndDate.Format(_T("%Y-%m-%d"))));
	}

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
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
					MessageBoxW(theApp.ConvertIDToString(IDS_MSGBT_ERROR_FETCH_TAXREPORT), theApp.ConvertIDToString(IDS_MSGBC_ERROR_CONNECT_DS), MB_ICONERROR);
					break;
				}
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if (cbName == SQL_NULL_DATA) {
							return _T("");
						}
						return static_cast<CString>(szName);
						};

					SQLGetData(hstmt, REPORT_CONTRIBUTION_TAX.CONTRIBUTION_CREATEDATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lstReportResultTax.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_CONTRIBUTION_TAX.CONTRIBUITION_WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstReportResultTax.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_CONTRIBUTION_TAX.CUSTOMER_SURNAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstReportResultTax.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_CONTRIBUTION_TAX.CUSTOMER_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstReportResultTax.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_CONTRIBUTION_TAX.CUSTOMER_CELL_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstReportResultTax.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

					SQLGetData(hstmt, REPORT_CONTRIBUTION_TAX.CONTRIBUTION_AMOUNT, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					auto strAmount = CheckForNull(szName, cbName);
					dTotalAmount += _tstof(strAmount);
					strAmount = strAmount.Left(strAmount.Find(_T('.')) + 3);
					m_lstReportResultTax.SetItemText(nIndex, 5, strAmount);
				}
				else
					break;
			}
			m_strTotalAmountContribution.Format(_T("€%.2f"), dTotalAmount);
			UpdateData(FALSE);
		}
		if (!sql.CheckReturnCodeForClosing(retcode)) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		}
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);

		if (m_lstReportResultTax.GetItemCount() > 0) {
			m_lstReportResultTax.EnableWindow();
		}
		else {
			m_lstReportResultTax.EnableWindow(FALSE);
		}
	}
	sql.CloseConnection();
	m_btnCreateReport.EnableWindow(FALSE);
	theApp.EndWaitCursor();
}

/* Member methods */

// SetPrinterOrientation: Set the printer orientation
// @param h: A handle to the printer device
// @param dc: A pointer to a CDC object
// @return: A boolean value
bool CReportTaxView::SetPrinterOrientation(HANDLE h, CDC* dc) const noexcept
{
	DEVMODE* devMode;
	if (!h) return false;
	devMode = (DEVMODE*)::GlobalLock(h);    if (!devMode) return false;

	switch (m_ePrinterOrientation) {
	case PORTRAIT:
		devMode->dmPaperSize = DMPAPER_A4;
		devMode->dmOrientation = DMORIENT_PORTRAIT;  // portrait mode
		devMode->dmFields |= (DM_ORIENTATION | DM_PAPERSIZE);
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