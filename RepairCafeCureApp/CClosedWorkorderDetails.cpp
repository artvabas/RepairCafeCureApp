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
* File: CClosedWorkorderDetails.cpp, implements of the class CClosedWorkorderDetails
*
* This class is the dialog of the closed workorder tab control
* it is the placeholder for the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the closed workorder details dialog
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.3 (Alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 18-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkorderDetails.h"

using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CClosedWorkorderDetails, CDialogEx)

CClosedWorkorderDetails::CClosedWorkorderDetails(unsigned int& unCustomerID, unsigned int& unAssetID,
	unsigned int& unWorkorderID, unsigned int& unInvoiceID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSED_WORKORDER_DIALOG, pParent),
	m_ctrTabClosedWorkorderDetails(unCustomerID, unAssetID, unWorkorderID, unInvoiceID)
{}

CClosedWorkorderDetails::~CClosedWorkorderDetails()
{}

// DoDataExchange method is used for binding ID's with their controls
// - DDX_Control binds the control with the ID
void CClosedWorkorderDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLOSED_WORKORDER_DETAILS_TAB_CONTROL, m_ctrTabClosedWorkorderDetails);
}

// OnInitDialog method is called by the framework when dialog is created
BOOL CClosedWorkorderDetails::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ctrTabClosedWorkorderDetails.Init();
	return TRUE;
}

// Message maps, for handling app messages, int this case pass the message through to the Tab Control class
BEGIN_MESSAGE_MAP(CClosedWorkorderDetails, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CLOSED_WORKORDER_DETAILS_TAB_CONTROL, &CClosedWorkorderDetailsTabCtrl::OnTcnSelChange)
END_MESSAGE_MAP()
