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
* File: CDatabaseConnection.cpp, implemented in CDatabaseConnection.h
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

#include "pch.h"
#include "securitydata.h"	// For security reasons, this file is not included in the repository.
#include "CDatabaseConnection.h"

// Because of exception: An invalid parameter was passed to a function that considers invalid parameters fatal.
// The CryptoPP library disabled in debug mode. In release mode, the library is enabled and works fine.
#ifndef _DEBUG
#include <rsa.h>
#include <osrng.h>
#include <hex.h>
#include <files.h>
#include <base64.h>
#endif

using namespace artvabas::rcc::database;
using namespace artvabas::rcc::database::security::data;

CDatabaseConnection::CDatabaseConnection()
	//: m_dataBaseInstance()
	: m_sDriver(ODBC_DRIVER_NAME)
{
	m_strDsn = GetConnectionString();
	
}

CDatabaseConnection::~CDatabaseConnection()
{
	if (IsOpen()) Close();
}

/* General public methods */


/// <summary>
/// Gets the connection string, for the Native SQL Class (CSqlNativeAVB).
/// </summary>
/// <returns>Connection string</returns>
wchar_t* CDatabaseConnection::ConnectionString()
{
	wchar_t* pwszDsn = m_strDsn.GetBuffer();
	m_strDsn.ReleaseBuffer();

	if (m_strDsn.GetLength() > 0)
	{
		auto pos = m_strDsn.Find(_T("DRIVER="));
		if(pos != 0)
			pwszDsn += pos;
		return pwszDsn;
	}
	return nullptr;
}

/// <summary>
/// Opens the query.
/// </summary>
/// <param name="rcsRecords">The CRecordSet instance as reference.</param>
/// <param name="strQuery">The string query.</param>
/// <returns>TRUE if successful, FALSE otherwise</returns>

/**/
BOOL CDatabaseConnection::OpenQuery(CRecordset* rcsRecords, CString& strQuery)
{ //CDatabase::noOdbcDialog
	try
	{
		BOOL b = FALSE;
		SetLoginTimeout(60);
		if (Open(NULL, false, false, m_strDsn))
		{
			if (rcsRecords->m_pDatabase == NULL)
			{
				rcsRecords->m_pDatabase = this;
				b = rcsRecords->Open(AFX_DB_USE_DEFAULT_TYPE, strQuery );
			}

			return b;
		}
		else {
			AfxMessageBox(_T("Unable to open database"));
			return FALSE;
		}

	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		AfxMessageBox(_T("Unable to open database"));
		return FALSE;
	}
}

/// <summary>
/// Closes the query.
/// </summary>
/// <param name="rcsRecords">The CRecordSet instance as reference.</param>
void CDatabaseConnection::CloseQuery(CRecordset* rcsRecords)
{
	rcsRecords->Close();
	Close();
}
/**/

/* General private methods */

