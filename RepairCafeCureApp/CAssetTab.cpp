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
* File: CAssetTab.cpp, defines class CAssetTab
*
* This class is the view of the asset-tab in the CTabCtrlAssetWorkorder class, 
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
* 
* With this form the user can search for existing assets, add new assets, update existing assets.
* After selecting an asset, the user can create a workorder for the selected asset.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 08-11-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "MainFrm.h"
#include "CAssetTab.h"

using namespace artvabas::sql;
using namespace artvabas::rcc::ui::dialogs;

IMPLEMENT_DYNAMIC(CAssetTab, CDialogEx)

CAssetTab::CAssetTab(CTabCtrlAssetWorkorder* pTabControl, CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSET_TAB, pParent)
	, m_pTabControl(pTabControl)
	, m_strCustomerSurname(strCustomerSurname)
	, m_strCustomerName(strCustomerName)
	, m_nAssetID(0)
	, m_nAssetCustomerID(nCustomerID)
	, m_nAssetWorkorderID(0)
	, m_strAssetCreateDate(_T(""))
	, m_strDescription(_T(""))
	, m_strModelNumber(_T(""))
	, m_strBrand(_T(""))
	, m_sAssetDisposed(0)
	, m_strHistoryLog(_T(""))
	, m_bIsSelectedFromAssetList(false)
{
	// Shared data between the asset and workorder tab.
	m_pAssetDetailsRecords = &(m_pTabControl->m_assetDetailsRecords);
}

CAssetTab::~CAssetTab()
{
}

// CAssetTab message handlers
BEGIN_MESSAGE_MAP(CAssetTab, CDialogEx)
	ON_EN_CHANGE(IDC_ASSETTAB_DESCRIPTION, &CAssetTab::OnEnChangeAssetDetails)
	ON_EN_CHANGE(IDC_ASSTETTAB_MODEL_NUMBER, &CAssetTab::OnEnChangeAssetDetails)
	ON_EN_CHANGE(IDC_ASSETTAB_BRAND, &CAssetTab::OnEnChangeAssetDetails)
	ON_NOTIFY(NM_DBLCLK, IDC_ASSETTAB_ASSET_LIST, &CAssetTab::OnDoubleClickAssetTabAssetList)
	ON_BN_CLICKED(IDC_ASSETTAB_UPDATE, &CAssetTab::OnBnClickedAssetTabUpdate)
	ON_BN_CLICKED(IDC_ASSETTAB_NEW, &CAssetTab::OnBnClickedAssetTabNew)
	ON_BN_CLICKED(IDC_ASSETTAB_CREATE_WORKORDER, &CAssetTab::OnBnClickedAssetTabCreateWorkorder)
END_MESSAGE_MAP()

/* override methods */

