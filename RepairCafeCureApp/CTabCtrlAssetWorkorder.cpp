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
* File: CTabCtrlAssetWorkorder.cp, implementation of the CTabCtrlAssetWorkorder class
*
* This class is a tab control with two tabs.
* The first tab is the asset details tab, the second tab is the workorder tab.
* The asset details tab contains the asset details form, the workorder tab contains the workorder form.
*
* With this class, the user can switch between the asset details form and the workorder form.
* The asset details form is used to add a new asset to the database or to select an existing asset from the database.
* The workorder form is used to add a new workorder to the database.
* THis class is also responsible for the communication between the two forms.
*
* Target: Windows 10/11 64bit
* Version: 0.0.1.0 (Alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 28-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CTabCtrlAssetWorkorder.h"
#include "CAssetTab.h"
#include "CWorkorderTab.h"

using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CTabCtrlAssetWorkorder, CTabCtrl)

/* Con-deconstructs */

CTabCtrlAssetWorkorder::CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID)
	: m_nNumberOfTabs{ 2 }
	, m_tabCurrent{ 0 }
{
	try {
		m_tabPages[0] = new CAssetTab(this, strCustomerSurname, strCustomerName, nCustomerID);
		m_tabPages[1] = new CWorkorderTab(this);
	}
	catch (std::bad_alloc& e) {
		AfxMessageBox(static_cast<CString>(e.what()));
		if (m_tabPages[0] != nullptr) {
			delete m_tabPages[0];
			m_tabPages[0] = nullptr;
		}
		if (m_tabPages[1] != nullptr) {
			delete m_tabPages[1];
			m_tabPages[1] = nullptr;
		}
		exit(1);
	}

	m_assetDetailsRecords.m_nAssetID = 0U;
	m_assetDetailsRecords.m_nAssetCustomerID = 0U;
	m_assetDetailsRecords.m_strCustomerSurname =_T("");
	m_assetDetailsRecords.m_strCustomerName = _T("");
	m_assetDetailsRecords.m_strDescription = _T("");
	m_assetDetailsRecords.m_strModelNumber = _T("");
	m_assetDetailsRecords.m_strBrand = _T("");
}

CTabCtrlAssetWorkorder::~CTabCtrlAssetWorkorder()
{
	for ( auto nCount = 0; nCount < m_nNumberOfTabs; nCount++)  {
		delete m_tabPages[nCount];
	}
}

/* Message map handles */
BEGIN_MESSAGE_MAP(CTabCtrlAssetWorkorder, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/* Message map methods */

// Handles the WM_LBUTTONDOWN message.
// Activates when user click on one of the tabs.
// Swap between the asset details tab and the workorder tab.
void CTabCtrlAssetWorkorder::OnLButtonDown(UINT nFlags, CPoint point) noexcept
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if  (m_tabCurrent != GetCurFocus() ) {
		m_tabCurrent = GetCurFocus();
		if ( m_tabCurrent == 1 ) {
			// Manipulate as if the create new workorder button was clicked
			CAssetTab* pAssetTab = reinterpret_cast<CAssetTab*>(m_tabPages[0]);
			// Fill datatable structure
			if ( pAssetTab != nullptr ) pAssetTab->OnBnClickedAssetTabCreateWorkorder();

			m_tabPages[0]->ShowWindow(SW_HIDE);
			m_tabPages[1]->ShowWindow(SW_SHOW);

			// Init workorder tab with values from datatable structure
			CWorkorderTab* pWorkorderTab = reinterpret_cast<CWorkorderTab*>(m_tabPages[1]);
			if ( pWorkorderTab != nullptr ) pWorkorderTab->InitWithAssetDetailsRecords();

			m_tabCurrent = 1;
			SetCurFocus(m_tabCurrent);
		} else {
			m_tabPages[1]->ShowWindow(SW_HIDE);
			m_tabPages[0]->ShowWindow(SW_SHOW);
			m_tabCurrent = 0;
			SetCurFocus(m_tabCurrent);
		}
	}
}

/* Methods */

// Initializes the tab control.
// Creates the asset details tab and the workorder tab.
// Is called by the parent dialog, CAssedDialog class during initialization.
void CTabCtrlAssetWorkorder::Init() noexcept
{
	m_tabPages[0]->Create(IDD_ASSET_TAB, this);
	m_tabPages[1]->Create(IDD_WORKORDER_TAB, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	InsertItem(0, _T("Asset"));
	InsertItem(1, _T("Workorder"));

	SetRectangle();
}

// Changes between tab views.
// This function is called when the user clicks on the create new workorder button on one of the tabs.
// 
// Situation 1] Create new workorder button is clicked on the asset details tab. (receive user input for new workorder)	
// - The function hides the asset details tab and shows the workorder tab.
// - The function initializes the workorder tab with the values from the asset details tab.
// 
// Situation 2] Create new workorder button is clicked on the workorder tab. (Add new workorder to the database)
// - The function hides the workorder tab and shows the asset details tab.
// - The function clears the asset details tab for new input.
void CTabCtrlAssetWorkorder::ChangeTabView(bool bClearForNewInput) noexcept
{
	m_tabCurrent = GetCurFocus();
	if (m_tabCurrent == 0 ) {							// Situation 1
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabPages[1]->ShowWindow(SW_SHOW);
		m_tabCurrent = 1;
		SetCurFocus(m_tabCurrent);
		// Init workorder tab with asset details records
		CWorkorderTab* pWorkorderTab = reinterpret_cast<CWorkorderTab*>(m_tabPages[1]);
		if ( pWorkorderTab != nullptr ) pWorkorderTab->InitWithAssetDetailsRecords();
	} else {											// Situation 2			
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabPages[0]->ShowWindow(SW_SHOW);
		m_tabCurrent = 0;
		SetCurFocus(m_tabCurrent);

		if( bClearForNewInput ) {
			// Clear asset tab for new input
			CAssetTab* pAssetTab = reinterpret_cast<CAssetTab*>(m_tabPages[0]);
			if ( pAssetTab != nullptr)  pAssetTab->ClearForNewInput();
		}
	}
}

// This method is used for initialization the size of the tab control.
// It set the size to the size of the tab control. Called by Init() method.
void CTabCtrlAssetWorkorder::SetRectangle() noexcept
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
}
