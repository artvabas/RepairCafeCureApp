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
* File: CAssetView.h, implements class CAssetView
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

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "RepairCafeCureAppDoc.h"
#include "CAssetView.h"
#include "DatabaseTables.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace artvabas::rcc::ui;
using namespace artvabas::sql;
using namespace artvabas::database::tables::historysearch;

IMPLEMENT_DYNCREATE(CAssetView, CFormView)

CAssetView::CAssetView() noexcept
	: CFormView(IDD_ASSET_FORM)
	, m_chkHistorySearchDetailsIsDisposed(FALSE)
	, m_strHistorySearchAssetDescription(_T(""))
	, m_strHistorySearchWorkorderDescription(_T(""))
	, m_strHistorySearchHistoryLog(_T(""))
	, m_strHistorySearchDetailsAssetDescription(_T(""))
	, m_strHistorySearchDetailsAssetModelNumber(_T(""))
	, m_strHistorySearchDetailsAssetBrand(_T(""))
	, m_strHistorySearchDetailsWorkorderCreated(_T(""))
	, m_strHistorySearchDetailsWorkorderCreatedBy(_T(""))
	, m_strHistorySearchDetailsWorkorderResponsible(_T(""))
	, m_strHistorySearchDetailsWorkorderHistoryLog(_T(""))
{}

CAssetView::~CAssetView()
{}

/* message handle binders */
BEGIN_MESSAGE_MAP(CAssetView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAssetView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_SEARCH_SEARCH, &CAssetView::OnClickedHistorySearch)
	ON_BN_CLICKED(IDC_SEARCH_CLEAR, &CAssetView::OnClickedHistorySearchClear)
	ON_NOTIFY(NM_CLICK, IDC_SEARCH_RESULTS, &CAssetView::OnClickSearchResultsItem)
END_MESSAGE_MAP()

/* Override methods */

BOOL CAssetView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			OnClickedHistorySearch();
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

// DoDataExchange: Exchange and validate form data between controls and class members.
// - pDX: The data exchange object
void CAssetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SEARCH_RESULT_DETAILS_WORKORDER_ASSET_DISPOSED, m_chkHistorySearchDetailsIsDisposed);
	DDX_Control(pDX, IDC_SEARCH_CLEAR, m_btnHistorySearchClear);
	DDX_Control(pDX, IDC_SEARCH_SEARCH, m_btnHistorySearchSearch);
	DDX_Control(pDX, IDC_SEARCH_RESULTS, m_lstHistorySearchResults);
	DDX_Text(pDX, IDC_SEARCH_ASSET_DESCRIPTION, m_strHistorySearchAssetDescription);
	DDX_Text(pDX, IDC_SEARCH_WORKORDER_DESCRIPTION, m_strHistorySearchWorkorderDescription);
	DDX_Text(pDX, IDC_SEARCH_WORKORDER_HISTORY_LOG, m_strHistorySearchHistoryLog);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_ASSET_DESCRIPTION, m_strHistorySearchDetailsAssetDescription);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_ASSET_MODEL_NUMBER, m_strHistorySearchDetailsAssetModelNumber);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_ASSET_BRAND, m_strHistorySearchDetailsAssetBrand);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_WORKORDER_CREATED_DATE, m_strHistorySearchDetailsWorkorderCreated);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_WORKORDER_CREATED_BY, m_strHistorySearchDetailsWorkorderCreatedBy);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_WORKORDER_RESPOSIBLE, m_strHistorySearchDetailsWorkorderResponsible);
	DDX_Text(pDX, IDC_SEARCH_RESULT_DETAILS_WORKORDER_HISTORY_LOG, m_strHistorySearchDetailsWorkorderHistoryLog);
}

// OnInitialUpdate: Called after the view is first attached to the document
void CAssetView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_lstHistorySearchResults.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstHistorySearchResults.InsertColumn(0, _T("Asset Description"), LVCFMT_LEFT, 100);
	m_lstHistorySearchResults.InsertColumn(1, _T("Asset Model Number"), LVCFMT_LEFT, 0);
	m_lstHistorySearchResults.InsertColumn(2, _T("Asset Brand"), LVCFMT_LEFT, 0);
	m_lstHistorySearchResults.InsertColumn(3, _T("Asset Disposed"), LVCFMT_LEFT, 0);
	m_lstHistorySearchResults.InsertColumn(4, _T("Created Date"), LVCFMT_LEFT, 100);
	m_lstHistorySearchResults.InsertColumn(5, _T("Created By"), LVCFMT_LEFT, 0);
	m_lstHistorySearchResults.InsertColumn(6, _T("Responsible"), LVCFMT_LEFT, 100);
	m_lstHistorySearchResults.InsertColumn(7, _T("Description"), LVCFMT_LEFT, 300);
	m_lstHistorySearchResults.InsertColumn(8, _T("History Log"), LVCFMT_LEFT, 0);

	m_btnHistorySearchSearch.SetTextColor(RGB(255, 0, 0));
	m_btnHistorySearchClear.SetTextColor(RGB(0, 0, 255));
}

// OnPreparePrinting: Prepare the printing process
// - pInfo: The print information
BOOL CAssetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

// OnBeginPrinting: Initialize the printing process
// - pDC: The device context
// - pInfo: The print information
void CAssetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

// OnEndPrinting: Clean up after printing
// - pDC: The device context
// - pInfo: The print information
void CAssetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// OnPrint: Print the view
// - pDC: The device context
// - pInfo: The print information
void CAssetView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

