// CWorkorderTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CWorkorderTab.h"

// CWorkorderTab dialog

IMPLEMENT_DYNAMIC(CWorkorderTab, CDialogEx)

CWorkorderTab::CWorkorderTab(CTabCtrlAssetWorkorder* pTabControl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORKORDER_TAB, pParent)
	, m_pTabControl(pTabControl)
{
	m_pAssetDetailsRecords = &(m_pTabControl->m_assetDetailsRecords);
}

CWorkorderTab::~CWorkorderTab()
{
}

void CWorkorderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWorkorderTab, CDialogEx)
END_MESSAGE_MAP()


// CWorkorderTab message handlers