/// <summary>
/// Gets the connection string (DSN).
/// The connection string is saved in a file. If the file or parts of the file, is not found, the connection string is created.
/// The connection string is encrypted in release mode, and plain text in debug mode.
/// </summary>
/// <returns>CString with DNS</returns>
/// <exception cref="CryptoPP::Exception">e.what()</exception>
/// <exception cref="CFileException">e->m_strFileName</exception>
/// <exception cref="...">Unable to load DSN (Data Server Name)</exception>
CString CDatabaseConnection::GetConnectionString()
{
	TCHAR szPath[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
	{
		PathAppend(szPath, APP_DESTINATION_FOLDER);

		
#ifdef _DEBUG // Debug mode, use debug file for connection string (plain text)
		PathAppendW(szPath, APP_INI_DEBUG_FILE);

		// Does the file exist?	
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
		{
			//File not found
			//Create the connection string
			return CreateConnectionString();
		}
		else
		{
			try
			{
				//File found
				CString sDsn, sUser, sPassword;
				//Read the connection string from the file
				CStdioFile file(szPath, CFile::modeRead);
				CString strLine;
				while (file.ReadString(strLine))
				{
					if (strLine.Find(_T("[ConnectionString]")) != -1)
					{
						sDsn = strLine.Mid(strLine.Find(_T("=")) + 1);
					}
					if (strLine.Find(_T("[User]")) != -1)
					{
						sUser = strLine.Mid(strLine.Find(_T("=")) + 1);
					}
					if (strLine.Find(_T("[Password]")) != -1)
					{
						sPassword = strLine.Mid(strLine.Find(_T("=")) + 1);
					}
				}
				file.Close();
				AddCredentials(sUser, sPassword, sDsn);
				return sDsn;
			}
			catch (CFileException* e)
			{
				AfxMessageBox(e->m_strFileName);
				e->Delete();
				return _T("");
			}
			catch (...)
			{
				AfxMessageBox(_T("Unable to load DSN (Data Server Name)"));
			}
		}
#else	// Release mode, use encrypted file for connection string
		try
		{
			CString sDsn, sUser, sPassword;
			std::string userCipher, passwordCipher;

			std::string userPathFile = CW2A(szPath);
			userPathFile += APP_CRDSU_FILE;

			// Does the file exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(CA2W(userPathFile.c_str())))
			{
				//File not found
				//Create the connection string
				return CreateConnectionString();
			}
			else
			{
				CryptoPP::FileSource fsUser(userPathFile.c_str(), true, new CryptoPP::StringSink(userCipher));
			}

			std::string passwordPathFile = CW2A(szPath);
			passwordPathFile += APP_CRDSP_FILE;

			// Does the file exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(CA2W(passwordPathFile.c_str())))
			{
				//File not found
				//Create the connection string
				return CreateConnectionString();
			}
			else
			{
				CryptoPP::FileSource fsPassword(passwordPathFile.c_str(), true, new CryptoPP::StringSink(passwordCipher));
			}

			PathAppendW(szPath, APP_INI_RELEASE_FILE);

			// Does the file exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				//File not found
				//Create the connection string
				return CreateConnectionString();
			}
			else
			{
				//File found
				//Read the connection string from the file
				CStdioFile file(szPath, CFile::modeRead);
				CString strLine;
				while (file.ReadString(strLine))
				{
					if (strLine.Find(_T("[ConnectionString]")) != -1)
					{
						sDsn = strLine.Mid(strLine.Find(_T("=")) + 1);
					}
				}
				file.Close();
			}

			DecryptCredentials(userCipher, passwordCipher, sUser, sPassword);
			AddCredentials(sUser, sPassword, sDsn);
			return sDsn;
		}
		catch (CryptoPP::Exception& e)
		{
			AfxMessageBox(static_cast<CString>(e.what()));
			OutputDebugStringA(e.what());
		}
		catch (CFileException* e)
		{
			AfxMessageBox(e->m_strFileName);
			e->Delete();
			return _T("");
		}
		catch (...)
		{
			AfxMessageBox(_T("Unable to load DSN (Data Server Name)"));
		}
#endif
	}
	else
	{
		AfxMessageBox(_T("Unable to load DSN (Data Server Name)"));
	}
	return _T("");
}

/// <summary>
/// Creates the connection string.
/// The connection string is saved in a file, without credentials. The credentials are saved in a separate file.
/// But this function returns the connection string (DSN) with credentials.
/// The connection string is encrypted in release mode, and plain text in debug mode.
/// </summary>
/// <returns>CString with DNS</returns>
/// <exception cref="CDBException"e->m_strError</exception>
/// <exception cref="...">>Unable to open database</exception>
CString CDatabaseConnection::CreateConnectionString()
{
	CString sDsn;

	//Use SQL Server authentication Dialog for creating the connection string
	sDsn.Format(static_cast<CString>(_T("DRIVER={%s}")), m_sDriver);

	try
	{
		
		if (/*m_dataBaseInstance.*/Open(NULL, false, false, sDsn))
		{
			sDsn = /*m_dataBaseInstance.*/GetConnect();
			/*m_dataBaseInstance.*/Close();

			//Get the credentials from the connection string
			// User ID
			CString sUser = sDsn.Mid(sDsn.Find(_T("UID=")) + 4);
			sUser = sUser.Left(sUser.Find(_T(";")));
			// Password
			CString sPassword = sDsn.Mid(sDsn.Find(_T("PWD=")) + 4);
			sPassword = sPassword.Left(sPassword.Find(_T(";")));

			RemoveCredentials(sUser, sPassword, sDsn);
			SaveConnectionString(sUser, sPassword, sDsn);

#ifndef _DEBUG	// Release mode, encrypt credentials
			//Encrypt the credentials
			EncryptCredentials(sUser, sPassword);
#endif
			AddCredentials(sUser, sPassword, sDsn);

			return sDsn;
		}
		else
		{
			AfxMessageBox(_T("Unable to open database"));
			return _T("");
		}
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		e->Delete();
		return _T("");
	}
	catch (...)
	{
		AfxMessageBox(_T("Unable to open database"));
		return _T("");
	}
}

