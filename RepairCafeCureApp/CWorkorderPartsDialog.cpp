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
* File: CWorkorderPartsDialog.cpp, implements class CWorkorderPartsDialog
*
* This class is the dialog of the adding parts to workorder dialog (CWorkorderPartsDialog)
*
* Target: Windows 10/11 64bit
* Version: 0.0.1.0
* Created: 15-11-2023, (dd-mm-yyyy)
* Updated: 30-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CWorkorderPartsDialog.h"
#include "DatabaseTables.h"

using namespace artvabas::sql;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::database::tables::sparepartstock;
using namespace artvabas::database::tables::workorderparts;

IMPLEMENT_DYNAMIC(CWorkorderPartsDialog, CDialogEx)

CWorkorderPartsDialog::CWorkorderPartsDialog(const unsigned int& unWorkorderID, CWnd* pParent) noexcept
	: CDialogEx(IDD_WORKORDER_PARTS, pParent)
	, m_unWorkorderID(unWorkorderID)
	, m_strWorkorderPartDescription(_T(""))
	, m_strWorkorderPartAmount(_T(""))
	, m_strWorkorderPartUnitPrice(_T(""))
	, m_strWorkorderPartTotalPrice(_T(""))
	, m_bIsAddedPartListSelected(false)
{}

CWorkorderPartsDialog::~CWorkorderPartsDialog()
{}

/* Massage handle bindings */
BEGIN_MESSAGE_MAP(CWorkorderPartsDialog, CDialogEx)
	ON_EN_CHANGE(IDC_WORKORDER_DESCRIPTION_PART, &CWorkorderPartsDialog::OnEnChangeWorkorderAddParts)
	ON_EN_CHANGE(IDC_WORKORDER_AMOUNT_PART, &CWorkorderPartsDialog::OnEnChangeWorkorderAddParts)
	ON_EN_CHANGE(IDC_WORKORDER_UNIT_PRICE_PART, &CWorkorderPartsDialog::OnEnChangeWorkorderAddParts)
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDER_STOCK_PARTS, &CWorkorderPartsDialog::OnNMDoubleClickWorkorderStockPartsList)
	ON_NOTIFY(NM_CLICK, IDC_WORKORDER_ADDED_PARTS, &CWorkorderPartsDialog::OnNMClickWorkorderAddedPartsList)
	ON_BN_CLICKED(IDC_WORKORDER_ADD_PART, &CWorkorderPartsDialog::OnBnClickedWorkorderAddPart)
	ON_BN_CLICKED(IDC_WORKORDER_DELETE_ADDED_PART, &CWorkorderPartsDialog::OnBnClickedWorkorderDeleteAddedPart)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WORKORDER_ADDED_PARTS, &CWorkorderPartsDialog::OnLvnItemChangedWorkorderAddedParts)
	ON_NOTIFY(NM_KILLFOCUS, IDC_WORKORDER_ADDED_PARTS, &CWorkorderPartsDialog::OnNMKillFocusWorkorderAddedParts)
	ON_BN_CLICKED(IDC_WORKORDER_CHANGE, &CWorkorderPartsDialog::OnBnClickedWorkorderChange)
END_MESSAGE_MAP()

/* Override methods */

BOOL CWorkorderPartsDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			if  (m_btnWorkorderPartAdd.IsWindowEnabled() ) {
				OnBnClickedWorkorderAddPart();
				return TRUE;
			}
			if (m_btnWorkorderPartChange.IsWindowEnabled()) {
				OnBnClickedWorkorderChange();
				return TRUE;
			}
		}
		else {
			return CDialogEx::PreTranslateMessage(pMsg);
		}
	}
	return FALSE;
}

// DoDataExchange, for data exchange and validation control variables
void CWorkorderPartsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORKORDER_STOCK_PARTS, m_lscWorkorderStockPartList);
	DDX_Control(pDX, IDC_WORKORDER_ADDED_PARTS, m_lscWorkorderAddedPartList);
	DDX_Text(pDX, IDC_WORKORDER_DESCRIPTION_PART, m_strWorkorderPartDescription);
	DDX_Text(pDX, IDC_WORKORDER_AMOUNT_PART, m_strWorkorderPartAmount);
	DDX_Text(pDX, IDC_WORKORDER_UNIT_PRICE_PART, m_strWorkorderPartUnitPrice);
	DDX_Text(pDX, IDC_WORKORDER_TOTAL_PRICE_PARTS, m_strWorkorderPartTotalPrice);
	DDX_Control(pDX, IDC_WORKORDER_ADD_PART, m_btnWorkorderPartAdd);
	DDX_Control(pDX, IDC_WORKORDER_DELETE_ADDED_PART, m_btnWorkorderPartDelete);
	DDX_Control(pDX, IDC_WORKORDER_CHANGE, m_btnWorkorderPartChange);
}

