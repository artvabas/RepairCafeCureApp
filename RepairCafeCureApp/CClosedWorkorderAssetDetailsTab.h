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
* File: CClosedWorkorderAssetDetailsTab.h, defines class CClosedWorkorderAssetDetailsTab
*
* This class is the view of the Asset-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the Asset-details of a closed workorder.
* What is selected in the listview of the CReportWorkorderClosedView and passed to this form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.3 (alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 30-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"

namespace artvabas::rcc::ui::dialogs {

	class CClosedWorkorderAssetDetailsTab : public CDialogEx
	{
		DECLARE_DYNAMIC(CClosedWorkorderAssetDetailsTab)
		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CLOSED_WORKORDER_ASSET_DETAILS_TAB };
#endif

	private:
		unsigned int m_unAssetID;
		BOOL m_bAssetDsiposed;

		CString m_strAssetID;
		CString m_strAssetCustomerID;
		CString m_strAssetCreateDate;
		CString m_strAssetDescription;
		CString m_strAssetModelNumber;
		CString m_strAsserBrand;
		CString m_strAssetHistoryLog;

	public:
		CClosedWorkorderAssetDetailsTab(unsigned int unID, CWnd* pParent = nullptr);
		virtual ~CClosedWorkorderAssetDetailsTab();

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnInitDialog() override;
		DECLARE_MESSAGE_MAP()
	};
}
