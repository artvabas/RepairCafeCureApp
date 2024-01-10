// CContributionPaymentDialog.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CContributionPaymentDialog.h"


// CContributionPaymentDialog dialog

IMPLEMENT_DYNAMIC(CContributionPaymentDialog, CDialogEx)

CContributionPaymentDialog::CContributionPaymentDialog(const InvoiceData* pInvoiceData, const ContributionData* pContributionData, CWnd* pParent /*=nullptr*/)
	: m_pInvoiceData((InvoiceData*)pInvoiceData)
	, m_pContributionData((ContributionData*)pContributionData)
	, CDialogEx(IDD_CONTRIBUTION, pParent)
	, m_strContributionPaymentInvoice(_T(""))
	, m_strContributionPaymentPaid(_T(""))
	, m_strContributionPaymentContribution(_T(""))
	, m_strContributionPaymentReturn(_T(""))
	, m_blPinTransaction(FALSE)
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
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_RETURN, m_strContributionPaymentReturn);
	DDX_Check(pDX, IDC_CONTRIBUTION_PAYMENT_PIN, m_blPinTransaction);
}


BEGIN_MESSAGE_MAP(CContributionPaymentDialog, CDialogEx)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_PAID, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CLEAR, &CContributionPaymentDialog::OnBnClickedContributionPaymentClear)
	ON_BN_CLICKED(IDOK, &CContributionPaymentDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CContributionPaymentDialog message handlers


void CContributionPaymentDialog::OnEnChangeContributionPayment()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CContributionPaymentDialog::OnBnClickedContributionPaymentClear()
{
	
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE);
	pEdit->SetFocus();
}


void CContributionPaymentDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CContributionPaymentDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(m_strContributionPaymentInvoice.IsEmpty())
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE);
		pEdit->SetFocus();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
