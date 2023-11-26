// CWorkorderPartsDialog.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CWorkorderPartsDialog.h"

using namespace artvabas::sql;


// CWorkorderPartsDialog dialog

IMPLEMENT_DYNAMIC(CWorkorderPartsDialog, CDialogEx)

CWorkorderPartsDialog::CWorkorderPartsDialog(const unsigned int& unWorkorderID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORKORDER_PARTS, pParent)
	, m_unWorkorderID(unWorkorderID)
	, m_strWorkorderPartDescription(_T(""))
	, m_strWorkorderPartAmount(_T(""))
	, m_strWorkorderPartUnitPrice(_T(""))
	, m_strWorkorderPartTotalPrice(_T(""))
	, m_bIsAddedPartListSelected(false)
{

}

CWorkorderPartsDialog::~CWorkorderPartsDialog()
{
}

void CWorkorderPartsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORKORDER_STOCK_PARTS, m_lscWorkorderStockPartList);
	DDX_Control(pDX, IDC_WORKORDER_ADDED_PARTS, m_lscWorkorderAddedPartList);
	DDX_Text(pDX, IDC_WORKORDER_DESCRIPTION_PART, m_strWorkorderPartDescription);
	DDX_Text(pDX, IDC_WORKORDER_AMOUNT_PART, m_strWorkorderPartAmount);
	DDX_Text(pDX, IDC_WORKORDER_UNIT_PRICE_PART, m_strWorkorderPartUnitPrice);
	DDX_Text(pDX, IDC_WORKORDER_TOTAL_PRICE_PARTS, m_strWorkorderPartTotalPrice);
	DDX_Control(pDX, IDC_WORKORDER_ADD_PART, m_btnWorkorderPartAdd);
	DDX_Control(pDX, IDC_WORKORDER_DELETE_ADDED_PART, m_btnWorkorderPartDelete);
	DDX_Control(pDX, IDC_WORKORDER_CHANGE, m_btnWorkorderPartChange);
}


BEGIN_MESSAGE_MAP(CWorkorderPartsDialog, CDialogEx)
	ON_EN_CHANGE(IDC_WORKORDER_DESCRIPTION_PART, &CWorkorderPartsDialog::OnEnChangeWorkorderAddParts)
	ON_EN_CHANGE(IDC_WORKORDER_AMOUNT_PART, &CWorkorderPartsDialog::OnEnChangeWorkorderAddParts)
	ON_EN_CHANGE(IDC_WORKORDER_UNIT_PRICE_PART, &CWorkorderPartsDialog::OnEnChangeWorkorderAddParts)
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDER_STOCK_PARTS, &CWorkorderPartsDialog::OnNMDoubleClickWorkorderStockPartsList)
	ON_NOTIFY(NM_CLICK, IDC_WORKORDER_ADDED_PARTS, &CWorkorderPartsDialog::OnNMClickWorkorderAddedPartsList)
	ON_BN_CLICKED(IDC_WORKORDER_ADD_PART, &CWorkorderPartsDialog::OnBnClickedWorkorderAddPart)
	ON_BN_CLICKED(IDC_WORKORDER_DELETE_ADDED_PART, &CWorkorderPartsDialog::OnBnClickedWorkorderDeleteAddedPart)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WORKORDER_ADDED_PARTS, &CWorkorderPartsDialog::OnLvnItemChangedWorkorderAddedParts)
	ON_NOTIFY(NM_KILLFOCUS, IDC_WORKORDER_ADDED_PARTS, &CWorkorderPartsDialog::OnNMKillFocusWorkorderAddedParts)
	ON_BN_CLICKED(IDC_WORKORDER_CHANGE, &CWorkorderPartsDialog::OnBnClickedWorkorderChange)
END_MESSAGE_MAP()


// CWorkorderPartsDialog message handlers


BOOL CWorkorderPartsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	if (InitStockPartList() && InitAddedPartList())
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	else
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);


	return TRUE;
}


