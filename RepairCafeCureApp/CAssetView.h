/*
	Copyright (C) 2023/24  artvabas

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
* File: CAssetView.h, defines class CAssetView
*
* This class is the view of the Asset form
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.1 (alpha)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 02-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#pragma once
namespace artvabas::rcc::ui {

	class CAssetView : public CFormView
	{
		DECLARE_DYNCREATE(CAssetView)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ASSET_FORM };
#endif
	private:
		BOOL m_chkHistorySearchDetailsIsDisposed;

		CMFCButton m_btnHistorySearchClear;
		CMFCButton m_btnHistorySearchSearch;

		CListCtrl m_lstHistorySearchResults;

		CString m_strHistorySearchAssetDescription;
		CString m_strHistorySearchWorkorderDescription;
		CString m_strHistorySearchHistoryLog;

		CString m_strHistorySearchDetailsAssetDescription;
		CString m_strHistorySearchDetailsAssetModelNumber;
		CString m_strHistorySearchDetailsAssetBrand;

		CString m_strHistorySearchDetailsWorkorderCreated;
		CString m_strHistorySearchDetailsWorkorderCreatedBy;
		CString m_strHistorySearchDetailsWorkorderResponsible;
		CString m_strHistorySearchDetailsWorkorderHistoryLog;

	public:
		CAssetView() noexcept;
		virtual ~CAssetView();

	private:
		BOOL PreTranslateMessage(MSG* pMsg) override;
		void OnInitialUpdate() override;
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
		void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;
#ifdef _DEBUG
		void AssertValid() const override;
		void Dump(CDumpContext& dc) const override;
#endif
	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnFilePrintPreview() noexcept;
		afx_msg void OnClickedHistorySearch();
		afx_msg void OnClickedHistorySearchClear();
		afx_msg void OnClickSearchResultsItem(NMHDR* pNMHDR, LRESULT* pResult);

	};
}