// OnInitDialog, for initializing dialog
BOOL CWorkorderPartsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	if (InitStockPartList() && InitAddedPartList()) {
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_WORKORDER_DESCRIPTION_PART);
		pEdit->SetFocus();
		return FALSE;
	}
	else
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);

	return TRUE;
}

// OnOK, for saving the added parts to the database
// and deleting the old parts from the database, to keep it clean
void CWorkorderPartsDialog::OnOK()
{
	CString strQuery;
	strQuery.Format(_T("DELETE FROM [WORKORDER_PARTS] WHERE ([WORKORDER_PARTS_WORKORDER_ID] = %d)"), m_unWorkorderID);

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());

	if ( sql.CreateSQLConnection() ) {

		if ( sql.ExecuteQuery(strQuery.GetBuffer()) ) {
			strQuery.ReleaseBuffer();

			if ( m_lscWorkorderAddedPartList.GetItemCount() > 0 ) {
				for (int i = 0; i < m_lscWorkorderAddedPartList.GetItemCount(); i++) {

					CString strDescription = m_lscWorkorderAddedPartList.GetItemText(i, 1);

					unsigned int uiAmount = _ttoi(m_lscWorkorderAddedPartList.GetItemText(i, 2));

					double dUnitPrice = _ttof(m_lscWorkorderAddedPartList.GetItemText(i, 3));
					double dTotalPrice = _ttof(m_lscWorkorderAddedPartList.GetItemText(i, 4));

					// Build the fields value for the query.
					auto buildFieldValue = [](CString str) -> CString
					{
						CString strResult;
						if (str.IsEmpty())
							return  _T("NULL");
						strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
						return strResult;
					};

					strQuery.Format(_T("INSERT INTO [WORKORDER_PARTS] ([WORKORDER_PARTS_WORKORDER_ID], [WORKORDER_PARTS_DESCRIPTION], [WORKORDER_PARTS_AMOUNT], [WORKORDER_PARTS_UNIT_PRICE], [WORKORDER_PARTS_TOTAL_PRICE]) VALUES (%d, %s, %d, %f, %f)"),
						m_unWorkorderID, static_cast<LPCTSTR>(buildFieldValue(strDescription)), uiAmount, dUnitPrice, dTotalPrice);

					if ( !sql.ExecuteQuery(strQuery.GetBuffer()) )
						theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
					else
						theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);

					strQuery.ReleaseBuffer();
				}
			}
		} else {
			theApp.SetStatusBarText(IDS_STATUSBAR_DELETE_FAIL);
			strQuery.ReleaseBuffer();
		}
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	CDialogEx::OnOK();
}

/* Massage handle methods */

// OnEnChangeWorkorderAddParts, for changing the sate of the input fields
// All input fields must have values before the buttons are active
void CWorkorderPartsDialog::OnEnChangeWorkorderAddParts() noexcept
{
	UpdateData(TRUE);

	if ( m_strWorkorderPartDescription.IsEmpty() || m_strWorkorderPartAmount.IsEmpty() || m_strWorkorderPartUnitPrice.IsEmpty() ) {
		m_btnWorkorderPartAdd.EnableWindow(FALSE);
		m_btnWorkorderPartChange.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnWorkorderPartAdd, BLACK, false);
		SetCustomFocusButton(&m_btnWorkorderPartChange, BLACK, false);
	} else {
		if (!m_bIsAddedPartListSelected) {
			m_btnWorkorderPartAdd.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnWorkorderPartAdd, RED, false);
		}
		if (m_bIsAddedPartListSelected) {
			m_btnWorkorderPartChange.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnWorkorderPartChange, RED, false);
		}
	}
}

// OnNMDoubleClickWorkorderStockPartsList, for getting selected stock parts data
// - pNMHDR: Pointer to the NMHDR structure
// - pResult: Pointer to the result
void CWorkorderPartsDialog::OnNMDoubleClickWorkorderStockPartsList(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	auto nIndex = m_lscWorkorderStockPartList.GetNextItem(-1, LVNI_SELECTED);

	if  ( nIndex != -1 ) {
		m_strWorkorderPartDescription = m_lscWorkorderStockPartList.GetItemText(nIndex, 1);
		m_strWorkorderPartUnitPrice = m_lscWorkorderStockPartList.GetItemText(nIndex, 3);
		m_strWorkorderPartAmount = _T("1");
		UpdateData(FALSE);
	
		m_bIsAddedPartListSelected = false;
		SetChangeDeleteButtonState(FALSE);
		m_btnWorkorderPartAdd.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnWorkorderPartAdd, RED, true);
		m_btnWorkorderPartChange.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnWorkorderPartChange, BLACK, false);
	}
	*pResult = 0;
}