void CWorkorderPartsDialog::OnOK()
{
	CString strQuery;
	strQuery.Format(_T("DELETE FROM [WORKORDER_PARTS] WHERE ([WORKORDER_PARTS_WORKORDER_ID] = %d)"), m_unWorkorderID);

	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());

	sql.ExecuteQuery(strQuery.GetBuffer());

	if (m_lscWorkorderAddedPartList.GetItemCount() > 0)
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
		
		strQuery.ReleaseBuffer();

		for (int i = 0; i < m_lscWorkorderAddedPartList.GetItemCount(); i++)
		{
			CString strDescription = m_lscWorkorderAddedPartList.GetItemText(i, 1);
			unsigned int uiAmount = _ttoi(m_lscWorkorderAddedPartList.GetItemText(i, 2));
			double dUnitPrice = _ttof(m_lscWorkorderAddedPartList.GetItemText(i, 3));
			double dTotalPrice = _ttof(m_lscWorkorderAddedPartList.GetItemText(i, 4));

			// Build the fields value for the query.
			auto buildFieldValue = [](CString str) -> CString
				{
					CString strResult;
					if (str.IsEmpty())
						return  _T("NULL");
					strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
					return strResult;
				};

			strQuery.Format(_T("INSERT INTO [WORKORDER_PARTS] ([WORKORDER_PARTS_WORKORDER_ID], [WORKORDER_PARTS_DESCRIPTION], [WORKORDER_PARTS_AMOUNT], [WORKORDER_PARTS_UNIT_PRICE], [WORKORDER_PARTS_TOTAL_PRICE]) VALUES (%d, %s, %d, %f, %f)"),
				m_unWorkorderID, static_cast<LPCTSTR>(buildFieldValue(strDescription)), uiAmount, dUnitPrice, dTotalPrice);
				
			if (!sql.ExecuteQuery(strQuery.GetBuffer()))
			{
				theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
			}
			else
			{
				theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
			}
			strQuery.ReleaseBuffer();
		}
	}
	CDialogEx::OnOK();
}


void CWorkorderPartsDialog::OnEnChangeWorkorderAddParts()
{
	UpdateData(TRUE);

	if (m_strWorkorderPartDescription.IsEmpty() || m_strWorkorderPartAmount.IsEmpty() || m_strWorkorderPartUnitPrice.IsEmpty())
	{
		m_btnWorkorderPartAdd.EnableWindow(FALSE);
		m_btnWorkorderPartChange.EnableWindow(FALSE);
	}
	else
	{
		if(!m_bIsAddedPartListSelected) m_btnWorkorderPartAdd.EnableWindow(TRUE);
		if(m_bIsAddedPartListSelected) m_btnWorkorderPartChange.EnableWindow(TRUE);
	}
}


void CWorkorderPartsDialog::OnNMDoubleClickWorkorderStockPartsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto nIndex = m_lscWorkorderStockPartList.GetNextItem(-1, LVNI_SELECTED);

	if (nIndex != -1)
	{
		m_strWorkorderPartDescription = m_lscWorkorderStockPartList.GetItemText(nIndex, 1);
		m_strWorkorderPartUnitPrice = m_lscWorkorderStockPartList.GetItemText(nIndex, 3);
		m_strWorkorderPartAmount = _T("1");
		UpdateData(FALSE);
		OnEnChangeWorkorderAddParts();
	}

	*pResult = 0;
}


void CWorkorderPartsDialog::OnNMClickWorkorderAddedPartsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto nIndex = m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED);

	if (nIndex != -1)
	{
		m_strWorkorderPartDescription = m_lscWorkorderAddedPartList.GetItemText(nIndex, 1);
		m_strWorkorderPartAmount = m_lscWorkorderAddedPartList.GetItemText(nIndex, 2);
		m_strWorkorderPartUnitPrice = m_lscWorkorderAddedPartList.GetItemText(nIndex, 3);
		UpdateData(FALSE);

		m_bIsAddedPartListSelected = true;
	}

	*pResult = 0;
}


void CWorkorderPartsDialog::OnBnClickedWorkorderAddPart()
{
	
	CString strWorkorderID;
	strWorkorderID.Format(_T("%d"), m_unWorkorderID);

	auto nAmount = _ttoi(m_strWorkorderPartAmount);
	auto dUnitPrice = _ttof(m_strWorkorderPartUnitPrice);
	auto nTotalPrice = nAmount * dUnitPrice;

	CString strUnitPrice;
	strUnitPrice.Format(_T("%.2f"), dUnitPrice);

	auto row = m_lscWorkorderAddedPartList.GetItemCount();

	auto nIndex = m_lscWorkorderAddedPartList.InsertItem(row, strWorkorderID);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, m_strWorkorderPartDescription);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, m_strWorkorderPartAmount);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, strUnitPrice);


	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), nTotalPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, m_strWorkorderPartTotalPrice);

	ClearPartInputFields();

	m_bIsAddedPartListSelected = false;

	OnEnChangeWorkorderAddParts();

	CalculateTotalPrice();
}


void CWorkorderPartsDialog::OnLvnItemChangedWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		SetChangeDeleteButtonState();
	}
	else
	{
		SetChangeDeleteButtonState(FALSE);
	}
	*pResult = 0;
}


void CWorkorderPartsDialog::OnNMKillFocusWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult)
{	
	if (m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED) == -1)
	{
		SetChangeDeleteButtonState(FALSE);
	}

	*pResult = 0;
}

