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
{

}

CCustomerView::~CCustomerView()
{
}

void CCustomerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCustomerView, CFormView)
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
