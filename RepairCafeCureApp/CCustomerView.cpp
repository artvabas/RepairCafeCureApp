// CCustomerView.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CCustomerView.h"

//using namespace artvabas::rcc::ui;
// CCustomerView

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
{

}

CCustomerView::~CCustomerView()
{
	
}

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
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_CELLPHONE, m_ctrCustomerCellPhone);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_COMMENT, m_ctrCustomerComment);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_LOG, m_ctrCustomerLog);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_NAME, m_ctrCustomerName);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_PHONE, m_ctrCustomerPhone);
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_SURNAME, m_ctrCustomerSurname);
}

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
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_ADD_NEW_CUSTOMER, &CCustomerView::OnClickedCustViewButtonAddNewCustomer)
END_MESSAGE_MAP()


// CCustomerView diagnostics

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


// CCustomerView message handlers

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

void CCustomerView::OnClickedCustomViewButtonSearch()
{
	UpdateData(TRUE);
	m_btnAddNewCustomer.EnableWindow();
	m_ctlExistingCustomersList.EnableWindow();

	// test data
	int nIndex = m_ctlExistingCustomersList.InsertItem(0, _T("artvabas"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 1, _T("Arthur"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 2, _T("0636272731"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 3, _T("0725426587"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 4, _T("arthur@domain.com"));

	nIndex = m_ctlExistingCustomersList.InsertItem(1, _T("Repair cafe"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 1, _T("Cure"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 2, _T("06112112112"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 3, _T("07291191191"));
	m_ctlExistingCustomersList.SetItemText(nIndex, 4, _T("cafe@domain.com"));
}

/// <summary>
/// OnChangeCustomViewEditBoxSurnameSearch is used to enable/disable the search button
/// and the add new customer button. And clear the existing customers list control.
/// </summary>
void CCustomerView::OnChangeCustomViewEditBoxSurnameSearch()
{
	UpdateData(TRUE);
	if (m_strSearchCustomerSurname.IsEmpty())
	{
		m_btnCustomerSurnameSearch.EnableWindow(FALSE);
		m_btnAddNewCustomer.EnableWindow(FALSE);
	}
	else
	{
		if(m_ctlExistingCustomersList.GetItemCount() > 0) 
		{
			m_ctlExistingCustomersList.DeleteAllItems();
			m_ctlExistingCustomersList.EnableWindow(FALSE);
		}

		m_btnCustomerSurnameSearch.EnableWindow();
	}
}

/// <summary>
/// OnUpdateUIState is used to disable all child controls of the view.
/// </summary>
/// <param name="nAction">The action that is performed.</param>
/// <param name="nUIElement">The UI element that is affected.</param>
void CCustomerView::OnUpdateUIState(UINT nAction, UINT nUIElement)
{
	CWnd* pChild = GetWindow(GW_CHILD);
	switch (nAction)
	{
		case 1:
			while (pChild)
			{
				pChild->EnableWindow(TRUE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			// Disable the search button and the add new customer button.
			// nUIElement = 3 means this method is called by the framework at startup.
			// At startup the controls are not active/accessibly.
			// nUIElement = 0 means this method is called by the framework when the view is activated, controls are accessible.	
			if( 0 == nUIElement)
			{
				m_btnCustomerSurnameSearch.EnableWindow(FALSE);
				m_btnAddNewCustomer.EnableWindow(FALSE);
				m_btnAddCustomer.EnableWindow(FALSE);
				m_btnCustomAssets.EnableWindow(FALSE);
				m_btnUpdateCustomer.EnableWindow(FALSE);
				m_ctrCustomerCellPhone.EnableWindow(FALSE);
				m_ctrCustomerComment.EnableWindow(FALSE);
				m_ctrCustomerLog.EnableWindow(FALSE);
				m_ctrCustomerName.EnableWindow(FALSE);
				m_ctrCustomerPhone.EnableWindow(FALSE);
				m_ctrCustomerSurname.EnableWindow(FALSE);
				m_ctlExistingCustomersList.EnableWindow(FALSE);
				m_strSearchCustomerSurname.Empty();
				m_strCustomerCellPhone.Empty();
				m_strCustomerComment.Empty();
				m_strCustomerLog.Empty();
				m_strCustomerName.Empty();
				m_strCustomerPhone.Empty();
				m_strCustomerSurname.Empty();
				UpdateData(FALSE);
			}
			break;
		case 0:
			while (pChild)
			{
				pChild->EnableWindow(FALSE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			break;
		default:
			pChild->EnableWindow(TRUE);
			pChild = pChild->GetWindow(GW_HWNDNEXT);
			break;
	}
}


void CCustomerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctlExistingCustomersList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ctlExistingCustomersList.InsertColumn(0, _T("SURNAME"), LVCFMT_LEFT, 150);
	m_ctlExistingCustomersList.InsertColumn(1, _T("NAME"), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(2, _T("CELLPHONE"), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(3, _T("PHONE"), LVCFMT_LEFT, 90);
	m_ctlExistingCustomersList.InsertColumn(4, _T("EMAIL"), LVCFMT_LEFT, 150);
}


void CCustomerView::OnDoubleClickCustViewCustomerList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//  pNMItemActivate->iItem = -1 means no existing item is selected.
	if (pNMItemActivate->iItem != -1)
	{
		m_btnAddCustomer.EnableWindow(FALSE);
		m_btnCustomerSurnameSearch.EnableWindow(FALSE);
		m_strSearchCustomerSurname.Empty();
		
		m_ctrCustomerCellPhone.EnableWindow();
		m_ctrCustomerComment.EnableWindow();
		m_ctrCustomerLog.EnableWindow();
		m_ctrCustomerName.EnableWindow();
		m_ctrCustomerPhone.EnableWindow();
		m_ctrCustomerSurname.EnableWindow();

		UpdateData(FALSE);
	}
	*pResult = 0;
}


void CCustomerView::OnChangeCustViewCustomerDetails()
{
	UpdateData(TRUE);
	if (m_strCustomerName.IsEmpty() || m_strCustomerSurname.IsEmpty() ||
		(m_strCustomerCellPhone.IsEmpty() && m_strCustomerPhone.IsEmpty()))
	{
		m_btnAddCustomer.EnableWindow(FALSE);
		m_btnCustomAssets.EnableWindow(FALSE);
		m_btnUpdateCustomer.EnableWindow(FALSE);
	}
	else
	{
		m_btnAddCustomer.EnableWindow();
		m_btnCustomAssets.EnableWindow();
		m_btnUpdateCustomer.EnableWindow();
	}
}



void CCustomerView::OnClickedCustViewButtonAddNewCustomer()
{
	m_btnAddCustomer.EnableWindow(FALSE);
	m_btnCustomerSurnameSearch.EnableWindow(FALSE);
	m_strSearchCustomerSurname.Empty();

	m_ctrCustomerCellPhone.EnableWindow();
	m_ctrCustomerComment.EnableWindow();
	m_ctrCustomerLog.EnableWindow();
	m_ctrCustomerName.EnableWindow();
	m_ctrCustomerPhone.EnableWindow();
	m_ctrCustomerSurname.EnableWindow();
}
