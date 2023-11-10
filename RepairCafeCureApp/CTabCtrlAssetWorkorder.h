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
#pragma once

namespace artvabas {

	namespace rcc {

		namespace ui {

			namespace controls {

				// Struct for shared data between the two forms
				struct AssetDetailsRecords
				{
					CString m_strCustomerSurname;
					CString m_strCustomerName;

					unsigned int m_nAssetID;
					unsigned int m_nAssetCustomerID;
					CString m_strDescription;
					CString m_strModelNumber;
					CString m_strBrand;
				};

				class CTabCtrlAssetWorkorder : public CTabCtrl
				{
					DECLARE_DYNAMIC(CTabCtrlAssetWorkorder)

				private:
					CDialog* m_tabPages[2];
					int m_tabCurrent;
					int m_nNumberOfTabs;
				public:
					AssetDetailsRecords m_assetDetailsRecords;

				public:
					CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID);
					virtual ~CTabCtrlAssetWorkorder();

					void Init();
					void ChangeTabView(bool bClearForNewInput = false);

				private:
					void SetRectangle();

					DECLARE_MESSAGE_MAP()
				private:
					afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

				};
			}
		}
	}
}


