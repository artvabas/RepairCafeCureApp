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
* File: CClosedWorkorderDetails.cpp, definition of the class CClosedWorkorderDetails
*
* This class is the dialog of the closed workorder tab control
* it is the placeholder for the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the closed workorder details dialog
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.2 (Alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 18-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"

namespace artvabas::rcc::ui::dialogs {

	using namespace artvabas::rcc::ui::controls;

	class CClosedWorkorderDetails : public CDialogEx
	{
		DECLARE_DYNAMIC(CClosedWorkorderDetails)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CLOSED_WORKORDER_DIALOG };
#endif
	private:
		CClosedWorkorderDetailsTabCtrl m_ctrTabClosedWorkorderDetails;

	public:
		CClosedWorkorderDetails(unsigned int& unCustomerID, unsigned int& unAssetID,
			unsigned int& unWorkorderID, unsigned int& unInvoiceID, CWnd* pParent = nullptr);
		virtual ~CClosedWorkorderDetails();

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnInitDialog() override;
		DECLARE_MESSAGE_MAP()
	};
}