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
				CListCtrl m_ctlExistingCustomersList;
				
				CButton m_btnCustomerSurnameSearch;
				CButton m_btnAddNewCustomer;
				CButton m_btnAddCustomer;
				CButton m_btnCustomAssets;
				CButton m_btnUpdateCustomer;

				CEdit m_ctrCustomerCellPhone;
				CEdit m_ctrCustomerComment;
				CEdit m_ctrCustomerLog;
				CEdit m_ctrCustomerName;
				CEdit m_ctrCustomerPhone;
				CEdit m_ctrCustomerSurname;
				CEdit m_ctrlCustomerEmail;

				CString m_strSearchCustomerSurname;
				CString m_strCustomerCellPhone;
				CString m_strCustomerComment;
				CString m_strCustomerLog;
				CString m_strCustomerName;
				CString m_strCustomerPhone;
				CString m_strCustomerSurname;
				CString m_strCustomerEmail;
				int m_nCustomerID;

				bool m_bIsNewCustomer;
				bool m_bIsDirtyCustomerDetails;

			public:
				CCustomerView();          
				virtual ~CCustomerView();

			private:
#ifdef AFX_DESIGN_TIME
				enum { IDD = IDD_CUSTOMER_FORM };
#endif
#ifdef _DEBUG
				void AssertValid() const override;
#ifndef _WIN32_WCE
				void Dump(CDumpContext& dc) const override;;
#endif
#endif
			private:
				BOOL PreTranslateMessage(MSG* pMsg) override;
				void OnInitialUpdate() override;
				void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

				DECLARE_MESSAGE_MAP()
			private:
				afx_msg void OnClickedCustomViewButtonSearch();
				afx_msg void OnChangeCustomViewEditBoxSurnameSearch();
				afx_msg void OnUpdateUIState(UINT nAction, UINT nUIElement);
				afx_msg void OnDoubleClickCustViewCustomerList(NMHDR* pNMHDR, LRESULT* pResult);
				afx_msg void OnChangeCustViewCustomerDetails();
				afx_msg void OnClickedCustViewButtonAddNewCustomer();
				afx_msg void OnClickedCustViewButtonCustomerAdd();
				afx_msg void OnClickedCustViewButtonCustomerUpdate();

			private:
				void DisableCustomerSearchAndAddButtons();
				void UpdateCustomerDetailsControls(BOOL bShow = TRUE);
				void DisableCustomerDetailsButtons();
				void EmptyCustomerDetailsControls();
				void EmptyAndDisableExistingCustomersList();
			};
		}
	}
}