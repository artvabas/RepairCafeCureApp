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
	DDX_Control(pDX, IDC_CUSTVIEW_CUSTOMER_LIST, m_ctrExistingCustomerList);
}

BEGIN_MESSAGE_MAP(CCustomerView, CFormView)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_SEARCH, &CCustomerView::OnClickedCustomViewButtonSearch)
	ON_EN_CHANGE(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH, &CCustomerView::OnChangeCustomViewEditBoxSurnameSearch)
	ON_WM_UPDATEUISTATE()
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
	m_ctrExistingCustomerList.DeleteAllItems();
	while (m_ctrExistingCustomerList.DeleteColumn(0));
	m_ctrExistingCustomerList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_ctrExistingCustomerList.InsertColumn(1, _T("NAME"), LVCFMT_LEFT, 50);

	m_ctrExistingCustomerList.InsertItem(0, _T("1"));
	m_ctrExistingCustomerList.SetItemText(0, 1, _T("John"));

	m_ctrExistingCustomerList.InsertItem(1, _T("2"));
	m_ctrExistingCustomerList.SetItemText(1, 1, _T("Doe"));
}

/// <summary>
/// OnChangeCustomViewEditBoxSurnameSearch is used to enable/disable the search button.
/// </summary>
void CCustomerView::OnChangeCustomViewEditBoxSurnameSearch()
{
	UpdateData(TRUE);
	if(m_strSearchCustomerSurname.IsEmpty())
		m_btnCustomerSurnameSearch.EnableWindow(FALSE);
	else
		m_btnCustomerSurnameSearch.EnableWindow();
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
				m_strSearchCustomerSurname.Empty();
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
