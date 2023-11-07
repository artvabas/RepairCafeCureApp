// CTabCtrlAssetWorkorder.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CTabCtrlAssetWorkorder.h"
#include "CAssetTab.h"
#include "CWorkorderTab.h"

// CTabCtrlAssetWorkorder

IMPLEMENT_DYNAMIC(CTabCtrlAssetWorkorder, CTabCtrl)

CTabCtrlAssetWorkorder::CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID)
	: m_nNumberOfTabs(2)
	, m_tabCurrent(0)
{
	m_tabPages[0] = new CAssetTab(this, strCustomerSurname, strCustomerName, nCustomerID);
	m_tabPages[1] = new CWorkorderTab(this);
}

CTabCtrlAssetWorkorder::~CTabCtrlAssetWorkorder()
{
	for (int nCount = 0; nCount < m_nNumberOfTabs; nCount++) {
		delete m_tabPages[nCount];
	}
}

void CTabCtrlAssetWorkorder::Init()
{
	m_tabPages[0]->Create(IDD_ASSET_TAB, this);
	m_tabPages[1]->Create(IDD_WORKORDER_TAB, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	InsertItem(0, _T(" Add or Select Asset"));
	InsertItem(1, _T(" Create Workorder"));

	SetRectangle();
	
}

void CTabCtrlAssetWorkorder::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 1;
	nYc = tabRect.bottom - nY - 1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_tabPages[1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	
}

void CTabCtrlAssetWorkorder::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (m_tabCurrent != GetCurFocus()) {
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent = GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
	}
}


BEGIN_MESSAGE_MAP(CTabCtrlAssetWorkorder, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

