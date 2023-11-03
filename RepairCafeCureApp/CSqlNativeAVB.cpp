#include "pch.h"
#include "CSqlNativeAVB.h"

/******** IMPORTANT NOTICE *********
* Somehow a diverted class from the MFC CRecordset class, can't be used for working with SQL data-tables.
* Form some reason DoFieldExchange() what is used for data exchange from the data-table to the class and visa versa,
* is not working. So, the MFC CRecordset class can't be used for working with SQL data-tables for writing and
* can only be used for reading with GetFieldValue() data from the database.
* 
* Therefore this native SQL class is created. It uses the ODBC API for connecting to the database and
* and is able to make changes for writing, like update, insert and delete queries.
**********/

using namespace artvabas::sql;

CSqlNativeAVB::CSqlNativeAVB(SQLWCHAR* pszSqlConnectionString)
	: m_pszSqlConnectionString(pszSqlConnectionString)
	, m_hEnvironment(NULL)
	, m_hDatabaseConnection(NULL)
	, m_hStatement(NULL)
{
}

CSqlNativeAVB::~CSqlNativeAVB()
{
    CloseConnection();
}

bool CSqlNativeAVB::ExecuteQuery(SQLWCHAR* pszQuery)
{
    // Allocate an environment
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnvironment) == SQL_ERROR)
    {
        wchar_t buffer[256];
        swprintf_s(buffer, L"Unable to allocate an environment handle\n");
        AfxMessageBox(buffer);
        ASSERT(fwprintf(stderr, L"Unable to allocate an environment handle\n"));
        return false;
    }

    // Register this as an application that expects 3.x behavior,
    // you must register something if you use AllocHandle
    TryODBC(m_hEnvironment, SQL_HANDLE_ENV, SQLSetEnvAttr(m_hEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0));

    // Allocate a connection
    TryODBC(m_hEnvironment, SQL_HANDLE_ENV, SQLAllocHandle(SQL_HANDLE_DBC, m_hEnvironment, &m_hDatabaseConnection));
	
    // Connect to the driver.  Use the connection string if supplied
    // on the input, otherwise let the driver manager prompt for input.
    TryODBC(m_hDatabaseConnection, SQL_HANDLE_DBC, SQLDriverConnect(m_hDatabaseConnection, GetDesktopWindow(), m_pszSqlConnectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE));

    TryODBC(m_hDatabaseConnection, SQL_HANDLE_DBC, SQLAllocHandle(SQL_HANDLE_STMT, m_hDatabaseConnection, &m_hStatement));
   
    auto RetCode = SQLExecDirect(m_hStatement, pszQuery, SQL_NTS);

    switch (RetCode)
    {
    case SQL_SUCCESS_WITH_INFO:
    {
        HandleDiagnosticRecord(m_hStatement, SQL_HANDLE_STMT, RetCode);
        // fall through
    }
    case SQL_SUCCESS:
    {
        TryODBC(m_hStatement, SQL_HANDLE_STMT, SQLFreeStmt(m_hStatement, SQL_CLOSE));
        return true;
        break;
    }

    case SQL_ERROR:
    {
        HandleDiagnosticRecord(m_hStatement, SQL_HANDLE_STMT, RetCode);
        TryODBC(m_hStatement, SQL_HANDLE_STMT, SQLFreeStmt(m_hStatement, SQL_CLOSE));
        return false;
        break;
    }

    default:
        ASSERT(fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode));
    }

    TryODBC(m_hStatement, SQL_HANDLE_STMT, SQLFreeStmt(m_hStatement, SQL_CLOSE));
    return false;
}

void CSqlNativeAVB::CloseConnection()
{
    if (m_hStatement)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
    }

    if (m_hDatabaseConnection)
    {
        SQLDisconnect(m_hDatabaseConnection);
        SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
    }

    if (m_hEnvironment)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, m_hEnvironment);
    }
}

void CSqlNativeAVB::TryODBC(SQLHANDLE h, SQLSMALLINT ht, SQLRETURN x)
{
	RETCODE rc = x;
    if (rc != SQL_SUCCESS)
    {
		HandleDiagnosticRecord(h, ht, rc);
	}
    if (rc == SQL_ERROR)
    {
        wchar_t buffer[256];
        swprintf_s(buffer, L"Error in " L"\n");
        AfxMessageBox(buffer);
        ASSERT(fwprintf(stderr, L"Error in " L"\n"));
	}
}

void CSqlNativeAVB::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
    SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	wchar_t wszMessage[SQL_QUERY_SIZE];
	wchar_t wszState[SQL_SQLSTATE_SIZE + 1];

    if (RetCode == SQL_INVALID_HANDLE)
    {
        wchar_t buffer[256];
        swprintf_s(buffer, L"Invalid handle!\n");
        AfxMessageBox(buffer);
        ASSERT(fwprintf(stderr, L"Invalid handle!\n"));
        return;
    }

    while (SQLGetDiagRecW(hType, hHandle, ++iRec, wszState, &iError, wszMessage, (SQLSMALLINT)(sizeof(wszMessage) / sizeof(wchar_t)), (SQLSMALLINT*)NULL) == SQL_SUCCESS)
    {
        // Hide data truncated..
        if (wcsncmp(wszState, L"01004", 5) && wcsncmp(wszState, L"01000", 5))
        {
            wchar_t buffer[256];
            swprintf_s(buffer, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
            AfxMessageBox(buffer);
            ASSERT(fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError));
        }
    }
}
