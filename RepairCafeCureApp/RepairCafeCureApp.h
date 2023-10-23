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
* Version: 1.0.230.0
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 23-10-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "CDatabaseConnection.h"

using namespace artvabas::rcc::database;

class CRepairCafeCureApp : public CWinAppEx
{
// Members 
private:
		CDatabaseConnection* m_dbConnection;
		CView* m_pAppView;
		CView* m_pCustomerView;
		CView* m_pWorkorderView;

		enum ViewType { VIEW_APP, VIEW_CUSTOMER, VIEW_WORKORDER };

public:
	CRepairCafeCureApp() noexcept;
	virtual ~CRepairCafeCureApp();

// Overrides
public:
	BOOL InitInstance() override;
	int ExitInstance() override;
	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

// Merage handlers
private:
	afx_msg void OnAppAbout();
	afx_msg void OnCustomerView();
	afx_msg void OnAppView();
	afx_msg void OnWorkorderView();
	DECLARE_MESSAGE_MAP()

// General methods
public:
	// inline methods for getting the database connection
	inline CDatabaseConnection* GetDatabaseConnection() { return m_dbConnection; }
private:
	CView* SwitchView(ViewType vtView);
};

extern CRepairCafeCureApp theApp;
