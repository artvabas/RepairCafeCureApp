/*
	Copyright (C) 2023  artvabas

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
* Version: 1.0.230.0
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 03-11-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/


#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CCustomerView.h"
#include "CAssetDialog.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::sql;

IMPLEMENT_DYNCREATE(CCustomerView, CFormView)

CCustomerView::CCustomerView()
	: CFormView(IDD_CUSTOMER_FORM)
	, m_strSearchCustomerSurname(_T(""))
	, m_strCustomerCellPhone(_T(""))
	, m_strCustomerComment(_T(""))
	, m_strCustomerLog(_T(""))
	, m_strCustomerName(_T(""))
	, m_strCustomerPhone(_T(""))
	, m_strCustomerSurname(_T(""))
	, m_strCustomerEmail(_T(""))
	, m_bIsNewCustomer(false)
	, m_bIsDirtyCustomerDetails(false)
	, m_nCustomerID(0)
{

}

CCustomerView::~CCustomerView()
{
	
}

/* Message maps fro events */

/// <summary>
/// BEGIN_MESSAGE_MAP is used to map messages to functions.
/// </summary>
/// <param name="CCustomerView">The class that is mapped.</param>
BEGIN_MESSAGE_MAP(CCustomerView, CFormView)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_SEARCH, &CCustomerView::OnClickedCustomViewButtonSearch)
	ON_EN_CHANGE(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH, &CCustomerView::OnChangeCustomViewEditBoxSurnameSearch)
	ON_WM_UPDATEUISTATE()
	ON_NOTIFY(NM_DBLCLK, IDC_CUSTVIEW_CUSTOMER_LIST, &CCustomerView::OnDoubleClickCustViewCustomerList)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_NAME, &CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_SURNAME, &CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_CELLPHONE, &CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_PHONE, &CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_COMMENT, &CCustomerView::OnChangeCustViewCustomerDetails)
	ON_EN_CHANGE(IDC_CUSTVIEW_CUSTOMER_EMAIL, &CCustomerView::OnChangeCustViewCustomerDetails)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_ADD_NEW_CUSTOMER, &CCustomerView::OnClickedCustViewButtonAddNewCustomer)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_CUSTOMER_ADD, &CCustomerView::OnClickedCustViewButtonCustomerAdd)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_CUSTOMER_UPDATE, &CCustomerView::OnClickedCustViewButtonCustomerUpdate)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_CUSTOMER_ASSETS, &CCustomerView::OnClickedCustViewButtonCustomerAssets)
END_MESSAGE_MAP()


/* Override methods */