/// <summary>
/// Initializes the dialog on creation.
/// Creates the list control columns.
/// Fills the list control with the existing assets, if any, from the database.
/// </summary>
/// <returns></returns>
BOOL CAssetTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrExistingAssetList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctrExistingAssetList.InsertColumn(0, _T("Asset ID"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(1, _T("Customer ID"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(2, _T("Workorder ID"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(3, _T("Creation Date"), LVCFMT_LEFT, 100);
	m_ctrExistingAssetList.InsertColumn(4, _T("Description"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(5, _T("Model Number"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(6, _T("Brand"), LVCFMT_LEFT, 200);
	m_ctrExistingAssetList.InsertColumn(7, _T("Disposed"), LVCFMT_LEFT, 0);
	m_ctrExistingAssetList.InsertColumn(8, _T("History Log"), LVCFMT_LEFT, 0);

	int nIndex;			// Index of the list control item.	
	int row(0);			// Row of the list control item.
	CString strQuery;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT ASSET.*, ASSET_CUSTOMER_ID AS Expr1 FROM ASSET WHERE(ASSET_CUSTOMER_ID = %d)"), m_nAssetCustomerID);

	if (!theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		pMainFrame->m_wndStatusBar.SetInformation(_T("ERROR: Couldn't open the database for customer asset list!"));
		delete rs;
		EndDialog(IDCANCEL);
		return FALSE;
	}

	while (!rs->IsEOF())
	{
		CString strValue = _T("");

		rs->GetFieldValue(_T("ASSET_ID"), strValue);
		nIndex = m_ctrExistingAssetList.InsertItem(row, strValue);

		rs->GetFieldValue(_T("ASSET_CUSTOMER_ID"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 1, strValue);

		rs->GetFieldValue(_T("ASSET_WORKORDER_ID"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 2, strValue);

		rs->GetFieldValue(_T("ASSET_CREATE_DATE"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 3, strValue);

		rs->GetFieldValue(_T("ASSET_DESCRIPTION"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 4, strValue);

		rs->GetFieldValue(_T("ASSET_MODEL_NUMBER"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 5, strValue);

		rs->GetFieldValue(_T("ASSET_BRAND"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 6, strValue);

		rs->GetFieldValue(_T("ASSET_DISPOSED"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 7, strValue);

		rs->GetFieldValue(_T("ASSET_HISTORY_LOG"), strValue);
		m_ctrExistingAssetList.SetItemText(nIndex, 8, strValue);

		rs->MoveNext();
	}

	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;

	return TRUE;
}

// Data exchange between the dialog controls and the variables.
void CAssetTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_SURENAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_ASSETTAB_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_ASSETTAB_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_ASSTETTAB_MODEL_NUMBER, m_strModelNumber);
	DDX_Text(pDX, IDC_ASSETTAB_BRAND, m_strBrand);
	DDX_Text(pDX, IDC_ASSETTAB_HISTORY_LOG, m_strHistoryLog);
	DDX_Control(pDX, IDC_ASSETTAB_UPDATE, m_btnUpdateAsset);
	DDX_Control(pDX, IDC_ASSETTAB_NEW, m_btnNewAsset);
	DDX_Control(pDX, IDC_ASSETTAB_CREATE_WORKORDER, m_btnCreateWorkorder);
	DDX_Control(pDX, IDC_ASSETTAB_ASSET_LIST, m_ctrExistingAssetList);
}

/// <summary>
/// Method OnEnChangeAssetDetails() is called when the user changes the text in the description, model number or brand edit controls.
/// When the user changes the text in the description, model number or brand edit controls,
/// the new asset button is enabled if all the edit controls have text.
/// If the user selects an existing asset from the list control and changes the text in the description, model number or brand edit controls,
/// the update asset button is enabled.
/// If asset details is not dirty, the create workorder button is enabled.
/// </summary>
void CAssetTab::OnEnChangeAssetDetails()
{
	UpdateData(TRUE);

	if (!m_bIsSelectedFromAssetList)
	{
		if (!m_strDescription.IsEmpty() && !m_strModelNumber.IsEmpty() && !m_strBrand.IsEmpty())
		{
			m_btnNewAsset.EnableWindow(TRUE);
		}
		else
		{
			m_btnNewAsset.EnableWindow(FALSE);
		}
	}
	else if(!m_strDescription.IsEmpty() && !m_strModelNumber.IsEmpty() && !m_strBrand.IsEmpty())
	{
		m_btnUpdateAsset.EnableWindow(TRUE);
		m_btnCreateWorkorder.EnableWindow(FALSE);
	}
	else
	{
		m_btnUpdateAsset.EnableWindow(FALSE);
		m_btnCreateWorkorder.EnableWindow(FALSE);
	}

	m_btnCreateWorkorder.EnableWindow(FALSE);
}

/// <summary>
/// Method OnDoubleClickAssetTabAssetList() is called when the user double clicks on an item in the list control.
/// When the user double clicks on an item in the list control,
/// the selected asset details are loaded into the edit controls.
/// The update asset button is enabled.
/// </summary>
/// <param name="pNMHDR">A pointer to an NMHDR structure that contains information about a notification message.</param>
/// <param name="pResult">A pointer to an LRESULT structure that is used to return the result of the message processing.</param>
/// <returns></returns>
void CAssetTab::OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//  pNMItemActivate->iItem = -1 means no existing item is selected.
	if (pNMItemActivate->iItem != -1)
	{
		// Get the selected item's text.
		m_nAssetID = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 0));
		m_nAssetCustomerID = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 1));
		m_nAssetWorkorderID = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 2));
		m_strAssetCreateDate = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 3);
		m_strDescription = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 4);
		m_strModelNumber = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 5);
		m_strBrand = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 6);
		m_sAssetDisposed = _ttoi(m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 7));
		m_strHistoryLog = m_ctrExistingAssetList.GetItemText(pNMItemActivate->iItem, 8);

		m_bIsSelectedFromAssetList = true;

		m_btnCreateWorkorder.EnableWindow(TRUE);

		// Update the data in the dialog.
		UpdateData(FALSE);
	}
	*pResult = 0;
}

/// <summary>
/// Method OnBnClickedAssetTabUpdate() is called when the user clicks on the update asset button.
/// When the user clicks on the update asset button,
/// the asset details are updated in the database.
/// After update the update asset button is disabled and the create workorder button is enabled.
/// </summary>
/// <returns></returns>
void CAssetTab::OnBnClickedAssetTabUpdate()
{
	CString strQuery;

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString
		{
			CString strResult;
			if (str.IsEmpty())
				return  _T("NULL");
			strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
			return strResult;
		};

	// A numeric zero is converted to a string zero or as an empty string, depending on the isNull parameter.
	// Because when a CString format %d is used, a numeric zero must be converted to an zero string.
	// otherwise the CString format will see this as a NUL-character, and causes an error on the string length check.
	// If the isNull parameter is true, the numeric zero is converted to an empty string, so that lambda function buildFieldValue() returns "NULL".
	auto intToCString = [](unsigned int n, bool isNull = false) -> CString
		{
			CString strResult(_T(""));
			if (n == 0)
			{
				if (isNull)
					return  strResult;
				else
					return  _T("0");
			}
				
			strResult.Format(_T("%d"), n);
			return strResult;
		};

	strQuery.Format(_T("UPDATE [ASSET] SET [ASSET_CUSTOMER_ID] = %d, [ASSET_WORKORDER_ID] = %s, [ASSET_CREATE_DATE] = %s, [ASSET_DESCRIPTION] = %s, [ASSET_MODEL_NUMBER] = %s, [ASSET_BRAND] = %s, [ASSET_DISPOSED] = %s, [ASSET_HISTORY_LOG] = %s WHERE [ASSET_ID]  = %d"),
		m_nAssetCustomerID,
		static_cast<LPCTSTR>(buildFieldValue(intToCString(m_nAssetWorkorderID, true))),
		static_cast<LPCTSTR>(buildFieldValue(m_strAssetCreateDate)),
		static_cast<LPCTSTR>(buildFieldValue(m_strDescription)),
		static_cast<LPCTSTR>(buildFieldValue(m_strModelNumber)),
		static_cast<LPCTSTR>(buildFieldValue(m_strBrand)),
		static_cast<LPCTSTR>(buildFieldValue(intToCString(m_sAssetDisposed))),
		static_cast<LPCTSTR>(buildFieldValue(m_strHistoryLog)),
		m_nAssetID);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());

	if (!sql.ExecuteQuery(strQuery.GetBuffer()))
	{
		pMainFrame->m_wndStatusBar.SetInformation(_T("ERROR: Asset couldn't be updated!"));
	}
	else
	{
		pMainFrame->m_wndStatusBar.SetInformation(_T("Ready: Asset has been updated."));
	}
	
	strQuery.ReleaseBuffer();

	m_btnCreateWorkorder.EnableWindow(TRUE);
	m_btnUpdateAsset.EnableWindow(FALSE);
}

/// <summary>
/// Method OnBnClickedAssetTabNew() is called when the user clicks on the new asset button.
/// When the user clicks on the new asset button,
/// a new asset is created in the database.
/// After creation the new asset button is disabled and the create workorder button is enabled.
/// </summary>
/// <returns></returns>
void CAssetTab::OnBnClickedAssetTabNew()
{
	// Creation date is the current date.
	CString strDate;
	CTime time = CTime().GetCurrentTime();
	strDate = time.Format(_T("%m/%d/%Y"));

	CString strQuery;

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString
	{
		CString strResult;
		if (str.IsEmpty())
			return  _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return strResult;
	};

	strQuery.Format(_T("INSERT INTO [ASSET] ([ASSET_CUSTOMER_ID], [ASSET_WORKORDER_ID], [ASSET_CREATE_DATE], [ASSET_DESCRIPTION], [ASSET_MODEL_NUMBER], [ASSET_BRAND], [ASSET_DISPOSED], [ASSET_HISTORY_LOG]) VALUES (%d, NULL, %s, %s, %s, %s, 0, NULL)"),
		m_nAssetCustomerID,
		static_cast<LPCTSTR>(buildFieldValue(strDate)),
		static_cast<LPCTSTR>(buildFieldValue(m_strDescription)),
		static_cast<LPCTSTR>(buildFieldValue(m_strModelNumber)),
		static_cast<LPCTSTR>(buildFieldValue(m_strBrand)));

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());

	if (!sql.ExecuteQuery(strQuery.GetBuffer()))
	{
		pMainFrame->m_wndStatusBar.SetInformation(_T("ERROR: Asset couldn't be created!"));
	}
	else
	{
		auto lastID = sql.GetLastAddedID(_T("SELECT IDENT_CURRENT('ASSET')"));
		if (lastID > 0)
		{
			m_nAssetID = lastID;
			pMainFrame->m_wndStatusBar.SetInformation(_T("Ready: New asset has been created."));
		}
		else
			pMainFrame->m_wndStatusBar.SetInformation(_T("ERROR: Asset couldn't be created!"));
	}
	
	strQuery.ReleaseBuffer();

	m_btnCreateWorkorder.EnableWindow(TRUE);
	m_btnNewAsset.EnableWindow(FALSE);
	m_bIsSelectedFromAssetList = true;
}

/// <summary>
/// Method OnBnClickedAssetTabCreateWorkorder() is called when the user clicks on the create workorder button.
/// When the user clicks on the create workorder button,
/// the asset details are loaded into the shared data structure.
/// The tab control is switched to the workorder tab.
/// </summary>
/// <returns></returns>
void CAssetTab::OnBnClickedAssetTabCreateWorkorder()
{
	UpdateData(TRUE);

	m_pAssetDetailsRecords->m_nAssetID = m_nAssetID;
	m_pAssetDetailsRecords->m_nAssetCustomerID = m_nAssetCustomerID;
	m_pAssetDetailsRecords->m_strDescription = m_strDescription;
	m_pAssetDetailsRecords->m_strModelNumber = m_strModelNumber;
	m_pAssetDetailsRecords->m_strBrand = m_strBrand;

	m_pTabControl->ChangeTabView();

}
