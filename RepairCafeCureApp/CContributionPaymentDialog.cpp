// CContributionPaymentDialog.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CContributionPaymentDialog.h"


// CContributionPaymentDialog dialog

IMPLEMENT_DYNAMIC(CContributionPaymentDialog, CDialogEx)

CContributionPaymentDialog::CContributionPaymentDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTRIBUTION, pParent)
	, m_strContributionPaymentInvoice(_T(""))
	, m_strContributionPaymentPaid(_T(""))
	, m_strContributionPaymentContribution(_T(""))
{

}

CContributionPaymentDialog::~CContributionPaymentDialog()
{
}

void CContributionPaymentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_INVOICE, m_strContributionPaymentInvoice);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_PAID, m_strContributionPaymentPaid);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE, m_strContributionPaymentContribution);
}


BEGIN_MESSAGE_MAP(CContributionPaymentDialog, CDialogEx)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_PAID, &CContributionPaymentDialog::OnEnChangeContributionPaymentPayed)
END_MESSAGE_MAP()


// CContributionPaymentDialog message handlers


void CContributionPaymentDialog::OnEnChangeContributionPaymentPayed()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
