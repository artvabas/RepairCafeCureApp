// CAssetTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CAssetTab.h"


// CAssetTab dialog

IMPLEMENT_DYNAMIC(CAssetTab, CDialogEx)

CAssetTab::CAssetTab(CString strCustomerSurname, CString strCustomerName, unsigned int nCustomerID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSET_TAB, pParent)
	, m_strCustomerSurename(strCustomerSurname)
	, m_strCustomerName(strCustomerName)
	, m_nCustomerID(nCustomerID)
{

}

CAssetTab::~CAssetTab()
{
}

void CAssetTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_SURENAME, m_strCustomerSurename);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_NAME, m_strCustomerName);
}


BEGIN_MESSAGE_MAP(CAssetTab, CDialogEx)
END_MESSAGE_MAP()


// CAssetTab message handlers
