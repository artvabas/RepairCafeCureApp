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
* File: CCustomerView.h, defines class CCustomerView
*
* This class is the view of the customer form.
* With this form the user can search for existing customers, add new customers, update existing customers.
* And when all the required fields are filled in, an asset can be added to the customer.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.3.5 (beta)
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 18-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CCustomerView.h"
#include "CAssetDialog.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;
using namespace artvabas::database::tables::customer;

IMPLEMENT_DYNCREATE(CCustomerView, CFormView)

CCustomerView::CCustomerView() noexcept
	: CFormView(IDD_CUSTOMER_FORM)
	, m_strSearchCustomerSurname{ _T("") }
	, m_strCustomerCellPhone{ _T("") }
	, m_strCustomerComment{ _T("") }
	, m_strCustomerLog{ _T("") }
	, m_strCustomerName{ _T("") }
	, m_strCustomerPhone{ _T("") }
	, m_strCustomerSurname{ _T("") }
	, m_strCustomerEmail{ _T("") }
	, m_strCustomerPartialInvoice{ _T("") }
	, m_bIsNewCustomer{ false }
	, m_bIsDirtyCustomerDetails{ false }
	, m_nCustomerID{ 0 }
{}

CCustomerView::~CCustomerView()
{}

/* message handle binders */
BEGIN_MESSAGE_MAP(CCustomerView, CFormView)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_SEARCH,
		&CCustomerView::OnClickedCustomViewButtonSearch)
	ON_EN_CHANGE(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH, 
		&CCustomerView::OnChangeCustomViewEditBoxSurnameSearch)
	ON_WM_UPDATEUISTATE()
	ON_NOTIFY(NM_DBLCLK, IDC_CUSTVIEW_CUSTOMER_LIST,
		&CCustomerView::OnDoubleClickCustViewCustomerList)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_NAME,
		&CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_SURNAME,
		&CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_CELLPHONE,
		&CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_PHONE,
		&CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_COMMENT,
		&CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_EMAIL,
		&CCustomerView::OnChangeCustViewCustomerDetails)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_ADD_NEW_CUSTOMER,
		&CCustomerView::OnClickedCustViewButtonAddNewCustomer)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_CUSTOMER_ADD,
		&CCustomerView::OnClickedCustViewButtonCustomerAdd)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_CUSTOMER_UPDATE,
		&CCustomerView::OnClickedCustViewButtonCustomerUpdate)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_CUSTOMER_ASSETS, 
		&CCustomerView::OnClickedCustViewButtonCustomerAssets)
END_MESSAGE_MAP()

/* Override methods */

// PreTranslateMessage: Is used to handle the keyboard messages before they are dispatched to the view.
// Depending on the message (which control trigger) , the method will call the appropriate event handler.
// - pMsg: A pointer to a MSG structure that contains the keyboard message.
BOOL CCustomerView::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			// Check if the Shift key is also down
			if (GetKeyState(VK_SHIFT) & 0x8000) // 0x8000 checks if the high-order bit is set
			{
				// Shift + Enter was pressed
				// Return to App for handling, means new line in edit Box

				return CFormView::PreTranslateMessage(pMsg);
			}
			// Does the message come from the search surname edit control?
			if (pMsg->hwnd == GetDlgItem(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH)->m_hWnd) {
				// Yes, then virtual click the search button.
				OnClickedCustomViewButtonSearch();

				if ( m_ctlExistingCustomersList.GetItemCount() > 0 ) {
					auto rowIndex{ 0 }; 
					UINT state = LVIS_SELECTED;

					m_ctlExistingCustomersList.SetItemState(rowIndex, state, LVIS_SELECTED);
					m_ctlExistingCustomersList.Update(rowIndex);
					m_ctlExistingCustomersList.SetFocus();
				}
				else
					m_btnAddNewCustomer.SetFocus();
				return TRUE;
			}
			if ( pMsg->hwnd == GetDlgItem(IDC_CUSTVIEW_BUTTON_ADD_NEW_CUSTOMER)->m_hWnd ) {
				// Yes, then click the search button.
				OnClickedCustViewButtonAddNewCustomer();
				return TRUE;
			}
			if ( pMsg->hwnd == GetDlgItem(IDC_CUSTVIEW_BUTTON_CUSTOMER_ASSETS)->m_hWnd ) {
				OnClickedCustViewButtonCustomerAssets();
				return TRUE;
			}
			if ( m_bIsNewCustomer && m_bIsDirtyCustomerDetails ) {
				// Yes, then click the search button.
				OnClickedCustViewButtonCustomerAdd();
				return TRUE;
			}
			if ( !m_bIsNewCustomer && m_bIsDirtyCustomerDetails ) {
				// Yes, then click the search button.
				OnClickedCustViewButtonCustomerUpdate();
				return TRUE;
			} else
				return FALSE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

