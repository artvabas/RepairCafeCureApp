/*
	Copyright (C) 2023  artvabas

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
* File: CTabCtrlAssetWorkorder.h, defines the CTabCtrlAssetWorkorder class
* This class is the CTabCtrl derived class what interact with the tab-control in the CClosedWorkorderDetails dialog class
*
* This class is a tab control with two tabs.
* The first tab is the asset details tab, the second tab is the workorder tab.
* The asset details tab contains the asset details form, the workorder tab contains the workorder form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.2.5 (beta)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 15-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CClosedWorkorderDetailsTabCtrl.h"
#include "CClosedWorkOrderDetailsTab.h"
#include "CClosedWorkorderAssetDetailsTab.h"
#include "CClosedWorkorderInvoiceDetailsTab.h"
#include "CClosedWorkorderCustomerDetailsTab.h"

using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::rcc::ui::controls;

IMPLEMENT_DYNAMIC(CClosedWorkorderDetailsTabCtrl, CTabCtrl)

CClosedWorkorderDetailsTabCtrl::CClosedWorkorderDetailsTabCtrl(unsigned int& unCustomerID,
	unsigned int& unAssetID, unsigned int& unWorkorderID, unsigned int& unInvoiceID)
	: m_nNumberOfTabs{ 4 }
	, m_tabCurrent{ 0 }
{
	try {
		m_tabPages[0] = new CClosedWorkOrderDetailsTab(unWorkorderID);
		m_tabPages[1] = new CClosedWorkorderAssetDetailsTab(unAssetID);
		m_tabPages[2] = new CClosedWorkorderInvoiceDetailsTab(unInvoiceID);
		m_tabPages[3] = new CClosedWorkorderCustomerDetailsTab(unCustomerID);
	}
	catch (std::bad_alloc& e) {
		AfxMessageBox(static_cast<CString>(e.what()));
		for (auto nCount = 0; nCount < m_nNumberOfTabs; nCount++) {
			if (m_tabPages[nCount] != nullptr) {
				delete m_tabPages[nCount];
				m_tabPages[nCount] = nullptr;
			}
		}
		exit(1);
	}
}

CClosedWorkorderDetailsTabCtrl::~CClosedWorkorderDetailsTabCtrl()
{
	for (auto nCount = 0; nCount < m_nNumberOfTabs; nCount++) {
		if (m_tabPages[nCount] != nullptr ) 
			delete m_tabPages[nCount];
	}
}

BEGIN_MESSAGE_MAP(CClosedWorkorderDetailsTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CClosedWorkorderDetailsTabCtrl::OnTcnSelChange)
END_MESSAGE_MAP()

// OnTcnSelChange method is used for handling the tab control selection change
// and is triggerd by the framework when user click on a tab
void CClosedWorkorderDetailsTabCtrl::OnTcnSelChange(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	ChangeTabView(GetCurSel());
	*pResult = 0;
}

// Init method is used for initializing the tab control
void CClosedWorkorderDetailsTabCtrl::Init() noexcept
{
	m_tabPages[0]->Create(IDD_CLOSED_WORKORDER_DETAILS_TAB, this);
	m_tabPages[1]->Create(IDD_CLOSED_WORKORDER_ASSET_DETAILS_TAB, this);
	m_tabPages[2]->Create(IDD_CLOSED_WORKORDER_INVOICE_DETAILS_TAB, this);
	m_tabPages[3]->Create(IDD_CLOSED_WORKORDER_CUSTOMER_DETAILS_TAB, this);

	InsertItem(0, theApp.ConvertIDToString(IDS_WORKORDER));
	InsertItem(1, theApp.ConvertIDToString(IDS_ASSET));
	InsertItem(2, theApp.ConvertIDToString(IDS_INVOICE));
	InsertItem(3, theApp.ConvertIDToString(IDS_CUSTOMER));

	SetRectangle();
}

// ChangeTabView method is used for changing the tab view
void CClosedWorkorderDetailsTabCtrl::ChangeTabView(int nTab) noexcept
{
	m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
	m_tabPages[nTab]->ShowWindow(SW_SHOW);
	m_tabCurrent = nTab;
	SetCurSel(m_tabCurrent);
}

// SetRectangle method is used for getting the size of the tab control
// for deciding the size and postion fo the tab dialogs which will be place in the tab control.
void CClosedWorkorderDetailsTabCtrl::SetRectangle() noexcept
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 1;
	nYc = tabRect.bottom - nY - 1;

	m_tabPages[0]->SetWindowPos(&CWnd::wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_tabPages[1]->SetWindowPos(&CWnd::wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_tabPages[2]->SetWindowPos(&CWnd::wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_tabPages[3]->SetWindowPos(&CWnd::wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
}