#ifdef _DEBUG
void CCustomerView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCustomerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/// <summary>
/// PreTranslateMessage is used to handle the VK_RETURN key press event.
/// If key press event is VK_RETURN and the focus is on the search surname edit control, then the search button is clicked.
/// </summary>
/// <param name="pMsg" the message that is send></param>
/// <returns></returns>
BOOL CCustomerView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{ 
		if (pMsg->wParam == VK_RETURN)
		{
			// Does the message come from the search surname edit control?
			if (pMsg->hwnd == GetDlgItem(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH)->m_hWnd)
			{
				// Yes, then click the search button.
				OnClickedCustomViewButtonSearch();
				return TRUE;
			}
			else
			{
				// No, then let the base class handle the message.
				return FALSE;
			}
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

/// <summary>
/// OnInitialUpdate is used to set the extended style of the existing customers list control.
/// And to insert the columns of the existing customers list control.
/// OnitialUpdate is called by the framework after the view is created.
/// </summary>
void CCustomerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctlExistingCustomersList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ctlExistingCustomersList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 0);
	m_ctlExistingCustomersList.InsertColumn(1, _T("SURNAME"), LVCFMT_LEFT, 150);
	m_ctlExistingCustomersList.InsertColumn(2, _T("NAME"), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(3, _T("CELLPHONE"), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(4, _T("PHONE"), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(5, _T("EMAIL"), LVCFMT_LEFT, 150);
	m_ctlExistingCustomersList.InsertColumn(6, _T("COMMENT"), LVCFMT_LEFT, 0);
	m_ctlExistingCustomersList.InsertColumn(7, _T("LOG"), LVCFMT_LEFT, 0);

	// Disable all child controls of the view.
	OnUpdateUIState(UIS_INITIALIZE, 0);

}

/// <summary>
/// DoDataExchange() is used to exchange and validate FormView data
/// </summary>
/// <param name="pDX">A pointer to a CDataExchange object.</param>
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


/* Event handlers */

/// <summary>
/// OnClickedCustomViewButtonSearch is used to search for customers with the surname that is entered in the search surname edit control.
/// It will fill the existing customers list control with the found customers from the database.
/// </summary>
void CCustomerView::OnClickedCustomViewButtonSearch()
{
	UpdateData(TRUE);

	m_btnAddNewCustomer.EnableWindow();
	m_ctlExistingCustomersList.EnableWindow();
	m_ctlExistingCustomersList.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strQuery;

	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT CUSTOMER.*, CUSTOMER_SURNAME AS Expr1 FROM CUSTOMER WHERE(CUSTOMER_SURNAME = N\'%s\')"),
		static_cast<LPCTSTR>(m_strSearchCustomerSurname));

	theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery);
	
	// Fill the existing customers list control with the found customers from the database.
	while (!rs->IsEOF())
	{
		CString strValue = _T("");
		rs->GetFieldValue(_T("CUSTOMER_ID"), strValue);
		nIndex = m_ctlExistingCustomersList.InsertItem(row++, strValue);

		rs->GetFieldValue(_T("CUSTOMER_SURNAME"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 1, strValue);

		rs->GetFieldValue(_T("CUSTOMER_NAME"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 2, strValue);

		rs->GetFieldValue(_T("CUSTOMER_CELL_PHONE"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 3, strValue);

		rs->GetFieldValue(_T("CUSTOMER_PHONE"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 4, strValue);

		rs->GetFieldValue(_T("CUSTOMER_EMAIL"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 5, strValue);

		rs->GetFieldValue(_T("CUSTOMER_COMMENT"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 6, strValue);

		rs->GetFieldValue(_T("CUSTOMER_GENERAL_LOG"), strValue);
		m_ctlExistingCustomersList.SetItemText(nIndex, 7, strValue);

		rs->MoveNext();
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
}

/// <summary>
/// OnChangeCustomViewEditBoxSurnameSearch is used to enable/disable the search button
/// and the add new customer button. And clear the existing customers list control.
/// 
/// Disable the customer search and add buttons, when the search surname edit control is empty.
/// 
/// Else if the existing customers list control is not empty, then empty the existing customers list control.
/// And disable the customer details buttons. But enable the customer search button when the search surname edit control is not empty.
/// </summary>
void CCustomerView::OnChangeCustomViewEditBoxSurnameSearch()
{
	UpdateData(TRUE);
	if (m_strSearchCustomerSurname.IsEmpty())
	{
		DisableCustomerSearchAndAddButtons();
	}
	else
	{
		if(m_ctlExistingCustomersList.GetItemCount() > 0) 
		{
			EmptyAndDisableExistingCustomersList();
		}
		
		if (m_ctrCustomerSurname.IsWindowEnabled())
		{
			DisableCustomerDetailsButtons();
			
			UpdateCustomerDetailsControls(FALSE);

			EmptyCustomerDetailsControls();

			UpdateData(FALSE);
		}

		m_btnCustomerSurnameSearch.EnableWindow();
	}
}

/// <summary>
/// OnUpdateUIState is used to disable all child controls of the view.
/// This method is trigged by the framework when the view is activated,
/// or created at first time.
/// </summary>
/// <param name="nAction">The action that is performed.</param>
/// <param name="nUIElement">The UI element that is affected.</param>
void CCustomerView::OnUpdateUIState(UINT nAction, UINT nUIElement)
{
	CWnd* pChild = GetWindow(GW_CHILD);
	
	switch (nAction)
	{
		case 1:	// UIS_SET
			// nUIElement = 0 means this method is called by the framework when the view is activated, controls are accessible.	
			if( 0 == nUIElement)
			{
				while (pChild)	// Go through all child controls of the view and acitvate all.
				{
					pChild->EnableWindow(TRUE);
					pChild = pChild->GetWindow(GW_HWNDNEXT);
				}

				// Disable the controls that are not needed at this moment.
				DisableCustomerSearchAndAddButtons();
				DisableCustomerDetailsButtons();
				
				UpdateCustomerDetailsControls(FALSE);

				EmptyAndDisableExistingCustomersList();
				EmptyCustomerDetailsControls();

				UpdateData(FALSE);
			}
			break;
		case 0:	
		case 3:	// UIS_INITIALIZE
			while (pChild)
			{
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

/// <summary>
/// OnDoubleClickCustViewCustomerList is used to get the customer details from the list and assign them to the customer details controls.
/// </summary>
/// <param name="pNMHDR">The notification message header.</param>
/// <param name="pResult">The result of the notification message.</param>
void CCustomerView::OnDoubleClickCustViewCustomerList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get a pointer to the selected item in the list control.
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//  pNMItemActivate->iItem = -1 means no existing item is selected.
	if (pNMItemActivate->iItem != -1)
	{
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
		UpdateData(FALSE);

		// Enable the customer Asset button and disable the add and update customer buttons.
		OnChangeCustViewCustomerDetails();
	}
	*pResult = 0;
}

/// <summary>
/// OnChangeCustViewCustomerDetails is used to enable/disable the add and update customer buttons.
/// </summary>
void CCustomerView::OnChangeCustViewCustomerDetails()
{
	UpdateData(TRUE);
	// Disable the add, update and asset buttons when the customer details controls are empty.
	if (m_strCustomerName.IsEmpty() || m_strCustomerSurname.IsEmpty() ||
		(m_strCustomerCellPhone.IsEmpty() && m_strCustomerPhone.IsEmpty()))
	{
		DisableCustomerDetailsButtons();
	}
	else
	{
		// If the customer details controls are changed and the customer is new, then enable the add customer button.
		// Else if the customer details controls are changed and the customer is not new, then enable the update customer button.
		// Else disable the asset button.
		if (m_bIsDirtyCustomerDetails)
		{
			m_bIsNewCustomer ? m_btnAddCustomer.EnableWindow() : m_btnUpdateCustomer.EnableWindow();
			m_btnCustomAssets.EnableWindow(FALSE);
		}
		else
			m_btnCustomAssets.EnableWindow(); // All other cases enable the asset button.
	}
	m_bIsDirtyCustomerDetails = true;
	
}

/// <summary>
/// OnClickedCustViewButtonAddNewCustomer will move the given customer search surname to the customer surname edit control.
/// And empty the customer search surname edit control, and disable the customer search and add buttons.
/// </summary>
void CCustomerView::OnClickedCustViewButtonAddNewCustomer()
{
	m_bIsNewCustomer = true;
	UpdateData(TRUE);

	DisableCustomerSearchAndAddButtons();

	EmptyAndDisableExistingCustomersList();

	UpdateCustomerDetailsControls();

	m_strCustomerSurname = m_strSearchCustomerSurname;
	m_strSearchCustomerSurname.Empty();
	UpdateData(FALSE);
}

/// <summary>
/// OnClickedCustViewButtonCustomerAdd is used to add (INSERT) a new customer to the database.
/// </summary>
void CCustomerView::OnClickedCustViewButtonCustomerAdd()
{
	UpdateData(TRUE);
	m_btnAddCustomer.EnableWindow(FALSE);
	m_btnCustomAssets.EnableWindow();

	CString strQuery;

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString
		{
			CString strResult;
			if (str.IsEmpty())
				return  _T("NULL");
			strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
			return strResult;
		};

	strQuery.Format(_T("INSERT INTO [CUSTOMER] ([CUSTOMER_SURNAME], [CUSTOMER_NAME], [CUSTOMER_CELL_PHONE], [CUSTOMER_PHONE], [CUSTOMER_EMAIL], [CUSTOMER_COMMENT], [CUSTOMER_GENERAL_LOG]) VALUES (%s, %s, %s, %s, %s, %s, %s)"),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerSurname)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerName)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerCellPhone)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerPhone)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerEmail)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerComment)),
		static_cast<LPCTSTR>(buildFieldValue(m_strCustomerLog)));

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());
	sql.ExecuteQuery(strQuery.GetBuffer());

	strQuery.ReleaseBuffer();
}

/// <summary>
/// OnClickedCustViewButtonCustomerUpdate is used to update (UPDATE) an existing customer in the database.
/// </summary>
void CCustomerView::OnClickedCustViewButtonCustomerUpdate()
{
	UpdateData(TRUE);
	m_btnUpdateCustomer.EnableWindow(FALSE);
	m_btnCustomAssets.EnableWindow();

	CString strQuery;

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString
	{
		CString strResult;
		if (str.IsEmpty())
			return  _T("NULL");
		strResult.Format(_T("N\'%s\'"),static_cast<LPCTSTR>(str));
		return strResult;
	};
	
	strQuery.Format(_T("UPDATE [CUSTOMER] SET [CUSTOMER_SURNAME] = %s, [CUSTOMER_NAME] = %s, [CUSTOMER_CELL_PHONE] = %s, [CUSTOMER_PHONE] = %s, [CUSTOMER_EMAIL] = %s, [CUSTOMER_COMMENT] = %s, [CUSTOMER_GENERAL_LOG] = %s WHERE [CUSTOMER_ID] = %d"),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerSurname)),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerName)),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerCellPhone)),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerPhone)),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerEmail)),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerComment)),
				static_cast<LPCTSTR>(buildFieldValue(m_strCustomerLog)),
				m_nCustomerID);

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());
	sql.ExecuteQuery(strQuery.GetBuffer());

	strQuery.ReleaseBuffer();
}

