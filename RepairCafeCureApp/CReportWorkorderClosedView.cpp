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
#include "CReportWorkorderClosedView.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"
#include "CClosedWorkorderDetails.h"
#include "CPrintHelper.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::sql;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::database::tables::closedworkorders;
using namespace artvabas::rcc::support;

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

	m_lstWorkorderClosedReport.InsertColumn(0, theApp.ConvertIDToString(IDS_DBT_WORKORDER_ID), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(1, theApp.ConvertIDToString(IDS_DBT_WORKORDER_DESCRIPTION), LVCFMT_LEFT, 300);
	m_lstWorkorderClosedReport.InsertColumn(2, theApp.ConvertIDToString(IDS_DBT_RESPONSIBLE), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(3, theApp.ConvertIDToString(IDS_DBT_CLOSED_DATE), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(4, theApp.ConvertIDToString(IDS_DBT_STATUS), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(5, theApp.ConvertIDToString(IDS_DBT_ASSET), LVCFMT_LEFT, 0);
	m_lstWorkorderClosedReport.InsertColumn(6, theApp.ConvertIDToString(IDS_DBT_ASSET_DESCRIPTION), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(7, theApp.ConvertIDToString(IDS_DBT_CUSTOMER_ID), LVCFMT_LEFT, 0);
	m_lstWorkorderClosedReport.InsertColumn(8, theApp.ConvertIDToString(IDS_DBT_INVOICE_ID), LVCFMT_LEFT, 0);
}

// Is called before printing, for preparing the print job
BOOL CReportWorkorderClosedView::OnPreparePrinting(CPrintInfo* pInfo)
{
	int nPages = m_lstWorkorderClosedReport.GetItemCount() / 55;
	if (m_lstWorkorderClosedReport.GetItemCount() % 55 > 0) nPages++;
	pInfo->SetMaxPage(nPages);

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
	CPrintHelper PH{ pDC };
	pDC->m_bPrinting = TRUE;
	pDC->StartPage();
	
	PH.PrintLogo();

	PH.PrintHeader(_T("Overzicht gesloten werkorders"));
	
	CRect rctWorkorderID(*PH.m_pxPos, *PH.m_pyPos, *PH.m_pxPos + PH.TotalTabInPixels(4), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderID, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderDescription(rctWorkorderID.right, *PH.m_pyPos, rctWorkorderID.right + PH.TotalTabInPixels(14), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderDescription, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderResponsible(rctWorkorderDescription.right, *PH.m_pyPos, rctWorkorderDescription.right + PH.TotalTabInPixels(5), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderResponsible, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderCloseDate(rctWorkorderResponsible.right, *PH.m_pyPos, rctWorkorderResponsible.right + PH.TotalTabInPixels(5), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderCloseDate, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctWorkorderStatus(rctWorkorderCloseDate.right, *PH.m_pyPos, rctWorkorderCloseDate.right + PH.TotalTabInPixels(4), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctWorkorderStatus, RGB(255, 255, 255), RGB(255, 255, 255));

	CRect rctAssetDescription(rctWorkorderStatus.right, *PH.m_pyPos, rctWorkorderStatus.right + PH.TotalTabInPixels(10), *PH.m_pyPos + PH.BodyTextLineDown(20));
	pDC->Draw3dRect(rctAssetDescription, RGB(255, 255, 255), RGB(255, 255, 255));

	// Print Table Header
	//PH.m_pFont = &PH.m_fontBoldBody;
	//pDC->SelectObject(PH.m_pFont);

	// print work comment area
	PH.m_pFont = &PH.m_fontBoldBody;
	pDC->SelectObject(PH.m_pFont);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(_T("Werkorder:"), rctWorkorderID, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Defect:"), rctWorkorderDescription, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Technicus:"), rctWorkorderResponsible, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Datum:"), rctWorkorderCloseDate, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Status:"), rctWorkorderStatus, DT_LEFT | DT_TABSTOP);
	pDC->DrawText(_T("Apparaat:"), rctAssetDescription, DT_LEFT | DT_TABSTOP);

	// Print Table Body
	PH.m_pFont = &PH.m_fontPlainBody;
	pDC->SelectObject(PH.m_pFont);

	// Print body text
	*PH.m_pyPos += PH.BodyTextLineDown(1);

	int nPage = pInfo->m_nCurPage;
	int i = ( nPage - 1) * 55;
	if (i > 0) i += (nPage - 1);
	
	// Print table body
	for ( i;  i < m_lstWorkorderClosedReport.GetItemCount(); i++) {
		CString strWorkorderID = m_lstWorkorderClosedReport.GetItemText(i, 0);
		CString strWorkorderDescription = m_lstWorkorderClosedReport.GetItemText(i, 1);
		CString strWorkorderResponsible = m_lstWorkorderClosedReport.GetItemText(i, 2);
		CString strWorkorderCloseDate = m_lstWorkorderClosedReport.GetItemText(i, 3);
		CString strWorkorderStatus = m_lstWorkorderClosedReport.GetItemText(i, 4);
		CString strAssetDescription = m_lstWorkorderClosedReport.GetItemText(i, 6);

		CRect rctWorkorderID(*PH.m_pxPos, *PH.m_pyPos, *PH.m_pxPos + PH.TotalTabInPixels(4), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strWorkorderID, rctWorkorderID, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderDescription(rctWorkorderID.right, *PH.m_pyPos, rctWorkorderID.right + PH.TotalTabInPixels(14), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strWorkorderDescription, rctWorkorderDescription, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderResponsible(rctWorkorderDescription.right, *PH.m_pyPos, rctWorkorderDescription.right + PH.TotalTabInPixels(5), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strWorkorderResponsible, rctWorkorderResponsible, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderCloseDate(rctWorkorderResponsible.right, *PH.m_pyPos, rctWorkorderResponsible.right + PH.TotalTabInPixels(5), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strWorkorderCloseDate, rctWorkorderCloseDate, DT_LEFT | DT_TABSTOP);

		CRect rctWorkorderStatus(rctWorkorderCloseDate.right, *PH.m_pyPos, rctWorkorderCloseDate.right + PH.TotalTabInPixels(4), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strWorkorderStatus, rctWorkorderStatus, DT_LEFT | DT_TABSTOP);

		CRect rctAssetDescription(rctWorkorderStatus.right, *PH.m_pyPos, rctWorkorderStatus.right + PH.TotalTabInPixels(10), *PH.m_pyPos + PH.BodyTextLineDown(1));
		pDC->DrawText(strAssetDescription, rctAssetDescription, DT_LEFT | DT_TABSTOP);

		*PH.m_pyPos += PH.BodyTextLineDown(1);

		if (*PH.m_pyPos == PH.m_pixEndPage) {
			break;
		}
	}
	
	PH.PrintFooter(pInfo->m_nCurPage);

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
					if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
						MessageBoxW(theApp.ConvertIDToString(IDS_MSGBT_ERROR_FETCH_WOCLOSED), theApp.ConvertIDToString(IDS_MSGBC_ERROR_CONNECT_DS), MB_ICONERROR);
						break;
					}
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