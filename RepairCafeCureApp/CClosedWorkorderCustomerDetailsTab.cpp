// CClosedWorkorderCustomerDetailsTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkorderCustomerDetailsTab.h"


// CClosedWorkorderCustomerDetailsTab dialog

IMPLEMENT_DYNAMIC(CClosedWorkorderCustomerDetailsTab, CDialogEx)

CClosedWorkorderCustomerDetailsTab::CClosedWorkorderCustomerDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent /*=nullptr*/)
	: CDialogEx{ IDD_CLOSED_WORKORDER_CUSTOMER_DETAILS_TAB, pParent }
	, m_pTabControl{ pTabControl }
	, m_unCustomerID{ unID }
{

}

CClosedWorkorderCustomerDetailsTab::~CClosedWorkorderCustomerDetailsTab()
{
}

void CClosedWorkorderCustomerDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClosedWorkorderCustomerDetailsTab, CDialogEx)
END_MESSAGE_MAP()


// CClosedWorkorderCustomerDetailsTab message handlers
