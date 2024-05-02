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
    * File: CSqlNative.cpp, implements the class CSqlNativeAVB.
    *
    * This class is used to create a connection to a SQL database using ODBC.
    * It can execute queries and return the result.
    * It can also handle errors and close the connection.
    *
    * Target: Windows 10/11 64bit
    * Version: 0.0.1.0 (Alpha)
    * Created: 18-10-2023, (dd-mm-yyyy)
    * Updated: 1-05-2024, (dd-mm-yyyy)
    * Creator: artvabasDev / artvabas
    *
    * License: GPLv3
    */
#include "pch.h"
#include "CSqlNativeAVB.h"

using namespace artvabas::sql;

CSqlNativeAVB::CSqlNativeAVB(SQLWCHAR* pszSqlConnectionString) noexcept
    : m_pszSqlConnectionString{ pszSqlConnectionString }
    , m_hEnvironment{ NULL }
    , m_hDatabaseConnection{ NULL }
    , m_hStatement{ NULL }
    , m_nTryCount{ 0 }
{}

CSqlNativeAVB::~CSqlNativeAVB()
{
    CloseConnection();
}

// ExecuteQuery - Execute a query on the database
// - pszQuery: the query to execute
bool CSqlNativeAVB::ExecuteQuery(SQLWCHAR* pszQuery) noexcept
{  
    auto retCode = SQLExecDirectW(m_hStatement, pszQuery, SQL_NTS);

    return CheckReturnCodeForClosing(retCode);
}

// ExecuteReceiveQuery - Execute a query on the database and return the result
// - pszQuery: the query to execute
SQLINTEGER artvabas::sql::CSqlNativeAVB::ExecuteReceiveQuery(SQLWCHAR* pszQuery) const noexcept
{
    return SQLExecDirectW(m_hStatement, pszQuery, SQL_NTS);
}

// GetLastAddedID - Get the last added ID from the database
// - pszQuery: the query to execute
SQLINTEGER CSqlNativeAVB::GetLastAddedID(SQLWCHAR* pszQuery) noexcept
{
    SQLINTEGER id = 0L;

    auto checkErrorReturn = [&](SQLRETURN retCode) -> bool {
        if ( retCode == SQL_ERROR || retCode == SQL_SUCCESS_WITH_INFO )
            return HandleDiagnosticRecord(m_hStatement, SQL_HANDLE_STMT, retCode);
		return true;
    };
    
    SQLRETURN retCode = 0L;
    // Execute the SQL query
    if( !checkErrorReturn(retCode = SQLExecDirectW(m_hStatement, pszQuery, SQL_NTS)) )
		return 0L;

    // Bind the result column to a variable
    if( !checkErrorReturn(retCode = SQLBindCol(m_hStatement, 1, SQL_C_LONG, &id, sizeof(id), NULL)) )
        return 0L;

    // Fetch the result
    if( !checkErrorReturn(retCode = SQLFetch(m_hStatement)) )
        return 0L;

    return CheckReturnCodeForClosing(retCode) ? id : 0L;
}

// CreateSQLConnection - Create a connection to the database
// m_nTryCount: the number of tries to connect to the database,
// if it fails, it will try again up to 20 times, then it will return false.
// This is to prevent connection timeout errors.
bool CSqlNativeAVB::CreateSQLConnection()
{
    m_nTryCount = 0;
    bool bSuccess = false;

    do {
        m_nTryCount++;

        // Allocate an environment
        if ( m_hEnvironment == NULL) {
            if (SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnvironment) == SQL_ERROR ) {
                wchar_t buffer[256];
                swprintf_s(buffer, L"Unable to allocate a SQL environment handle\n");
                AfxMessageBox(buffer, MB_ICONEXCLAMATION);
                ASSERT(fwprintf(stderr, L"Unable to allocate an environment handle\n"));
                continue;
            }
        }

        // Register this as an application that expects 3.x behavior,
        // you must register something if you use AllocHandle
        if ( !TryODBC(m_hEnvironment, SQL_HANDLE_ENV, SQLSetEnvAttr(m_hEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) )
            continue;

        // Allocate a connection
        if  (m_hDatabaseConnection == NULL )
            if ( !TryODBC(m_hEnvironment, SQL_HANDLE_ENV, SQLAllocHandle(SQL_HANDLE_DBC, m_hEnvironment, &m_hDatabaseConnection)) )
                continue;

        // Connect to the DSN
        if ( !TryODBC(m_hDatabaseConnection, SQL_HANDLE_DBC, SQLDriverConnectW(m_hDatabaseConnection, NULL /*GetDesktopWindow()*/,
            m_pszSqlConnectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT /*SQL_DRIVER_COMPLETE*/)) ) {
            CloseConnection(); // if failed call CloseConnection to set all handles to NULL
            continue;
        }
        
        // Allocate a statement handle
        if ( m_hStatement == NULL )
            if ( !TryODBC(m_hDatabaseConnection, SQL_HANDLE_DBC, SQLAllocHandle(SQL_HANDLE_STMT, m_hDatabaseConnection, &m_hStatement)) )
                continue;

        bSuccess = true;
       
    } while (m_nTryCount < 20 && !bSuccess);

    if  (m_nTryCount == 20 ) {
		wchar_t buffer[256];
		swprintf_s(buffer, L"Unable to connect to the database\n");
		AfxMessageBox(buffer, MB_ICONEXCLAMATION);
		ASSERT(fwprintf(stderr, L"Unable to connect to the database\n"));
        m_hStatement = 0;
		return HandleDiagnosticRecord(m_hDatabaseConnection, SQL_HANDLE_DBC, SQL_ERROR);
	}
    return true;
}

