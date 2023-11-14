// CWorkorderView.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderView.h"

//using namespace artvabas::rcc::ui;
using namespace artvabas::sql;
// CWorkorderView

IMPLEMENT_DYNCREATE(CWorkorderView, CFormView)

CWorkorderView::CWorkorderView() : CFormView(IDD_WORKORDER_FORM)
	, m_nWorkorderId(0)
	, m_strCustomerSurname(_T(""))
	, m_strCustomerName(_T(""))
	, m_strCustomerCellPhone(_T(""))
	, m_strCustomerPhone(_T(""))
	, m_strCustomerEmail(_T(""))
	, m_strCustomerComments(_T(""))
	, m_strAssetDescription(_T(""))
	, m_strAssetModelNumber(_T(""))
	, m_strAssetBrand(_T(""))
	, m_strAssetHistoryLog(_T(""))
	, m_strWorkorderCreatedDate(_T(""))
	, m_strWorkorderCreatedBy(_T(""))
	, m_strWorkorderStatus(_T(""))
	, m_strWorkorderNewLog(_T(""))
	, m_strWorkorderHistoryLog(_T(""))
	, m_bWorkorderSelected(false)
	, m_bResponsibleChanged(false)
{

}

CWorkorderView::~CWorkorderView()
{

}

void CWorkorderView::InitWorkorderExistingList()
{
	m_lscWorkorderExisting.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strQuery;
	CString strWorkorderStatus;

	switch (theApp.GetWorkorderViewType())
	{
		case VIEW_WORKORDER_OPEN:
			strWorkorderStatus = _T("Open");
			break;
		case VIEW_WORKORDER_PROGRESS:
			strWorkorderStatus = _T("Progress");
			break;
		case VIEW_WORKORDER_REPAIRED:
			strWorkorderStatus = _T("Repaired");
			break;
		default:
			strWorkorderStatus = _T("Open");
			break;
	}

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT WORKORDER.*, WORKORDER_ASSET_ID AS Expr1, WORKORDER_CUSTOMER_ID AS Expr2 FROM WORKORDER WHERE(WORKORDER_STATUS = N\'%s\')"),
		strWorkorderStatus);

	if (theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		// Fill the existing customers list control with the found customers from the database.
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("WORKORDER_ID"), strValue);
			nIndex = m_lscWorkorderExisting.InsertItem(row++, strValue);

			rs->GetFieldValue(_T("WORKORDER_ASSET_ID"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 1, strValue);

			rs->GetFieldValue(_T("WORKORDER_CUSTOMER_ID"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 2, strValue);

			rs->GetFieldValue(_T("WORKORDER_INVOICE_ID"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 3, strValue);

			rs->GetFieldValue(_T("WORKORDER_CREATE_DATE"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 4, strValue);

			rs->GetFieldValue(_T("WORKORDER_CREATE_BY"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 5, strValue);

			rs->GetFieldValue(_T("WORKORDER_DESCRIPTION"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 6, strValue);

			rs->GetFieldValue(_T("WORKORDER_RESPONSIBLE"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 7, strValue);

			rs->GetFieldValue(_T("WORKORDER_STATUS"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 8, strValue);

			rs->GetFieldValue(_T("WORKORDER_LOG"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 9, strValue);

			rs->GetFieldValue(_T("WORKORDER_HISTORY"), strValue);
			m_lscWorkorderExisting.SetItemText(nIndex, 10, strValue);

			rs->MoveNext();
		}
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	else
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	UpdateData(FALSE);

}

void CWorkorderView::InitWorkorderEmployeeResponsibleComboBox()
{
	m_cbxWorkorderEmployeeResponsible.ResetContent();
	m_cbxWorkorderEmployeeResponsible.AddString(_T(">> Select name <<"));

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CRecordset* rs = new CRecordset();
	CString strQuery = _T("SELECT EMPLOYEE_NAME FROM EMPLOYEE ORDER BY EMPLOYEE_NAME");
	if (theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("EMPLOYEE_NAME"), strValue);
			m_cbxWorkorderEmployeeResponsible.AddString(strValue);
			rs->MoveNext();
		}
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	else
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	m_cbxWorkorderEmployeeResponsible.SetCurSel(0);
}

bool CWorkorderView::GetAssetInfo(const unsigned int& nAssetId)
{
	CString strQuery;
	bool bResult = false;

	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT ASSET_DESCRIPTION, ASSET_MODEL_NUMBER, ASSET_BRAND, ASSET_HISTORY_LOG FROM ASSET WHERE ASSET_ID = %d"), nAssetId);

	if (bResult = theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("ASSET_DESCRIPTION"), strValue);
			m_strAssetDescription = strValue;

			rs->GetFieldValue(_T("ASSET_MODEL_NUMBER"), strValue);
			m_strAssetModelNumber = strValue;

			rs->GetFieldValue(_T("ASSET_BRAND"), strValue);
			m_strAssetBrand = strValue;

			rs->GetFieldValue(_T("ASSET_HISTORY_LOG"), strValue);
			m_strAssetHistoryLog = strValue;

			rs->MoveNext();
		}
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	UpdateData(FALSE);

	return bResult;
}

bool CWorkorderView::GetCustomerInfo(const unsigned int& nCustomerId)
{
	CString strQuery;
	bool bResult = false;

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT CUSTOMER_SURNAME, CUSTOMER_NAME, CUSTOMER_CELL_PHONE, CUSTOMER_PHONE, CUSTOMER_EMAIL, CUSTOMER_COMMENT FROM CUSTOMER WHERE CUSTOMER_ID = %d"), nCustomerId);

	if (bResult = theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("CUSTOMER_SURNAME"), strValue);
			m_strCustomerSurname = strValue;

			rs->GetFieldValue(_T("CUSTOMER_NAME"), strValue);
			m_strCustomerName = strValue;

			rs->GetFieldValue(_T("CUSTOMER_CELL_PHONE"), strValue);
			m_strCustomerCellPhone = strValue;

			rs->GetFieldValue(_T("CUSTOMER_PHONE"), strValue);
			m_strCustomerPhone = strValue;

			rs->GetFieldValue(_T("CUSTOMER_EMAIL"), strValue);
			m_strCustomerEmail = strValue;

			rs->GetFieldValue(_T("CUSTOMER_COMMENT"), strValue);
			m_strCustomerComments = strValue;

			rs->MoveNext();
		}
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	UpdateData(FALSE);

	return bResult;
}

#ifdef _DEBUG
void CWorkorderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorkorderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CWorkorderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WORKORDERVIEW_CUSTOMER_SURNAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_WORKORDERVIEW_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_WORKORDER_CUSTOMER_CELLPHONE, m_strCustomerCellPhone);
	DDX_Text(pDX, IDC_WORKORDERVIEW_CUSTOMER_PHONE, m_strCustomerPhone);
	DDX_Text(pDX, IDC_WORDORDERVIEW_CUSTOMER_EMAIL, m_strCustomerEmail);
	DDX_Text(pDX, IDC_WORKORDERVIEW_CUSTOMER_COMMENTS, m_strCustomerComments);
	DDX_Text(pDX, IDC_WORKORDERVIEW_ASSET_DESCRIPTION, m_strAssetDescription);
	DDX_Text(pDX, IDC_WORKORDERVIEW_ASSET_MODEL_NUMBER, m_strAssetModelNumber);
	DDX_Text(pDX, IDC_WORKORDERVIEW_ASSET_BRAND, m_strAssetBrand);
	DDX_Text(pDX, IDC_WORKORDERVIEW_ASSET_HISTORY_LOG, m_strAssetHistoryLog);
	DDX_Text(pDX, IDC_WORKORDERVIEW_CREATED_DATE, m_strWorkorderCreatedDate);
	DDX_Text(pDX, IDC_WORKORDERVIEW_CREATED_BY, m_strWorkorderCreatedBy);
	DDX_Text(pDX, IDC_WORKORDERVIEW_STATUS, m_strWorkorderStatus);
	DDX_Text(pDX, IDC_WORKORDERVIEW_LOG, m_strWorkorderNewLog);
	DDX_Text(pDX, IDC_WORKORDERVIEW_HISTORY, m_strWorkorderHistoryLog);
	DDX_Control(pDX, IDC_WORKORDERVIEW_RESPONSIBLE, m_cbxWorkorderEmployeeResponsible);
	DDX_Control(pDX, IDC_WORKORDERVIEW_ASSET_DISPOSED, m_chbWorkorderAssetDisposed);
	DDX_Control(pDX, IDC_WORKORDERVIEW_CUSTOMER_CONTACTED_CUSTOMER, m_chbWorkorderContactedCustomer);
	DDX_Control(pDX, IDC_WORKORDERVIEW_UPDATE, m_btnWorkorderUpdate);
	DDX_Control(pDX, IDC_WORKORDERVIEW_FINISHED, m_btnWorkorderFinished);
	DDX_Control(pDX, IDC_WORKORDERVIEW_EXISTING, m_lscWorkorderExisting);
	DDX_Control(pDX, IDC_WORKORDERVIEW_USED_PARTS, m_lscWorkorderSpareParts);
	DDX_Control(pDX, IDC_WORKORDERVIEW_PARTS, m_btnWorkorderParts);
	DDX_Control(pDX, IDC_WORKORDERVIEW_LOG, m_edtWorkorderNewLog);
}

void CWorkorderView::OnInitialUpdate()
{
		CFormView::OnInitialUpdate();

		m_lscWorkorderExisting.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_lscWorkorderExisting.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 100);
		m_lscWorkorderExisting.InsertColumn(1, _T("ASSET ID"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(2, _T("CUSTOMER ID"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(3, _T("INVOICE ID"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(4, _T("CREATION DATE"), LVCFMT_LEFT, 100);
		m_lscWorkorderExisting.InsertColumn(5, _T("CREATED BY"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(6, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
		m_lscWorkorderExisting.InsertColumn(7, _T("RESPOSIBLE"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(8, _T("STATUS"), LVCFMT_LEFT, 100);
		m_lscWorkorderExisting.InsertColumn(9, _T("LOG"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(10, _T("HISTORY"), LVCFMT_LEFT, 0);

		m_lscWorkorderSpareParts.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_lscWorkorderSpareParts.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 0);
		m_lscWorkorderSpareParts.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 300);
		m_lscWorkorderSpareParts.InsertColumn(2, _T("AMOUNT"), LVCFMT_LEFT, 100);
		m_lscWorkorderSpareParts.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);
		m_lscWorkorderSpareParts.InsertColumn(4, _T("TOTAL"), LVCFMT_LEFT, 100);

		InitWorkorderEmployeeResponsibleComboBox();
}

BEGIN_MESSAGE_MAP(CWorkorderView, CFormView)
	ON_WM_UPDATEUISTATE()
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDERVIEW_EXISTING, &CWorkorderView::OnNMDoubleClickWorkorderViewExisting)
	ON_CBN_SELCHANGE(IDC_WORKORDERVIEW_RESPONSIBLE, &CWorkorderView::OnCbnSelectChangeWorkorderViewResponsible)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_UPDATE, &CWorkorderView::OnBnClickedWorkorderViewUpdate)
	ON_EN_CHANGE(IDC_WORKORDERVIEW_LOG, &CWorkorderView::OnEnChangeWorkorderViewLog)
END_MESSAGE_MAP()

void CWorkorderView::OnUpdateUIState(UINT nAction, UINT nUIElement)
{
	CWnd* pChild = GetWindow(GW_CHILD);

	switch (nAction)
	{
	case 1:	// UIS_SET. Employee name is selected in the caption bar.
		// nUIElement = 0 means this method is called by the framework when the view is activated, controls are accessible.	
		if (0 == nUIElement)
		{
			while (pChild)	// Go through all child controls of the view and acitvate all.
			{
				pChild->EnableWindow(TRUE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}

			m_btnWorkorderUpdate.EnableWindow(FALSE);
			m_btnWorkorderFinished.EnableWindow(FALSE);
			m_btnWorkorderParts.EnableWindow(FALSE);
			m_cbxWorkorderEmployeeResponsible.EnableWindow(FALSE);
			m_chbWorkorderAssetDisposed.EnableWindow(FALSE);
			m_chbWorkorderContactedCustomer.EnableWindow(FALSE);
			m_edtWorkorderNewLog.EnableWindow(FALSE);

			InitWorkorderExistingList();
			//m_cbxWorkorderEmployeeResponsible.SetCurSel(0);
			m_bWorkorderSelected = false;
		}
		break;
		default:
			while (pChild)	// Go through all child controls of the view and acitvate all.
			{
				pChild->EnableWindow(FALSE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			break;
	}
}

// CWorkorderView message handlers

void CWorkorderView::OnNMDoubleClickWorkorderViewExisting(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1)
	{
		// Get the selected item's text.
		m_nWorkorderId = _ttoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 0));

		const unsigned int unAssetID = _ttoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 1));
		const unsigned int unCustomerID = _ttoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 2));
	
		m_strWorkorderCreatedDate = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 4);
		m_strWorkorderCreatedBy = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 5);
		m_strWorkorderStatus = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 8);
		m_strWorkorderNewLog = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 9);
		m_strWorkorderHistoryLog = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 10);

		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

		if(GetAssetInfo(unAssetID) && GetCustomerInfo(unCustomerID))
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);

		m_bWorkorderSelected = true;

		CString strValue = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 7);

		if(!strValue.IsEmpty())
			m_cbxWorkorderEmployeeResponsible.SelectString(0, strValue.Trim());
		else
			m_cbxWorkorderEmployeeResponsible.SetCurSel(0);

		m_cbxWorkorderEmployeeResponsible.EnableWindow(TRUE);

		if (theApp.GetWorkorderViewType() != VIEW_WORKORDER_OPEN)
		{
			m_edtWorkorderNewLog.EnableWindow(TRUE);
			m_btnWorkorderParts.EnableWindow(TRUE);
			m_btnWorkorderFinished.EnableWindow(TRUE);
			m_chbWorkorderAssetDisposed.EnableWindow(TRUE);
			m_chbWorkorderContactedCustomer.EnableWindow(TRUE);
			m_btnWorkorderUpdate.EnableWindow(FALSE);
			m_bResponsibleChanged = false;
		}
		else
			m_edtWorkorderNewLog.EnableWindow(FALSE);
		

		//UpdateData(FALSE);
	}
	*pResult = 0;
}

