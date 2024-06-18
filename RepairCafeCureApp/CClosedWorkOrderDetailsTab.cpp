// CClosedWordorderDetailsTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkOrderDetailsTab.h"

IMPLEMENT_DYNAMIC(CClosedWorkOrderDetailsTab, CDialogEx)

CClosedWorkOrderDetailsTab::CClosedWorkOrderDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent)
	: CDialogEx(IDD_CLOSED_WORKORDER_DETAILS_TAB, pParent)
	, m_pTabControl{ pTabControl }
	, m_unWorkorderID{ unID }
{

}

CClosedWorkOrderDetailsTab::~CClosedWorkOrderDetailsTab()
{
}

void CClosedWorkOrderDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClosedWorkOrderDetailsTab, CDialogEx)
END_MESSAGE_MAP()


