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
			int m_nTryCount;

		public:
			CSqlNativeAVB(SQLWCHAR* pszSqlConnectionString);
			virtual ~CSqlNativeAVB();

			bool CreateSQLConnection();
			bool ExecuteQuery(SQLWCHAR* pszQuery);
			SQLINTEGER ExecuteReceiveQuery(SQLWCHAR* pszQuery);
			SQLINTEGER GetLastAddedID(SQLWCHAR* pszQuery);
			bool CheckReturnCodeForClosing(SQLRETURN RetCode);

			inline SQLHSTMT& GetStatementHandle() { return m_hStatement; }
			void CloseConnection();
		private:
			//bool CreateSQLConnection();
			//void CloseConnection();
			bool TryODBC(SQLHANDLE h, SQLSMALLINT ht, SQLRETURN x);
			bool HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
		};
	}
}


