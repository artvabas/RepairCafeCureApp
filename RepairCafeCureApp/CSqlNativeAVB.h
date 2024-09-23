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
	* File: CSqlNative.h, defines the class CSqlNativeAVB.
	*
	* This class is used to create a connection to a SQL database using ODBC.
	* It can execute queries and return the result.
	* It can also handle errors and close the connection.
	*
	* Target: Windows 10/11 64bit
	* Version: 1.0.3.5 (Beta)
	* Created: 18-10-2023, (dd-mm-yyyy)
	* Updated: 16-09-2024, (dd-mm-yyyy)
	* Creator: artvabasDev / artvabas
	*
	* License: GPLv3
	*/
#pragma once
namespace artvabas::sql
{
	constexpr auto SQL_QUERY_SIZE = 1000;

	class CSqlNativeAVB
	{
	private:
		wchar_t* m_pszSqlConnectionString;
		int m_nTryCount;

		SQLHENV     m_hEnvironment;
		SQLHDBC     m_hDatabaseConnection;
		SQLHSTMT    m_hStatement;
		
	public:
		CSqlNativeAVB(SQLWCHAR* pszSqlConnectionString) noexcept;
		virtual ~CSqlNativeAVB();

		bool CreateSQLConnection();
		bool ExecuteQuery(SQLWCHAR* pszQuery) noexcept;
		SQLINTEGER GetLastAddedID(SQLWCHAR* pszQuery) noexcept;
		bool CheckReturnCodeForClosing(SQLRETURN RetCode);

		inline SQLHSTMT& GetStatementHandle() noexcept { return m_hStatement; }
		void CloseConnection() noexcept;

	private:
		bool TryODBC(SQLHANDLE h, SQLSMALLINT ht, SQLRETURN x) noexcept;
		bool HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode) const;
	};
}
