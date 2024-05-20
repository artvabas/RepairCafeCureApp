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
* File: CAssetTab.cpp, defines class CAssetTab
*
* This class is the view of the asset-tab in the CTabCtrlAssetWorkorder class, 
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
* 
* With this form the user can search for existing assets, add new assets, update existing assets.
* After selecting an asset, the user can create a workorder for the selected asset.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 0.0.1.0 (alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 29-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CAssetTab.h"
#include "DatabaseTables.h"

using namespace artvabas::sql;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::database::tables::asset;

IMPLEMENT_DYNAMIC(CAssetTab, CDialogEx)

CAssetTab::CAssetTab(CTabCtrlAssetWorkorder* pTabControl, CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID,
	CWnd* pParent) noexcept

	: CDialogEx{ IDD_ASSET_TAB, pParent }
	, m_pTabControl{ pTabControl }
	, m_strCustomerSurname{ strCustomerSurname }
	, m_strCustomerName{ strCustomerName }
	, m_nAssetID{ 0 }
	, m_nAssetCustomerID{ nCustomerID }
	, m_nAssetWorkorderID{ 0 }
	, m_strAssetCreateDate{ _T("") }
	, m_strDescription{ _T("") }
	, m_strModelNumber{ _T("") }
	, m_strBrand{ _T("") }
	, m_sAssetDisposed{ 0 }
	, m_strHistoryLog{ _T("") }
	, m_bIsSelectedFromAssetList{ false }
{
	// Shared data between the asset and workorder tab.
	m_pAssetDetailsRecords = &(m_pTabControl->m_assetDetailsRecords);
}

CAssetTab::~CAssetTab(){}

/* CAssetTab message handlers */
BEGIN_MESSAGE_MAP(CAssetTab, CDialogEx)
	ON_EN_CHANGE(IDC_ASSETTAB_DESCRIPTION, &CAssetTab::OnEnChangeAssetDetails)
	ON_EN_CHANGE(IDC_ASSTETTAB_MODEL_NUMBER, &CAssetTab::OnEnChangeAssetDetails)
	ON_EN_CHANGE(IDC_ASSETTAB_BRAND, &CAssetTab::OnEnChangeAssetDetails)
	ON_NOTIFY(NM_DBLCLK, IDC_ASSETTAB_ASSET_LIST, &CAssetTab::OnDoubleClickAssetTabAssetList)
	ON_BN_CLICKED(IDC_ASSETTAB_UPDATE, &CAssetTab::OnBnClickedAssetTabUpdate)
	ON_BN_CLICKED(IDC_ASSETTAB_NEW, &CAssetTab::OnBnClickedAssetTabNew)
	ON_BN_CLICKED(IDC_ASSETTAB_CREATE_WORKORDER, &CAssetTab::OnBnClickedAssetTabCreateWorkorder)
	ON_BN_CLICKED(IDC_ASSETTAB_CLEAR, &CAssetTab::OnBnClickedAssetTabClear)
END_MESSAGE_MAP()

/* override methods */

// Initializes the dialog on creation.
// Creates the list control columns.
// Fills the list control with the existing assets, if any, from the database.
BOOL CAssetTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrExistingAssetList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctrExistingAssetList.InsertColumn(0, _T("Asset ID"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(1, _T("Customer ID"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(2, _T("Workorder ID"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(3, _T("Creation Date"), LVCFMT_LEFT, 100);
	m_ctrExistingAssetList.InsertColumn(4, _T("Description"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(5, _T("Model Number"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(6, _T("Brand"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(7, _T("Disposed"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(8, _T("History Log"), LVCFMT_LEFT, 0);

	int nIndex{};			// Index of the list control item.	
	auto row{ 0 };			// Row of the list control item.
	CString strBuildQuery{};

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	strBuildQuery.Format(_T("SELECT ASSET.*, ASSET_CUSTOMER_ID AS Expr1 FROM ASSET WHERE(ASSET_CUSTOMER_ID = %d)"), m_nAssetCustomerID);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		SQLCHAR szName[SQLCHARVSMAL] {};
		SQLCHAR szNameLong[SQLCHARVMAX] {};
		SQLLEN cbName {};
		SQLRETURN retcode {};
		SQLHSTMT hstmt { sql.GetStatementHandle() };
		SQLWCHAR* strQuery { strBuildQuery.GetBuffer() };
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

					SQLGetData(hstmt, ASSET.ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					nIndex = m_ctrExistingAssetList.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_MODEL_NUMBER, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_BRAND, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 6, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_DISPOSED, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 7, CheckForNull(szName, cbName));

					SQLGetData(hstmt, ASSET.ASSET_HISTORY_LOG, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_ctrExistingAssetList.SetItemText(nIndex, 8, CheckForNull(szNameLong, cbName));

				}
				else {
					break;
				}
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) ) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
			EndDialog(IDCANCEL);
			return FALSE;
		} else 
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	return TRUE;
}

// Data exchange between the dialog controls and the variables.
void CAssetTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_SURENAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_ASSETTAB_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_ASSTETTAB_MODEL_NUMBER, m_strModelNumber);
	DDX_Text(pDX, IDC_ASSETTAB_BRAND, m_strBrand);
	DDX_Text(pDX, IDC_ASSETTAB_HISTORY_LOG, m_strHistoryLog);
	DDX_Control(pDX, IDC_ASSETTAB_UPDATE, m_btnUpdateAsset);
	DDX_Control(pDX, IDC_ASSETTAB_NEW, m_btnNewAsset);
	DDX_Control(pDX, IDC_ASSETTAB_CREATE_WORKORDER, m_btnCreateWorkorder);
	DDX_Control(pDX, IDC_ASSETTAB_ASSET_LIST, m_ctrExistingAssetList);
	DDX_Control(pDX, IDC_ASSETTAB_CLEAR, m_btnClear);
}

// Catch the Enter key press event from the OS
// Depend on state, when enter is pressed asset will be updated or inserted (new) 
BOOL CAssetTab::PreTranslateMessage(MSG* pMsg)
{
	if (m_pTabControl->GetCurFocus() == 0) {
		if (pMsg->message == WM_KEYDOWN) {
			if (pMsg->wParam == VK_RETURN) {
				if (m_btnUpdateAsset.IsWindowEnabled()) {
					OnBnClickedAssetTabUpdate();
					return TRUE;
				}
				if (m_btnNewAsset.IsWindowEnabled()) {
					OnBnClickedAssetTabNew();
					return TRUE;
				}
			} else {
				return CDialogEx::PreTranslateMessage(pMsg);
			}
		}
	}
	return FALSE; //CAssetTab::PreTranslateMessage(pMsg);
}

/* Message handle methods */

// OnEnChangeAssetDetails is called when the user changes the text in the description, model number or brand edit controls.
// the new asset button is enabled if all the edit controls have text.
// If the user selects an existing asset from the list control and changes the text in these controls,
// the update asset button is enabled.
// If asset details is not dirty, the create workorder button is enabled.
void CAssetTab::OnEnChangeAssetDetails() noexcept
{
	UpdateData(TRUE);

	if (!m_bIsSelectedFromAssetList){
		if (!m_strDescription.IsEmpty()){
			m_btnNewAsset.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnNewAsset, RED, false);
		} else {
			m_btnNewAsset.EnableWindow(FALSE);
			SetCustomFocusButton(&m_btnNewAsset, BLACK, false);
		}
	} else if(!m_strDescription.IsEmpty()){
		m_btnUpdateAsset.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnUpdateAsset, RED, false);
		m_btnClear.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnClear, BLUE, false);
		m_btnCreateWorkorder.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnCreateWorkorder, BLACK, false);
	} else {
		m_btnUpdateAsset.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnUpdateAsset, BLACK, false);
		m_btnClear.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnClear, BLACK, false);
		m_btnCreateWorkorder.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnCreateWorkorder, BLACK, false);
	}
	m_btnCreateWorkorder.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnCreateWorkorder, BLACK, false);
}

// OnDoubleClickAssetTabAssetList is called when the user double clicks on an item in the list control.
// The selected asset details are loaded into the edit controls.
// and the update asset button is enabled.
// - pNMHDR is a pointer to an NMHDR structure that contains information about a notification message.
// - pResult is a pointer to an LRESULT structure that is used to return the result of the message processing.
void CAssetTab::OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	auto pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//  iItem = -1 means no existing item is selected.
	if ( pNMItemActivate->iItem != -1 ) {
		m_nAssetID = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 0));
		m_nAssetCustomerID = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 1));
		m_nAssetWorkorderID = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 2));
		m_strAssetCreateDate = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 3);
		m_strDescription = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 4);
		m_strModelNumber = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 5);
		m_strBrand = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 6);
		m_sAssetDisposed = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 7));
		m_strHistoryLog = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 8);

		m_bIsSelectedFromAssetList = true;

		m_btnCreateWorkorder.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnCreateWorkorder, RED);
		m_btnClear.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnClear, BLUE, false);

		UpdateData(FALSE);
	}
	*pResult = 0;
}

