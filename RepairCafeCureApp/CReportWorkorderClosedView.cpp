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
* File: CReportTaxView.h, implements class CReportWorkorderClosedView
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

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CReportWorkorderClosedView.h"


// CReportWorkorderClosedView

IMPLEMENT_DYNCREATE(CReportWorkorderClosedView, CFormView)

CReportWorkorderClosedView::CReportWorkorderClosedView()
	: CFormView(IDD_REPORT_WORKOREER_CLOSED_FORM)
	, m_ePrinterOrientation(PORTRAIT)
{}

CReportWorkorderClosedView::~CReportWorkorderClosedView()
{}

BEGIN_MESSAGE_MAP(CReportWorkorderClosedView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CReportWorkorderClosedView::OnFilePrintPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDER_CLOSED_REPORT, &CReportWorkorderClosedView::OnNMDoubleClickWorkorderClosedReport)
END_MESSAGE_MAP()

/* Overrode methods */

void CReportWorkorderClosedView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORKORDER_CLOSED_REPORT, m_lstWorkorderClosedReport);
}

void CReportWorkorderClosedView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_lstWorkorderClosedReport.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstWorkorderClosedReport.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(1, _T("WORKORDER DESCRIPTION"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(2, _T("EMPLOYEE RESPONSIBLE"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(3, _T("WORKORDER DESCRIPTION"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(4, _T("WORKORDER STATUS"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(5, _T("ASSET ID"), LVCFMT_LEFT, 0);
	m_lstWorkorderClosedReport.InsertColumn(6, _T("ASSET DESCRIPTION"), LVCFMT_LEFT, 150);
	m_lstWorkorderClosedReport.InsertColumn(7, _T("CUSTOMER ID"), LVCFMT_LEFT, 0);
	m_lstWorkorderClosedReport.InsertColumn(8, _T("INVOICE ID"), LVCFMT_LEFT, 0);
}

BOOL CReportWorkorderClosedView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

BOOL CReportWorkorderClosedView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CReportWorkorderClosedView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	SetPrinterOrientation(theApp.GetDeviceMode(), pDC);
	CFormView::OnBeginPrinting(pDC, pInfo);
}

void CReportWorkorderClosedView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CReportWorkorderClosedView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// Code here
	CFormView::OnPrint(pDC, pInfo);
}

#ifdef _DEBUG
void CReportWorkorderClosedView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CReportWorkorderClosedView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/* Message map methods */

void CReportWorkorderClosedView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

void CReportWorkorderClosedView::OnNMDoubleClickWorkorderClosedReport(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
/* Member methods */

bool CReportWorkorderClosedView::SetPrinterOrientation(HANDLE h, CDC* dc) const noexcept
{
	DEVMODE* devMode;
	if (!h) return false;
	devMode = (DEVMODE*)::GlobalLock(h);    if (!devMode) return false;

	switch (m_ePrinterOrientation) {
	case PORTRAIT:
		devMode->dmOrientation = DMORIENT_PORTRAIT;  // portrait mode
		devMode->dmFields |= DM_ORIENTATION;
		break;

	case LANDSCAPE:
		devMode->dmOrientation = DMORIENT_LANDSCAPE; // landscape mode
		devMode->dmFields |= DM_ORIENTATION;
		break;
	}

	if (dc) dc->ResetDC(devMode);
	::GlobalUnlock(h);
	return true;
}