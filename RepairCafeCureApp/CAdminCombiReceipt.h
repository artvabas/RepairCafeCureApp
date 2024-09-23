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
* File: CAdminCombiReceipt.h, defines class CAdminCombiReceipt
*
* This class is the dialog for maintaining the general data of the customer receipt.
* It is a part of the administration of the application, for printing the customer combi-receipt.
*
* Target: Windows 10/11 64bit
* Version: 1.0.3.5 (beta)
* Created: 22-09-2024, (dd-mm-yyyy)
* Updated: 23-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/
#pragma once
namespace artvabas::rcc::dialog::admin
{
	class CAdminCombiReceipt : public CDialogEx
	{
		DECLARE_DYNAMIC(CAdminCombiReceipt)
		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ADMIN_PRINT_RECEIPT };
#endif
	private:
		CString m_strUmbrellaOrganization;
		CString m_strOrganization;
		CString m_strAdministrationCosts;

		CButton m_btnOK;

	public:
		CAdminCombiReceipt(CWnd* pParent = nullptr);
		virtual ~CAdminCombiReceipt();

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnInitDialog() override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnBnClickedOk() noexcept;
		afx_msg void OnEnChangeAdminCombi() noexcept;
	};
}