/// <summary>
/// Removes the credentials from the connection string.
/// </summary>
/// <param name="strUser">The user.</param>
/// <param name="strPassword">The password.</param>
/// <param name="sDsn">The DSN.</param>
void CDatabaseConnection::RemoveCredentials(CString& strUser, CString& strPassword, CString& sDsn)
{
	sDsn.Replace(strUser, _T(""));
	sDsn.Replace(strPassword, _T(""));
}

/// <summary>
/// Saves the connection string.
/// </summary>
/// <param name="strUser">The user.</param>
/// <param name="strPassword">The password.</param>
/// <param name="strDsn">The DSN.</param>
/// <exception cref="CFileException">e->m_strFileName</exception>
/// <exception cref="...">Unable to save connection string</exception>
void CDatabaseConnection::SaveConnectionString(CString& strUser, CString& strPassword, CString& strDsn)
{
	try
	{
		TCHAR szPath[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
		{
			PathAppendW(szPath, APP_DEV_NAME);
			// Does the folder exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				// Folder not found, create it
				CreateDirectoryW(szPath, NULL);
			}

			PathAppendW(szPath, APP_NAME);
			// Does the folder exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				// Folder not found, create it
				CreateDirectoryW(szPath, NULL);
			}

#ifndef _DEBUG	// Release mode, use encrypted file for connection string
			PathAppendW(szPath, APP_INI_RELEASE_FILE);
#else
			PathAppendW(szPath, APP_INI_DEBUG_FILE);	// Debug mode, use debug file for connection string (plain text)
#endif
			// Save the connection string to the file
			CStdioFile file(szPath, CFile::modeCreate | CFile::modeWrite);
			CString strLine;
			strLine.Format(static_cast<CString>(_T("[ConnectionString]=%s")), strDsn);
			file.WriteString(strLine);

#ifdef _DEBUG	// Debug mode, save credentials in debug file (plain text)
			file.WriteString(_T("\n"));
			strLine.Format(static_cast<CString>(_T("[User]=%s")), strUser);
			file.WriteString(strLine);
			file.WriteString(_T("\n"));
			strLine.Format(static_cast<CString>(_T("[Password]=%s")), strPassword);
			file.WriteString(strLine);
#endif
			file.Close();
		}
	}
	catch (CFileException* e)
	{
		AfxMessageBox(e->m_strFileName);
		e->Delete();
	}
	catch (...)
	{
		AfxMessageBox(_T("Unable to save connection string"));
	}
}

/// <summary>
/// Adds the credentials to the connection string.
/// </summary>
/// <param name="strUser">The user.</param>
/// <param name="strPassword">The password.</param>
/// <param name="strDsn">The DSN.</param>
void CDatabaseConnection::AddCredentials(CString& strUser, CString& strPassword, CString& strDsn)
{
	strDsn.Replace(_T("UID="), _T("UID=") + strUser);
	strDsn.Replace(_T("PWD="), _T("PWD=") + strPassword);
}