// OnInitialUpdate: Is used to initialize the view.
// It will set the extended style of the existing customers list control.
// And insert the columns of the existing customers list control.
void CCustomerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctlExistingCustomersList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ctlExistingCustomersList.InsertColumn(0, theApp.ConvertIDToString(IDS_DBT_CUSTOMER_ID), LVCFMT_LEFT, 0);
	m_ctlExistingCustomersList.InsertColumn(1, theApp.ConvertIDToString(IDS_DBT_SURNAME), LVCFMT_LEFT, 150);
	m_ctlExistingCustomersList.InsertColumn(2, theApp.ConvertIDToString(IDS_DBT_NAME), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(3, theApp.ConvertIDToString(IDS_DBT_CELL_PHONE), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(4, theApp.ConvertIDToString(IDS_DBT_PHONE), LVCFMT_LEFT, 100);
	m_ctlExistingCustomersList.InsertColumn(5, theApp.ConvertIDToString(IDS_DBT_EMAIL), LVCFMT_LEFT, 150);
	m_ctlExistingCustomersList.InsertColumn(6, theApp.ConvertIDToString(IDS_DBT_COMMENT), LVCFMT_LEFT, 0);
	m_ctlExistingCustomersList.InsertColumn(7, theApp.ConvertIDToString(IDS_DBT_LOG), LVCFMT_LEFT, 0);
	m_ctlExistingCustomersList.InsertColumn(8, theApp.ConvertIDToString(IDS_DBT_PARTIAL_INVOICE), LVCFMT_LEFT, 0);

	// Disable all child controls of the view.
	OnUpdateUIState(UIS_INITIALIZE, 0);
}

// DoDataExchange: Exchange and validate form data between controls and class members.
// - pDX: The data exchange object
void CCustomerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH, m_strSearchCustomerSurname);
	DDV_MaxChars(pDX, m_strSearchCustomerSurname, 50);
	DDX_Control(pDX, IDC_CUSTVIEW_BUTTON_SEARCH, m_btnCustomerSurnameSearch);
	DDX_Control(pDX, IDC_CUSTVIEW_BUTTON_ADD_NEW_CUSTOMER, m_btnAddNewCustomer);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_LIST, m_ctlExistingCustomersList);
	DDX_Control(pDX, IDC_CUSTVIEW_BUTTON_CUSTOMER_ADD, m_btnAddCustomer);
	DDX_Control(pDX, IDC_CUSTVIEW_BUTTON_CUSTOMER_ASSETS, m_btnCustomAssets);
	DDX_Control(pDX, IDC_CUSTVIEW_BUTTON_CUSTOMER_UPDATE, m_btnUpdateCustomer);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_CELLPHONE, m_strCustomerCellPhone);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_COMMENT, m_strCustomerComment);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_LOG, m_strCustomerLog);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_PHONE, m_strCustomerPhone);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_SURNAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_CUSTVIEW_CUSTOMER_EMAIL, m_strCustomerEmail);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_CELLPHONE, m_ctrCustomerCellPhone);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_COMMENT, m_ctrCustomerComment);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_LOG, m_ctrCustomerLog);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_NAME, m_ctrCustomerName);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_PHONE, m_ctrCustomerPhone);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_SURNAME, m_ctrCustomerSurname);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_EMAIL, m_ctrlCustomerEmail);
}