// OnNMClickWorkorderAddedPartsList, for getting selected parts data from the added part list
// Is trigged when the user clicks on the added part list
// - pNMHDR: Pointer to the NMHDR structure
// - pResult: Pointer to the result
void CWorkorderPartsDialog::OnNMClickWorkorderAddedPartsList(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	auto nIndex = m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED);

	if ( nIndex != -1 ) {
		m_strWorkorderPartDescription = m_lscWorkorderAddedPartList.GetItemText(nIndex, 1);
		m_strWorkorderPartAmount = m_lscWorkorderAddedPartList.GetItemText(nIndex, 2);
		m_strWorkorderPartUnitPrice = m_lscWorkorderAddedPartList.GetItemText(nIndex, 3);
		m_btnWorkorderPartDelete.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnWorkorderPartDelete, BLUE, false);
		UpdateData(FALSE);
		m_bIsAddedPartListSelected = true;
	} else {
		m_btnWorkorderPartDelete.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnWorkorderPartDelete, BLACK, false);
	}
	*pResult = 0;
}

// OnBnClickedWorkorderAddPart, calculate the total price of the given parts
// and add it to the added part list. Is trigged when the user clicks on the add button
void CWorkorderPartsDialog::OnBnClickedWorkorderAddPart() noexcept
{
	
	CString strWorkorderID;
	strWorkorderID.Format(_T("%d"), m_unWorkorderID);

	auto nAmount = _ttoi(m_strWorkorderPartAmount);
	auto dUnitPrice = _ttof(m_strWorkorderPartUnitPrice);
	auto nTotalPrice = nAmount * dUnitPrice;

	CString strUnitPrice;
	strUnitPrice.Format(_T("%.2f"), dUnitPrice);

	auto row = m_lscWorkorderAddedPartList.GetItemCount();

	auto nIndex = m_lscWorkorderAddedPartList.InsertItem(row, strWorkorderID);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, m_strWorkorderPartDescription);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, m_strWorkorderPartAmount);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, strUnitPrice);


	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), nTotalPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, m_strWorkorderPartTotalPrice);

	ClearPartInputFields();

	m_bIsAddedPartListSelected = false;

	OnEnChangeWorkorderAddParts();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_WORKORDER_DESCRIPTION_PART);
	pEdit->SetFocus();

	CalculateTotalPrice();
}

// OnLvnItemChangedWorkorderAddedParts, for changing the state of the change and delete button
// Is trigged when the state of the added part list is changed
// - pNMHDR: Pointer to the NMHDR structure
// - pResult: Pointer to the result
void CWorkorderPartsDialog::OnLvnItemChangedWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if ( (pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED) ) {
		SetChangeDeleteButtonState();
	} else {
		SetChangeDeleteButtonState(FALSE);
	}
	*pResult = 0;
}

// OnNMKillFocusWorkorderAddedParts, for changing the state of the change and delete button
// Is trigged when the focus is lost from the added part list
// - pNMHDR: Pointer to the NMHDR structure
// - pResult: Pointer to the result
void CWorkorderPartsDialog::OnNMKillFocusWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{	
	if ( m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED) == -1 ) {
		SetChangeDeleteButtonState(FALSE);
	}
	*pResult = 0;
}

// OnBnClickedWorkorderChange, for changing the selected part in the added part list
// Is trigged when the user clicks on the change button
void CWorkorderPartsDialog::OnBnClickedWorkorderChange() noexcept
{
	int nIndex = m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, m_strWorkorderPartDescription);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, m_strWorkorderPartAmount);

	auto nAmount = _ttoi(m_strWorkorderPartAmount);
	auto dUnitPrice = _ttof(m_strWorkorderPartUnitPrice);
	auto nTotalPrice = nAmount * dUnitPrice;

	CString strUnitPrice;
	strUnitPrice.Format(_T("%.2f"), dUnitPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, strUnitPrice);

	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), nTotalPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, m_strWorkorderPartTotalPrice);

	ClearPartInputFields();

	m_bIsAddedPartListSelected = false;

	OnEnChangeWorkorderAddParts();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_WORKORDER_DESCRIPTION_PART);
	pEdit->SetFocus();

	CalculateTotalPrice();
}

// OnBnClickedWorkorderDeleteAddedPart, for deleting the selected part from the added part list
// Is trigged when the user clicks on the delete button
void CWorkorderPartsDialog::OnBnClickedWorkorderDeleteAddedPart() noexcept
{
	int nIndex = m_lscWorkorderAddedPartList.GetNextItem(- 1, LVNI_SELECTED);
	m_lscWorkorderAddedPartList.DeleteItem(nIndex);

	ClearPartInputFields();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_WORKORDER_DESCRIPTION_PART);
	pEdit->SetFocus();

	CalculateTotalPrice();
}

 /* Member methods */

