/*
	Copyright (C) 2023/24  artvabas

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>

	To see the license for this source code, please visit:
		<https://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit:
		<https://artvabas.com>
		<https://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at:
		<https://artvabas.com/contact>

*/

/*
* This file is part of RepairCafeCureApp.
* File: CContributionPaymentDialog.h, defines class CContributionPaymentDialog
*
* This dialog class displayed the total price of the invoice and give user the possibility
* to enter the amount paid and the amount of the contribution.
* The dialog calculates the return amount and enables the OK button if the amount paid is sufficient.
*
* Target: Windows 10/11 64bit
* Version: 0.0.1.0 (alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 29-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CContributionPaymentDialog.h"

using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CContributionPaymentDialog, CDialogEx)

CContributionPaymentDialog::CContributionPaymentDialog(InvoiceData& invoiceData, ContributionData& contributionData, CWnd* pParent) noexcept
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
{}

/* Message map handles */
BEGIN_MESSAGE_MAP(CContributionPaymentDialog, CDialogEx)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_PAID, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CLEAR, &CContributionPaymentDialog::OnBnClickedContributionPaymentClear)
	ON_BN_CLICKED(IDOK, &CContributionPaymentDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CALCULATE, &CContributionPaymentDialog::OnBnClickedContributionPaymentCalculate)
END_MESSAGE_MAP()

/* Overrides methods */

// For data exchange and validation of the dialog controls variables
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

// Initialize the dialog
BOOL CContributionPaymentDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// set focus the the right control at startup
	if ( m_strContributionPaymentInvoice.IsEmpty() ) {
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE);
		pEdit->SetFocus();
		return FALSE;
	}
	return TRUE;  
}

/* Message handlers methods */

// OnEnChangeContributionPayment is called when the user changes the value of the paid or contribution edit control
// It calculates the return amount and enables the OK button if the amount paid is sufficient.
void CContributionPaymentDialog::OnEnChangeContributionPayment() noexcept
{
	UpdateData(TRUE);

	const auto dInvoice{ _tstof(m_strContributionPaymentInvoice) };
	auto dPaid{ 0.0 };
	auto dContribution{ 0.0 };

	if ( !m_strContributionPaymentPaid.IsEmpty() )
		m_strContributionPaymentPaid.Format(_T("%.2f"), dPaid);

	if ( !m_strContributionPaymentContribution.IsEmpty() )
		m_strContributionPaymentContribution.Format(_T("%.2f"), dContribution);

	Calculate(dPaid, dInvoice, dContribution);
}

// OnBnClickedContributionPaymentClear is called when the user clicks the clear button
// It clears the paid, contribution and return edit controls and sets the focus to the paid edit control
void CContributionPaymentDialog::OnBnClickedContributionPaymentClear() noexcept
{
	m_strContributionPaymentPaid.Empty();
	m_strContributionPaymentContribution.Empty();
	m_strContributionPaymentReturn.Empty();

	SetOKButtonState();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_PAID);
	pEdit->SetFocus();

	UpdateData(FALSE);
}

// OnBnClickedOk is called when the user clicks the OK button
// It updates the contribution data structure with the paid and contribution amounts and the payment method
void CContributionPaymentDialog::OnBnClickedOk() noexcept
{
	UpdateData(TRUE);

	m_stuContributionData->dPaid = _tstof(m_strContributionPaymentPaid);
	m_stuContributionData->dContribution = _tstof(m_strContributionPaymentContribution);
	m_stuContributionData->bPaymentWithPin = m_blPinTransaction ? true : false;

	CDialogEx::OnOK();
}

// OnBnClickedContributionPaymentCalculate is called when the user clicks the calculate button
// It calculates the return amount and enables the OK button if the amount paid is sufficient.
void CContributionPaymentDialog::OnBnClickedContributionPaymentCalculate() noexcept
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

/* Member methods */

// Calculate calculates the return amount and enables the OK button if the amount paid is sufficient.
// - dPaid: the amount paid
// - dInvoice: the total amount of the invoice
// - dContribution: the amount of the contribution
void CContributionPaymentDialog::Calculate(const double& dPaid, const double& dInvoice, const double& dContribution) noexcept
{
	if ( dPaid >= dInvoice ) {
		if ( dContribution > 0.0 ) {
			auto dReturn{ dPaid - (dInvoice + dContribution) };
			m_strContributionPaymentReturn.Format(_T("%.2f"), dReturn);

			if (dReturn < 0.0) {

				AfxMessageBox(_T("The amount paid is not enough to pay the invoice!"));
				m_btnContributionPaymentOK.EnableWindow(FALSE);
			} else {
				SetOKButtonState();
			}
		} else {
			m_strContributionPaymentContribution.Format(_T("%.2f"), dPaid - dInvoice);
			m_strContributionPaymentReturn = _T("0.00");
			SetOKButtonState();
		}
	} else {
		m_strContributionPaymentReturn.Empty();
	}
	UpdateData(FALSE);
}
