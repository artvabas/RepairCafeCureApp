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
* File: CAdminEmployeeMaintenance.cpp, implements class CAdminEmployeeMaintenance
*
* This class is the dialog for maintaining the employees with their roles.
* It is a part of the administration of the application, for add new, change or delete employees.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5
* Created: 15-07-2024, (dd-mm-yyyy)
* Updated: 21-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CAdminEmployeeMaintenance.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::dialog::admin;
using namespace artvabas::sql;
using namespace artvabas::database::tables::employee;

IMPLEMENT_DYNAMIC(CAdminEmployeeMaintenance, CDialogEx)

CAdminEmployeeMaintenance::CAdminEmployeeMaintenance(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMIN_EMPLOYEE_MAINTENANCE, pParent)
	, m_bEmployeeIsAdmin(FALSE)
	, m_strEmployeeName(_T(""))
	, m_bIsDirty(false)
{}

CAdminEmployeeMaintenance::~CAdminEmployeeMaintenance()
{}

/* Message handlers */
BEGIN_MESSAGE_MAP(CAdminEmployeeMaintenance, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ADMIN_EMPLOYEE_MAINTENANCE, &CAdminEmployeeMaintenance::OnLvnItemChangedAdminEmployeeMaintenance)
	ON_EN_CHANGE(IDC_ADMIN_EMPLOYEE_MAINTENANCE_NAME, &CAdminEmployeeMaintenance::OnEnChangeAdminEmployeeMaintenanceName)
	ON_BN_CLICKED(IDC_ADMIN_EMPLOYEE_MAINTENNACE_ADMIN, &CAdminEmployeeMaintenance::OnBnClickedAdminEmployeeMaintenanceAdmin)
	ON_BN_CLICKED(IDC_ADMIN_EMPLOYEE_MAINTENANCE_ADD, &CAdminEmployeeMaintenance::OnBnClickedAdminEmployeeMaintenanceAdd)
	ON_BN_CLICKED(IDC_ADMIN_EMPLOYEE_MAINTENANCE_DELETE, &CAdminEmployeeMaintenance::OnBnClickedAdminEmployeeMaintenanceDelete)
	ON_BN_CLICKED(IDOK, &CAdminEmployeeMaintenance::OnBnClickedOk)
END_MESSAGE_MAP()

/* overrides */

// OnInitDialog, called when the dialog is initialized.
// It sets the list control style, inserts the columns and calls GetData.
// It returns TRUE if the focus is set to a control.
BOOL CAdminEmployeeMaintenance::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_lstExistingEmployees.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstExistingEmployees.InsertColumn(0, _T("EMPLOYEE ID"), LVCFMT_LEFT, 0);
	m_lstExistingEmployees.InsertColumn(1, _T("EMPLOYEE NAME"), LVCFMT_LEFT, 100);
	m_lstExistingEmployees.InsertColumn(2, _T("ADMIN"), LVCFMT_LEFT, 100);

	GetData();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// DoDataExchange, called by the framework to exchange and validate dialog data.
// - pDX: A pointer to a CDataExchange object.
void CAdminEmployeeMaintenance::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADMIN_EMPLOYEE_MAINTENANCE, m_lstExistingEmployees);
	DDX_Check(pDX, IDC_ADMIN_EMPLOYEE_MAINTENNACE_ADMIN, m_bEmployeeIsAdmin);
	DDX_Text(pDX, IDC_ADMIN_EMPLOYEE_MAINTENANCE_NAME, m_strEmployeeName);
	DDX_Control(pDX, IDC_ADMIN_EMPLOYEE_MAINTENANCE_ADD, m_btnEmployeeAddChange);
	DDX_Control(pDX, IDC_ADMIN_EMPLOYEE_MAINTENANCE_DELETE, m_btnEmployeeRemove);
	DDX_Control(pDX, IDC_ADMIN_EMPLOYEE_MAINTENNACE_ADMIN, m_chkEmployeeIsAdmin);
}
/* Overrides  methods interfaces */

// GetData, gets the data from the database and fills the list control.
void CAdminEmployeeMaintenance::GetData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	m_lstExistingEmployees.DeleteAllItems();

	CString strBuildQuery{ _T("SELECT * FROM EMPLOYEE ORDER BY EMPLOYEE_NAME") };

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		auto nIndex{ 0 };			// Index of the list control item.
		auto row{ 0 };		// Row of the list control item.
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
					SQLGetData(hstmt, EMPLOYEE.EMPLOYEE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lstExistingEmployees.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, EMPLOYEE.EMPLOYEE_NAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstExistingEmployees.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, EMPLOYEE.EMPLOYEE_ROLE_ADMIN, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstExistingEmployees.SetItemText(nIndex, 2, CheckForNull(szName, cbName));
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

// SetData, sets the data to the database.
// If the list control has a selected item, it updates the employee name and role.
// Otherwise, it inserts a new employee with the name and role.
void CAdminEmployeeMaintenance::SetData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery{};

	if (m_lstExistingEmployees.GetSelectedCount() != 0) {
		auto nIndex{ m_lstExistingEmployees.GetNextItem(-1, LVNI_SELECTED) };
		strBuildQuery.Format(_T("UPDATE EMPLOYEE SET EMPLOYEE_NAME = '%s', EMPLOYEE_ROLE_ADMIN = '%d' WHERE EMPLOYEE_ID = %s"),
			static_cast<LPCTSTR>(m_strEmployeeName),
			m_bEmployeeIsAdmin,
			static_cast<LPCTSTR>(m_lstExistingEmployees.GetItemText(nIndex, 0)));
	}
	else {
		strBuildQuery.Format(_T("INSERT INTO EMPLOYEE (EMPLOYEE_NAME, EMPLOYEE_ROLE_ADMIN) VALUES ('%s', '%d')"),
			static_cast<LPCTSTR>(m_strEmployeeName),
			m_bEmployeeIsAdmin);
	}

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
}

