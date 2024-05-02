/*
Copyright(C) 2023/24  artvabas

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
	* File: RepairCafeCureAppDoc.h, defines the Document class of this MFC application
	*
	* This class is used to print a workorder in a combi or invoice format.
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
class CRepairCafeCureAppDoc : public CDocument
{
	DECLARE_DYNCREATE(CRepairCafeCureAppDoc)

protected: // create from serialization only
	CRepairCafeCureAppDoc() noexcept;
public:
	virtual ~CRepairCafeCureAppDoc();

private:
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

private:
	DECLARE_MESSAGE_MAP()
};
