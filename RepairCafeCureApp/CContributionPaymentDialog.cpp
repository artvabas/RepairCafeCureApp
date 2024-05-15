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
* Version: 1.0.0.1 (alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 15-05-2024, (dd-mm-yyyy)
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
	, m_bIsZeroInvoice(false)
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
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_PIN, &CContributionPaymentDialog::OnClickedContributionPaymentPin)
	ON_EN_CHANGE(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE, &CContributionPaymentDialog::OnEnChangeContributionPayment)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CLEAR, &CContributionPaymentDialog::OnBnClickedContributionPaymentClear)
	ON_BN_CLICKED(IDOK, &CContributionPaymentDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CONTRIBUTION_PAYMENT_CALCULATE, &CContributionPaymentDialog::OnBnClickedContributionPaymentCalculate)
END_MESSAGE_MAP()

/* Overrides methods */

// PreTranslateMessage: Is used to handle the keyboard messages before they are dispatched to the view.
// Depending on the message (which control trigger) , the method will call the appropriate event handler.
// - pMsg: A pointer to a MSG structure that contains the keyboard message.
BOOL CContributionPaymentDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			if (GetFocus() == GetDlgItem(IDC_CONTRIBUTION_PAYMENT_PAID) || GetFocus() == GetDlgItem(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE)) {
				if (!m_bIsZeroInvoice) {
					if (m_btnContributionPaymentOK.IsWindowEnabled())
						OnBnClickedOk();
					else
						OnBnClickedContributionPaymentCalculate();
				} else {
					if ( m_btnContributionPaymentOK.IsWindowEnabled() )
						OnBnClickedOk();
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

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
	DDX_Control(pDX, IDC_CONTRIBUTION_PAYMENT_CALCULATE, m_btnContributionPaymentCalculate);
	DDX_Control(pDX, IDC_CONTRIBUTION_PAYMENT_CLEAR, m_btnContributionPaymentClear);
	DDX_Control(pDX, IDC_CONTRIBUTION_PAYMENT_PAID, m_ctrContributionPaymentPaid);
	DDX_Control(pDX, IDC_CONTRIBUTION_PAYMENT_PIN, m_ctrContributionPaymentPin);
}

// Initialize the dialog
BOOL CContributionPaymentDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(TRUE);
	// set focus the the right control at startup
	if (_tstof(m_strContributionPaymentInvoice)) {
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_PAID);
		pEdit->SetFocus();
		m_bIsZeroInvoice = false;
		return FALSE;
	} else {
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_CONTRIBUTE);
		pEdit->SetFocus();
		m_ctrContributionPaymentPaid.EnableWindow(FALSE);
		m_bIsZeroInvoice = true;
		return FALSE;
	}
	return TRUE;  
}

/* Message handlers methods */

// OnEnChangeContributionPayment is called when the user changes the value of the paid or contribution edit control
// Depending on the state of the invoice (zero, pin) and values of the paid and contribution edit controls
// the method will enable or disable the calculate and clear buttons and set the focus to the right button.
// Focus in this case is the color of the button text (red).
void CContributionPaymentDialog::OnEnChangeContributionPayment() noexcept
{
	UpdateData(TRUE);
	if ( m_bIsZeroInvoice ) {
		if (m_strContributionPaymentContribution.IsEmpty()) {
			m_btnContributionPaymentOK.EnableWindow(FALSE);
			SetCustomFocusButton(&m_btnContributionPaymentOK, BLACK, false);
		} else {
			m_btnContributionPaymentOK.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnContributionPaymentOK, RED, false);
		}
	} else {
		m_btnContributionPaymentOK.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnContributionPaymentOK, BLACK, false);

		if (m_strContributionPaymentPaid.IsEmpty()) {
			m_btnContributionPaymentCalculate.EnableWindow(FALSE);
			SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLACK, false);

			if (m_blPinTransaction && !m_strContributionPaymentContribution.IsEmpty()) {
				m_btnContributionPaymentClear.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnContributionPaymentClear, BLUE, false);
				m_btnContributionPaymentCalculate.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnContributionPaymentCalculate, RED, false);
				m_strContributionPaymentPaid = m_strContributionPaymentInvoice;
				UpdateData(FALSE);
			}
		} else {
			m_btnContributionPaymentCalculate.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnContributionPaymentCalculate, RED, false);
			m_btnContributionPaymentClear.EnableWindow(TRUE);
			SetCustomFocusButton(&m_btnContributionPaymentClear, BLUE, false);
		}
	}
}

