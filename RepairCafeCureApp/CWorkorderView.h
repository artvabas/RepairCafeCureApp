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

				CButton m_chbWorkorderAssetDisposed;
				CButton m_chbWorkorderContactedCustomer;

				CButton m_btnWorkorderUpdate;
				CButton m_btnWorkorderFinished;

				CComboBox m_cbxWorkorderEmployeeResponsible;

				CListCtrl m_lscWorkorderExisting;
				CListCtrl m_lscWorkorderSpareParts;

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
		//}
	//}
//}



