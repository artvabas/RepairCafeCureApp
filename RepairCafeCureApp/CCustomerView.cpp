// CCustomerView.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CCustomerView.h"

using namespace artvabas::rcc::ui;
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
	DDX_Control(pDX, IDC_CUSTVIEW_BUTTON_ADD_NEW_CUSTOMER, m_btnCustomerSurnameSearch);
}

BEGIN_MESSAGE_MAP(CCustomerView, CFormView)
	ON_BN_CLICKED(IDC_CUSTVIEW_BUTTON_SEARCH, &CCustomerView::OnClickedCustomViewButtonSearch)
	ON_EN_CHANGE(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH, &CCustomerView::OnChangeCustomViewEditBoxSurnameSearch)
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


BOOL CCustomerView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{ 
		if (pMsg->wParam == VK_RETURN)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_CUSTVIEW_EDITBOX_SURNAME_SEARCH)->m_hWnd)
			{
				OnClickedCustomViewButtonSearch();

				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CCustomerView::OnClickedCustomViewButtonSearch()
{
	UpdateData(TRUE);

	if (!m_strSearchCustomerSurname.IsEmpty())
	{
		m_btnCustomerSurnameSearch.EnableWindow();
		AfxMessageBox(m_strSearchCustomerSurname);
	}
}


void CCustomerView::OnChangeCustomViewEditBoxSurnameSearch()
{
	UpdateData(TRUE);
	if(m_strSearchCustomerSurname.IsEmpty())
		m_btnCustomerSurnameSearch.EnableWindow(FALSE);
}