#ifndef _DEBUG // Release mode, encrypt credentials
/// <summary>
/// Encrypts the credentials.
/// </summary>
/// <param name="strUser">The user.</param>
/// <param name="strPassword">The password.</param>
/// <exception cref="CryptoPP::Exception">e.what()</exception>
/// <exception cref="const char*">msg</exception>
/// <exception cref="...">Unable to save encrypted credentials</exception>
void CDatabaseConnection::EncryptCredentials(CString strUser, CString strPassword)
{
	try
	{
		TCHAR szPath[MAX_PATH];
		
		// Generate keys
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::RSA::PrivateKey privateKey;
		privateKey.GenerateRandomWithKeySize(rng, 2048);
		CryptoPP::RSA::PublicKey publicKey(privateKey);

		// Encryption user
		std::string plainUser = CW2A(strUser), cipherUser;
		CryptoPP::RSAES_OAEP_SHA_Encryptor eUser(publicKey);
		CryptoPP::StringSource ssUser(plainUser, true,
			new CryptoPP::PK_EncryptorFilter(rng, eUser,
				new CryptoPP::StringSink(cipherUser)
				) // PK_EncryptorFilter
			); // StringSource

		strUser = CA2W(cipherUser.c_str());

		// Encryption password
		std::string plainPassword = CW2A(strPassword), cipherPassword;
		CryptoPP::RSAES_OAEP_SHA_Encryptor ePassword(publicKey);
		CryptoPP::StringSource ssPassword(plainPassword, true,
			new CryptoPP::PK_EncryptorFilter(rng, ePassword,
				new CryptoPP::StringSink(cipherPassword)
			) // PK_EncryptorFilter
		); // StringSource

		strPassword = CA2W(cipherPassword.c_str());

		// Save encrypted credentials and private key
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
		{
			PathAppendW(szPath, APP_DESTINATION_FOLDER);
			// Does the folder exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				// Folder not found
				throw(_T("Folder not found"));
			}
			else
			{
				// Save encrypted user
				std::string userPathFile = CW2A(szPath);
				userPathFile += APP_CRDSU_FILE;
				CryptoPP::FileSink fsUser(userPathFile.c_str());
				fsUser.Put((byte*)cipherUser.data(), cipherUser.size());

				// Save encrypted password
				std::string passwordPathFile = CW2A(szPath);
				passwordPathFile += APP_CRDSP_FILE;
				CryptoPP::FileSink fsPassword(passwordPathFile.c_str());
				fsPassword.Put((byte*)cipherPassword.data(), cipherPassword.size());
			}

			//PathAppendW(szPath, APP_CRDSK_SHORT_FILE);
			// Does the folder exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				// Folder not found
				throw(_T("File not found, key-Save"));
			}
			else
			{
				// Save private key
				PathAppendW(szPath, APP_CRDSK_SHORT_FILE);
				CryptoPP::ByteQueue privateKeyBytes;
				privateKey.Save(privateKeyBytes);
				CryptoPP::FileSink privateKeyFile(szPath);
				privateKeyBytes.CopyTo(privateKeyFile);
				privateKeyFile.MessageEnd();
			}
		}
	}
	catch (CryptoPP::Exception& e)
	{
		AfxMessageBox(static_cast<CString>(e.what()));
		OutputDebugStringA(e.what());
	}
	catch (const wchar_t* msg)
	{
		AfxMessageBox(msg);
	}
	catch (...)
	{
		AfxMessageBox(_T("Unable to save encrypted credentials"));
	}

}

/// <summary>
/// Decrypts the credentials.
/// </summary>
/// <param name="cphUser">The Cipher user.</param>
/// <param name="cphPassword">The Cipher password.</param>
/// <param name="strUser">The string user.</param>
/// <param name="strPassword">The string password.</param>
/// <exception cref="CryptoPP::Exception">e.what()</exception>
/// <exception cref="const char*">msg</exception>
/// <exception cref="...">Unable to load encrypted credentials</exception>
void CDatabaseConnection::DecryptCredentials(std::string& cphUser, std::string& cphPassword, CString& strUser, CString& strPassword)
{
	try
	{
		TCHAR szPath[MAX_PATH];
		CryptoPP::RSA::PrivateKey privateKey;
		CryptoPP::AutoSeededRandomPool rng;

		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
		{
			PathAppendW(szPath, APP_DESTINATION_FOLDER);
			PathAppendW(szPath, APP_CRDSK_LONG_FILE);
			// Does the folder exist?
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				// Folder not found
				throw(_T("File not found, Key-load"));
			}
			else
			{
				CryptoPP::FileSource privateKeyFile(szPath, true);
				privateKey.Load(privateKeyFile);
			}
		}

		//// Decryption
		CryptoPP::RSAES_OAEP_SHA_Decryptor d(privateKey);

		// Decryption user
		std::string recoveredUser;
		CryptoPP::StringSource ssUser(cphUser, true,
			new CryptoPP::PK_DecryptorFilter(rng, d,
				new CryptoPP::StringSink(recoveredUser)
				) // PK_DecryptorFilter
			); // StringSource
		strUser = CA2W(recoveredUser.c_str());

		// Decryption password
		std::string recoveredPassword;
		CryptoPP::StringSource ssPassword(cphPassword, true,
			new CryptoPP::PK_DecryptorFilter(rng, d,
				new CryptoPP::StringSink(recoveredPassword)
			) // PK_DecryptorFilter
		); // StringSourc
		strPassword = CA2W(recoveredPassword.c_str());
	}
	catch (CryptoPP::Exception& e)
	{
		AfxMessageBox(static_cast<CString>(e.what()));
		OutputDebugStringA(e.what());
	}
	catch (const wchar_t* msg)
	{
		AfxMessageBox(msg);
	}
	catch (...)
	{
		AfxMessageBox(_T("Unable to load encrypted credentials"));
	}
	
}
#endif

