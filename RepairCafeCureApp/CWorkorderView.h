#pragma once



// CWorkorderView form view

//namespace artvabas
//{
	//namespace rcc
	//{
		//namespace ui
		//{
			class CWorkorderView : public CFormView
			{
				DECLARE_DYNCREATE(CWorkorderView)

			private:
				unsigned int m_nWorkorderId;
				CString m_strCustomerSurname;
				CString m_strCustomerName;
				CString m_strCustomerCellPhone;
				CString m_strCustomerPhone;
				CString m_strCustomerEmail;
				CString m_strCustomerComments;
				CString m_strAssetDescription;
				CString m_strAssetModelNumber;
				CString m_strAssetBrand;
				CString m_strAssetHistoryLog;
				CString m_strWorkorderCreatedDate;
				CString m_strWorkorderCreatedBy;
				CString m_strWorkorderStatus;
				CString m_strWorkorderNewLog;
				CString m_strWorkorderHistoryLog;

				CEdit m_edtWorkorderNewLog;

				CButton m_chbWorkorderAssetDisposed;
				CButton m_chbWorkorderContactedCustomer;

				CButton m_btnWorkorderUpdate;
				CButton m_btnWorkorderFinished;
				CButton m_btnWorkorderParts;

				CComboBox m_cbxWorkorderEmployeeResponsible;

				CListCtrl m_lscWorkorderExisting;
				CListCtrl m_lscWorkorderSpareParts;

				bool m_bWorkorderSelected;
				bool m_bResponsibleChanged;

			public:
				CWorkorderView();           // protected constructor used by dynamic creation
				virtual ~CWorkorderView();

			/* Custom */
			private:
				void InitWorkorderExistingList();
				void InitWorkorderEmployeeResponsibleComboBox();
				bool GetAssetInfo(const unsigned int& nAssetId);
				bool GetCustomerInfo(const unsigned int& nCustomerId);

			/* Overrides */
			private:
#ifdef AFX_DESIGN_TIME
				enum { IDD = IDD_WORKORDER_FORM };
#endif
#ifdef _DEBUG
				void AssertValid() const override;
#ifndef _WIN32_WCE
				void Dump(CDumpContext& dc) const override;
#endif
#endif
				void DoDataExchange(CDataExchange* pDX) override;
				void OnInitialUpdate() override;

			/* Massagers */
			private:
				DECLARE_MESSAGE_MAP()
				afx_msg void OnUpdateUIState(UINT nAction, UINT nUIElement);
				afx_msg void OnNMDoubleClickWorkorderViewExisting(NMHDR* pNMHDR, LRESULT* pResult);
				afx_msg void OnCbnSelectChangeWorkorderViewResponsible();
				afx_msg void OnEnChangeWorkorderViewLog();
				afx_msg void OnBnClickedWorkorderViewUpdate();
			};
		//}
	//}
//}