#ifdef _DEBUG
// AssertValid: Is used to check the validity of the view.
void CCustomerView::AssertValid() const { CFormView::AssertValid(); }
#ifndef _WIN32_WCE
// Dump: Is used to dump the view.
void CCustomerView::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif
#endif //_DEBUG

/* Massage handlers methods*/

// OnClickedCustomViewButtonSearch: Is used to search for customers with the given surname.
void CCustomerView::OnClickedCustomViewButtonSearch()
{
	UpdateData(TRUE);
	
	m_ctlExistingCustomersList.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	auto row(0);		// Row of the list control item.
	CString strBuildQuery;

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	auto buildString = [](CString str) -> CString {
		CString strResult;
		strResult = _T("%") + str + _T("%");
		return strResult;
	};

	strBuildQuery.Format(_T("SELECT CUSTOMER.*, CUSTOMER_SURNAME AS Expr1 FROM CUSTOMER WHERE(CUSTOMER_SURNAME LIKE N\'%s\')"),
		static_cast<LPCTSTR>(buildString(m_strSearchCustomerSurname)));

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
					MessageBoxW(theApp.ConvertIDToString(IDS_MSGBT_ERROR_FETCH_CUSTOMER), theApp.ConvertIDToString(IDS_MSGBC_ERROR_CONNECT_DS), MB_ICONERROR);
					break;
				}
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if (cbName == SQL_NULL_DATA) {
							return _T("");
						}
						return static_cast<CString>(szName);
					};

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_ctlExistingCustomersList.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_SURNAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_NAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_CELL_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_EMAIL, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_COMMENT, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 6, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_GENERAL_LOG, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 7, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_PARTIAL_INVOICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_ctlExistingCustomersList.SetItemText(nIndex, 8, CheckForNull(szName, cbName));
				}
				else 
					break;
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) ) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		} else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);

		if ( m_ctlExistingCustomersList.GetItemCount() > 0) {
			m_ctlExistingCustomersList.EnableWindow();
			m_btnCustomerSurnameSearch.EnableWindow(FALSE);
			m_btnAddNewCustomer.EnableWindow();
			SetCustomFocusButton(&m_btnAddNewCustomer, ColorButton::BLUE);
		} else {
			m_btnCustomerSurnameSearch.EnableWindow(FALSE);
			m_btnAddNewCustomer.EnableWindow();
			SetCustomFocusButton(&m_btnAddNewCustomer, ColorButton::RED);
		}
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
}

// OnChangeCustomViewEditBoxSurnameSearch: Is used to enable/disable the search and add new customer buttons.
// On the state of the customer surname edit control.
void CCustomerView::OnChangeCustomViewEditBoxSurnameSearch() noexcept
{
	UpdateData(TRUE);
	if ( m_strSearchCustomerSurname.IsEmpty() ) {
		DisableCustomerSearchAndAddButtons();
	} else if (m_btnAddNewCustomer.IsWindowEnabled() ) {
		m_btnAddNewCustomer.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnAddNewCustomer, ColorButton::BLACK, false);
	} else {
		if( m_ctlExistingCustomersList.GetItemCount() > 0 ) {
			EmptyAndDisableExistingCustomersList();
		}

		if ( m_ctrCustomerSurname.IsWindowEnabled() )	{
			DisableCustomerDetailsButtons();
			
			UpdateCustomerDetailsControls(FALSE);

			EmptyCustomerDetailsControls();

			UpdateData(FALSE);
		}
	}
	if ( !m_btnCustomerSurnameSearch.IsWindowEnabled() ) {
		m_btnCustomerSurnameSearch.EnableWindow();
		SetCustomFocusButton(&m_btnCustomerSurnameSearch, ColorButton::RED, false);
	}
}

