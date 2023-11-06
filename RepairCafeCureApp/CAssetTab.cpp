// CAssetTab.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CAssetTab.h"


using namespace artvabas::sql;
// CAssetTab dialog

IMPLEMENT_DYNAMIC(CAssetTab, CDialogEx)

CAssetTab::CAssetTab(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSET_TAB, pParent)
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
	, m_bIsDirtyAssetDetails(false)
	, m_bIsSelectedFromAssetList(false)
{

}

CAssetTab::~CAssetTab()
{
}

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


BEGIN_MESSAGE_MAP(CAssetTab, CDialogEx)
	ON_EN_CHANGE(IDC_ASSETTAB_DESCRIPTION, &CAssetTab::OnEnChangeAssetDetails)
	ON_EN_CHANGE(IDC_ASSTETTAB_MODEL_NUMBER, &CAssetTab::OnEnChangeAssetDetails)
	ON_EN_CHANGE(IDC_ASSETTAB_BRAND, &CAssetTab::OnEnChangeAssetDetails)
	ON_NOTIFY(NM_DBLCLK, IDC_ASSETTAB_ASSET_LIST, &CAssetTab::OnDoubleClickAssetTabAssetList)
	ON_BN_CLICKED(IDC_ASSETTAB_UPDATE, &CAssetTab::OnBnClickedAssetTabUpdate)
	ON_BN_CLICKED(IDC_ASSETTAB_NEW, &CAssetTab::OnBnClickedAssetTabNew)
END_MESSAGE_MAP()


// CAssetTab message handlers


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

	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT ASSET.*, ASSET_CUSTOMER_ID AS Expr1 FROM ASSET WHERE(ASSET_CUSTOMER_ID = %d)"), m_nAssetCustomerID);
	
	theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery);

	while (!rs->IsEOF())
	{
		CString strValue = _T("");

		// Get the Asset ID.
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssetTab::OnEnChangeAssetDetails()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

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

	m_bIsDirtyAssetDetails =true;
}


void CAssetTab::OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

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

		//m_btnCreateWorkorder.EnableWindow(TRUE);
		m_bIsSelectedFromAssetList = true;

		m_btnCreateWorkorder.EnableWindow(TRUE);

		// Update the data in the dialog.
		UpdateData(FALSE);
	}

	*pResult = 0;
}


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

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());
	sql.ExecuteQuery(strQuery.GetBuffer());
	strQuery.ReleaseBuffer();

	m_btnCreateWorkorder.EnableWindow(TRUE);
	m_btnUpdateAsset.EnableWindow(FALSE);
	m_bIsDirtyAssetDetails = false;
}


void CAssetTab::OnBnClickedAssetTabNew()
{
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

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());
	sql.ExecuteQuery(strQuery.GetBuffer());
	strQuery.ReleaseBuffer();

	m_btnCreateWorkorder.EnableWindow(TRUE);
	m_btnNewAsset.EnableWindow(FALSE);
	m_bIsDirtyAssetDetails = false;
	m_bIsSelectedFromAssetList = true;

				
}
