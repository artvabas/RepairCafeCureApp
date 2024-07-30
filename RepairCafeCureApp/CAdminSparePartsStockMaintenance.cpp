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
* File: CAdminSparePartsStockMaintenance.cpp, implements class CAdminSparePartsStockMaintenance
*
* This class is the dialog for maintaining the stock parts.
* It is a part of the administration of the application, for add new, change or delete stock parts.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5
* Created: 21-07-2024, (dd-mm-yyyy)
* Updated: 30-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CAdminSparePartsStockMaintenance.h"
#include "CSQLNativeAVB.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::dialog::admin;
using namespace artvabas::sql;
using namespace artvabas::database::tables::sparepartstock;

IMPLEMENT_DYNAMIC(CAdminSparePartsStockMaintenance, CDialogEx)

CAdminSparePartsStockMaintenance::CAdminSparePartsStockMaintenance(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMIN_SPAREPARTS_STOCK_MAINTENANCE, pParent)
	, m_strSparePartDescription(_T(""))
	, m_strSparePartsUnitPrice(_T(""))
{}

CAdminSparePartsStockMaintenance::~CAdminSparePartsStockMaintenance()
{}

/* Message handlers */

BEGIN_MESSAGE_MAP(CAdminSparePartsStockMaintenance, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ADMIN_STOCK_SPAREPARTS, &CAdminSparePartsStockMaintenance::OnLvnItemChangedAdminStockSpareParts)
	ON_EN_CHANGE(IDC_ADMIN_STOCK_ITEM_DESCRIPTION, &CAdminSparePartsStockMaintenance::OnEnAdminStockItem)
	ON_EN_CHANGE(IDC_ADMIN_STOCK_ITEM_UNIT_PRICE, &CAdminSparePartsStockMaintenance::OnEnAdminStockItem)
	ON_BN_CLICKED(IDC_ADMIN_STOCK_CHANGE, &CAdminSparePartsStockMaintenance::OnBnClickedAdminStockChange)
	ON_BN_CLICKED(IDC_ADMIN_STOCK_ITEM_DELETE, &CAdminSparePartsStockMaintenance::OnBnClickedAdminStockItemDelete)
END_MESSAGE_MAP()

/* Overrides methohds */

// OnInitDialog, called before the dialog is displayed.
// It initializes the list control and gets the data from the database.
// - return: BOOL, TRUE if the dialog is initialized, FALSE otherwise.
BOOL CAdminSparePartsStockMaintenance::OnInitDialog()
{
	__super::OnInitDialog();

	m_lstSparePartStock.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstSparePartStock.InsertColumn(0, _T("SPARE PART ID"), LVCFMT_LEFT, 0);
	m_lstSparePartStock.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
	m_lstSparePartStock.InsertColumn(2, _T("IN STOCK"), LVCFMT_LEFT, 0);
	m_lstSparePartStock.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);

	GetData();

	return TRUE;  
}

// DoDataExchange, called by the framework to exchange and validate dialog data.
// It is used to transfer data between the dialog box and class members.
// - pDX: A pointer to a CDataExchange object.
void CAdminSparePartsStockMaintenance::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADMIN_STOCK_SPAREPARTS, m_lstSparePartStock);
	DDX_Text(pDX, IDC_ADMIN_STOCK_ITEM_DESCRIPTION, m_strSparePartDescription);
	DDX_Text(pDX, IDC_ADMIN_STOCK_ITEM_UNIT_PRICE, m_strSparePartsUnitPrice);
	DDX_Control(pDX, IDC_ADMIN_STOCK_CHANGE, m_btnSparePartChange);
	DDX_Control(pDX, IDC_ADMIN_STOCK_ITEM_DELETE, m_btnSparePartDelete);
}