// OnUpdateUIState: Is used to update the state of the view.
// - nAction: The action to be taken.
// - nUIElement: The UI element to be updated.
void CCustomerView::OnUpdateUIState(UINT nAction, UINT nUIElement) noexcept
{
	auto *pChild{ GetWindow(GW_CHILD) };
	
	switch (nAction) {
		case UIS_SET:	// 1 means - Employee name is selected in the caption bar.
			// nUIElement = 0 means this method is called by the framework when the view is activated, controls are accessible.	
			if( 0 == nUIElement) {
				// Go through all child controls of the view and activate all.
				while (pChild) {
					pChild->EnableWindow(TRUE);
					pChild = pChild->GetWindow(GW_HWNDNEXT);
				}

				// Disable the controls that are not needed at this moment.
				DisableCustomerSearchAndAddButtons();
				DisableCustomerDetailsButtons();
				
				UpdateCustomerDetailsControls(FALSE);

				EmptyAndDisableExistingCustomersList();
				EmptyCustomerDetailsControls();

				auto *pEdit =  static_cast<CEdit*>(GetDlgItem(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH));
				pEdit->SetFocus();

				UpdateData(FALSE);
			}
			break;
		case NULL:
		case UIS_INITIALIZE:
			while (pChild) {
				pChild->EnableWindow(FALSE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			EmptyAndDisableExistingCustomersList();
			EmptyCustomerDetailsControls();
			UpdateData(FALSE);
			break;
		default:
			pChild->EnableWindow(TRUE);
			pChild = pChild->GetWindow(GW_HWNDNEXT);
			break;
	}
}

// OnDoubleClickCustViewCustomerList: Is used to get the customer details from the selected item in the list control.
// And enable the customer details controls.
// - pNMHDR: A pointer to a NMHDR structure that contains the notification code and additional information.
// - pResult: A pointer to an LRESULT variable.
void CCustomerView::OnDoubleClickCustViewCustomerList(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	// Get a pointer to the selected item in the list control.
	auto pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//  pNMItemActivate->iItem = -1 means no existing item is selected.
	if ( pNMItemActivate->iItem != -1 ) {
		// Set control flags.
		m_bIsNewCustomer = false;
		m_bIsDirtyCustomerDetails = false;
		
		// Disable the search button and the add new customer button.
		DisableCustomerSearchAndAddButtons();

		m_strSearchCustomerSurname.Empty();
		
		// Enable the customer details controls.
		UpdateCustomerDetailsControls();

		// Get the customer details from the selected item.
		m_nCustomerID = _tstoi(m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 0));
		m_strCustomerSurname = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 1);
		m_strCustomerName = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 2);
		m_strCustomerCellPhone = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 3);
		m_strCustomerPhone = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 4);
		m_strCustomerEmail = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 5);
		m_strCustomerComment = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 6);
		m_strCustomerLog = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 7);
		m_strCustomerPartialInvoice = m_ctlExistingCustomersList.GetItemText(pNMItemActivate->iItem, 8);
		UpdateData(FALSE);

		// Enable the customer Asset button and disable the add and update customer buttons.
		m_btnCustomAssets.EnableWindow();
		SetCustomFocusButton(&m_btnCustomAssets, ColorButton::RED);
	}
	*pResult = 0;
}

// OnChangeCustViewCustomerDetails: Is used to enable/disable the add, update and asset buttons.
// Depending on the state of the customer details controls.
void CCustomerView::OnChangeCustViewCustomerDetails() noexcept
{
	UpdateData(TRUE);
	// Disable the add, update and asset buttons when the customer details controls are empty.
	if (m_strCustomerName.IsEmpty() || m_strCustomerSurname.IsEmpty() ||
		(m_strCustomerCellPhone.IsEmpty() && m_strCustomerPhone.IsEmpty()))	{
		DisableCustomerDetailsButtons();
		m_bIsDirtyCustomerDetails = false;
	} else
		m_bIsDirtyCustomerDetails = true;
	
	// If the customer details controls are changed and the customer is new, then enable the add customer button.
	// Else if the customer details controls are changed and the customer is not new, then enable the update customer button.
	// Else disable the asset button.
	if (m_bIsDirtyCustomerDetails) {
		if (m_bIsNewCustomer && m_bIsDirtyCustomerDetails) {
			m_btnAddCustomer.EnableWindow();
			SetCustomFocusButton(&m_btnAddCustomer, ColorButton::RED, false);
			m_btnCustomAssets.EnableWindow(FALSE);
			SetCustomFocusButton(&m_btnCustomAssets, ColorButton::BLACK, false);
		} else if (!m_bIsNewCustomer && m_bIsDirtyCustomerDetails) {
			m_btnUpdateCustomer.EnableWindow();
			SetCustomFocusButton(&m_btnUpdateCustomer, ColorButton::RED, false);
			m_btnCustomAssets.EnableWindow(FALSE);
			SetCustomFocusButton(&m_btnCustomAssets, ColorButton::BLACK, false);
		}
	} else if (!m_bIsNewCustomer) {
		m_btnCustomAssets.EnableWindow(); // All other cases enable the asset button.
		SetCustomFocusButton(&m_btnCustomAssets, ColorButton::RED);
	}
}

