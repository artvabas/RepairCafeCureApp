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
* File: CClosedWorkOrderDetailsTab.h, defines class CClosedWorkOrderDetailsTab
*
* This class is the view of the Workorder-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the workorder-details of a closed workorder.
* What is selected in the listview of the CReportWorkorderClosedView and passed to this form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.2 (alpha)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 28-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/
#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"

namespace artvabas::rcc::ui::dialogs {

	using namespace artvabas::rcc::ui::controls;

	class CClosedWorkOrderDetailsTab : public CDialogEx
	{
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_CLOSED_WORKORDER_DETAILS_TAB };
#endif
		DECLARE_DYNAMIC(CClosedWorkOrderDetailsTab)

	private:
		CClosedWorkorderDetailsTabCtrl* m_pTabControl;
		unsigned int m_unWorkorderID;

		CString m_strWorkorderID;
		CString m_strWorkorderAssetID;
		CString m_strWorkorderCustomerID;
		CString m_strWorkorderInvoiceID;
		CString m_strWorkorderCreateDate;
		CString m_strWorkorderClosedDate;
		CString m_strWorkorderCreatedBy;
		CString m_strWorkorderEmployeeResponsible;
		CString m_strWorkorderDescription;
		CString m_strWorkorderStatus;
		CString m_strWorkorderHistoryLog;

	public:
		CClosedWorkOrderDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent = nullptr);   // standard constructor
		virtual ~CClosedWorkOrderDetailsTab();

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
	};
}