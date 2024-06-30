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
* File: CClosedWorkorderInvoiceDetailsTab.h, defines class CClosedWorkorderInvoiceDetailsTab
*
* This class is the view of the Invoice-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the invoice-details of a closed workorder.
* What is selected in the listview of the CReportWorkorderClosedView and passed to this form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.3 (alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 30-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once

namespace artvabas::rcc::ui::dialogs {
	class CClosedWorkorderInvoiceDetailsTab : public CDialogEx
	{
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CLOSED_WORKORDER_INVOICE_DETAILS_TAB };
#endif
		DECLARE_DYNAMIC(CClosedWorkorderInvoiceDetailsTab)

	private:
		unsigned int m_unInvoiceID;
		BOOL m_bInvoicePinTransaction;

		CString m_strInvoiceID;
		CString m_strInvoiceCustomerID;
		CString m_strInvoiceWorkorderID;
		CString m_strInvoiceAssetID;
		CString m_strInvoiceCreateDate;
		CString m_strInvoiceAmount;

		CListCtrl m_lstInvoiceSummary;

	public:
		CClosedWorkorderInvoiceDetailsTab(unsigned int& unID, CWnd* pParent = nullptr);
		virtual ~CClosedWorkorderInvoiceDetailsTab();

	private:
		BOOL OnInitDialog() override;
		void DoDataExchange(CDataExchange* pDX) override;

		DECLARE_MESSAGE_MAP()
	};
}