// OnBnClickedAssetTabUpdate is called when the user clicks on the update asset button.
// The asset details are updated in the database.
// After update the update asset button is disabled and the create workorder button is enabled.
void CAssetTab::OnBnClickedAssetTabUpdate()
{
	UpdateData(TRUE);
	CString strQuery;

	auto strCurDate{ COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")) };
	if (!m_strHistoryLog.IsEmpty())
		m_strHistoryLog = _T("\r\n");
	m_strHistoryLog += _T("[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Asset details are updated.");

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) ->CString{
		CString strResult;
		if (str.IsEmpty()) return  _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return static_cast<LPCTSTR>(strResult);
	};

	// A numeric zero is converted to a string zero or as an empty string, depending on the isNull parameter.
	// Because when a CString format %d is used, a numeric zero must be converted to an zero string.
	// otherwise the CString format will see this as a NUL-character, and causes an error on the string length check.
	// If the isNull parameter is true, the numeric zero is converted to an empty string, so that lambda function buildFieldValue() returns "NULL".
	auto intToCString = [](unsigned int n, bool isNull = false) -> CString{
		CString strResult{ _T("") };
		if (n == 0){
			if (isNull)	return strResult;
			else return _T("0");
		}
		strResult.Format(_T("%d"), n);
		return strResult;
	};

	strQuery.Format(_T("UPDATE [ASSET] SET [ASSET_CUSTOMER_ID] = %d, [ASSET_WORKORDER_ID] = %s, [ASSET_CREATE_DATE] = %s, ")
		_T("[ASSET_DESCRIPTION] = % s, [ASSET_MODEL_NUMBER] = % s, [ASSET_BRAND] = % s, [ASSET_DISPOSED] = % s, ")
		_T("[ASSET_HISTORY_LOG] = % s WHERE[ASSET_ID] = % d"),
		m_nAssetCustomerID,
		buildFieldValue(intToCString(m_nAssetWorkorderID, true)),
		buildFieldValue(m_strAssetCreateDate),
		buildFieldValue(m_strDescription),
		buildFieldValue(m_strModelNumber),
		buildFieldValue(m_strBrand),
		buildFieldValue(intToCString(m_sAssetDisposed)),
		buildFieldValue(m_strHistoryLog),
		m_nAssetID);

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {
		if (!sql.ExecuteQuery(strQuery.GetBuffer())) {
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
		} else {
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_OK);
			m_btnCreateWorkorder.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnCreateWorkorder, RED);
			m_btnUpdateAsset.EnableWindow(FALSE);
			SetCustomFocusButton(&m_btnUpdateAsset, BLACK, false);
		}
	} else {
		theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
	}
	strQuery.ReleaseBuffer();
	sql.CloseConnection();
	theApp.EndWaitCursor();
	UpdateData(FALSE);
} 
// OnBnClickedAssetTabNew is called when the user clicks on the new asset button.
// a new asset is created in the database.
// After creation the new asset button is disabled and the create workorder button is enabled.
void CAssetTab::OnBnClickedAssetTabNew(){
	UpdateData(TRUE);

	CString strQuery;
	auto strCurDate{ COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")) };
	if (!m_strHistoryLog.IsEmpty())
		m_strHistoryLog = _T("\r\n");
	m_strHistoryLog += _T("[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Asset Created.");

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString{
		CString strResult;
		if (str.IsEmpty()) return _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return static_cast<LPCTSTR>(strResult);
	};

	strQuery.Format(_T("INSERT INTO [ASSET] ([ASSET_CUSTOMER_ID], [ASSET_WORKORDER_ID], [ASSET_CREATE_DATE], [ASSET_DESCRIPTION], ")
		_T("[ASSET_MODEL_NUMBER], [ASSET_BRAND], [ASSET_DISPOSED], [ASSET_HISTORY_LOG]) VALUES(% d, NULL, % s, % s, % s, % s, 0, NULL)"),
		m_nAssetCustomerID,
		buildFieldValue(strCurDate),
		buildFieldValue(m_strDescription),
		buildFieldValue(m_strModelNumber),
		buildFieldValue(m_strBrand));

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {


		if (!sql.ExecuteQuery(strQuery.GetBuffer())) {
			theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
		}
		else {
			auto lastID = sql.GetLastAddedID(_T("SELECT IDENT_CURRENT('ASSET')"));
			if (lastID > 0) {
				m_nAssetID = lastID;
				theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
				m_btnCreateWorkorder.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnCreateWorkorder, RED);
				m_btnNewAsset.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnNewAsset, BLACK, false);
			}
			else {
				theApp.SetStatusBarText(IDS_STATUSBAR_LASTID_FAIL);
			}
		}
	}
	strQuery.ReleaseBuffer();
	sql.CloseConnection();
	theApp.EndWaitCursor();
	m_bIsSelectedFromAssetList = true;
	UpdateData(FALSE);
}