// OnClickedCustViewButtonAddNewCustomer: Is used to add a new customer.
// The customer details controls are enabled and the existing customers list is disabled.
void CCustomerView::OnClickedCustViewButtonAddNewCustomer() noexcept
{
	m_bIsNewCustomer = true;
	UpdateData(TRUE);

	DisableCustomerSearchAndAddButtons();

	EmptyAndDisableExistingCustomersList();

	UpdateCustomerDetailsControls();

	m_strCustomerSurname = m_strSearchCustomerSurname;
	m_ctrCustomerName.SetFocus();
	m_strSearchCustomerSurname.Empty();
	UpdateData(FALSE);
}

// OnClickedCustViewButtonCustomerAdd: Is used to add a new customer to the database.
void CCustomerView::OnClickedCustViewButtonCustomerAdd() {
	UpdateData(TRUE);
	m_btnAddCustomer.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnAddCustomer, ColorButton::BLACK, false);

	CString strQuery{};
	auto strCurDate{ COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")) };

	if ( !m_strCustomerLog.IsEmpty() )
		m_strCustomerLog = _T("\r\n");
	m_strCustomerLog += _T("[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Customer Created.");

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString {
			CString strResult;
			if (str.IsEmpty()) return _T("NULL");
			strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
			return strResult;
	};

	strQuery.Format(_T("INSERT INTO [CUSTOMER] ([CUSTOMER_SURNAME], [CUSTOMER_NAME], [CUSTOMER_CELL_PHONE], [CUSTOMER_PHONE], ")
		_T("[CUSTOMER_EMAIL], [CUSTOMER_COMMENT], [CUSTOMER_GENERAL_LOG], [CUSTOMER_PARTIAL_INVOICE]) VALUES(%s, %s, %s, %s, %s, %s, %s, %d)"),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerSurname)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerName)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerCellPhone)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerPhone)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerEmail)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerComment)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerLog)),
		0);

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {
		if (!sql.ExecuteQuery(strQuery.GetBuffer()))
			theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL); 
		else {
			auto lastID{ sql.GetLastAddedID(_T("SELECT IDENT_CURRENT('CUSTOMER')")) };
			if ( lastID > 0 ) {
				m_bIsNewCustomer = false;
				m_bIsDirtyCustomerDetails = false;
				m_nCustomerID = lastID;
				theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
				m_btnCustomAssets.EnableWindow();
				SetCustomFocusButton(&m_btnCustomAssets, ColorButton::RED);
			} else 
				theApp.SetStatusBarText(IDS_STATUSBAR_LASTID_FAIL);
		}
	}
	sql.CloseConnection();
	strQuery.ReleaseBuffer();
	theApp.EndWaitCursor();
	UpdateData(FALSE);
}

