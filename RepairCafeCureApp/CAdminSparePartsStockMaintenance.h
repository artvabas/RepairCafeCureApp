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
* File: CAdminSparePartsStockMaintenance.cpp, defines class CAdminSparePartsStockMaintenance
*
* This class is the dialog for maintaining the stock parts.
* It is a part of the administration of the application, for add new, change or delete stock parts.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5
* Created: 21-07-2024, (dd-mm-yyyy)
* Updated: 30-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/
#pragma once

#include "IAdminMaintenance.h"
namespace artvabas::rcc::dialog::admin {
	class CAdminSparePartsStockMaintenance : public CDialogEx, private artvabas::rcc::interfaces::IAdminMaintenance
	{
		DECLARE_DYNAMIC(CAdminSparePartsStockMaintenance)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ADMIN_SPAREPARTS_STOCK_MAINTENANCE };
#endif
	private:
		CListCtrl m_lstSparePartStock;
		CString m_strSparePartDescription;
		CString m_strSparePartsUnitPrice;
		CButton m_btnSparePartChange;
		CButton m_btnSparePartDelete;

	public:
		CAdminSparePartsStockMaintenance(CWnd* pParent = nullptr);
		virtual ~CAdminSparePartsStockMaintenance();

	private:
		BOOL OnInitDialog() override;
		void DoDataExchange(CDataExchange* pDX) override;
		void GetData() noexcept override;
		void SetData() noexcept override;
		void DeleteData() noexcept override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnLvnItemChangedAdminStockSpareParts(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnEnAdminStockItem() noexcept;
		afx_msg void OnBnClickedAdminStockChange() noexcept;
		afx_msg void OnBnClickedAdminStockItemDelete() noexcept;
	};
}