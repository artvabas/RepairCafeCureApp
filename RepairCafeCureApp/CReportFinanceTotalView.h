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
* File: CReportFinanceTotalView.h, defines class CReportFinanceTotalView
*
* This class is the view for generating a period report with the total finance flow.
* With this view, the user can select a period and generate a report.
* The report is displayed in a list control and can be printed.
*
* Target: Windows 10/11 64bit
* Version: 1.0.1.0 (beta)
* Created: 25-08-2024, (dd-mm-yyyy)
* Updated: 09-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once
namespace artvabas::rcc::ui {
	class CReportFinanceTotalView : public CFormView
	{
		DECLARE_DYNCREATE(CReportFinanceTotalView)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_REPORT_FINANCE_TOTAL_FORM };
#endif

	private:
		COleDateTime m_dtStart;
		COleDateTime m_dtEnd;
		CString m_strTotalInvoiceCash;
		CString m_strTotalInvoicePin;
		CString m_strTotalInvoice;
		CString m_strTotalContributionCash;
		CString m_strTotalContributionPin;
		CString m_strTotalContribution;
		CListCtrl m_lstTotalReportResults;
		CButton m_btnCreateReport;
		PrinterOrientation m_ePrinterOrientation;

	public:
		CReportFinanceTotalView();           // protected constructor used by dynamic creation
		virtual ~CReportFinanceTotalView();

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		void OnInitialUpdate() override;
		BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
		void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;
#ifdef _DEBUG
		void AssertValid() const override;
#ifndef _WIN32_WCE
		void Dump(CDumpContext& dc) const override;
#endif
#endif

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnFilePrintPreview() noexcept;
		afx_msg void OnDtnDateTimeChangeFinanceTotalFlowStartDate(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnBnClickedFinanceTotalFlowCreate() noexcept;

	public:
		bool SetPrinterOrientation(HANDLE h, CDC* dc = NULL) const noexcept;
	};
}