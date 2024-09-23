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
* File: CAdminCombiReceipt.cpp, implements class CAdminCombiReceipt
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

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CAdminCombiReceipt.h"

using namespace artvabas::rcc::dialog::admin;

IMPLEMENT_DYNAMIC(CAdminCombiReceipt, CDialogEx)

CAdminCombiReceipt::CAdminCombiReceipt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMIN_PRINT_RECEIPT, pParent)
	, m_strUmbrellaOrganization{ _T("") }
	, m_strOrganization{ _T("") }
	, m_strAdministrationCosts{ _T("") }
{}

CAdminCombiReceipt::~CAdminCombiReceipt()
{}

BEGIN_MESSAGE_MAP(CAdminCombiReceipt, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAdminCombiReceipt::OnBnClickedOk)
	ON_EN_CHANGE(IDC_ADMIN_COMBI_UMBRELLA_NAME, &CAdminCombiReceipt::OnEnChangeAdminCombi)
	ON_EN_CHANGE(IDC_ADMIN_COMBI_ORGANIZATION, &CAdminCombiReceipt::OnEnChangeAdminCombi)
	ON_EN_CHANGE(IDC_ADMIN_COMBI_ADMIN_COSTS, &CAdminCombiReceipt::OnEnChangeAdminCombi)
END_MESSAGE_MAP()

// DoDataExchange, method for exchanging and validating dialog data
// @param pDX, pointer to a CDataExchange object
void CAdminCombiReceipt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADMIN_COMBI_UMBRELLA_NAME, m_strUmbrellaOrganization);
	DDX_Text(pDX, IDC_ADMIN_COMBI_ORGANIZATION, m_strOrganization);
	DDX_Text(pDX, IDC_ADMIN_COMBI_ADMIN_COSTS, m_strAdministrationCosts);
	DDX_Control(pDX, IDOK, m_btnOK);
}

// OnInitDialog, method for initializing the dialog
// @return BOOL, TRUE if successful, FALSE otherwise
BOOL CAdminCombiReceipt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Load init settings
	TCHAR szPath[MAX_PATH]{};
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
		PathAppendW(szPath, _T("artvabas\\repairCafeCureApp\\rccacr.ini"));
		// Does destination exist?
		if (GetFileAttributesW(szPath)) {
			wchar_t szCompanyOff[100]{}; // Company official name
			wchar_t szCompany[100]{}; // Company name
			wchar_t szAdminCoverage[10]{}; // Admin coverage
			GetPrivateProfileStringW(_T("Combi-receipt"), _T("Company-official"), _T("[Company-official-name]"), szCompanyOff, 100, szPath);
			GetPrivateProfileStringW(_T("Combi-receipt"), _T("Company"), _T("[Company-name]"), szCompany, 100, szPath);
			GetPrivateProfileStringW(_T("Combi-receipt"), _T("Admin-coverage"), _T("[€1,00]"), szAdminCoverage, 10, szPath);
			m_strUmbrellaOrganization = szCompanyOff;
			m_strOrganization = szCompany;
			m_strAdministrationCosts = szAdminCoverage;
			UpdateData(FALSE);
		}
	}
	return TRUE;
}

// OnBnClickedOk, method for handling the OK button click event
// Save the settings to the ini file
void CAdminCombiReceipt::OnBnClickedOk() noexcept
{
	UpdateData(TRUE);
	// Save init settings
	TCHAR szPath[MAX_PATH]{};
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
		PathAppendW(szPath, _T("artvabas\\repairCafeCureApp\\rccacr.ini"));
		// Does destination exist?
		if (GetFileAttributesW(szPath)) {
			UpdateData(TRUE);
			WritePrivateProfileStringW(_T("Combi-receipt"), _T("Company-official"), m_strUmbrellaOrganization, szPath);
			WritePrivateProfileStringW(_T("Combi-receipt"), _T("Company"), m_strOrganization, szPath);
			WritePrivateProfileStringW(_T("Combi-receipt"), _T("Admin-coverage"), m_strAdministrationCosts, szPath);
		}
	}
	CDialogEx::OnOK();
}

// OnEnChangeAdminCombi, method for handling the change event of the edit controls
// Enable the OK button if all edit controls are filled
void CAdminCombiReceipt::OnEnChangeAdminCombi() noexcept
{
	UpdateData(TRUE);
	m_btnOK.EnableWindow(!m_strUmbrellaOrganization.IsEmpty() && !m_strOrganization.IsEmpty() && !m_strAdministrationCosts.IsEmpty());
}