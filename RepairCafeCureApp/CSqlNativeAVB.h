#pragma once

namespace artvabas
{
	namespace sql
	{
		constexpr auto SQL_QUERY_SIZE = 1000;

		class CSqlNativeAVB
		{
		private:
			wchar_t* m_pszSqlConnectionString;
			// SQLHANDLE(S) for ODBC API calls (tye of void*)
			SQLHENV     m_hEnvironment;
			SQLHDBC     m_hDatabaseConnection;
			SQLHSTMT    m_hStatement;

		public:
			CSqlNativeAVB(SQLWCHAR* pszSqlConnectionString);
			virtual ~CSqlNativeAVB();
			bool ExecuteQuery(SQLWCHAR* pszQuery);
			SQLINTEGER GetLastAddedID(SQLWCHAR* pszQuery);
		private:
			bool CreateSQLConnection();
			bool CheckReturnCodeForClosing(SQLRETURN RetCode);
			void CloseConnection();
			bool TryODBC(SQLHANDLE h, SQLSMALLINT ht, SQLRETURN x);
			bool HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
		};
	}
}


