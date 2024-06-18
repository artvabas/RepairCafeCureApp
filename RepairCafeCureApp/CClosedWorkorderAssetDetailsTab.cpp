// CClosedWorkorderAssetDetailsTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkorderAssetDetailsTab.h"


// CClosedWorkorderAssetDetailsTab dialog

IMPLEMENT_DYNAMIC(CClosedWorkorderAssetDetailsTab, CDialogEx)

CClosedWorkorderAssetDetailsTab::CClosedWorkorderAssetDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSED_WORKORDER_ASSET_DETAILS_TAB, pParent)
	, m_pTabControl{ pTabControl }
	, m_unAssetID{ unID }

{

}

CClosedWorkorderAssetDetailsTab::~CClosedWorkorderAssetDetailsTab()
{
}

void CClosedWorkorderAssetDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClosedWorkorderAssetDetailsTab, CDialogEx)
END_MESSAGE_MAP()


// CClosedWorkorderAssetDetailsTab message handlers
