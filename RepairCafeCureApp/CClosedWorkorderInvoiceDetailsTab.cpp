// CClosedWorkorderInvoiceDetailsTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkorderInvoiceDetailsTab.h"

IMPLEMENT_DYNAMIC(CClosedWorkorderInvoiceDetailsTab, CDialogEx)

CClosedWorkorderInvoiceDetailsTab::CClosedWorkorderInvoiceDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSED_WORKORDER_INVOICE_DETAILS_TAB, pParent)
	, m_pTabControl{ pTabControl }
	, m_unInvoiceID{ unID }
{

}

CClosedWorkorderInvoiceDetailsTab::~CClosedWorkorderInvoiceDetailsTab()
{
}

void CClosedWorkorderInvoiceDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClosedWorkorderInvoiceDetailsTab, CDialogEx)
END_MESSAGE_MAP()