// InitStockPartList, for initializing the stock part list
// It queries all parts from the database and fills the list control with the found parts
bool CWorkorderPartsDialog::InitStockPartList()
{
	m_lscWorkorderStockPartList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lscWorkorderStockPartList.InsertColumn(0, _T("PART ID"), LVCFMT_LEFT, 0);
	m_lscWorkorderStockPartList.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
	m_lscWorkorderStockPartList.InsertColumn(2, _T("IN STOCK"), LVCFMT_LEFT, 0);
	m_lscWorkorderStockPartList.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strBuildQuery;	// Query string.

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	// Query all parts from the database.
	strBuildQuery.Format(_T("SELECT * FROM SPAREPARTSTOCK"));
	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO ) {
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				}
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
					if (cbName == SQL_NULL_DATA) {
						return _T("");
					}
					return static_cast<CString>(szName);
					};

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lscWorkorderStockPartList.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderStockPartList.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_IN_STOCK, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderStockPartList.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, SPAREPARTSTOCK.SPAREPART_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					CString strValue = static_cast<CString>(szName);
					auto dPrice = _ttof(strValue);
					strValue.Format(_T("%.2f"), dPrice);
					m_lscWorkorderStockPartList.SetItemText(nIndex, 3, strValue);
				} else {
					break;
				}
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) ) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
			return false;
		}else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	return true;
}

// InitAddedPartList, for initializing the added part list
// It queries all parts from the database and fills the list control with the found parts
bool CWorkorderPartsDialog::InitAddedPartList()
{
	m_lscWorkorderAddedPartList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lscWorkorderAddedPartList.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 0);
	m_lscWorkorderAddedPartList.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
	m_lscWorkorderAddedPartList.InsertColumn(2, _T("AMOUNT"), LVCFMT_LEFT, 100);
	m_lscWorkorderAddedPartList.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);
	m_lscWorkorderAddedPartList.InsertColumn(4, _T("TOTAL PRICE"), LVCFMT_LEFT, 100);

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strBuildQuery;	// Query string.

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	// Query all parts from the database.
	strBuildQuery.Format(_T("SELECT * FROM WORKORDER_PARTS WHERE WORKORDER_PARTS_WORKORDER_ID = %d"), m_unWorkorderID);
	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS)  {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO ) {
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				}
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					CString strValue{};
					double dConvertToMoney{ 0.0 };

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
					if (cbName == SQL_NULL_DATA) {
						return _T("");
					}
					return static_cast<CString>(szName);
					};

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lscWorkorderAddedPartList.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_AMOUNT, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_UNIT_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					dConvertToMoney = _ttof(static_cast<CString>(szName));
					strValue.Format(_T("%.2f"), dConvertToMoney);
					m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, strValue);

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_TOTAL_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					dConvertToMoney = _ttof(static_cast<CString>(szName));
					strValue.Format(_T("%.2f"), dConvertToMoney);
					m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, strValue);
				} else {
					break;
				}
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) ) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
			return false;
		} else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	
	UpdateData(FALSE);
	CalculateTotalPrice();

	return true;
}

// CalculateTotalPrice, for calculating the total price of the added parts
void CWorkorderPartsDialog::CalculateTotalPrice() noexcept
{
	double dTotalPrice(0.0);
	CString strValue = _T("");

	for ( auto i = 0; i < m_lscWorkorderAddedPartList.GetItemCount(); i++ ) {
		strValue = m_lscWorkorderAddedPartList.GetItemText(i, 4);
		dTotalPrice += _ttof(strValue);
	}

	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), dTotalPrice);
	UpdateData(FALSE);
}

// SetChangeDeleteButtonState, for changing the state of the change and delete button
void CWorkorderPartsDialog::SetChangeDeleteButtonState(BOOL bFlag) noexcept
{
	//m_btnWorkorderPartChange.EnableWindow(bFlag);
	m_btnWorkorderPartDelete.EnableWindow(bFlag);
}

// ClearPartInputFields, for clearing the input fields
void CWorkorderPartsDialog::ClearPartInputFields() noexcept
{
	m_strWorkorderPartAmount = _T("");
	m_strWorkorderPartDescription = _T("");
	m_strWorkorderPartUnitPrice = _T("");

	m_btnWorkorderPartDelete.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnWorkorderPartDelete, BLACK, false);
	UpdateData(FALSE);
}

void CWorkorderPartsDialog::SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus) noexcept
{
	auto color = RGB(255, 0, 0);
	switch (Color) {
	case RED:
		color = RGB(255, 0, 0);
		break;
	case BLUE:
		color = RGB(0, 0, 255);
		break;
	case BLACK:
		color = RGB(0, 0, 0);
		break;
	}
	pButton->SetTextColor(color);
	pButton->RedrawWindow();
	if (bFocus) pButton->SetFocus();
}