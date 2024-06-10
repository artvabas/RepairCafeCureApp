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
* File: RepairCafeCureApp.h, defines class CRepairCafeCureApp
*
* This class is the main application class. It is used to create the database connection and
* to switch between the views.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.1 (alpha)
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 05-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols
#include "CDatabaseConnection.h"
#include "CSplashScreen.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::rcc::database;

// The available views
enum ViewType { VIEW_ASSET, VIEW_CUSTOMER, VIEW_WORKORDER, VIEW_REPORT_FINANCE_TAX, VIEW_REPORT_WORKORDER_CLOSED };
// The workorder status views
enum WorkorderViewType { VIEW_WORKORDER_OPEN, VIEW_WORKORDER_PROGRESS, VIEW_WORKORDER_REPAIRED };

class CRepairCafeCureApp : public CWinAppEx
{
public:
	CSplashScreen m_SplashScreen;
	bool m_bIsIdle;
private:
		CDatabaseConnection* m_dbConnection;

		CView* m_pCustomerView;
		CView* m_pAssetView;
		CView* m_pWorkorderView;
		CView* m_pReportTaxView;
		CView* m_pReportWorkorderClosedView;

		WorkorderViewType m_enuWorkorderViewType;

public:
	CRepairCafeCureApp() noexcept;
	virtual ~CRepairCafeCureApp();

private:
	BOOL InitInstance() override;
	int ExitInstance() override;

private:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout() noexcept;
	afx_msg void OnCustomerView() noexcept;
	afx_msg void OnAssetView() noexcept;
	afx_msg void OnWorkorderViewOpen() noexcept;
	afx_msg void OnWorkorderViewProgress() noexcept;
	afx_msg void OnWorkorderViewRepaired() noexcept;
	afx_msg void OnReportViewFinanceTax() noexcept;
	afx_msg void OnReportWorkorderClosed() noexcept;
	afx_msg void OnFilePrintSetup() noexcept;

public:
	inline CDatabaseConnection* GetDatabaseConnection() const noexcept { return m_dbConnection; }
	inline WorkorderViewType GetWorkorderViewType() const noexcept { return m_enuWorkorderViewType; }
	CView* SwitchView(ViewType vtView) const noexcept;
	void SetStatusBarText(UINT nStrID) const noexcept;
	CString GetSelectedEmployeeName() const noexcept;
	ViewType GetActiveViewType() const noexcept;
	void IsIdle() const noexcept;
	inline HANDLE GetDeviceMode() noexcept { return m_hDevMode ? m_hDevMode : DefineDeviceMode(); }

private:
	HANDLE DefineDeviceMode() noexcept;
};
extern CRepairCafeCureApp theApp;