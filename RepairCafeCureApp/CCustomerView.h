#pragma once



// CCustomerView form view
namespace artvabas
{
	namespace rcc
	{
		namespace ui
		{
			class CCustomerView : public CFormView
			{
				DECLARE_DYNCREATE(CCustomerView)

			public:
				CCustomerView();           // protected constructor used by dynamic creation
				virtual ~CCustomerView();

			public:
#ifdef AFX_DESIGN_TIME
				enum { IDD = IDD_CUSTOMER_FORM };
#endif
#ifdef _DEBUG
				virtual void AssertValid() const;
#ifndef _WIN32_WCE
				virtual void Dump(CDumpContext& dc) const;
#endif
#endif

			protected:
				virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

				DECLARE_MESSAGE_MAP()
			};
		}
	}
}