#ifdef _DEBUG
// AssertValid: Assert the view is valid
void CAssetView::AssertValid() const
{
	CFormView::AssertValid();
}

// Dump: Dump the view
// - dc: The dump context
void CAssetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

/* Message handlers methods */

// OnFilePrintPreview: Print preview the view
void CAssetView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

/* Member methods */

// OnClickedHistorySearch: Handle the search button click event
// This method will search the database for the asset and workorder history
// and display the results in the list control
void CAssetView::OnClickedHistorySearch()
{
	UpdateData(TRUE);

	m_lstHistorySearchResults.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	auto row(0);		// Row of the list control item.
	CString strBuildQuery;

	theApp.BeginWaitCursor();
	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	auto buildString = [](CString str) -> CString {
		CString strResult;
		if (str.IsEmpty()) strResult = _T("%");
		else strResult = _T("%") + str + _T("%");
		return strResult;
		};

	strBuildQuery.Format(_T("SELECT [ASSET_DESCRIPTION], [ASSET_MODEL_NUMBER], [ASSET_BRAND], [ASSET_DISPOSED], [WORKORDER_CREATE_DATE], [WORKORDER_CREATE_BY], ")
		_T("[WORKORDER_RESPONSIBLE], [WORKORDER_DESCRIPTION], [WORKORDER_HISTORY] FROM [dbo].[ASSET], [dbo].[WORKORDER] ")
		_T("WHERE([ASSET_DESCRIPTION] LIKE N\'%s\' AND[WORKORDER_DESCRIPTION] LIKE N\'%s\' AND[WORKORDER_HISTORY] LIKE N\'%s\')"),
		static_cast<LPCTSTR>(buildString(m_strHistorySearchAssetDescription)),
		static_cast<LPCTSTR>(buildString(m_strHistorySearchWorkorderDescription)),
		static_cast<LPCTSTR>(buildString(m_strHistorySearchHistoryLog)));

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		SQLCHAR szName[SQLCHARVSMAL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if (retcode == SQL_SUCCESS) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if (cbName == SQL_NULL_DATA) {
							return _T("");
						}
						return static_cast<CString>(szName);
						};

					SQLGetData(hstmt, HISTORY_SEARCH.ASSET_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					nIndex = m_lstHistorySearchResults.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.ASSET_MODEL_NUMBER, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.ASSET_BRAND, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.ASSET_DISPOSED, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.WORKORDER_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.WORKORDER_CREATE_BY, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMAL, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 6, CheckForNull(szName, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.WORKORDER_DESCRIPTION, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 7, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, HISTORY_SEARCH.WORKORDER_HISTORY, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lstHistorySearchResults.SetItemText(nIndex, 8, CheckForNull(szNameLong, cbName));
				}
				else
					break;
			}
		}
		if (!sql.CheckReturnCodeForClosing(retcode)) {
			sql.CloseConnection();
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		}
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);

		if (m_lstHistorySearchResults.GetItemCount() > 0) {
			m_lstHistorySearchResults.EnableWindow();
		}
		else {
			m_lstHistorySearchResults.EnableWindow(FALSE);
		}
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
}

// OnClickedHistorySearchClear: Handle the clear button click event
// This method will clear the search fields and the search results list control
void CAssetView::OnClickedHistorySearchClear()
{
	m_strHistorySearchAssetDescription.Empty();
	m_strHistorySearchWorkorderDescription.Empty();
	m_strHistorySearchHistoryLog.Empty();
	m_strHistorySearchDetailsAssetBrand.Empty();
	m_strHistorySearchDetailsAssetDescription.Empty();
	m_strHistorySearchDetailsAssetModelNumber.Empty();
	m_strHistorySearchDetailsWorkorderCreatedBy.Empty();
	m_strHistorySearchDetailsWorkorderCreated.Empty();
	m_strHistorySearchDetailsWorkorderHistoryLog.Empty();
	m_strHistorySearchDetailsWorkorderResponsible.Empty();
	m_chkHistorySearchDetailsIsDisposed = FALSE;
	m_lstHistorySearchResults.DeleteAllItems();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_SEARCH_ASSET_DESCRIPTION);
	pEdit->SetFocus();

	UpdateData(FALSE);
}

// OnClickSearchResultsItem: Handle the search results list item click event
// This method will get the details of the selected item and display them in the details group box
// - pNMHDR: The notification message header
// - pResult: The result of the message
void CAssetView::OnClickSearchResultsItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//  pNMItemActivate->iItem = -1 means no existing item is selected.
	if (pNMItemActivate->iItem != -1) {
		
		// Get the customer details from the selected item.
		m_strHistorySearchDetailsAssetDescription = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 0);
		m_strHistorySearchDetailsAssetModelNumber = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 1);
		m_strHistorySearchDetailsAssetBrand = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 2);
		m_chkHistorySearchDetailsIsDisposed = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 3) == _T("1") ? TRUE : FALSE;
		m_strHistorySearchDetailsWorkorderCreated = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 4);
		m_strHistorySearchDetailsWorkorderCreatedBy = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 5);
		m_strHistorySearchDetailsWorkorderResponsible = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 6);
		m_strHistorySearchDetailsWorkorderHistoryLog = m_lstHistorySearchResults.GetItemText(pNMItemActivate->iItem, 8);

		UpdateData(FALSE);
	}
	*pResult = 0;
}