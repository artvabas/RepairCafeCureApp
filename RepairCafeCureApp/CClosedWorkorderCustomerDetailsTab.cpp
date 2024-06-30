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
* File: CClosedWorkorderCustomerDetailsTab.cpp, implements class CClosedWorkorderCustomerDetailsTab
*
* This class is the view of the Customer-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the Customer-details of a closed workorder.
* What is selected in the listview of the CReportWorkorderClosedView and passed to this form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.3 (alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 30-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkorderCustomerDetailsTab.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;
using namespace artvabas::database::tables::customer;

IMPLEMENT_DYNAMIC(CClosedWorkorderCustomerDetailsTab, CDialogEx)

CClosedWorkorderCustomerDetailsTab::CClosedWorkorderCustomerDetailsTab(unsigned int& unID, CWnd* pParent /*=nullptr*/)
	: CDialogEx{ IDD_CLOSED_WORKORDER_CUSTOMER_DETAILS_TAB, pParent }
	, m_unCustomerID{ unID }
	, m_strCustomerID{ _T("") }
	, m_strCustomerName{ _T("") }
	, m_strCustomerSurname{ _T("") }
	, m_strCustomerCellPhone{ _T("") }
	, m_strCustomerPhone{ _T("") }
	, m_strCustomerEmail{ _T("") }
	, m_strCustomerComment{ _T("") }
	, m_strCustomerHistoryLog{ _T("") }
{}

CClosedWorkorderCustomerDetailsTab::~CClosedWorkorderCustomerDetailsTab()
{}

BEGIN_MESSAGE_MAP(CClosedWorkorderCustomerDetailsTab, CDialogEx)
END_MESSAGE_MAP()

BOOL CClosedWorkorderCustomerDetailsTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery;
	strBuildQuery.Format(_T("SELECT * FROM CUSTOMER WHERE CUSTOMER_ID = %d"), m_unCustomerID);

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
					SQLGetData(hstmt, CUSTOMER.CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_SURNAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerSurname = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_NAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerName = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_CELL_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerCellPhone = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerPhone = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_EMAIL, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerEmail = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_COMMENT, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strCustomerComment = CheckForNull(szNameLong, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_GENERAL_LOG, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strCustomerHistoryLog = CheckForNull(szNameLong, cbName);
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

void CClosedWorkorderCustomerDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_ID, m_strCustomerID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_SURNAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_CELLPHONE, m_strCustomerCellPhone);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_PHONE, m_strCustomerPhone);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_EMAIL, m_strCustomerEmail);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_COMMENT, m_strCustomerComment);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_CUSTOMER_TAB_CUSTOMER_HISTORY_LOG, m_strCustomerHistoryLog);
}