// CAssetDialog.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CAssetDialog.h"



// CAssetDialog dialog

IMPLEMENT_DYNAMIC(CAssetDialog, CDialogEx)

CAssetDialog::CAssetDialog(CString strCustomerSurname, CString strCustomerName, unsigned int nCustomerID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSET_DIALOG, pParent)
	, m_ctrTabAssetWorkorder(strCustomerSurname, strCustomerName, nCustomerID)
{
	

}

CAssetDialog::~CAssetDialog()
{
}

void CAssetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASSET_WORKORDER_TAB, m_ctrTabAssetWorkorder);
}

BOOL CAssetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrTabAssetWorkorder.Init();

	return TRUE;
}



BEGIN_MESSAGE_MAP(CAssetDialog, CDialogEx)
END_MESSAGE_MAP()


// CAssetDialog message handlers