// DeleteData, deletes the data from the database.
// Only if the list control has a selected item, it deletes the employee with the selected employee id.
void CAdminEmployeeMaintenance::DeleteData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	if (m_lstExistingEmployees.GetSelectedCount() != 0) {
		auto nIndex{ m_lstExistingEmployees.GetNextItem(-1, LVNI_SELECTED) };
		CString strBuildQuery{};
		strBuildQuery.Format(_T("DELETE FROM EMPLOYEE WHERE EMPLOYEE_ID = %s"),
			static_cast<LPCTSTR>(m_lstExistingEmployees.GetItemText(nIndex, 0)));

		CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

		if (sql.CreateSQLConnection()) {

			SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
			strBuildQuery.ReleaseBuffer();

			if (!sql.ExecuteQuery(strQuery))
				theApp.SetStatusBarText(IDS_STATUSBAR_DELETE_FAIL);
			else
				theApp.SetStatusBarText(IDS_STATUSBAR_DELETE_OK);
		}
		sql.CloseConnection();
	}
	theApp.EndWaitCursor();
}

/* Message handlers */

// OnLvnItemChangedAdminEmployeeMaintenance, called when the list control item is changed.
// - pNMHDR: A pointer to an NMHDR structure that contains the notification code and additional information.
// - pResult: A pointer to an LRESULT variable.
void CAdminEmployeeMaintenance::OnLvnItemChangedAdminEmployeeMaintenance(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem != -1) {
		// Get the selected item's text.
		m_strEmployeeName = m_lstExistingEmployees.GetItemText(pNMLV->iItem, 1);
		m_bEmployeeIsAdmin = m_lstExistingEmployees.GetItemText(pNMLV->iItem, 2) == _T("1") ? TRUE : FALSE;
		m_chkEmployeeIsAdmin.EnableWindow(TRUE);
		m_btnEmployeeRemove.EnableWindow(TRUE);
	} else {
		m_strEmployeeName = _T("");
		m_bEmployeeIsAdmin = FALSE;
		m_chkEmployeeIsAdmin.EnableWindow(FALSE);
		m_btnEmployeeRemove.EnableWindow(FALSE);
	}
	m_btnEmployeeAddChange.EnableWindow(FALSE);
	UpdateData(FALSE);
	*pResult = 0;
}

// OnEnChangeAdminEmployeeMaintenanceName, called when the employee name is changed.
void CAdminEmployeeMaintenance::OnEnChangeAdminEmployeeMaintenanceName() noexcept
{
	UpdateData(TRUE);
	if (m_strEmployeeName.IsEmpty()) {
		m_btnEmployeeAddChange.EnableWindow(FALSE);
		m_chkEmployeeIsAdmin.EnableWindow(FALSE);
	} else {
		m_btnEmployeeAddChange.EnableWindow(TRUE);
		m_chkEmployeeIsAdmin.EnableWindow(TRUE);
	}
}

// OnBnClickedAdminEmployeeMaintenanceAdmin, called when the admin checkbox is clicked.
void CAdminEmployeeMaintenance::OnBnClickedAdminEmployeeMaintenanceAdmin() noexcept
{
	m_btnEmployeeAddChange.EnableWindow(TRUE);
}

// OnBnClickedAdminEmployeeMaintenanceAdd, called when the add button is clicked.
// It calls SetData, GetData, clears the employee name and sets the admin checkbox to FALSE.
void CAdminEmployeeMaintenance::OnBnClickedAdminEmployeeMaintenanceAdd() noexcept
{
	UpdateData(TRUE);
	SetData();
	GetData();
	m_strEmployeeName.Empty();
	m_bEmployeeIsAdmin = FALSE;
	m_btnEmployeeAddChange.EnableWindow(FALSE);
	m_btnEmployeeRemove.EnableWindow(FALSE);
	UpdateData(FALSE);
	m_bIsDirty = true;
}

// OnBnClickedAdminEmployeeMaintenanceDelete, called when the delete button is clicked.
// It calls DeleteData, GetData, clears the employee name and sets the admin checkbox to FALSE.
void CAdminEmployeeMaintenance::OnBnClickedAdminEmployeeMaintenanceDelete() noexcept
{
	if (m_lstExistingEmployees.GetSelectedCount() != 0) {
		DeleteData();
		GetData();
		m_strEmployeeName.Empty();
		m_bEmployeeIsAdmin = FALSE;
		m_chkEmployeeIsAdmin.EnableWindow(FALSE);
		m_btnEmployeeRemove.EnableWindow(FALSE);
		m_btnEmployeeAddChange.EnableWindow(FALSE);
		UpdateData(FALSE);
		m_bIsDirty = true;
	}
}

// OnBnClickedOk, called when the OK button is clicked.
// It calls OnOK if the dialog is dirty, otherwise it calls OnCancel.
void CAdminEmployeeMaintenance::OnBnClickedOk() noexcept
{
	if (m_bIsDirty) CDialogEx::OnOK();
	else CDialogEx::OnCancel();
}