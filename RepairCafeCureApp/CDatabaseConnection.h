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
				std::string ConnectionString();

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