// OnClickedContributionPaymentPin is called when the user clicks the pin checkbox
// It clears the paid, contribution and return edit controls and sets the focus to the paid edit control.
// Through DoDataExchange method the bool m_blPinTransactio is toggeled between true and false.
void CContributionPaymentDialog::OnClickedContributionPaymentPin()
{
	UpdateData(TRUE); // Needed to toggle between true and false for member variable m_blPinTransaction
	m_strContributionPaymentPaid = _T("");
	m_strContributionPaymentContribution = _T("");
	m_strContributionPaymentReturn = _T("");

	m_btnContributionPaymentOK.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnContributionPaymentOK, BLACK, false);
	m_btnContributionPaymentCalculate.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLACK, false);
	m_btnContributionPaymentClear.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnContributionPaymentClear, BLACK, false);
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_PAID);
	pEdit->SetFocus();
	UpdateData(FALSE);
}

// OnBnClickedContributionPaymentClear is called when the user clicks the clear button
// It clears the paid, contribution and return edit controls and sets the focus to the paid edit control
void CContributionPaymentDialog::OnBnClickedContributionPaymentClear() noexcept
{
	m_strContributionPaymentPaid.Empty();
	m_strContributionPaymentContribution.Empty();
	m_strContributionPaymentReturn.Empty();

	m_btnContributionPaymentCalculate.EnableWindow(FALSE);
	m_btnContributionPaymentClear.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLACK, false);
	SetCustomFocusButton(&m_btnContributionPaymentClear, BLACK, false);

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

// SetOKButtonState enables or disables the OK button depending on the state of the paid edit control
void CContributionPaymentDialog::SetOKButtonState() noexcept
{
	if (m_strContributionPaymentPaid.IsEmpty()) {
		m_btnContributionPaymentOK.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnContributionPaymentOK, BLACK, false);
		if ( m_btnContributionPaymentCalculate.IsWindowEnabled() )
			SetCustomFocusButton(&m_btnContributionPaymentCalculate, RED, false);
		else
			SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLACK, false);
	} else {
		m_btnContributionPaymentOK.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnContributionPaymentOK, RED, false);
		if ( m_btnContributionPaymentCalculate.IsWindowEnabled() )
			SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLUE, false);
		else
			SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLACK, false);
	}
}

// Calculate calculates the return amount and enables the OK button if the amount paid is sufficient.
// If pin transaction is selected, the return amount is not calculated.
// - dPaid: the amount paid
// - dInvoice: the total amount of the invoice
// - dContribution: the amount of the contribution
void CContributionPaymentDialog::Calculate(const double& dPaid, const double& dInvoice, const double& dContribution) noexcept
{
	if ( m_blPinTransaction ) {
		if  (dPaid == dInvoice ) SetOKButtonState();
		else if ( dPaid == 0.00 && dContribution > 0.00 ) {
			m_strContributionPaymentPaid.Format(_T("%.2f"), dInvoice);
			m_strContributionPaymentReturn = _T("");
			SetOKButtonState();
		}
		else if ( dContribution >= dPaid ) {
			AfxMessageBox(_T("Payment can't be done, wrong input!"));
		}
	}
	
	if ( dPaid >= dInvoice || (m_blPinTransaction && (( dPaid > dInvoice ) || ( dContribution >= dPaid) )) ) {
		if (dContribution > 0.0) {
			auto dReturn{ dPaid - (dInvoice + dContribution) };
			if (!m_blPinTransaction)
				m_strContributionPaymentReturn.Format(_T("%.2f"), dReturn);
			else
				m_strContributionPaymentReturn = _T("");
			m_strContributionPaymentContribution.Format(_T("%.2f"), dContribution);
			if (dReturn < 0.0 && !m_blPinTransaction) {

				AfxMessageBox(_T("The contribution is to high!"));
				m_btnContributionPaymentOK.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnContributionPaymentOK, BLACK, false);

			}
			else {
				SetOKButtonState();
			}
		}
		else {
			m_strContributionPaymentContribution.Format(_T("%.2f"), dPaid - dInvoice);
			if (!m_blPinTransaction)
				m_strContributionPaymentReturn = _T("0.00");
			else
				m_strContributionPaymentReturn = _T("");
			SetOKButtonState();
		}
	}
	else {
		m_strContributionPaymentReturn.Empty();
		AfxMessageBox(_T("The amount paid is not enough to pay the invoice!"));
		m_btnContributionPaymentOK.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnContributionPaymentOK, BLACK, false);
	}
	
	m_strContributionPaymentPaid.Format(_T("%.2f"), dPaid);

	m_btnContributionPaymentCalculate.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnContributionPaymentCalculate, BLACK, false);
	
	UpdateData(FALSE);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CONTRIBUTION_PAYMENT_PAID);
	pEdit->SetFocus();
}

// SetCustomFocusButton sets the focus to the button with the given color
void CContributionPaymentDialog::SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus) noexcept
{
	auto color = RGB(255, 0, 0);
	switch (Color) {
	case RED:
		color = RGB(255, 0, 0);
		break;
	case BLUE:
		color = RGB(0, 0, 255);
		break;
	case BLACK:
		color = RGB(0, 0, 0);
		break;
	}
	pButton->SetTextColor(color);
	pButton->RedrawWindow();
	if (bFocus) pButton->SetFocus();
}