// CAssetTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CAssetTab.h"


// CAssetTab dialog

IMPLEMENT_DYNAMIC(CAssetTab, CDialogEx)

CAssetTab::CAssetTab(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSET_TAB, pParent)
	, m_strCustomerSurname(strCustomerSurname)
	, m_strCustomerName(strCustomerName)
	, m_nCustomerID(nCustomerID)
	, m_strDescription(_T(""))
	, m_strModelNumber(_T(""))
	, m_strBrand(_T(""))
	, m_strHistoryLog(_T(""))
	, m_bIsDirtyAssetDetails(false)
	, m_bIsSelectedFromAssetList(false)
{

}

CAssetTab::~CAssetTab()
{
}

void CAssetTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_SURENAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_ASSETTAB_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_ASSTETTAB_MODEL_NUMBER, m_strModelNumber);
	DDX_Text(pDX, IDC_ASSETTAB_BRAND, m_strBrand);
	DDX_Text(pDX, IDC_ASSETTAB_HISTORY_LOG, m_strHistoryLog);
	DDX_Control(pDX, IDC_ASSETTAB_UPDATE, m_btnUpdateAsset);
	DDX_Control(pDX, IDC_ASSETTAB_NEW, m_btnNewAsset);
	DDX_Control(pDX, IDC_ASSETTAB_CREATE_WORKORDER, m_btnCreateWorkorder);
	DDX_Control(pDX, IDC_ASSETTAB_ASSET_LIST, m_ctrExistingAssetList);
}


BEGIN_MESSAGE_MAP(CAssetTab, CDialogEx)
	ON_EN_CHANGE(IDC_ASSETTAB_DESCRIPTION, &CAssetTab::OnEnChangeAssetDetails)
	ON_NOTIFY(NM_DBLCLK, IDC_ASSETTAB_ASSET_LIST, &CAssetTab::OnDoubleClickAssetTabAssetList)
	ON_BN_CLICKED(IDC_ASSETTAB_UPDATE, &CAssetTab::OnBnClickedAssetTabUpdate)
	ON_BN_CLICKED(IDC_ASSETTAB_NEW, &CAssetTab::OnBnClickedAssetTabNew)
END_MESSAGE_MAP()


// CAssetTab message handlers


BOOL CAssetTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrExistingAssetList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctrExistingAssetList.InsertColumn(0, _T("Description"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(1, _T("Model Number"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(2, _T("Brand"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(3, _T("Creation Date"), LVCFMT_LEFT, 50);
	m_ctrExistingAssetList.InsertColumn(4, _T("History Log"), LVCFMT_LEFT, 0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssetTab::OnEnChangeAssetDetails()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	
	m_bIsDirtyAssetDetails =true;
}


void CAssetTab::OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	//  pNMItemActivate->iItem = -1 means no existing item is selected.
	if (pNMItemActivate->iItem != -1)
	{
		// Get the selected item's text.
		m_strDescription = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 0);
		m_strDescription = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 1);
		m_strBrand = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 2);
		m_strHistoryLog = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 4);

		//m_btnCreateWorkorder.EnableWindow(TRUE);
		m_bIsSelectedFromAssetList = true;

		// Update the data in the dialog.
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CAssetTab::OnBnClickedAssetTabUpdate()
{
	// TODO: Add your control notification handler code here
}


void CAssetTab::OnBnClickedAssetTabNew()
{
	
}
