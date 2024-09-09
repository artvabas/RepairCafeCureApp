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
* File: CCustomerView.h, defines class CCustomerView
* 
* This class is the view of the customer form.
* With this form the user can search for existing customers, add new customers, update existing customers.
* And when all the required fields are filled in, an asset can be added to the customer.
* 
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5 (alpha)
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 14-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#pragma once
namespace artvabas::rcc::ui {

	class CCustomerView : public CFormView
	{
		DECLARE_DYNCREATE(CCustomerView)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CUSTOMER_FORM };
#endif
	private:
		CListCtrl m_ctlExistingCustomersList;

		CMFCButton m_btnCustomerSurnameSearch;
		CMFCButton m_btnAddNewCustomer;
		CMFCButton m_btnAddCustomer;
		CMFCButton m_btnCustomAssets;
		CMFCButton m_btnUpdateCustomer;

		CEdit m_ctrCustomerCellPhone;
		CEdit m_ctrCustomerComment;
		CEdit m_ctrCustomerLog;
		CEdit m_ctrCustomerName;
		CEdit m_ctrCustomerPhone;
		CEdit m_ctrCustomerSurname;
		CEdit m_ctrlCustomerEmail;

		CString m_strSearchCustomerSurname;
		CString m_strCustomerCellPhone;
		CString m_strCustomerComment;
		CString m_strCustomerLog;
		CString m_strCustomerName;
		CString m_strCustomerPhone;
		CString m_strCustomerSurname;
		CString m_strCustomerEmail;
		CString m_strCustomerPartialInvoice;

		unsigned m_nCustomerID;

		bool m_bIsNewCustomer;
		bool m_bIsDirtyCustomerDetails;

	public:
		CCustomerView() noexcept;
		virtual ~CCustomerView();

	private:
		BOOL PreTranslateMessage(MSG* pMsg) override;
		void OnInitialUpdate() override;
		void DoDataExchange(CDataExchange* pDX) override;
#ifdef _DEBUG
		void AssertValid() const override;
#ifndef _WIN32_WCE
		void Dump(CDumpContext& dc) const override;
#endif
#endif
	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnClickedCustomViewButtonSearch();
		afx_msg void OnChangeCustomViewEditBoxSurnameSearch() noexcept;
		afx_msg void OnUpdateUIState(UINT nAction, UINT nUIElement) noexcept;
		afx_msg void OnDoubleClickCustViewCustomerList(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnChangeCustViewCustomerDetails() noexcept;
		afx_msg void OnClickedCustViewButtonAddNewCustomer() noexcept;
		afx_msg void OnClickedCustViewButtonCustomerAdd();
		afx_msg void OnClickedCustViewButtonCustomerUpdate();
		afx_msg void OnClickedCustViewButtonCustomerAssets() noexcept;

	private:
		void DisableCustomerSearchAndAddButtons() noexcept;
		void UpdateCustomerDetailsControls(BOOL bShow = TRUE) noexcept;
		void DisableCustomerDetailsButtons() noexcept;
		void EmptyCustomerDetailsControls() noexcept;
		void EmptyAndDisableExistingCustomersList() noexcept;
		void SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus = true) noexcept;
		void BuildLogMessage(const CString& strMessage) noexcept;
	};
}