#include "pch.h"
#include "resource.h"
#include "securitydata.h"
#include "CDatabaseConnection.h"

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
	: m_dataBaseInstance()
	, m_sDriver(ODBC_DRIVER_NAME)
{
	m_strDsn = GetConnectionString();
}

CDatabaseConnection::~CDatabaseConnection()
{
	if (m_dataBaseInstance.IsOpen())
		m_dataBaseInstance.Close();
}

CString CDatabaseConnection::GetConnectionString()
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
	{
		PathAppend(szPath, APP_DESTINATION_FOLDER);


#ifdef _DEBUG
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
#else
		CString sDsn, sUser, sPassword;
		std::string userCipher, passwordCipher;

		std::string userPathFile = CW2A(szPath);
		userPathFile += APP_CRDSU_FILE;
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(CA2W(userPathFile.c_str())))
		{
			//File not found
			//Create the connection string
			return CreateConnectionString();
		}
		else
		{
			CryptoPP::FileSource fsUser(userPathFile.c_str(), true, new CryptoPP::StringSink(userCipher));
			//fsUser.Get((byte*)userCipher.data(), 256);
		}

		std::string passwordPathFile = CW2A(szPath);
		passwordPathFile += APP_CRDSP_FILE;
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(CA2W(passwordPathFile.c_str())))
		{
			//File not found
			//Create the connection string
			return CreateConnectionString();
		}
		else
		{
			CryptoPP::FileSource fsPassword(passwordPathFile.c_str(), true, new CryptoPP::StringSink(passwordCipher));
			//fsPassword.Get((byte*)passwordCipher.data(), 256);
		}

		PathAppendW(szPath, APP_INI_RELEASE_FILE);
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
#endif
	}
	else
	{
		AfxMessageBox(_T("Unable to load DSN (Data Server Name)"));
	}
	return _T("");
}

CString CDatabaseConnection::CreateConnectionString()
{
	CString sDsn;

	//Use SQL Server authentication Dialog for creating the connection string
	sDsn.Format(static_cast<CString>(_T("DRIVER={%s}")), m_sDriver);

	try
	{
		if (m_dataBaseInstance.Open(NULL, false, false, sDsn))
		{
			sDsn = m_dataBaseInstance.GetConnect();
			m_dataBaseInstance.Close();

			//Remove the password from the connection string
			CString sUser = sDsn.Mid(sDsn.Find(_T("UID=")) + 4);
			sUser = sUser.Left(sUser.Find(_T(";")));
			CString sPassword = sDsn.Mid(sDsn.Find(_T("PWD=")) + 4);
			sPassword = sPassword.Left(sPassword.Find(_T(";")));
			RemoveCredentials(sUser, sPassword, sDsn);
			SaveConnectionString(sUser, sPassword, sDsn);

#ifndef _DEBUG
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

void CDatabaseConnection::RemoveCredentials(CString& strUser, CString& strPassword, CString& sDsn)
{
	sDsn.Replace(strUser, _T(""));
	sDsn.Replace(strPassword, _T(""));
}

void CDatabaseConnection::SaveConnectionString(CString& strUser, CString& strPassword, CString& strDsn)
{
	try
	{
		TCHAR szPath[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
		{
			// save
			PathAppendW(szPath, APP_DEV_NAME);
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				CreateDirectoryW(szPath, NULL);
			}

			PathAppendW(szPath, APP_NAME);
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
			{
				CreateDirectoryW(szPath, NULL);
			}
#ifndef _DEBUG
			PathAppendW(szPath, APP_INI_RELEASE_FILE);
#else
			PathAppendW(szPath, APP_INI_DEBUG_FILE);
#endif

			CStdioFile file(szPath, CFile::modeCreate | CFile::modeWrite);
			CString strLine;
			strLine.Format(static_cast<CString>(_T("[ConnectionString]=%s")), strDsn);
			file.WriteString(strLine);

#ifdef _DEBUG
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

void CDatabaseConnection::AddCredentials(CString& strUser, CString& strPassword, CString& strDsn)
{
	strDsn.Replace(_T("UID="), _T("UID=") + strUser);
	strDsn.Replace(_T("PWD="), _T("PWD=") + strPassword);
}

#ifndef _DEBUG
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

		// Save private key
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
		{
			PathAppendW(szPath, APP_DESTINATION_FOLDER);

			std::string userPathFile = CW2A(szPath);
			userPathFile += APP_CRDSU_FILE;

			CryptoPP::FileSink fsUser(userPathFile.c_str());
			fsUser.Put((byte*)cipherUser.data(), cipherUser.size());

			std::string passwordPathFile = CW2A(szPath);
			passwordPathFile += APP_CRDSP_FILE;

			CryptoPP::FileSink fsPassword(passwordPathFile.c_str());
			fsPassword.Put((byte*)cipherPassword.data(), cipherPassword.size());

			PathAppendW(szPath, APP_CRDSK_SHORT_FILE);
			CryptoPP::ByteQueue privateKeyBytes;
			privateKey.Save(privateKeyBytes);
			CryptoPP::FileSink privateKeyFile(szPath);
			privateKeyBytes.CopyTo(privateKeyFile);
			privateKeyFile.MessageEnd();
		}


	}
	catch (CryptoPP::Exception& e)
	{
		AfxMessageBox(static_cast<CString>(e.what()));
		OutputDebugStringA(e.what());
	}

}

void CDatabaseConnection::DecryptCredentials(std::string& chpUser, std::string& chpPassword, CString& strUser, CString& strPassword)
{
	try
	{
		TCHAR szPath[MAX_PATH];
		CryptoPP::RSA::PrivateKey privateKey;
		CryptoPP::AutoSeededRandomPool rng;

		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_TOKEN, NULL, 0, szPath)))
		{
			PathAppendW(szPath, APP_CRDSK_LONG_FILE);
			CryptoPP::FileSource privateKeyFile(szPath, true);
			privateKey.Load(privateKeyFile);
		}

		//// Decryption
		CryptoPP::RSAES_OAEP_SHA_Decryptor d(privateKey);

		std::string recoveredUser;

		CryptoPP::StringSource ssUser(chpUser, true,
			new CryptoPP::PK_DecryptorFilter(rng, d,
				new CryptoPP::StringSink(recoveredUser)
				) // PK_DecryptorFilter
			); // StringSource
			
		
		strUser = CA2W(recoveredUser.c_str());

		std::string recoveredPassword;

		CryptoPP::StringSource ssPassword(chpPassword, true,
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
	
}
#endif

