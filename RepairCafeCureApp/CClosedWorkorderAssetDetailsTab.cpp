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
* File: CClosedWorkorderAssetDetailsTab.cpp, implements class CClosedWorkorderAssetDetailsTab
*
* This class is the view of the Asset-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the Asset-details of a closed workorder.
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
#include "CClosedWorkorderAssetDetailsTab.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"

using namespace artvabas::sql;
using namespace artvabas::database::tables::asset;
using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CClosedWorkorderAssetDetailsTab, CDialogEx)

CClosedWorkorderAssetDetailsTab::CClosedWorkorderAssetDetailsTab(unsigned int unID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSED_WORKORDER_ASSET_DETAILS_TAB, pParent)
	, m_unAssetID{ unID }
	, m_strAssetID(_T(""))
	, m_strAssetCustomerID(_T(""))
	, m_strAssetCreateDate(_T(""))
	, m_bAssetDsiposed(FALSE)
	, m_strAssetDescription(_T(""))
	, m_strAssetModelNumber(_T(""))
	, m_strAsserBrand(_T(""))
	, m_strAssetHistoryLog(_T(""))
{}

CClosedWorkorderAssetDetailsTab::~CClosedWorkorderAssetDetailsTab()
{}

BEGIN_MESSAGE_MAP(CClosedWorkorderAssetDetailsTab, CDialogEx)
END_MESSAGE_MAP()

// OnIntiDialog is called when the dialog is created
// It is used to load the data from the database and display it in the dialog
BOOL CClosedWorkorderAssetDetailsTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery;
	strBuildQuery.Format(_T("SELECT * FROM ASSET WHERE ASSET_ID = %d"), m_unAssetID);

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
					SQLGetData(hstmt, ASSET.ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetCustomerID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetCreateDate = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetDescription = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_MODEL_NUMBER, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetModelNumber = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_BRAND, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAsserBrand = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_DISPOSED, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_bAssetDsiposed = _wtoi(CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_HISTORY_LOG, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strAssetHistoryLog = CheckForNull(szNameLong, cbName);
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
// - pDX: A pointer to a CDataExchange object
void CClosedWorkorderAssetDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_ASSET_ID, m_strAssetID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_CUSTOMER_ID, m_strAssetCustomerID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_CREATE_DATE, m_strAssetCreateDate);
	DDX_Check(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_DISPOSED, m_bAssetDsiposed);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_DESCRIPTION, m_strAssetDescription);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_MODEL, m_strAssetModelNumber);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_BRAND, m_strAsserBrand);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_ASSET_TAB_HISTORY_LOG, m_strAssetHistoryLog);
}