void CWorkorderView::OnCbnSelectChangeWorkorderViewResponsible()
{
	switch (theApp.GetWorkorderViewType())
	{
		case VIEW_WORKORDER_OPEN:
			if (m_cbxWorkorderEmployeeResponsible.GetCurSel() && m_bWorkorderSelected)
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
			}
			else
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_edtWorkorderNewLog.EnableWindow(FALSE);
			}
		break;
		case VIEW_WORKORDER_PROGRESS:
			if (m_cbxWorkorderEmployeeResponsible.GetCurSel() && m_bWorkorderSelected)
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_btnWorkorderFinished.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
			}
			else
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
				m_edtWorkorderNewLog.EnableWindow(FALSE);
			}
	default:
		break;
	}
	m_bResponsibleChanged = true;
}

void CWorkorderView::OnEnChangeWorkorderViewLog()
{
	UpdateData(TRUE);
	switch (theApp.GetWorkorderViewType())
	{
		case VIEW_WORKORDER_OPEN:
			if (!m_strWorkorderNewLog.IsEmpty() && m_bWorkorderSelected)
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
			}
			else if(!m_bResponsibleChanged)
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
			}
			break;
		case VIEW_WORKORDER_PROGRESS:
			if (!m_strWorkorderNewLog.IsEmpty() && m_bWorkorderSelected)
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
			}
			else if (!m_bResponsibleChanged)
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
			}
			break;
		default:
			break;
	}
}


