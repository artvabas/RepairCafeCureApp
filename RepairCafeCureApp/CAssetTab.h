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
* File: CAssetTab.h, defines class CAssetTab
*
* This class is the view of the asset-tab in the CTabCtrlAssetWorkorder class,
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
*
* With this form the user can search for existing assets, add new assets, update existing assets.
* After selecting an asset, the user can create a workorder for the selected asset.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.465.0
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 03-03-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once

#include "CTabCtrlAssetWorkorder.h"

namespace artvabas::rcc::ui::dialogs {

	using namespace artvabas::rcc::ui::controls;

	class CAssetTab : public CDialogEx
	{
		DECLARE_DYNAMIC(CAssetTab)

	private:
		bool m_bIsSelectedFromAssetList;
		CTabCtrlAssetWorkorder* m_pTabControl;
		AssetDetailsRecords* m_pAssetDetailsRecords;

		CString m_strCustomerSurname;
		CString m_strCustomerName;

		unsigned int m_nAssetID;
		unsigned int m_nAssetCustomerID;
		unsigned int m_nAssetWorkorderID;
		unsigned short m_sAssetDisposed;

		CString m_strAssetCreateDate;
		CString m_strDescription;
		CString m_strModelNumber;
		CString m_strBrand;
		CString m_strHistoryLog;

		CMFCButton m_btnUpdateAsset;
		CMFCButton m_btnNewAsset;
		CMFCButton m_btnCreateWorkorder;
		CMFCButton m_btnClear;
		CListCtrl m_ctrExistingAssetList;

	public:
		CAssetTab(CTabCtrlAssetWorkorder* pTabControl, CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID,
			CWnd* pParent = nullptr);
		virtual ~CAssetTab();

		void ClearForNewInput();

	private:
		void SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus = true);
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ASSET_TAB };
#endif

	private:
		BOOL OnInitDialog() override;
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL PreTranslateMessage(MSG* pMsg) override;

		DECLARE_MESSAGE_MAP()
	private:
		afx_msg void OnEnChangeAssetDetails();
		afx_msg void OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnBnClickedAssetTabUpdate();
		afx_msg void OnBnClickedAssetTabNew();
		afx_msg void OnBnClickedAssetTabClear();
	public:
		afx_msg void OnBnClickedAssetTabCreateWorkorder();
	};
}