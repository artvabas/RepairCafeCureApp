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
	* File: CPrintWorkorder.h, defines the class CPrintWorkorder.
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
namespace artvabas::rcc::support
{
	class CPrintWorkorder
	{
	public:

		struct WorkorderData
		{
			CString strWorkorderID;
			CString strCustomerSurname;
			CString strCustomerName;
			CString strCustomerCellPhone;
			CString strCustomerPhone;
			CString strCustomerEmail;
			CString strCustomerComments;
			CString strAssetDescription;
			CString strAssetModelNumber;
			CString strAssetBrand;
			CString strWorkorderCreatedDate;
			CString strEmployeeResponsible;
			CString strWorkorderStatus;
			CString strWorkorderDescription;
			CString strWorkorderTotalPartsPrice;

			struct SparePart
			{
				CString strSparePartDescription;
				CString strSparePartPrice;
				CString strSparePartQuantity;
				CString strSparePartSubTotalPrice;
			}m_structSparePArts;

			std::vector<SparePart> m_vecSpareParts;

			struct WorkorderLog
			{
				CString strWorkorderLog;
				CString strWorkorderRepairedDate;
			} m_structWorkorderLog;
		};

	private:
		WorkorderData* m_pStructWorkorderData;

	public:
		explicit CPrintWorkorder(WorkorderData* pworkData) noexcept;
		virtual ~CPrintWorkorder();

		void PrintCombi(CDC* pDC) const noexcept;
		void PrintInvoice(CDC* pDC) const noexcept;
	};
}