void CCustomerView::OnClickedCustViewButtonCustomerAssets()
{
	CAssetDialog dlg(m_strCustomerSurname, m_strCustomerName, m_nCustomerID);
	dlg.DoModal();
	//theApp.SwitchView(CRepairCafeCureApp::VIEW_ASSET);
}

/// <summary>
/// DisableCustomerSearchAndAddButtons is used to disable the search button and the add new customer button.
/// </summary>
void CCustomerView::DisableCustomerSearchAndAddButtons()
{
	m_btnAddNewCustomer.EnableWindow(FALSE);
	m_btnCustomerSurnameSearch.EnableWindow(FALSE);
}

/// <summary>
/// UpdateCustomerDetailsControls is used to enable/disable the customer details controls.
/// </summary>
void CCustomerView::UpdateCustomerDetailsControls(BOOL bShow)
{
	m_ctrCustomerCellPhone.EnableWindow(bShow);
	m_ctrCustomerComment.EnableWindow(bShow);
	m_ctrCustomerLog.EnableWindow(bShow);
	m_ctrCustomerName.EnableWindow(bShow);
	m_ctrCustomerPhone.EnableWindow(bShow);
	m_ctrCustomerSurname.EnableWindow(bShow);
	m_ctrlCustomerEmail.EnableWindow(bShow);
}

/// <summary>
/// DisableCustomerDetailsButtons is used to disable the add and update customer buttons.
/// </summary>
void CCustomerView::DisableCustomerDetailsButtons()
{
	m_btnAddCustomer.EnableWindow(FALSE);
	m_btnCustomAssets.EnableWindow(FALSE);
	m_btnUpdateCustomer.EnableWindow(FALSE);
}

/// <summary>
/// EmptyCustomerDetailsControls is used to empty the customer details controls.
/// </summary>
void CCustomerView::EmptyCustomerDetailsControls()
{
	m_strCustomerCellPhone.Empty();
	m_strCustomerComment.Empty();
	m_strCustomerLog.Empty();
	m_strCustomerName.Empty();
	m_strCustomerPhone.Empty();
	m_strCustomerSurname.Empty();
	m_strCustomerEmail.Empty();
}

/// <summary>
/// EmptyAndDisableExistingCustomersList is used to empty the existing customers list control.
/// And empty the search surname edit control.
/// </summary>
void CCustomerView::EmptyAndDisableExistingCustomersList()
{
	m_ctlExistingCustomersList.DeleteAllItems();
	m_ctlExistingCustomersList.EnableWindow(FALSE);
}
