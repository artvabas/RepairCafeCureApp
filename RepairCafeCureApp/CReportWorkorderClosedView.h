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
* File: CReportTaxView.h, defines class CReportWorkorderClosedView
*
* This class is the view of the closed workorders report
* With this view, the user can print a list with closed orders and
* jump into the close workorder to see the details with a option to print them.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.1 (alpha)
* Created: 02-06-2023, (dd-mm-yyyy)
* Updated: 11-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#pragma once

class CReportWorkorderClosedView : public CFormView
{
	DECLARE_DYNCREATE(CReportWorkorderClosedView)
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPORT_WORKOREER_CLOSED_FORM };
#endif

private:
	PrinterOrientation m_ePrinterOrientation;
	CListCtrl m_lstWorkorderClosedReport;

public:
	CReportWorkorderClosedView();           // protected constructor used by dynamic creation
	virtual ~CReportWorkorderClosedView();

private:
	void DoDataExchange(CDataExchange* pDX) override;
	void OnInitialUpdate() override;
	BOOL PreTranslateMessage(MSG* pMsg) override;
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMDoubleClickWorkorderClosedReport(NMHDR* pNMHDR, LRESULT* pResult) noexcept;

public:
	bool SetPrinterOrientation(HANDLE h, CDC* dc = NULL) const  noexcept;
};