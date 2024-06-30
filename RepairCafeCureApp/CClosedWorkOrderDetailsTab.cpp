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
* File: CClosedWorkOrderDetailsTab.cpp, implements class CClosedWorkOrderDetailsTab
*
* This class is the view of the Workorder-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the workorder-details of a closed workorder.
* What is selected in the listview of the CReportWorkorderClosedView and passed to this form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.2 (alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 28-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"
#include "CClosedWorkOrderDetailsTab.h"

using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;
using namespace artvabas::database::tables::workorder;

IMPLEMENT_DYNAMIC(CClosedWorkOrderDetailsTab, CDialogEx)

CClosedWorkOrderDetailsTab::CClosedWorkOrderDetailsTab(unsigned int unID, CWnd* pParent)
	: CDialogEx(IDD_CLOSED_WORKORDER_DETAILS_TAB, pParent)
	, m_unWorkorderID{ unID }
	, m_strWorkorderID{ _T("") }
	, m_strWorkorderAssetID{ _T("") }
	, m_strWorkorderCustomerID{ _T("") }
	, m_strWorkorderInvoiceID{ _T("") }
	, m_strWorkorderCreateDate{ _T("") }
	, m_strWorkorderClosedDate{ _T("") }
	, m_strWorkorderCreatedBy{ _T("") }
	, m_strWorkorderEmployeeResponsible{ _T("") }
	, m_strWorkorderDescription{ _T("") }
	, m_strWorkorderStatus{ _T("") }
	, m_strWorkorderHistoryLog{ _T("") }
{}

CClosedWorkOrderDetailsTab::~CClosedWorkOrderDetailsTab()
{}

BEGIN_MESSAGE_MAP(CClosedWorkOrderDetailsTab, CDialogEx)
END_MESSAGE_MAP()

// OnIntDialog is called when the dialog is created, it is used to load the data from the database
// and display it in the dialog.
BOOL CClosedWorkOrderDetailsTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery;
	strBuildQuery.Format(_T("SELECT * FROM WORKORDER WHERE WORKORDER_ID = %d"), m_unWorkorderID);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
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
					SQLGetData(hstmt, WORKORDER.WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderAssetID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderCustomerID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderInvoiceID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderCreateDate = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_BY, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderCreatedBy = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_DESCRIPTION, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strWorkorderDescription = CheckForNull(szNameLong, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderEmployeeResponsible = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_STATUS, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderStatus = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_CLOSED_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strWorkorderClosedDate = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDER.WORKORDER_HISTORY, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strWorkorderHistoryLog = CheckForNull(szNameLong, cbName);
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

	return TRUE;  // return TRUE unless you set the focus to a control 
	// EXCEPTION: OCX Property Pages should return FALSE
}

// DoDataExchange is used to exchange and validate data between the dialog and variables
// - DDX_Text is used to exchange data between the dialog and variables
void CClosedWorkOrderDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_WORKORDER_ID, m_strWorkorderID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_ASSET_ID, m_strWorkorderAssetID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_CUSTOMER_ID, m_strWorkorderCustomerID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_INVOICE_ID, m_strWorkorderInvoiceID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_CREATE_DATE, m_strWorkorderCreateDate);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_CLOSED_DATE, m_strWorkorderClosedDate);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_CREATED_BY, m_strWorkorderCreatedBy);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_RESPONIBLE_BY, m_strWorkorderEmployeeResponsible);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_DESCRIPTION, m_strWorkorderDescription);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_STATUS, m_strWorkorderStatus);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_TAB_HISTORY_LOG, m_strWorkorderHistoryLog);
}