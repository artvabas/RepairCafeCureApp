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
* File: CClosedWorkorderInvoiceDetailsTab.cpp, implements class CClosedWorkorderInvoiceDetailsTab
*
* This class is the view of the Invoice-tab in the CClosedWorkorderDetailsTabCtrl class,
* which is the view of the CListCtrl created on the Closed Workorder Details dialog (CClosedWorkorderDetails)
*
* This form is used to display the invoice-details of a closed workorder.
* What is selected in the listview of the CReportWorkorderClosedView and passed to this form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.2.5 (beta)
* Created: 17-06-2024, (dd-mm-yyyy)
* Updated: 30-06-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CClosedWorkorderInvoiceDetailsTab.h"
#include "CSqlNativeAVB.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;
using namespace artvabas::database::tables::invoice;
using namespace artvabas::database::tables::workorderparts;

IMPLEMENT_DYNAMIC(CClosedWorkorderInvoiceDetailsTab, CDialogEx)

CClosedWorkorderInvoiceDetailsTab::CClosedWorkorderInvoiceDetailsTab(unsigned int& unID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSED_WORKORDER_INVOICE_DETAILS_TAB, pParent)
	, m_unInvoiceID{ unID }
	, m_strInvoiceID{ _T("") }
	, m_strInvoiceCustomerID{ _T("") }
	, m_strInvoiceWorkorderID{ _T("") }
	, m_strInvoiceAssetID{ _T("") }
	, m_strInvoiceCreateDate{ _T("") }
	, m_bInvoicePinTransaction{ FALSE }
	, m_strInvoiceAmount{ _T("") }
{}

CClosedWorkorderInvoiceDetailsTab::~CClosedWorkorderInvoiceDetailsTab()
{}

BEGIN_MESSAGE_MAP(CClosedWorkorderInvoiceDetailsTab, CDialogEx)
END_MESSAGE_MAP()

BOOL CClosedWorkorderInvoiceDetailsTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set up the list control
	m_lstInvoiceSummary.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstInvoiceSummary.InsertColumn(0, theApp.ConvertIDToString(IDS_DBT_DESCRIPTION), LVCFMT_LEFT, 200);
	m_lstInvoiceSummary.InsertColumn(1, theApp.ConvertIDToString(IDS_DBT_AMOUNT), LVCFMT_LEFT, 100);
	m_lstInvoiceSummary.InsertColumn(2, theApp.ConvertIDToString(IDS_DBT_UNIT_PRICE), LVCFMT_LEFT, 100);
	m_lstInvoiceSummary.InsertColumn(3, theApp.ConvertIDToString(IDS_DBT_TOTAL), LVCFMT_LEFT, 100);

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery;
	strBuildQuery.Format(_T("SELECT * FROM INVOICE WHERE INVOICE_ID = %d"), m_unInvoiceID);

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
						if (cbName == SQL_NULL_DATA) {
							return _T("");
						}
						return static_cast<CString>(szName);
						};
					SQLGetData(hstmt, INVOICE.INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strInvoiceID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, INVOICE.INVOICE_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strInvoiceCustomerID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, INVOICE.INVOICE_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strInvoiceAssetID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, INVOICE.INVOICE_WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strInvoiceWorkorderID = CheckForNull(szName, cbName);

					SQLGetData(hstmt, INVOICE.INVOICE_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strInvoiceCreateDate = CheckForNull(szName, cbName);

					SQLGetData(hstmt, INVOICE.INVOICE_PAYMENT_PIN, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_bInvoicePinTransaction = _wtoi(CheckForNull(szName, cbName));

					SQLGetData(hstmt, INVOICE.INVOICE_TOTAL, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strInvoiceAmount.Format(_T("€%.2f"), _wtof(CheckForNull(szName, cbName)));
				}
				else
					break;
			}
		}
		if (!sql.CheckReturnCodeForClosing(retcode))
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);

		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
		strBuildQuery.Format(_T("SELECT * FROM WORKORDER_PARTS WHERE WORKORDER_PARTS_WORKORDER_ID = %d"), _wtoi(m_strInvoiceWorkorderID));

		strQuery = strBuildQuery.GetBuffer();
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if (retcode == SQL_SUCCESS) {
			CString strDescription;
			CString strAmount;
			CString strTotalPrice;
			CString strUnitPrice;
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
					// Get data for columns 1, employee names
					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					strDescription = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_AMOUNT, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					strAmount = CheckForNull(szName, cbName);

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_UNIT_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					strUnitPrice.Format(_T("%.2f"), _wtof(CheckForNull(szName, cbName)));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_TOTAL_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					strTotalPrice.Format(_T("%.2f"), _wtof(CheckForNull(szName, cbName)));

					int nItem = m_lstInvoiceSummary.GetItemCount();
					m_lstInvoiceSummary.InsertItem(nItem, strDescription);
					m_lstInvoiceSummary.SetItemText(nItem, 1, strAmount);
					m_lstInvoiceSummary.SetItemText(nItem, 2, strUnitPrice);
					m_lstInvoiceSummary.SetItemText(nItem, 3, strTotalPrice);
				}
				else
					break;
			}
		}
		if (!sql.CheckReturnCodeForClosing(retcode))
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CClosedWorkorderInvoiceDetailsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_INVOICE_ID, m_strInvoiceID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_CUSTOMER_ID, m_strInvoiceCustomerID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_WORKORDER_ID, m_strInvoiceWorkorderID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_ASSET_ID, m_strInvoiceAssetID);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_CREATE_DATE, m_strInvoiceCreateDate);
	DDX_Check(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_PIN_PAYMENT, m_bInvoicePinTransaction);
	DDX_Control(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_INVOICE_SUMMARY, m_lstInvoiceSummary);
	DDX_Text(pDX, IDC_CLOSED_WORKORDER_INVOICE_TAB_INVOICE_AMOUNT, m_strInvoiceAmount);
}