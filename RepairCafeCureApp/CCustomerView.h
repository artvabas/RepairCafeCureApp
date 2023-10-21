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

			private:
				CString m_strSearchCustomerSurname;
				CButton m_btnCustomerSurnameSearch;
				CButton m_btnAddNewCustomer;

			public:
				CCustomerView();          
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
			private:
				virtual BOOL PreTranslateMessage(MSG* pMsg);

			protected:
				virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

				DECLARE_MESSAGE_MAP()
			private:
				afx_msg void OnClickedCustomViewButtonSearch();
				afx_msg void OnChangeCustomViewEditBoxSurnameSearch();
				afx_msg void OnUpdateUIState(UINT nAction, UINT nUIElement);
			};
		}
	}
}




