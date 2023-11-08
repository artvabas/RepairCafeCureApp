// CTabCtrlAssetWorkorder.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CTabCtrlAssetWorkorder.h"
#include "CAssetTab.h"
#include "CWorkorderTab.h"

using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CTabCtrlAssetWorkorder, CTabCtrl)

CTabCtrlAssetWorkorder::CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID)
	: m_nNumberOfTabs(2)
	, m_tabCurrent(0)
{
	m_tabPages[0] = new CAssetTab(this, strCustomerSurname, strCustomerName, nCustomerID);
	m_tabPages[1] = new CWorkorderTab(this);

	m_assetDetailsRecords.m_strCustomerSurname = _T("");
	m_assetDetailsRecords.m_strCustomerName = _T("");

	m_assetDetailsRecords.m_nAssetID = 0U;
	m_assetDetailsRecords.m_nAssetCustomerID = 0U;
	m_assetDetailsRecords.m_strDescription = _T("");
	m_assetDetailsRecords.m_strModelNumber = _T("");
	m_assetDetailsRecords.m_strBrand = _T("");

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

void CTabCtrlAssetWorkorder::ChangeTabView()
{
	m_tabCurrent = GetCurFocus();
	if(m_tabCurrent == 0)
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabPages[1]->ShowWindow(SW_SHOW);
		m_tabCurrent = 1;
		SetCurFocus(m_tabCurrent);
	}
	else
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabPages[0]->ShowWindow(SW_SHOW);
		m_tabCurrent = 0;
		SetCurFocus(m_tabCurrent);
	}
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

