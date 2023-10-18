#pragma once



// CWorkorderView form view

namespace artvabas
{
	namespace rcc
	{
		namespace ui
		{
			class CWorkorderView : public CFormView
			{
				DECLARE_DYNCREATE(CWorkorderView)

			public:
				CWorkorderView();           // protected constructor used by dynamic creation
				virtual ~CWorkorderView();

			public:
#ifdef AFX_DESIGN_TIME
				enum { IDD = IDD_WORKORDER_FORM };
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



