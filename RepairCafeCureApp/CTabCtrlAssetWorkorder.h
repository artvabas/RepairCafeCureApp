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
* This class is the CTabCtrl derived class what interact with the tab-control in the CAssetDialog class
*
* This class is a tab control with two tabs.
* The first tab is the asset details tab, the second tab is the workorder tab.
* The asset details tab contains the asset details form, the workorder tab contains the workorder form.
*
* With this class, the user can switch between the asset details form and the workorder form.
* The asset details form is used to add a new asset to the database or to select an existing asset from the database.
* The workorder form is used to add a new workorder to the database.
* This class is also responsible for the communication between the two forms/tabs.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.1 (Alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 28-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once

namespace artvabas::rcc::ui::controls {

	// Struct for shared data between the two tabs (Asset Details and Workorder)
	struct AssetDetailsRecords
	{
		unsigned int m_nAssetID{ 0 };
		unsigned int m_nAssetCustomerID{ 0 };

		CString m_strCustomerSurname;
		CString m_strCustomerName;
		CString m_strDescription;
		CString m_strModelNumber;
		CString m_strBrand;
	};

	class CTabCtrlAssetWorkorder : public CTabCtrl
	{
		DECLARE_DYNAMIC(CTabCtrlAssetWorkorder)

		public:
			AssetDetailsRecords m_assetDetailsRecords;
		private:
			CDialog* m_tabPages[2];
			int m_tabCurrent;
			int m_nNumberOfTabs;

		public:
			CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID);
			virtual ~CTabCtrlAssetWorkorder();

		private:
			DECLARE_MESSAGE_MAP()
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point) noexcept;

		public:
			void Init() noexcept;
			void ChangeTabView(bool bClearForNewInput = false) noexcept;
		private:
			void SetRectangle() noexcept;
		};
}