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
* File: CWorkorderView.h, defines class CWorkorderView
*
* This class is the view of the workorder form.
* With this form the user can search for existing workorders and update existing workorders.
* 
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 15-11-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

#pragma once

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
				unsigned int m_unWorkorderId;
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
				CString m_strWorkorderDescription;
				CString m_strWorkorderStatus;
				CString m_strWorkorderClosedDate;
				CString m_strWorkorderNewLog;
				CString m_strWorkorderHistoryLog;
				CString m_strWorkorderTotalPartsPrice;

				CEdit m_edtWorkorderNewLog;

				CButton m_chbWorkorderAssetDisposed;
				CButton m_chbWorkorderContactedCustomer;

				CButton m_btnWorkorderUpdate;
				CButton m_btnWorkorderFinished;
				CButton m_btnWorkorderClose;
				CButton m_btnWorkorderParts;

				CComboBox m_cbxWorkorderEmployeeResponsible;

				CListCtrl m_lscWorkorderExisting;
				CListCtrl m_lscWorkorderSpareParts;

				bool m_bWorkorderSelected;
				bool m_bResponsibleChanged;
				bool m_bPrintCombi;
				bool m_bPrintInvoice;

				CDC* m_pDC;

			public:
				CWorkorderView(); 
				virtual ~CWorkorderView();

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
				BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
				void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
				void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;
				void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;

			/* Massagers */
			private:
				DECLARE_MESSAGE_MAP()
				afx_msg void OnUpdateUIState(UINT nAction, UINT nUIElement);
				afx_msg void OnNMDoubleClickWorkorderViewExisting(NMHDR* pNMHDR, LRESULT* pResult);
				afx_msg void OnCbnSelectChangeWorkorderViewResponsible();
				afx_msg void OnEnChangeWorkorderViewLog();
				afx_msg void OnBnClickedWorkorderViewUpdate();
				afx_msg void OnBnClickedWorkorderVewCustomerContactedCustomer();
				afx_msg void OnBnClickedWorkorderViewFinished();
				afx_msg void OnBnClickedWorkorderViewClose();
				afx_msg void OnBnClickedWorkorderViewAssetDisposed();
				afx_msg void OnBnClickedWorkorderViewParts();
				afx_msg void OnFilePrintPreview();

				/* Custom */
			private:
				void InitWorkorderExistingList();
				void InitWorkorderEmployeeResponsibleComboBox();
				void InitWorkorderSparePartsList();
				void SetControlsAfterChangeContactedOrDisposed();
				bool GetAssetInfo(const unsigned int& nAssetId);
				bool GetCustomerInfo(const unsigned int& nCustomerId);
				void PerformWorkorderUpdate();
				void ResetAllControls();
			public:
				afx_msg void OnWorkorderExtraCombi();
				afx_msg void OnWorkorderExtraInvoice();
			};
		//}
	//}
//}



