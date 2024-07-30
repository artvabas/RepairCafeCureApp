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
* File: CAdminWorkorderToProgress.cpp, implements class CAdminWorkorderToProgress
*
* This class is the dialog for setting the workorder status to 'Progress'.
* It is a part of the administration of the application, for setting repaired workorders to progress.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5
* Created: 30-07-2024, (dd-mm-yyyy)
* Updated: 30-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CAdminWorkorderToProgress.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::dialog::admin;
using namespace artvabas::sql;
using namespace artvabas::database::tables::workorder;

IMPLEMENT_DYNAMIC(CAdminWorkorderToProgress, CDialogEx)

CAdminWorkorderToProgress::CAdminWorkorderToProgress(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMIN_WORKORDER_PROGRESS, pParent)
{}

CAdminWorkorderToProgress::~CAdminWorkorderToProgress()
{}

/* Message handles */
BEGIN_MESSAGE_MAP(CAdminWorkorderToProgress, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ADMIN_WORKORDER_REPAIRED, &CAdminWorkorderToProgress::OnLvnItemChangedAdminWorkorderRepaired)
	ON_BN_CLICKED(IDC_ADMIN_WORKORDER_TO_PROGRESS, &CAdminWorkorderToProgress::OnBnClickedAdminWorkorderToProgress)
END_MESSAGE_MAP()

/* Override methods */

// OnInitDialog, called before the dialog is shown.
// It initializes the dialog and sets the list control columns.
// It calls GetData() to get the data from the database.
// - Returns BOOL, TRUE if successful, FALSE otherwise.
BOOL CAdminWorkorderToProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_lstWorkorderRepaired.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstWorkorderRepaired.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 100);
	m_lstWorkorderRepaired.InsertColumn(1, _T("ASSET ID"), LVCFMT_LEFT, 0);
	m_lstWorkorderRepaired.InsertColumn(2, _T("CUSTOMER ID"), LVCFMT_LEFT,0);
	m_lstWorkorderRepaired.InsertColumn(3, _T("INVOICE ID"), LVCFMT_LEFT, 0);
	m_lstWorkorderRepaired.InsertColumn(4, _T("CREATED"), LVCFMT_LEFT, 100);
	m_lstWorkorderRepaired.InsertColumn(5, _T("CREATED BY"), LVCFMT_LEFT, 0);
	m_lstWorkorderRepaired.InsertColumn(6, _T("DESCRIPTION"), LVCFMT_LEFT, 100);
	m_lstWorkorderRepaired.InsertColumn(7, _T("RESPONSIBLE"), LVCFMT_LEFT, 100);
	m_lstWorkorderRepaired.InsertColumn(8, _T("STATUS"), LVCFMT_LEFT, 100);
	m_lstWorkorderRepaired.InsertColumn(9, _T("CLOSED DATE"), LVCFMT_LEFT, 0);
	m_lstWorkorderRepaired.InsertColumn(10, _T("HISTORY"), LVCFMT_LEFT, 0);

	GetData();
	return TRUE;  
}

// DoDataExchange, called by the framework to exchange and validate dialog data.
// It is used to set and get data from the dialog controls.
// - Parameters:
//		- CDataExchange* pDX, a pointer to a CDataExchange object.
void CAdminWorkorderToProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADMIN_WORKORDER_REPAIRED, m_lstWorkorderRepaired);
	DDX_Control(pDX, IDC_ADMIN_WORKORDER_TO_PROGRESS, m_btnSetToProgress);
}

/* Message handlers methods */

// OnLvnItemChangedAdminWorkorderRepaired, called when the list control item is changed.
// It enables the Set to Progress button if an item is selected.
// - Parameters:
//		- NMHDR* pNMHDR, a pointer to a NMHDR structure.
//		- LRESULT* pResult, a pointer to a LRESULT structure.
void CAdminWorkorderToProgress::OnLvnItemChangedAdminWorkorderRepaired(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	pNMLV->iItem != -1 ? m_btnSetToProgress.EnableWindow(TRUE) : m_btnSetToProgress.EnableWindow(FALSE);
	*pResult = 0;
}

// OnBnClickedAdminWorkorderToProgress, called when the Set to Progress button is clicked.
// It sets the workorder status to 'Progress' in the database.
void CAdminWorkorderToProgress::OnBnClickedAdminWorkorderToProgress() noexcept
{

	if (m_lstWorkorderRepaired.GetSelectedCount() != 0) {
		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
		theApp.BeginWaitCursor();

		CString strBuildQuery{};
		auto nIndex{ m_lstWorkorderRepaired.GetNextItem(-1, LVNI_SELECTED) };
		strBuildQuery.Format(_T("UPDATE WORKORDER SET WORKORDER_STATUS = 'Progress' WHERE WORKORDER_ID = %d"),
			_ttoi(m_lstWorkorderRepaired.GetItemText(nIndex, 0)));

		CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

		if (sql.CreateSQLConnection()) {
			SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
			strBuildQuery.ReleaseBuffer();

			if (!sql.ExecuteQuery(strQuery))
				theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
			else
				theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
		}
		sql.CloseConnection();
		theApp.EndWaitCursor();
		GetData();
	}
}

/* Private methods */

// GetData, gets the data from the database and fills the list control.
void CAdminWorkorderToProgress::GetData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	m_lstWorkorderRepaired.DeleteAllItems();

	CString strBuildQuery{ _T("SELECT * FROM WORKORDER WHERE WORKORDER_STATUS = 'Repaired'") };

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		auto nIndex{ 0 };			// Index of the list control item.
		auto row{ 0 };		// Row of the list control item.
		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();
		CString strTemp{};

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
					nIndex = m_lstWorkorderRepaired.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_BY, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_DESCRIPTION, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 6, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 7, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_STATUS, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 8, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CLOSED_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 9, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_HISTORY, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lstWorkorderRepaired.SetItemText(nIndex, 10, CheckForNull(szNameLong, cbName));
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
}