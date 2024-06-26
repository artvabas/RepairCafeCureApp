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
* Version: 1.0.0.1 (alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 02-06-2024, (dd-mm-yyyy)
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
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ASSET_TAB };
#endif
		DECLARE_DYNAMIC(CAssetTab)

	private:
		bool m_bIsSelectedFromAssetList;
		CTabCtrlAssetWorkorder* m_pTabControl;
		AssetDetailsRecords* m_pAssetDetailsRecords;

		unsigned int m_nAssetID;
		unsigned int m_nAssetCustomerID;
		unsigned short m_sAssetDisposed;

		CString m_strCustomerSurname;
		CString m_strCustomerName;

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
			CWnd* pParent = nullptr) noexcept;
		virtual ~CAssetTab();

	private:
		BOOL OnInitDialog() override;
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL PreTranslateMessage(MSG* pMsg) override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnEnChangeAssetDetails() noexcept;
		afx_msg void OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnBnClickedAssetTabUpdate();
		afx_msg void OnBnClickedAssetTabNew();
		afx_msg void OnBnClickedAssetTabClear() noexcept;
	public:
		afx_msg void OnBnClickedAssetTabCreateWorkorder() noexcept;

	public:
		void ClearForNewInput() noexcept;
	private:
		BOOL LoadAssetDetailsList() noexcept;
		void SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus = true) noexcept;
	};
}