// OnClickedCustViewButtonCustomerUpdate: Is used to update an existing customer in the database.
void CCustomerView::OnClickedCustViewButtonCustomerUpdate() {
	UpdateData(TRUE);
	m_btnUpdateCustomer.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnUpdateCustomer, ColorButton::BLACK, false);

	CString strQuery{};
	auto strCurDate{ COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")) };

	if (!m_strCustomerLog.IsEmpty())
		m_strCustomerLog += _T("\r\n");
	m_strCustomerLog += _T("[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Customer details Updated.");

	BuildLogMessage(m_strCustomerLog);

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString {
		CString strResult;
		if (str.IsEmpty()) return _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return strResult;
	};
	
	strQuery.Format(_T("UPDATE [CUSTOMER] SET [CUSTOMER_SURNAME] = %s, [CUSTOMER_NAME] = %s, [CUSTOMER_CELL_PHONE] = %s, [CUSTOMER_PHONE] = %s, ")
		_T("[CUSTOMER_EMAIL] = %s, [CUSTOMER_COMMENT] = %s, [CUSTOMER_GENERAL_LOG] = %s, [CUSTOMER_PARTIAL_INVOICE] = %s WHERE[CUSTOMER_ID] = %d"),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerSurname)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerName)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerCellPhone)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerPhone)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerEmail)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerComment)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerLog)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerPartialInvoice)),
				m_nCustomerID);

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		if ( !sql.ExecuteQuery(strQuery.GetBuffer()) )
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
		else {
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_OK);
			m_bIsDirtyCustomerDetails = false;
			m_btnCustomAssets.EnableWindow();
			SetCustomFocusButton(&m_btnCustomAssets, ColorButton::RED);
		}
	}
	sql.CloseConnection();
	strQuery.ReleaseBuffer();
	theApp.EndWaitCursor();
	UpdateData(FALSE);
}

// OnClickedCustViewButtonCustomerAssets: Is used to open the asset dialog.
void CCustomerView::OnClickedCustViewButtonCustomerAssets() noexcept
{
	CAssetDialog dlg{ m_strCustomerSurname, m_strCustomerName, m_nCustomerID };
	dlg.DoModal();
}

/* member methods */

// DisableCustomerSearchAndAddButtons: Is used to disable the search and add new customer buttons.
void CCustomerView::DisableCustomerSearchAndAddButtons() noexcept
{
	m_btnAddNewCustomer.EnableWindow(FALSE);
	m_btnCustomerSurnameSearch.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnAddNewCustomer, ColorButton::BLACK, false);
	SetCustomFocusButton(&m_btnCustomerSurnameSearch, ColorButton::BLACK, false);
}

// UpdateCustomerDetailsControls: Is used to enable/disable the customer details controls.
void CCustomerView::UpdateCustomerDetailsControls(BOOL bShow) noexcept
{
	m_ctrCustomerCellPhone.EnableWindow(bShow);
	m_ctrCustomerComment.EnableWindow(bShow);
	m_ctrCustomerLog.EnableWindow(bShow);
	m_ctrCustomerName.EnableWindow(bShow);
	m_ctrCustomerPhone.EnableWindow(bShow);
	m_ctrCustomerSurname.EnableWindow(bShow);
	m_ctrlCustomerEmail.EnableWindow(bShow);
}

// DisableCustomerDetailsButtons: Is used to disable the customer details buttons.
void CCustomerView::DisableCustomerDetailsButtons() noexcept
{
	m_btnAddCustomer.EnableWindow(FALSE);
	m_btnCustomAssets.EnableWindow(FALSE);
	m_btnUpdateCustomer.EnableWindow(FALSE);
}

// EmptyCustomerDetailsControls: Is used to empty the customer details controls.
void CCustomerView::EmptyCustomerDetailsControls() noexcept
{
	m_strCustomerCellPhone.Empty();
	m_strCustomerComment.Empty();
	m_strCustomerLog.Empty();
	m_strCustomerName.Empty();
	m_strCustomerPhone.Empty();
	m_strCustomerSurname.Empty();
	m_strCustomerEmail.Empty();
}

// EmptyAndDisableExistingCustomersList: Is used to empty and disable the existing customers list control.
void CCustomerView::EmptyAndDisableExistingCustomersList() noexcept
{
	m_ctlExistingCustomersList.DeleteAllItems();
	m_ctlExistingCustomersList.EnableWindow(FALSE);
}

// SetCustomFocusButton: Is used to set the focus on a button and change the text color.
// - pButton: The button to set.
// - Color: The color of the text.
// - bFocus: If true, the button will get the focus.
void CCustomerView::SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus) noexcept
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

void CCustomerView::BuildLogMessage(const CString& strMessage) noexcept
{
}
