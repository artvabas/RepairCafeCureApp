/*
	Copyright (C) 2024  artvabas

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
* File: DatabaseTables.h, defines sequences of SQL columns for database tables
*
* This header will be used everywhere in the project to define the SQL columns for the database tables
* It is used to avoid hardcoding the SQL column indexes in the project
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.3 (Alpha)
* Created: 25-04-2024, (dd-mm-yyyy)
* Updated: 30-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/
#pragma once
// Arrays sizes (wchar_t)
#define SQLCHARVSMALL 50
#define SQLCHARVMAX 1024

namespace artvabas::database::tables {
	namespace asset {
		constexpr struct asset {
			SQLUSMALLINT ASSET_ID { 1 };
			SQLUSMALLINT ASSET_CUSTOMER_ID { 2 };
			SQLUSMALLINT ASSET_CREATE_DATE { 3 };
			SQLUSMALLINT ASSET_DESCRIPTION { 4 };
			SQLUSMALLINT ASSET_MODEL_NUMBER { 5 };
			SQLUSMALLINT ASSET_BRAND { 6 };
			SQLUSMALLINT ASSET_DISPOSED { 7 };
			SQLUSMALLINT ASSET_HISTORY_LOG { 8 };
		}ASSET;
	}

	namespace customer {
		constexpr struct customer {
			SQLUSMALLINT CUSTOMER_ID { 1 };
			SQLUSMALLINT CUSTOMER_SURNAME { 2 };
			SQLUSMALLINT CUSTOMER_NAME { 3 };
			SQLUSMALLINT CUSTOMER_CELL_PHONE { 4 };
			SQLUSMALLINT CUSTOMER_PHONE { 5 };
			SQLUSMALLINT CUSTOMER_EMAIL { 6 };
			SQLUSMALLINT CUSTOMER_COMMENT { 7 };
			SQLUSMALLINT CUSTOMER_GENERAL_LOG { 8 };
		}CUSTOMER;
	}

	namespace sparepartstock {
		constexpr struct sparepartstock {
			SQLUSMALLINT SPAREPART_ID { 1 };
			SQLUSMALLINT SPAREPART_DESCRIPTION { 2 };
			SQLUSMALLINT SPAREPART_IN_STOCK { 3 };
			SQLUSMALLINT SPAREPART_PRICE { 4 };
		}SPAREPARTSTOCK;
	}

	namespace workorderparts {
		constexpr struct workorderparts {
			SQLUSMALLINT WORKORDER_PARTS_WORKORDER_ID { 1 };
			SQLUSMALLINT WORKORDER_PARTS_DESCRIPTION { 2 };
			SQLUSMALLINT WORKORDER_PARTS_AMOUNT { 3 };
			SQLUSMALLINT WORKORDER_PARTS_UNIT_PRICE { 4 };
			SQLUSMALLINT WORKORDER_PARTS_TOTAL_PRICE { 5 };
		}WORKORDERPARTS;
	}

	namespace workorder {
		constexpr struct workorder {
			SQLUSMALLINT WORKORDER_ID { 1 };
			SQLUSMALLINT WORKORDER_ASSET_ID { 2 };
			SQLUSMALLINT WORKORDER_CUSTOMER_ID { 3 };
			SQLUSMALLINT WORKORDER_INVOICE_ID { 4 };
			SQLUSMALLINT WORKORDER_CREATE_DATE { 5 };
			SQLUSMALLINT WORKORDER_CREATE_BY{ 6 };
			SQLUSMALLINT WORKORDER_DESCRIPTION { 7 };
			SQLUSMALLINT WORKORDER_RESPONSIBLE { 8 };
			SQLUSMALLINT WORKORDER_STATUS { 9 };
			SQLUSMALLINT WORKORDER_CLOSED_DATE{ 10 };
			SQLUSMALLINT WORKORDER_HISTORY { 11 };
		}WORKORDER;
	}

	namespace invoice {
		constexpr struct invoice {
			SQLUSMALLINT INVOICE_ID{ 1 };
			SQLUSMALLINT INVOICE_CUSTOMER_ID{ 2 };
			SQLUSMALLINT INVOICE_ASSET_ID{ 3 };
			SQLUSMALLINT INVOICE_WORKORDER_ID{ 4 };
			SQLUSMALLINT INVOICE_CREATE_DATE{ 5 };
			SQLUSMALLINT INVOICE_PAYMENT_PIN{ 6 };
			SQLUSMALLINT INVOICE_TOTAL{ 7 };
		}INVOICE;
	}
	
	namespace historysearch {
		constexpr struct search {
			SQLUSMALLINT ASSET_DESCRIPTION { 1 };
			SQLUSMALLINT ASSET_MODEL_NUMBER { 2 };
			SQLUSMALLINT ASSET_BRAND { 3 };
			SQLUSMALLINT ASSET_DISPOSED { 4 };
			SQLUSMALLINT WORKORDER_CREATE_DATE { 5 };
			SQLUSMALLINT WORKORDER_CREATE_BY { 6 };
			SQLUSMALLINT WORKORDER_RESPONSIBLE { 7 };
			SQLUSMALLINT WORKORDER_DESCRIPTION { 8 };
			SQLUSMALLINT WORKORDER_HISTORY { 9 };
		}HISTORY_SEARCH;
	}

	namespace reportcontributiontax {
		constexpr struct reportcontributiontax {
			SQLUSMALLINT CONTRIBUTION_CREATEDATE{ 1 };
			SQLUSMALLINT CONTRIBUITION_WORKORDER_ID{ 2 };
			SQLUSMALLINT CUSTOMER_SURNAME{ 3 };
			SQLSMALLINT CUSTOMER_PHONE{ 4 };
			SQLSMALLINT CUSTOMER_CELL_PHONE{ 5 };
			SQLSMALLINT CONTRIBUTION_AMOUNT{ 6 };
		}REPORT_CONTRIBUTION_TAX;
	}

	namespace closedworkorders {
		constexpr struct closedworkorders {
			SQLUSMALLINT WORKORDER_ID{ 1 };
			SQLUSMALLINT WORKORDER_DESCRIPTION{ 2 };
			SQLUSMALLINT WORKORDER_RESPONSIBLE{ 3 };
			SQLUSMALLINT WORKORDER_CLOSED_DATE{ 4 };
			SQLUSMALLINT WORKORDER_STATUS{ 5 };
			SQLUSMALLINT WORKORDER_ASSET_ID{ 6 };
			SQLUSMALLINT ASSET_DESCRIPTION{ 7 };
			SQLUSMALLINT WORKORDER_CUSTOMER_ID{ 8 };
			SQLUSMALLINT WORKORDER_INVOICE_ID{ 9 };
		}CLOSED_WORKORDERS;
	}
}