#pragma once

namespace artvabas
{
	namespace rcc
	{
		namespace database
		{
			class CDatabaseConnection
			{
				// Attributes
			private:
				CString m_sDriver;
				CString m_strDsn;
				CDatabase m_dataBaseInstance;

				// Operations
			private:
				CString GetConnectionString();
				CString CreateConnectionString();
				void EncryptCredentials(CString strUser, CString strPassword);
				void DecryptCredentials(std::string& chpUser, std::string& chpPassword, CString& strUser, CString& strPassword);
				void RemoveCredentials(CString& strUser, CString& strPassword, CString& strDsn);
				void SaveConnectionString(CString& strUser, CString& strPassword, CString& strDsn);
				void AddCredentials(CString& strUser, CString& strPassword, CString& strDsn);

				// Overrides
			public:


				// Implementation
			public:
				CDatabaseConnection();
				virtual ~CDatabaseConnection();
			};
		}
	}
}