// OnBnClickedAssetTabCreateWorkorder is called when the user clicks on the create workorder button.
// The asset details are loaded into the shared data structure.
// The tab control is switched to the workorder tab.
void CAssetTab::OnBnClickedAssetTabCreateWorkorder() noexcept
{
	UpdateData(TRUE);

	m_pAssetDetailsRecords->m_strCustomerSurname = m_strCustomerSurname;
	m_pAssetDetailsRecords->m_strCustomerName = m_strCustomerName;

	m_pAssetDetailsRecords->m_nAssetID = m_nAssetID;
	m_pAssetDetailsRecords->m_nAssetCustomerID = m_nAssetCustomerID;
	m_pAssetDetailsRecords->m_strDescription = m_strDescription;
	m_pAssetDetailsRecords->m_strModelNumber = m_strModelNumber;
	m_pAssetDetailsRecords->m_strBrand = m_strBrand;

	ClearForNewInput();

	m_pTabControl->ChangeTabView();

}

// OnBnClickedAssetTabClear is called when the user clicks on the clear button.
// The edit controls are cleared for new input.
void CAssetTab::OnBnClickedAssetTabClear() noexcept { ClearForNewInput(); }

/* Member methods*/

// ClearForNewInput clears the edit controls for new input.
void CAssetTab::ClearForNewInput() noexcept
{
	m_nAssetID = 0;
	m_nAssetWorkorderID = 0;
	m_strDescription = _T("");
	m_strModelNumber = _T("");
	m_strBrand = _T("");
	m_strHistoryLog = _T("");
	m_bIsSelectedFromAssetList = false;
	m_btnClear.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnClear, BLACK, false);
	UpdateData(FALSE);
	OnEnChangeAssetDetails();
}

// SetCustomFocusButton sets the focus on the button and changes the text color.
// - pButton is a pointer to the button control.
// - Color is the color of the text.
// - bFocus is a boolean value that indicates whether the button should have the focus.
void CAssetTab::SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus) noexcept 
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
