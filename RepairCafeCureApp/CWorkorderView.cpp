// CWorkorderView.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderView.h"

using namespace artvabas::rcc::ui;

// CWorkorderView

IMPLEMENT_DYNCREATE(CWorkorderView, CFormView)

CWorkorderView::CWorkorderView()
	: CFormView(IDD_WORKORDER_FORM)
{

}

CWorkorderView::~CWorkorderView()
{
}

void CWorkorderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWorkorderView, CFormView)
END_MESSAGE_MAP()


// CWorkorderView diagnostics

#ifdef _DEBUG
void CWorkorderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorkorderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWorkorderView message handlers
