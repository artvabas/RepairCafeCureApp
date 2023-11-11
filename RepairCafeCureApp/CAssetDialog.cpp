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
* File: CAssetDialog.cpp, implementation of the class CAssetDialog
*
* This class is the dialog of the asset/workorder tab control (CAssetDialog)
* it is the placeholder for the CTabCtrlAssetWorkorder class,
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 11-11-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CAssetDialog.h"

using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CAssetDialog, CDialogEx)

CAssetDialog::CAssetDialog(CString strCustomerSurname, CString strCustomerName, unsigned int nCustomerID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSET_DIALOG, pParent)
	, m_ctrTabAssetWorkorder(strCustomerSurname, strCustomerName, nCustomerID)
{
}

CAssetDialog::~CAssetDialog()
{
}

/// <summary>
/// Data exchange between controls and variables
/// </summary>
/// <param name="pDX">The pDX.</param>
/// <returns></returns>
void CAssetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASSET_WORKORDER_TAB, m_ctrTabAssetWorkorder);
}

/// <summary>
/// Initializes the dialog.
/// </summary>
/// <returns></returns>
BOOL CAssetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrTabAssetWorkorder.Init();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAssetDialog, CDialogEx)
END_MESSAGE_MAP()
