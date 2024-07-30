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
	* File: pch.h is a precompiled header file.
	*
	* Files listed below are compiled only once, improving build performance for future builds.
	* This also affects IntelliSense performance, including code completion and many code browsing features.
	* However, files listed here are ALL re-compiled if any one of them is updated between builds.
	* Do not add files here that you will be updating frequently as this negates the performance advantage.
	*
	* Target: Windows 10/11 64bit
	* Version: 1.0.0.5 (Alpha)
	* Created: 18-10-2023, (dd-mm-yyyy)
	* Updated: 19-07-2024, (dd-mm-yyyy)
	* Creator: artvabasDev / artvabas
	*
	* Description: Main application class for RepairCafeCureApp
	* License: GPLv3
	*/

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <afxdb.h>
#include <string>
#include "CSqlNativeAVB.h"
#include <vector>

#define ALL_CONTEXT_CATEGORY 0

enum ColorButton { RED, BLUE, BLACK };
enum PrinterOrientation { PORTRAIT, LANDSCAPE };


#endif //PCH_H