void CWorkorderView::OnBnClickedWorkorderViewUpdate()
{
	UpdateData(TRUE);
	
	CTime time = CTime::GetCurrentTime();

	CString strCurDate = time.Format(_T("%d-%m-%y"));
	CString strEmployee;

	int nIndex = m_cbxWorkorderEmployeeResponsible.GetCurSel();
	if (nIndex != CB_ERR && nIndex != 0)
	{
		int nLength = m_cbxWorkorderEmployeeResponsible.GetLBTextLen(nIndex);
		if (nLength != CB_ERR)
		{
			m_cbxWorkorderEmployeeResponsible.GetLBText(nIndex, strEmployee.GetBuffer(nLength));
			strEmployee.ReleaseBuffer();

		}
	}

	switch (theApp.GetWorkorderViewType())
	{
		case VIEW_WORKORDER_OPEN:
		case VIEW_WORKORDER_PROGRESS:
			m_strWorkorderStatus = _T("Progress");
			break;
		default:
			m_strWorkorderStatus = _T("Open");
			break;
	}

	if (m_bResponsibleChanged)
	{
		m_strWorkorderHistoryLog += _T("\r\n[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Assigned to ") + strEmployee;
		m_bResponsibleChanged = false;
	}

	if (!m_strWorkorderNewLog.IsEmpty())
	{
		m_strWorkorderHistoryLog += _T("\r\n[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Comment: ") + m_strWorkorderNewLog;
		m_strWorkorderNewLog.Empty();
	}

	CString strQuery;

	auto buildFieldValue = [](CString str) -> CString
		{
			CString strResult;
			if (str.IsEmpty())
				return  _T("NULL");
			strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
			return strResult;
		};

	strQuery.Format(_T("UPDATE WORKORDER SET WORKORDER_RESPONSIBLE = %s, WORKORDER_STATUS = %s, WORKORDER_LOG = %s, WORKORDER_HISTORY = %s WHERE WORKORDER_ID = %d"),
		static_cast<LPCTSTR>(buildFieldValue(strEmployee)),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderStatus)), 
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderNewLog)),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderHistoryLog)),
		m_nWorkorderId);

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());

	if (!sql.ExecuteQuery(strQuery.GetBuffer()))
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
	}
	else
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
	}

	strQuery.ReleaseBuffer();

	m_nWorkorderId = 0;
	m_strWorkorderCreatedDate.Empty();
	m_strWorkorderCreatedBy.Empty();
	m_strWorkorderStatus.Empty();
	m_strWorkorderNewLog.Empty();
	m_strWorkorderHistoryLog.Empty();
	m_strCustomerSurname.Empty();
	m_strCustomerName.Empty();
	m_strCustomerCellPhone.Empty();
	m_strCustomerPhone.Empty();
	m_strCustomerEmail.Empty();
	m_strCustomerComments.Empty();
	m_strAssetDescription.Empty();
	m_strAssetModelNumber.Empty();
	m_strAssetBrand.Empty();
	m_strAssetHistoryLog.Empty();

	m_chbWorkorderAssetDisposed.SetCheck(BST_UNCHECKED);
	m_chbWorkorderContactedCustomer.SetCheck(BST_UNCHECKED);

	m_lscWorkorderSpareParts.DeleteAllItems();

	m_bWorkorderSelected = false;
	m_bResponsibleChanged = false;

	m_cbxWorkorderEmployeeResponsible.SetCurSel(0);
	m_cbxWorkorderEmployeeResponsible.EnableWindow(FALSE);
	m_edtWorkorderNewLog.EnableWindow(FALSE);
	m_btnWorkorderUpdate.EnableWindow(FALSE);
	m_btnWorkorderFinished.EnableWindow(FALSE);
	m_btnWorkorderParts.EnableWindow(FALSE);
	m_chbWorkorderAssetDisposed.EnableWindow(FALSE);
	m_chbWorkorderContactedCustomer.EnableWindow(FALSE);

	InitWorkorderExistingList();
	UpdateData(FALSE);
}
