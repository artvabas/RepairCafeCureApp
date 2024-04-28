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
* File: CDatabaseConnection.h, defines class CDatabaseConnection
*
* This class is used to connect to the database. It uses ODBC Driver 17 for SQL Server.
* It create a connection string and save it in a file. The connection credentials are encrypted by RSA.
* using CryptoPP library. See: https://www.cryptopp.com/ and on GitHub: https://github.com/weidai11/cryptopp.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 17-10-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#pragma once

namespace artvabas
{
	namespace rcc
	{
		namespace database
		{
			class CDatabaseConnection : public CDatabase
			{
			// Members
			private:
				CString m_sDriver;
				CString m_strDsn;
				
			public:
				CDatabaseConnection();
				virtual ~CDatabaseConnection();

			// General Methods
			public:
				// inline method for getting the database instance
				inline CDatabase* GetDatabaseInstance() { return this;};
				wchar_t* ConnectionString();
			private:
				CString GetConnectionString();
				CString CreateConnectionString();
				void EncryptCredentials(CString strUser, CString strPassword);
				void DecryptCredentials(std::string& cphUser, std::string& cphPassword, CString& strUser, CString& strPassword);
				void RemoveCredentials(CString& strUser, CString& strPassword, CString& strDsn);
				void SaveConnectionString(CString& strUser, CString& strPassword, CString& strDsn);
				void AddCredentials(CString& strUser, CString& strPassword, CString& strDsn);
			};
		}
	}
}

