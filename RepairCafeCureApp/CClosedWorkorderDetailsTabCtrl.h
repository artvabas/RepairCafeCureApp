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
* File:ClosedWorkorderDetailsTabCtrl.h, defines the ClosedWorkorderDetailsTabCtrl class
* This class is the CTabCtrl derived class what interact with the tab-control in the CClosedWorkorderDetails dialog class
*
* This class is a tab control with the details of the closed workorder.
* These details are separated into tabs.
* The first tab is the workorder details tab, the second tab is the asset details tab,
* the third tab is the customer details tab and the fourth tab is the invoice details tab.
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
#pragma once

namespace artvabas::rcc::ui::controls {

	struct ClosedWorkorderDetailsIDs {
		unsigned int m_nWorkorderID;
		unsigned int m_nCustomerID;
		unsigned int m_nAssetID;
		unsigned int m_nInvoiceID;
	};

	class CClosedWorkorderDetailsTabCtrl : public CTabCtrl
	{
		DECLARE_DYNAMIC(CClosedWorkorderDetailsTabCtrl)

		private:
			CDialog* m_tabPages[4];
			int m_tabCurrent;
			int m_nNumberOfTabs;

		public:
			CClosedWorkorderDetailsTabCtrl(unsigned int& unCustomerID, unsigned int& unAssetID,
				unsigned int& unWorkorderID, unsigned int& unInvoiceID);
			virtual ~CClosedWorkorderDetailsTabCtrl();

		private:
			DECLARE_MESSAGE_MAP()
		public:
			afx_msg void OnTcnSelChange(NMHDR* pNMHDR, LRESULT* pResult) noexcept;

		public:
			void Init() noexcept;
		private:
			void ChangeTabView(int nTab) noexcept;
			void SetRectangle() noexcept;
	};
}