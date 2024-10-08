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
* File: CWorkorderTab.ccp, implementation of the class CWorkorderTab
*
* This class is the view of the workorder tab (IDD_WORKORDER_TAB) of the asset/workorder dialog (CAssetDialog)
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
*
* With this form the user can create a new workorder for the selected asset.
* The user can also view the history of the workorders of the selected asset.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.3.5 (beta)
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 19-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/ 

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderTab.h"
#include "CPrintWorkorder.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;
using namespace artvabas::rcc::support;
using namespace artvabas::database::tables::workorder;
using namespace artvabas::database::tables::customer;

IMPLEMENT_DYNAMIC(CWorkorderTab, CDialogEx)

CWorkorderTab::CWorkorderTab(CTabCtrlAssetWorkorder* pTabControl, CWnd* pParent) noexcept
	: CDialogEx{ IDD_WORKORDER_TAB, pParent }, 
	m_pTabControl{ pTabControl },
	m_uiCustomerID{ 0 },
	m_uiAssetID{ 0 },
	m_strCustomerSurname{ _T("") },
	m_strCustomerName{ _T("") },
	m_strAssetDescription{ _T("") },
	m_strAssetModelNumber{ _T("") },
	m_strAssetBrand{ _T("") },
	m_strWorkorderDescription{ _T("") },
	m_strHistoryWorkorderDescription{ _T("") },
	m_strHistoryWorkorderLog{ _T("") }
{
	m_pAssetDetailsRecords = &(m_pTabControl->m_assetDetailsRecords);
}

CWorkorderTab::~CWorkorderTab(){}

/* message handle binders */
BEGIN_MESSAGE_MAP(CWorkorderTab, CDialogEx)
	ON_BN_CLICKED(IDC_WOTAB_CREATE, &CWorkorderTab::OnBnClickedWoTabCreate)
	ON_EN_CHANGE(IDC_WOTAB_DESCRIPTION, &CWorkorderTab::OnEnChangeWoTabDescription)
	ON_NOTIFY(NM_CLICK, IDC_WOTAB_WORKORDERS_HISTORY_LIST, &CWorkorderTab::OnNMClickWoTabWorkordersHistoryList)
END_MESSAGE_MAP()

/* Override methods */

// DoDataExchange: Exchange and validate dialog data between controls and class members.
void CWorkorderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WOTAB_CUSTOMER_SURNAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_WOTAB_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_WOTAB_ASSET_DESCRIPTION, m_strAssetDescription);
	DDX_Text(pDX, IDC_WOTAB_ASSET_MODEL_NUMBER, m_strAssetModelNumber);
	DDX_Text(pDX, IDC_WOTAB_ASSET_BRAND, m_strAssetBrand);
	DDX_Text(pDX, IDC_WOTAB_DESCRIPTION, m_strWorkorderDescription);
	DDX_Text(pDX, IDC_WOTAB_WORKORDER_HISTORY_DESCRIPTION, m_strHistoryWorkorderDescription);
	DDX_Text(pDX, IDC_WOTAB_WORKORDER_HISTORY_LOG, m_strHistoryWorkorderLog);
	DDX_Control(pDX, IDC_WOTAB_CREATE, m_btnWorkorderCreate);
	DDX_Control(pDX, IDC_WOTAB_WORKORDERS_HISTORY_LIST, m_ctrWorkordersHistoryList);
	DDX_Control(pDX, IDC_WOTAB_DESCRIPTION, m_ctrWorkorderDescription);
}

