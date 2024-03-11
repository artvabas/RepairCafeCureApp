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
* Version: 1.0.230.0
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 10-11-2023, (dd-mm-yyyy)
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

CTabCtrlAssetWorkorder::CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID)
	: m_nNumberOfTabs(2)
	, m_tabCurrent(0)
{
	m_tabPages[0] = new CAssetTab(this, strCustomerSurname, strCustomerName, nCustomerID);
	m_tabPages[1] = new CWorkorderTab(this);

	m_assetDetailsRecords.m_strCustomerSurname = _T("");
	m_assetDetailsRecords.m_strCustomerName = _T("");

	m_assetDetailsRecords.m_nAssetID = 0U;
	m_assetDetailsRecords.m_nAssetCustomerID = 0U;
	m_assetDetailsRecords.m_strDescription = _T("");
	m_assetDetailsRecords.m_strModelNumber = _T("");
	m_assetDetailsRecords.m_strBrand = _T("");

}

CTabCtrlAssetWorkorder::~CTabCtrlAssetWorkorder()
{
	for (int nCount = 0; nCount < m_nNumberOfTabs; nCount++) {
		delete m_tabPages[nCount];
	}
}

/* Message handles */
BEGIN_MESSAGE_MAP(CTabCtrlAssetWorkorder, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/// <summary>
/// Handles the WM_LBUTTONDOWN message.
/// When the user clicks on the tab control, the tab control checks if the user clicked on the tab.
/// If the user clicked on the tab, the tab control checks if the user clicked on the asset tab or the workorder tab.
/// If the user clicked on the asset tab, the tab control shows the asset details form.
/// If the user clicked on the workorder tab, the tab control shows the workorder form.
/// If user goes from asset tab to workorder tab, this class manipulate as if the create new workorder button was clicked.
/// and then call the function InitWithAssetDetailsRecords() of the workorder form for the initialization of the workorder form.
/// </summary>
/// <param name="nFlags"></param>
/// <param name="point"></param>
/// <returns></returns>
void CTabCtrlAssetWorkorder::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (m_tabCurrent != GetCurFocus())
	{
		m_tabCurrent = GetCurFocus();
		if (m_tabCurrent == 1)
		{
			// Manipulate as if the create new workorder button was clicked
			CAssetTab* pAssetTab = dynamic_cast<CAssetTab*>(m_tabPages[0]);
			if (pAssetTab != nullptr) pAssetTab->OnBnClickedAssetTabCreateWorkorder();

			m_tabPages[0]->ShowWindow(SW_HIDE);
			m_tabPages[1]->ShowWindow(SW_SHOW);

			// Init workorder tab with asset details records
			CWorkorderTab* pWorkorderTab = dynamic_cast<CWorkorderTab*>(m_tabPages[1]);
			if (pWorkorderTab != nullptr) pWorkorderTab->InitWithAssetDetailsRecords();

			m_tabCurrent = 1;
			SetCurFocus(m_tabCurrent);
		}
		else
		{
			m_tabPages[1]->ShowWindow(SW_HIDE);
			m_tabPages[0]->ShowWindow(SW_SHOW);
			m_tabCurrent = 0;
			SetCurFocus(m_tabCurrent);
		}
	}

}

/// <summary>
/// Initializes the tab control.
/// </summary>
/// <returns></returns>
void CTabCtrlAssetWorkorder::Init()
{
	m_tabPages[0]->Create(IDD_ASSET_TAB, this);
	m_tabPages[1]->Create(IDD_WORKORDER_TAB, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	InsertItem(0, _T("Asset"));
	InsertItem(1, _T("Workorder"));

	SetRectangle();
	
}

/// <summary>
/// User can also click on the create new workorder button on the asset details form.
/// This function is called when the user clicks on the create new workorder button.
/// This function manipulates as if the user clicked on the workorder tab.
/// </summary>
/// /// <param name="bClearForNewInput">is true when asset tab form mist be cleared for new input.</param>
/// <returns></returns>
void CTabCtrlAssetWorkorder::ChangeTabView(bool bClearForNewInput)
{
	m_tabCurrent = GetCurFocus();
	if(m_tabCurrent == 0)
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabPages[1]->ShowWindow(SW_SHOW);
		m_tabCurrent = 1;
		SetCurFocus(m_tabCurrent);

		// Init workorder tab with asset details records
		CWorkorderTab* pWorkorderTab = reinterpret_cast<CWorkorderTab*>(m_tabPages[1]);
		if (pWorkorderTab != nullptr) pWorkorderTab->InitWithAssetDetailsRecords();

	}
	else
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabPages[0]->ShowWindow(SW_SHOW);
		m_tabCurrent = 0;
		SetCurFocus(m_tabCurrent);

		if(bClearForNewInput)
		{
			// Clear asset tab for new input
			CAssetTab* pAssetTab = reinterpret_cast<CAssetTab*>(m_tabPages[0]);
			if (pAssetTab != nullptr) pAssetTab->ClearForNewInput();
		}
	}
}

/// <summary>
/// This function is used for initialization the size of the tab control.
/// It set the size to the size of the tab control.
/// </summary>
/// <returns></returns>
void CTabCtrlAssetWorkorder::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 1;
	nYc = tabRect.bottom - nY - 1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_tabPages[1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	
}
