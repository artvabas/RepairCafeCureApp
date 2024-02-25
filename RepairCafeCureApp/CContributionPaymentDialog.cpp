// CContributionPaymentDialog.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CContributionPaymentDialog.h"


using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CContributionPaymentDialog, CDialogEx)

CContributionPaymentDialog::CContributionPaymentDialog(InvoiceData& invoiceData, ContributionData& contributionData, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTRIBUTION, pParent)
	, m_strContributionPaymentInvoice(invoiceData.strTotal)
	, m_strContributionPaymentPaid(_T(""))
	, m_strContributionPaymentContribution(_T(""))
	, m_strContributionPaymentReturn(_T(""))
	, m_blPinTransaction(FALSE)
{
	m_stuInvoiceData = &invoiceData;
	m_stuContributionData = &contributionData;
}

CContributionPaymentDialog::~CContributionPaymentDialog()
{
}

void CContributionPaymentDialog::Calculate(const double& dPaid, const double& dInvoice, const double& dContribution)
{
	if (dPaid >= dInvoice)
	{
		if (dContribution > 0.0)
		{
			auto dReturn{ dPaid - (dInvoice + dContribution) };
			m_strContributionPaymentReturn.Format(_T("%.2f"), dReturn);

			if (dReturn < 0.0) {

				AfxMessageBox(_T("Het bedrag dat betaald is, is niet voldoende om de factuur te betalen!."));
				m_btnContributionPaymentOK.EnableWindow(FALSE);
			}
			else {
				SetOKButtonState();
			}
		}
		else
		{
			m_strContributionPaymentContribution.Format(_T("%.2f"), dPaid - dInvoice);
			m_strContributionPaymentReturn = _T("0.00");
			SetOKButtonState();
		}
	}
	else
	{
		m_strContributionPaymentReturn.Empty();
	}

	UpdateData(FALSE);
}

void CContributionPaymentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_INVOICE, m_strContributionPaymentInvoice);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_PAID, m_strContributionPaymentPaid);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE, m_strContributionPaymentContribution);
	DDX_Text(pDX, IDC_CONTRIBUTION_PAYMENT_RETURN, m_strContributionPaymentReturn);
	DDX_Check(pDX, IDC_CONTRIBUTION_PAYMENT_PIN, m_blPinTransaction);
	DDX_Control(pDX, IDOK, m_btnContributionPaymentOK);
}


BEGIN_MESSAGE_MAP(CContributionPaymentDialog, CDialogEx)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_PAID, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CLEAR, &CContributionPaymentDialog::OnBnClickedContributionPaymentClear)
	ON_BN_CLICKED(IDOK, &CContributionPaymentDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CALCULATE, &CContributionPaymentDialog::OnBnClickedContributionPaymentCalculate)
END_MESSAGE_MAP()


// CContributionPaymentDialog message handlers


void CContributionPaymentDialog::OnEnChangeContributionPayment()
{
	UpdateData(TRUE);

	const auto dInvoice{ _tstof(m_strContributionPaymentInvoice) };
	auto dPaid{ 0.0 };
	auto dContribution{ 0.0 };

	if(!m_strContributionPaymentPaid.IsEmpty())
		m_strContributionPaymentPaid.Format(_T("%.2f"), dPaid);

	if(!m_strContributionPaymentContribution.IsEmpty())
		m_strContributionPaymentContribution.Format(_T("%.2f"), dContribution);

}		

void CContributionPaymentDialog::OnBnClickedContributionPaymentClear()
{
	m_strContributionPaymentPaid.Empty();
	m_strContributionPaymentContribution.Empty();
	m_strContributionPaymentReturn.Empty();

	SetOKButtonState();

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_PAID);
	pEdit->SetFocus();

	UpdateData(FALSE);
}


void CContributionPaymentDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_stuContributionData->dPaid = _tstof(m_strContributionPaymentPaid);
	m_stuContributionData->dContribution = _tstof(m_strContributionPaymentContribution);
	m_stuContributionData->bPaymentWithPin = m_blPinTransaction ? true : false;

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


void CContributionPaymentDialog::OnBnClickedContributionPaymentCalculate()
{
	UpdateData(TRUE);

	const auto dInvoice{ _tstof(m_strContributionPaymentInvoice) };
	auto dPaid{ 0.0 };
	auto dContribution{ 0.0 };

	if (!m_strContributionPaymentPaid.IsEmpty())
		dPaid = _tstof(m_strContributionPaymentPaid);
		
	if (!m_strContributionPaymentContribution.IsEmpty())
		dContribution = _tstof(m_strContributionPaymentContribution);
	
	Calculate(dPaid, dInvoice, dContribution);
}