// OnInitDialog: Called after the constructor and before the dialog is displayed.
// Here the list control is initialized with the columns.
BOOL CWorkorderTab::OnInitDialog() {
	CDialogEx::OnInitDialog();

	m_ctrWorkordersHistoryList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctrWorkordersHistoryList.InsertColumn(0, theApp.ConvertIDToString(IDS_DBT_WORKORDER_ID), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(1, theApp.ConvertIDToString(IDS_DBT_ASSET_ID), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(2, theApp.ConvertIDToString(IDS_DBT_CUSTOMER_ID), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(3, theApp.ConvertIDToString(IDS_DBT_CUSTOMER_ID), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(4, theApp.ConvertIDToString(IDS_DBT_CREATION_DATE), LVCFMT_LEFT, 100);
	m_ctrWorkordersHistoryList.InsertColumn(5, theApp.ConvertIDToString(IDS_DBT_CREATED_BY), LVCFMT_LEFT, 200);
	m_ctrWorkordersHistoryList.InsertColumn(6, theApp.ConvertIDToString(IDS_DBT_DESCRIPTION), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(7, theApp.ConvertIDToString(IDS_DBT_RESPONSIBLE), LVCFMT_LEFT, 200);
	m_ctrWorkordersHistoryList.InsertColumn(8, theApp.ConvertIDToString(IDS_DBT_STATUS), LVCFMT_LEFT, 100);
	m_ctrWorkordersHistoryList.InsertColumn(9, theApp.ConvertIDToString(IDS_DBT_CLOSED_DATE), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(10, theApp.ConvertIDToString(IDS_DBT_HISTORY), LVCFMT_LEFT, 0);

	return TRUE;
}

// PreTranslateMessage: Called before the message is dispatched to the message map.
// Here the focus is set to the create workorder button when the user presses the enter key.
// - pMsg: A pointer to an MSG structure that contains the message to process.
BOOL CWorkorderTab::PreTranslateMessage(MSG* pMsg) {

	if ( m_pTabControl->GetCurFocus() == 1 ) {
		if ( pMsg->message == WM_KEYDOWN  ) {
			if ( pMsg->wParam == VK_RETURN ) {
				if ( m_btnWorkorderCreate.IsWindowEnabled() ) {
					// Yes, then click the search button.
					OnBnClickedWoTabCreate();
					return TRUE;
				}
			} else {
				return CDialogEx::PreTranslateMessage(pMsg);
			}
		}
	}
	return FALSE;
}

/* Message handlers methods */	

// OnEnChangeWoTabDescription: Called when the workorder description edit control text is changed.
// The create workorder button is enabled or disabled depending on the state of the workorder description edit control.
void CWorkorderTab::OnEnChangeWoTabDescription() noexcept
{
	UpdateData(TRUE);
	if ( m_strWorkorderDescription.IsEmpty()) {
		m_btnWorkorderCreate.EnableWindow(FALSE);
		SetCustomFocusButton(&m_btnWorkorderCreate, BLACK, false);
	} else {
		m_btnWorkorderCreate.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnWorkorderCreate,RED, false);
	}
}

// onBnClickedWoTabCreate: Called when the user clicks on the create workorder button.
// The workorder is created and inserted into the database.
void CWorkorderTab::OnBnClickedWoTabCreate()
{
	UpdateData(TRUE);
	CString strQuery;
	auto strCurDate{ COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")) };
	CString strWorkorderLog{ _T("[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Workorder Created.") };

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString {
		CString strResult;
		if (str.IsEmpty()) return  _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return strResult;
	};

	strQuery.Format(_T("INSERT INTO [WORKORDER] ([WORKORDER_ASSET_ID], [WORKORDER_CUSTOMER_ID], [WORKORDER_INVOICE_ID], ")
		_T("[WORKORDER_CREATE_DATE], [WORKORDER_CREATE_BY], [WORKORDER_DESCRIPTION], [WORKORDER_RESPONSIBLE], [WORKORDER_STATUS], ")
		_T("[WORKORDER_HISTORY]) VALUES(%d, %d, NULL, %s, %s, %s, NULL, %s, %s)"),
		m_uiAssetID, m_uiCustomerID,
		static_cast<LPCTSTR>(buildFieldValue(COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")))),
		static_cast<LPCTSTR>(buildFieldValue(theApp.GetSelectedEmployeeName())),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderDescription)),
		static_cast<LPCTSTR>(buildFieldValue(_T("Open"))),
		static_cast<LPCTSTR>(buildFieldValue(strWorkorderLog)));

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	// Execute the query.
	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {
		if ( !sql.ExecuteQuery(strQuery.GetBuffer()) ) {
			theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
		} else {
			theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
		}
	} else {
		theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
	}
	strQuery.ReleaseBuffer();
	theApp.EndWaitCursor();

	PrintReceiptAndWorkorder();

	m_strWorkorderDescription.Empty();
	m_strHistoryWorkorderDescription.Empty();
	m_strHistoryWorkorderLog.Empty();
	UpdateData(FALSE);
	m_btnWorkorderCreate.EnableWindow(FALSE);
	SetCustomFocusButton(&m_btnWorkorderCreate, BLACK, false);

	m_pTabControl->ChangeTabView(true);
}

// OnNMClickWoTabWorkordersHistoryList: Called when the user clicks on the workorder history list control.
// The workorder description and log are displayed in the workorder description and log edit controls.
// - pNMHDR: A pointer to an NMHDR structure that contains the notification code and additional information.
// - pResult: A pointer to an LRESULT variable. The handler can fill in this variable to set the return value of the message.
void CWorkorderTab::OnNMClickWoTabWorkordersHistoryList(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	auto pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if  (pNMItemActivate->iItem != -1 ) {
		m_strHistoryWorkorderDescription = m_ctrWorkordersHistoryList.GetItemText(pNMItemActivate->iItem, 6);
		m_strHistoryWorkorderLog = m_ctrWorkordersHistoryList.GetItemText(pNMItemActivate->iItem, 10);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

/* Member methods */

// InitWithAssetDetailsRecords: Initializes the workorder tab with the asset details records.
// from the database.
void CWorkorderTab::InitWithAssetDetailsRecords()
{
	m_uiCustomerID = m_pAssetDetailsRecords->m_nAssetCustomerID;
	m_uiAssetID = m_pAssetDetailsRecords->m_nAssetID;
	m_strCustomerSurname = m_pAssetDetailsRecords->m_strCustomerSurname;
	m_strCustomerName = m_pAssetDetailsRecords->m_strCustomerName;
	m_strAssetDescription = m_pAssetDetailsRecords->m_strDescription;
	m_strAssetModelNumber = m_pAssetDetailsRecords->m_strModelNumber;
	m_strAssetBrand = m_pAssetDetailsRecords->m_strBrand;
	m_strHistoryWorkorderDescription.Empty();
	m_strHistoryWorkorderLog.Empty();

	if (m_uiAssetID == 0) {
		m_strWorkorderDescription.Empty();
		m_btnWorkorderCreate.EnableWindow(FALSE);
		m_ctrWorkorderDescription.EnableWindow(FALSE);
	}
	else m_ctrWorkorderDescription.EnableWindow(TRUE);

	m_ctrWorkordersHistoryList.DeleteAllItems(); 
	int nIndex{};			// Index of the list control item.
	auto row(0);			// Row of the list control item.
	CString strBuildQuery{};

	if ( m_uiAssetID != 0 && m_uiCustomerID != 0)  {

		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
		theApp.BeginWaitCursor();

		strBuildQuery.Format(_T("SELECT WORKORDER.*, WORKORDER_ASSET_ID AS Expr1, WORKORDER_CUSTOMER_ID AS Expr2 FROM WORKORDER WHERE ")
			_T("(WORKORDER_ASSET_ID = N\'%d\') AND (WORKORDER_CUSTOMER_ID = N\'%d\')"),
			m_uiAssetID, m_uiCustomerID);

		CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

		if (sql.CreateSQLConnection()) {

			SQLCHAR szName[SQLCHARVSMALL]{};
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
							if (cbName == SQL_NULL_DATA)
								return _T("");
							return static_cast<CString>(szName);
							};

						// Get data for columns 1, employee names
						SQLGetData(hstmt, WORKORDER.WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						nIndex = m_ctrWorkordersHistoryList.InsertItem(row++, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_BY, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 6, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 7, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_STATUS, SQL_C_CHAR, szName, SQLCHARVMAX, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 8, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_CLOSED_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 9, CheckForNull(szName, cbName));

						SQLGetData(hstmt, WORKORDER.WORKORDER_HISTORY, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
						m_ctrWorkordersHistoryList.SetItemText(nIndex, 10, CheckForNull(szNameLong, cbName));

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
		}
		sql.CloseConnection();
		theApp.EndWaitCursor();
	}
	UpdateData(FALSE);
}

// PrintReceiptAndWorkorder: Prints the receipt and workorder for the created workorder.
void CWorkorderTab::PrintReceiptAndWorkorder()
{
	try {
		// Set printer settings
		CPrintDialog dlg{ FALSE };
		dlg.m_pd.hDevMode = GlobalAlloc(GHND, sizeof(DEVMODE));

		ASSERT(dlg.m_pd.hDevMode != NULL);

		if ( dlg.m_pd.hDevMode != NULL ) {
			auto* devMode{ reinterpret_cast<DEVMODE*>(GlobalLock(dlg.m_pd.hDevMode)) };
			if ( devMode != NULL ) {
				devMode->dmSize = sizeof(DEVMODE);
				devMode->dmOrientation = DMORIENT_LANDSCAPE;
				devMode->dmFields = DM_ORIENTATION;
				GlobalUnlock(devMode);
			} else {
				// Free the memory allocated for the DEVMODE structure
				GlobalFree(dlg.m_pd.hDevMode);
				EndWaitCursor();
				throw 1;
			}
		} else {
			// Free the memory allocated for the DEVMODE structure
			GlobalFree(dlg.m_pd.hDevMode);
			EndWaitCursor();
			throw 1;
		}

		if ( dlg.DoModal() == IDOK ) {
			BeginWaitCursor();
			
			CString strLastWOID{ _T("") };
			CString strCustomerCellPhone{ _T("") };
			CString strCustomerPhone{ _T("") };
			CString strCustomerEmail{ _T("") };

			// Get last added workorder ID
			theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
			theApp.BeginWaitCursor();
			CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

			if ( sql.CreateSQLConnection() ) {

				auto lastID = sql.GetLastAddedID(_T("SELECT IDENT_CURRENT('WORKORDER')"));
				if ( lastID > 0 ) {
					theApp.SetStatusBarText(IDS_STATUSBAR_LASTID_OK);
					strLastWOID.Format(_T("%d"), lastID);
				} else {
					theApp.SetStatusBarText(IDS_STATUSBAR_LASTID_FAIL);
					sql.CloseConnection();
					EndWaitCursor();
					throw 2;
				}

				// Get customer cell phone, phone and email
				theApp.SetStatusBarText(IDS_STATUSBAR_LOADING); //SELECT CUSTOMER_CELL_PHONE, CUSTOMER_PHONE, CUSTOMER_EMAIL FROM CUSTOMER WHERE (CUSTOMER_ID = %d)

				CString strBuildQuery {};
				strBuildQuery.Format(_T("SELECT CUSTOMER.*, CUSTOMER_ID AS Expr1 FROM CUSTOMER WHERE(CUSTOMER_ID = %d)"),
					m_uiCustomerID);

				SQLCHAR szName[SQLCHARVSMALL]{};
				SQLLEN cbName{};
				SQLRETURN retcode{};
				SQLHSTMT hstmt{ sql.GetStatementHandle() };
				SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
				strBuildQuery.ReleaseBuffer();

				retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

				if ( retcode == SQL_SUCCESS ) {
					while ( TRUE ) {
						retcode = SQLFetch(hstmt);
						if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO ) {
							AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
							sql.CloseConnection();
							throw 2;
						}
						if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

							auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
								if ( cbName == SQL_NULL_DATA )
									return _T("");
								return static_cast<CString>(szName);
							};
							SQLGetData(hstmt, CUSTOMER.CUSTOMER_CELL_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
							strCustomerCellPhone = CheckForNull(szName, cbName);

							SQLGetData(hstmt, CUSTOMER.CUSTOMER_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
							strCustomerPhone = CheckForNull(szName, cbName);

							SQLGetData(hstmt, CUSTOMER.CUSTOMER_EMAIL, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
							strCustomerEmail = CheckForNull(szName, cbName);
						}
						else 
							break;
					}
				}
				if ( !sql.CheckReturnCodeForClosing(retcode) ) {
					sql.CloseConnection();
					theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
					EndWaitCursor();
					throw 2;
				} else
					theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
			}

			sql.CloseConnection();

			// Create printer device context
			auto pDC{ new CDC };
			// Attach the printer device context handle to the printer device context
			auto hDC{ CreateDCW(dlg.GetDriverName(), dlg.GetDeviceName(), NULL, dlg.GetDevMode()) };

			// Attach the printer device context handle to the printer device context, init pDC
			if ( pDC->Attach(hDC) ) {
				// Set printer title
				CString strTitle{ _T("RCCA - Combibon") };

				DOCINFO di;
				::ZeroMemory(&di, sizeof(DOCINFO));
				di.cbSize = sizeof(DOCINFO);
				di.lpszDocName = strTitle;
				pDC->m_hDC = dlg.GetPrinterDC();

				// Start printing
				if ( pDC->StartDoc(&di) ) {
					BeginWaitCursor();
					CPrintWorkorder::WorkorderData workData;
					workData.strWorkorderID = strLastWOID;
					workData.strCustomerSurname = m_strCustomerSurname;
					workData.strCustomerName = m_strCustomerName;
					workData.strCustomerCellPhone = strCustomerCellPhone;
					workData.strCustomerPhone = strCustomerPhone;
					workData.strCustomerEmail = strCustomerEmail;
					workData.strAssetDescription = m_strAssetDescription;
					workData.strAssetModelNumber = m_strAssetModelNumber;
					workData.strAssetBrand = m_strAssetBrand;
					workData.strWorkorderCreatedDate = COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));
					workData.strEmployeeResponsible = theApp.GetSelectedEmployeeName();
					workData.strWorkorderStatus = _T("Status: In Progress");
					workData.strWorkorderDescription = m_strWorkorderDescription;

					CPrintWorkorder printCombi{ &workData };
					printCombi.PrintCombi(pDC);

					theApp.SetStatusBarText(IDS_STATUSBAR_PRINTING_OK);
				} else {
					// End printing
					pDC->EndPage();
					pDC->EndDoc();
					// Detach the printer handle
					pDC->Detach();
					// Delete the printer device context
					ReleaseDC(pDC);
					delete pDC;
					// Free the memory allocated for the DEVMODE structure
					GlobalFree(dlg.m_pd.hDevMode);
					EndWaitCursor();
					throw 4;
				}
			} else {
				// End printing
				pDC->EndPage();
				pDC->EndDoc();
				// Detach the printer handle
				pDC->Detach();
				// Delete the printer device context
				ReleaseDC(pDC);
				delete pDC;
				// Free the memory allocated for the DEVMODE structure
				GlobalFree(dlg.m_pd.hDevMode);
				EndWaitCursor();
				throw 3;
			}

			// End printing
			pDC->EndPage();
			pDC->EndDoc();
			// Detach the printer handle
			pDC->Detach();
			// Delete the printer device context
			ReleaseDC(pDC);
			delete pDC;
			// Free the memory allocated for the DEVMODE structure
			GlobalFree(dlg.m_pd.hDevMode);
			EndWaitCursor();
		}
	} catch (CException* e) {
		e->ReportError();
		e->Delete();
	} catch (int e)	{
		CString errMsg;
		switch (e){
		case 1:
			errMsg = _T("An error occurred while printing the receipt and workorder. The printer device mode could not be created.");
			break;
		case 2:
			errMsg = _T("An error occurred while printing the receipt and workorder. Can't retrieve database values.");
			break;
		case 3:
			errMsg = _T("An error occurred while printing the receipt and workorder. Can't create handle to device context.");
			break;
		case 4:
			errMsg = _T("An error occurred while printing the receipt and workorder. Can't start document for printing.");
			break;
		}
		AfxMessageBox(errMsg);
	} catch (...) {
		AfxMessageBox(_T("An unknown error occurred while printing the receipt and workorder."));
	}
}

// SetCustomFocusButton: Sets the focus to the button and changes the text color.
// - pButton: A pointer to the button.
// - Color: The color of the text.
// - bFocus: A boolean value that indicates if the button should be focused.
void CWorkorderTab::SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus) noexcept
{
	auto color = RGB(255, 0, 0);
	switch (Color) {
	case RED:
		color = RGB(255, 0, 0);
		break;
	case BLUE:
		color = RGB(0, 0, 255);
		break;
	case BLACK:
		color = RGB(0, 0, 0);
		break;
	}
	pButton->SetTextColor(color);
	pButton->RedrawWindow();
	if ( bFocus ) pButton->SetFocus();
}