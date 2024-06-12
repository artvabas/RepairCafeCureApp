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
* Version: 1.0.0.1 (alpha)
* Created: 02-06-2023, (dd-mm-yyyy)
* Updated: 11-06-2024, (dd-mm-yyyy)
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

using namespace artvabas::sql;
using namespace artvabas::database::tables::closedworkorders;

IMPLEMENT_DYNCREATE(CReportWorkorderClosedView, CFormView)

CReportWorkorderClosedView::CReportWorkorderClosedView()
	: CFormView(IDD_REPORT_WORKOREER_CLOSED_FORM)
	, m_ePrinterOrientation(PORTRAIT)
{}

CReportWorkorderClosedView::~CReportWorkorderClosedView()
{}

BEGIN_MESSAGE_MAP(CReportWorkorderClosedView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CReportWorkorderClosedView::OnFilePrintPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDER_CLOSED_REPORT, &CReportWorkorderClosedView::OnNMDoubleClickWorkorderClosedReport)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/* Overrode methods */

void CReportWorkorderClosedView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORKORDER_CLOSED_REPORT, m_lstWorkorderClosedReport);
}

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

BOOL CReportWorkorderClosedView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

BOOL CReportWorkorderClosedView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CReportWorkorderClosedView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	SetPrinterOrientation(theApp.GetDeviceMode(), pDC);
	CFormView::OnBeginPrinting(pDC, pInfo);
}

void CReportWorkorderClosedView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CReportWorkorderClosedView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// Code here
	CFormView::OnPrint(pDC, pInfo);
}

#ifdef _DEBUG
void CReportWorkorderClosedView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CReportWorkorderClosedView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/* Message map methods */

void CReportWorkorderClosedView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

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

			SQLCHAR szName[SQLCHARVSMAL]{};
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
						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						nIndex = m_lstWorkorderClosedReport.InsertItem(row++, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_CLOSED_DATE, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_STATUS, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.ASSET_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 6, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
						m_lstWorkorderClosedReport.SetItemText(nIndex, 7, CheckForNull(szName, cbName));

						SQLGetData(hstmt, CLOSED_WORKORDERS.WORKORDER_INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
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

void CReportWorkorderClosedView::OnNMDoubleClickWorkorderClosedReport(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
/* Member methods */

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