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
#pragma once
#include "afxdialogex.h"

namespace artvabas::rcc::ui::dialogs {

	class CContributionPaymentDialog : public CDialogEx
	{
		DECLARE_DYNAMIC(CContributionPaymentDialog)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CONTRIBUTION };
#endif

	/* Data structures for value transfer between classes */
	public:
		struct InvoiceData {
			unsigned int unCustomerID;
			unsigned int unAssetID;
			unsigned int unWorkOrderID;
			CString strTotal;
		};

		struct ContributionData {
			double dContribution;
			double dPaid;
			bool bPaymentWithPin;
		};

	private:
		CString m_strContributionPaymentInvoice;
		CString m_strContributionPaymentPaid;
		CString m_strContributionPaymentContribution;
		CString m_strContributionPaymentReturn;

		CEdit m_ctrContributionPaymentPaid;
		CButton m_ctrContributionPaymentPin;

		CMFCButton m_btnContributionPaymentOK;
		CMFCButton m_btnContributionPaymentCalculate;
		CMFCButton m_btnContributionPaymentClear;

		InvoiceData* m_stuInvoiceData;
		ContributionData* m_stuContributionData;

		bool m_bIsZeroInvoice;
		BOOL m_blPinTransaction;

	public:
		CContributionPaymentDialog(InvoiceData& pInvoiceData, ContributionData& pContributionData, CWnd* pParent = nullptr) noexcept;
		virtual ~CContributionPaymentDialog();

	private:
		BOOL PreTranslateMessage(MSG* pMsg) override;
		void DoDataExchange(CDataExchange* pDX) override; 
		BOOL OnInitDialog() override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnEnChangeContributionPayment() noexcept;
		afx_msg void OnClickedContributionPaymentPin();
		afx_msg void OnBnClickedContributionPaymentCalculate() noexcept;
		afx_msg void OnBnClickedContributionPaymentClear() noexcept;
		afx_msg void OnBnClickedOk() noexcept;

	private:
		void SetOKButtonState() noexcept;
		void Calculate(const double& dPaid, const double& dInvoice, const double& dContribution) noexcept;
		void SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus = true) noexcept;
	};
}