// CheckReturnCodeForClosing - Check the return code for closing the connection
// - RetCode: the return code to check
bool CSqlNativeAVB::CheckReturnCodeForClosing(SQLRETURN RetCode)
{
    switch ( RetCode ) {
        case SQL_SUCCESS_WITH_INFO:
        {
            HandleDiagnosticRecord(m_hStatement, SQL_HANDLE_STMT, RetCode);
            // fall through
        }
        case SQL_SUCCESS:
        {
            return TryODBC(m_hStatement, SQL_HANDLE_STMT, SQLFreeStmt(m_hStatement, SQL_CLOSE));
            break;
        }

        case SQL_ERROR:
        {
            HandleDiagnosticRecord(m_hStatement, SQL_HANDLE_STMT, RetCode);
            return TryODBC(m_hStatement, SQL_HANDLE_STMT, SQLFreeStmt(m_hStatement, SQL_CLOSE));
            break;
        }

        default:
            ASSERT(fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode));
            break;
    }

    return TryODBC(m_hStatement, SQL_HANDLE_STMT, SQLFreeStmt(m_hStatement, SQL_CLOSE));
}

// CloseConnection - Close the connection to the database
// And set all handles to NULL
void CSqlNativeAVB::CloseConnection() noexcept
{
    if ( m_hStatement ) {
        if (SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement) == SQL_SUCCESS)
			m_hStatement = NULL;
    }

    if ( m_hDatabaseConnection ) {
        SQLDisconnect(m_hDatabaseConnection);
        if (SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection) == SQL_SUCCESS)
            m_hDatabaseConnection = NULL;
    }

    if ( m_hEnvironment ) {
        if ( SQLFreeHandle(SQL_HANDLE_ENV, m_hEnvironment) == SQL_SUCCESS )
            m_hEnvironment = NULL;
    }
}

// TryODBC - Try to execute an ODBC command
// - h: the handle to use
// - ht: the handle type
// - x: the return code
bool CSqlNativeAVB::TryODBC(SQLHANDLE h, SQLSMALLINT ht, SQLRETURN x) noexcept
{
	RETCODE rc = x;
    if (rc != SQL_SUCCESS)
		return HandleDiagnosticRecord(h, ht, rc);
    if (rc == SQL_ERROR)
        return false;

    return true;
}

// HandleDiagnosticRecord - Handle a diagnostic record
bool CSqlNativeAVB::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode) const
{
    SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	wchar_t wszMessage[SQL_QUERY_SIZE];
	wchar_t wszState[SQL_SQLSTATE_SIZE + 1];

    if ( RetCode == SQL_INVALID_HANDLE ) {
        AfxMessageBox(_T("Invalid SQL handle!"), MB_ICONEXCLAMATION);
        return false;
    }

    while ( SQLGetDiagRecW(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
        (SQLSMALLINT)(sizeof(wszMessage) / sizeof(wchar_t)), (SQLSMALLINT*)NULL) == SQL_SUCCESS)  {
        // Hide data truncated..
        if ( wcsncmp(wszState, L"01004", 5) && wcsncmp(wszState, L"01000", 5) ) {
            if ( m_nTryCount == 0 ) {
                wchar_t buffer[256];
                swprintf_s(buffer, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
                AfxMessageBox(buffer, MB_ICONERROR);
                return false; ////?
            }
            return false;
        }
    }
    return true;
}