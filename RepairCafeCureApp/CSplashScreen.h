/*
Copyright(C) 2023 / 24  artvabas

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>

To see the license for this source code, please visit :
< https ://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit :
< https ://artvabas.com>
	< https ://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at :
< https ://artvabas.com/contact>

	*/

	/*
	* This file is part of RepairCafeCureApp.
	* File: CSplashScreen.h, defines the class CSplashScreen.
	*
	* Shows a splash screen when the application starts.
	* At startup this application is trying to connect to a SQL database.
	* It can take some time, so the splash screen is shown.
	*
	* Target: Windows 10/11 64bit
	* Version: 0.0.1.0 (Alpha)
	* Created: 18-10-2023, (dd-mm-yyyy)
	* Updated: 2-05-2024, (dd-mm-yyyy)
	* Creator: artvabasDev / artvabas
	*
	* License: GPLv3
	*/
#pragma once
#include "afxdialogex.h"

namespace artvabas::rcc::ui
{
	class CSplashScreen : public CDialogEx
	{
		DECLARE_DYNAMIC(CSplashScreen)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_SPLASHSCREEN };
#endif
	public:
		CSplashScreen(CWnd* pParent = nullptr) noexcept;
		virtual ~CSplashScreen();

	private:
		void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
	};
}