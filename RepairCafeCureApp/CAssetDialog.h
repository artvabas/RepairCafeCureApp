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
* File: CAssetDialog.h, defines class CAssetDialog
*
* This class is the dialog of the asset/workorder tab control (CAssetDialog)
* it is the placeholder for the CTabCtrlAssetWorkorder class, 
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
* 
* Target: Windows 10/11 64bit
* Version: 1.0.465.0
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 03-03-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/
#pragma once
#include "CTabCtrlAssetWorkorder.h"

namespace artvabas::rcc::ui::dialogs {

	using namespace artvabas::rcc::ui::controls;

	class CAssetDialog : public CDialogEx {
		DECLARE_DYNAMIC(CAssetDialog)

	public:
		CAssetDialog(CString strCustomerSurname, CString strCustomerName, unsigned int nCustomerID, CWnd* pParent = nullptr);
		virtual ~CAssetDialog();

	private:
		CTabCtrlAssetWorkorder m_ctrTabAssetWorkorder;

		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ASSET_DIALOG };
#endif

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnInitDialog() override;

		DECLARE_MESSAGE_MAP()
	};
}