// GetData, gets the data from the database and fills the list control.
void CAdminSparePartsStockMaintenance::GetData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	m_lstSparePartStock.DeleteAllItems();

	CString strBuildQuery{ _T("SELECT * FROM SPAREPARTSTOCK") };

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
					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lstSparePartStock.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstSparePartStock.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_IN_STOCK, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lstSparePartStock.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					strTemp.Format(_T("%.2f"), _ttof(CheckForNull(szName, cbName)));
					m_lstSparePartStock.SetItemText(nIndex, 3, strTemp);
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
// It updates the data if a row is selected, otherwise inserts a new row.
void CAdminSparePartsStockMaintenance::SetData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery{};

	if (m_lstSparePartStock.GetSelectedCount() != 0) {
		auto nIndex{ m_lstSparePartStock.GetNextItem(-1, LVNI_SELECTED) };
		strBuildQuery.Format(_T("UPDATE SPAREPARTSTOCK SET SPAREPARTSTOCK_DESCRIPTION = '%s', SPAREPARTSTOCK_PRICE = %f WHERE SPAREPARTSTOCK_ID = %d"),
			static_cast<LPCTSTR>(m_strSparePartDescription),
			_ttof(m_strSparePartsUnitPrice),
			_ttoi(m_lstSparePartStock.GetItemText(nIndex, 0)));
	}
	else {
		strBuildQuery.Format(_T("INSERT INTO SPAREPARTSTOCK (SPAREPARTSTOCK_DESCRIPTION, SPAREPARTSTOCK_IN_STOCK, SPAREPARTSTOCK_PRICE) VALUES ('%s', %d, %f)"),
			static_cast<LPCTSTR>(m_strSparePartDescription),
			0,
			_ttof(m_strSparePartsUnitPrice));
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

// DeleteData, deletes the selected row from the database.
void CAdminSparePartsStockMaintenance::DeleteData() noexcept
{
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	if (m_lstSparePartStock.GetSelectedCount() != 0) {
		auto nIndex{ m_lstSparePartStock.GetNextItem(-1, LVNI_SELECTED) };
		CString strBuildQuery{};
		strBuildQuery.Format(_T("DELETE FROM SPAREPARTSTOCK WHERE SPAREPARTSTOCK_ID = %d"),
			_ttoi(m_lstSparePartStock.GetItemText(nIndex, 0)));

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

/* Message handlers methods */

// OnLvnItemChangedAdminStockSpareParts, called when the selected item in the list control is changed.
// It gets the selected item's text and enables the delete button.
// - pNMHDR: A pointer to a NMHDR structure that contains the notification code and additional information.
// - pResult: A pointer to an LRESULT variable.
void CAdminSparePartsStockMaintenance::OnLvnItemChangedAdminStockSpareParts(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem != -1) {
		// Get the selected item's text.
		m_strSparePartDescription = m_lstSparePartStock.GetItemText(pNMLV->iItem, 1);
		m_strSparePartsUnitPrice = m_lstSparePartStock.GetItemText(pNMLV->iItem, 3);
		m_btnSparePartDelete.EnableWindow(TRUE);
	}
	else {
		m_strSparePartDescription.Empty();
		m_strSparePartsUnitPrice.Empty();
		m_btnSparePartDelete.EnableWindow(FALSE);
	}
	m_btnSparePartChange.EnableWindow(FALSE);
	UpdateData(FALSE);
	*pResult = 0;
}

// OnEnAdminStockItem, called when the description or unit price is changed.
// It enables the change button if the description and unit price are not empty.
void CAdminSparePartsStockMaintenance::OnEnAdminStockItem() noexcept
{
	UpdateData(TRUE);
	if (m_strSparePartDescription.IsEmpty() || m_strSparePartsUnitPrice.IsEmpty())
		m_btnSparePartChange.EnableWindow(FALSE);
	else
		m_btnSparePartChange.EnableWindow(TRUE);
}

// OnBnClickedAdminStockChange, called when the change button is clicked.
// It sets the data and gets the data.
void CAdminSparePartsStockMaintenance::OnBnClickedAdminStockChange() noexcept
{
	UpdateData(TRUE);
	SetData();
	GetData();
	m_strSparePartDescription.Empty();
	m_strSparePartsUnitPrice.Empty();
	m_btnSparePartChange.EnableWindow(FALSE);
	m_btnSparePartDelete.EnableWindow(FALSE);
	UpdateData(FALSE);
}

// OnBnClickedAdminStockItemDelete, called when the delete button is clicked.
// It deletes the data and gets the data.
void CAdminSparePartsStockMaintenance::OnBnClickedAdminStockItemDelete() noexcept
{
	if (m_lstSparePartStock.GetSelectedCount() != 0) {
		DeleteData();
		GetData();
		m_strSparePartDescription.Empty();
		m_strSparePartsUnitPrice.Empty();
		m_btnSparePartChange.EnableWindow(FALSE);
		m_btnSparePartDelete.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
}