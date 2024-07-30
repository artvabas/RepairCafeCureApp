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
* File: CAdminEmployeeMaintenance.cpp, defines class CAdminEmployeeMaintenance
*
* This class is the dialog for maintaining the employees with their roles.
* It is a part of the administration of the application, for add new, change or delete employees.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5
* Created: 15-07-2024, (dd-mm-yyyy)
* Updated: 21-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/
#pragma once

#include "IAdminMaintenance.h"
namespace artvabas::rcc::dialog::admin {

	class CAdminEmployeeMaintenance : public CDialogEx, private artvabas::rcc::interfaces::IAdminMaintenance
	{
		DECLARE_DYNAMIC(CAdminEmployeeMaintenance)
		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ADMIN_EMPLOYEE_MAINTENANCE };
#endif

	private:
		CListCtrl m_lstExistingEmployees;
		CString m_strEmployeeName;
		BOOL m_bEmployeeIsAdmin;
		CButton m_chkEmployeeIsAdmin;
		CButton m_btnEmployeeAddChange;
		CButton m_btnEmployeeRemove;

		bool m_bIsDirty;

	public:
		CAdminEmployeeMaintenance(CWnd* pParent = nullptr);   // standard constructor
		virtual ~CAdminEmployeeMaintenance();

	private:
		BOOL OnInitDialog() override;
		void DoDataExchange(CDataExchange* pDX) override;
		void GetData() noexcept override;
		void SetData() noexcept override;
		void DeleteData() noexcept override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnLvnItemChangedAdminEmployeeMaintenance(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnEnChangeAdminEmployeeMaintenanceName() noexcept;
		afx_msg void OnBnClickedAdminEmployeeMaintenanceAdmin() noexcept;
		afx_msg void OnBnClickedAdminEmployeeMaintenanceAdd() noexcept;
		afx_msg void OnBnClickedAdminEmployeeMaintenanceDelete() noexcept;
		afx_msg void OnBnClickedOk() noexcept;
	};
}