void CWorkorderPartsDialog::OnBnClickedWorkorderChange()
{
	int nIndex = m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, m_strWorkorderPartDescription);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, m_strWorkorderPartAmount);

	auto nAmount = _ttoi(m_strWorkorderPartAmount);
	auto dUnitPrice = _ttof(m_strWorkorderPartUnitPrice);
	auto nTotalPrice = nAmount * dUnitPrice;

	CString strUnitPrice;
	strUnitPrice.Format(_T("%.2f"), dUnitPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, strUnitPrice);

	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), nTotalPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, m_strWorkorderPartTotalPrice);

	ClearPartInputFields();

	m_bIsAddedPartListSelected = false;

	OnEnChangeWorkorderAddParts();

	CalculateTotalPrice();
}

void CWorkorderPartsDialog::OnBnClickedWorkorderDeleteAddedPart()
{
	int nIndex = m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED);
	m_lscWorkorderAddedPartList.DeleteItem(nIndex);

	ClearPartInputFields();

	CalculateTotalPrice();
}

bool CWorkorderPartsDialog::InitStockPartList()
{
	bool bResult = false;

	m_lscWorkorderStockPartList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lscWorkorderStockPartList.InsertColumn(0, _T("PART ID"), LVCFMT_LEFT, 0);
	m_lscWorkorderStockPartList.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
	m_lscWorkorderStockPartList.InsertColumn(2, _T("IN STOCK"), LVCFMT_LEFT, 0);
	m_lscWorkorderStockPartList.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strQuery;	// Query string.

	// Query all parts from the database.
	strQuery.Format(_T("SELECT * FROM SPAREPARTSTOCK"));
	CRecordset* rs = new CRecordset();

	if (bResult = theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		// Fill the existing customers list control with the found customers from the database.
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("SPAREPARTSTOCK_ID"), strValue);
			nIndex = m_lscWorkorderStockPartList.InsertItem(row++, strValue);

			rs->GetFieldValue(_T("SPAREPARTSTOCK_DESCRIPTION"), strValue);
			m_lscWorkorderStockPartList.SetItemText(nIndex, 1, strValue);

			rs->GetFieldValue(_T("SPAREPARTSTOCK_IN_STOCK"), strValue);
			m_lscWorkorderStockPartList.SetItemText(nIndex, 2, strValue);

			rs->GetFieldValue(_T("SPAREPARTSTOCK_PRICE"), strValue);
			auto dPrice = _ttof(strValue);
			strValue.Format(_T("%.2f"), dPrice);
			m_lscWorkorderStockPartList.SetItemText(nIndex, 3, strValue);

			rs->MoveNext();
		}
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	UpdateData(FALSE);

	return bResult;
}

bool CWorkorderPartsDialog::InitAddedPartList()
{
	bool bResult = false;

	m_lscWorkorderAddedPartList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lscWorkorderAddedPartList.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 0);
	m_lscWorkorderAddedPartList.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
	m_lscWorkorderAddedPartList.InsertColumn(2, _T("AMOUNT"), LVCFMT_LEFT, 100);
	m_lscWorkorderAddedPartList.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);
	m_lscWorkorderAddedPartList.InsertColumn(4, _T("TOTAL PRICE"), LVCFMT_LEFT, 100);

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strQuery;	// Query string.

	// Query all parts from the database.
	strQuery.Format(_T("SELECT * FROM WORKORDER_PARTS WHERE WORKORDER_PARTS_WORKORDER_ID = %d"), m_unWorkorderID);
	CRecordset* rs = new CRecordset();

	if (bResult = theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("WORKORDER_PARTS_WORKORDER_ID"), strValue);
			nIndex = m_lscWorkorderAddedPartList.InsertItem(row++, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_DESCRIPTION"), strValue);
			m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_AMOUNT"), strValue);
			m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_UNIT_PRICE"), strValue);
			m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_TOTAL_PRICE"), strValue);
			m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, strValue);

			rs->MoveNext();
		}
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	UpdateData(FALSE);

	return bResult;
}

void CWorkorderPartsDialog::CalculateTotalPrice()
{
	double dTotalPrice(0.0);
	CString strValue = _T("");

	for (int i = 0; i < m_lscWorkorderAddedPartList.GetItemCount(); i++)
	{
	
		strValue = m_lscWorkorderAddedPartList.GetItemText(i, 4);
		dTotalPrice += _ttof(strValue);
	}

	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), dTotalPrice);
	UpdateData(FALSE);
}

void CWorkorderPartsDialog::SetChangeDeleteButtonState(BOOL bFlag)
{
	m_btnWorkorderPartChange.EnableWindow(bFlag);
	m_btnWorkorderPartDelete.EnableWindow(bFlag);
}

void CWorkorderPartsDialog::ClearPartInputFields()
{
	m_strWorkorderPartAmount = _T("");
	m_strWorkorderPartDescription = _T("");
	m_strWorkorderPartUnitPrice = _T("");
	UpdateData(FALSE);
}
