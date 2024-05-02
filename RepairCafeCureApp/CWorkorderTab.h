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
* File: CWorkorderTab.h, defines class CWorkorderTab
*
* This class is the view of the workorder tab (IDD_WORKORDER_TAB) of the asset/workorder dialog (CAssetDialog)
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
*
* With this form the user can create a new workorder for the selected asset.
* The user can also view the history of the workorders of the selected asset.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 0.0.1.0 (alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 30-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once
#include "CTabCtrlAssetWorkorder.h"

namespace artvabas::rcc::ui::dialogs {

	using namespace artvabas::rcc::ui::controls;

	class CWorkorderTab : public CDialogEx
	{
		DECLARE_DYNAMIC(CWorkorderTab)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_WORKORDER_TAB };
#endif

	private:
		CTabCtrlAssetWorkorder* m_pTabControl;
		AssetDetailsRecords* m_pAssetDetailsRecords;

		unsigned int m_uiCustomerID;
		unsigned int m_uiAssetID;

		CString m_strCustomerSurname;
		CString m_strCustomerName;
		CString m_strAssetDescription;
		CString m_strAssetModelNumber;
		CString m_strAssetBrand;
		CString m_strWorkorderDescription;
		CString m_strHistoryWorkorderDescription;
		CString m_strHistoryWorkorderLog;

		CMFCButton m_btnWorkorderCreate;
		CListCtrl m_ctrWorkordersHistoryList;

	public:
		CWorkorderTab(CTabCtrlAssetWorkorder* pTabControl, CWnd* pParent = nullptr) noexcept;
		virtual ~CWorkorderTab();

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnInitDialog() override;
		BOOL PreTranslateMessage(MSG* pMsg) override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnBnClickedWoTabCreate();
		afx_msg void OnEnChangeWoTabDescription() noexcept;
		afx_msg void OnNMClickWoTabWorkordersHistoryList(NMHDR* pNMHDR, LRESULT* pResult) noexcept;

	public:
		void InitWithAssetDetailsRecords();

	private:
		void PrintReceiptAndWorkorder();
		void SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus = true) noexcept;
	};
}