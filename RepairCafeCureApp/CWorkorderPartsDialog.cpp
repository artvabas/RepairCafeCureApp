// CWorkorderPartsDialog.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CWorkorderPartsDialog.h"


// CWorkorderPartsDialog dialog

IMPLEMENT_DYNAMIC(CWorkorderPartsDialog, CDialogEx)

CWorkorderPartsDialog::CWorkorderPartsDialog(const unsigned int& unWorkorderID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORKORDER_PARTS, pParent)
	, m_unWorkorderID(unWorkorderID)
	, m_strWorkorderPartDescription(_T(""))
	, m_strWorkorderPartAmount(_T(""))
	, m_strWorkorderPartUnitPrice(_T(""))
	, m_strWorkorderPartTotalPrice(_T(""))
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


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CWorkorderPartsDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void CWorkorderPartsDialog::OnEnChangeWorkorderAddParts()
{
	UpdateData(TRUE);

	if (m_strWorkorderPartDescription.IsEmpty() || m_strWorkorderPartAmount.IsEmpty() || m_strWorkorderPartUnitPrice.IsEmpty())
	{
		m_btnWorkorderPartAdd.EnableWindow(FALSE);
	}
	else
	{
		m_btnWorkorderPartAdd.EnableWindow(TRUE);
	}
}


void CWorkorderPartsDialog::OnNMDoubleClickWorkorderStockPartsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CWorkorderPartsDialog::OnNMClickWorkorderAddedPartsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CWorkorderPartsDialog::OnBnClickedWorkorderAddPart()
{
	
	CString strWorkorderID;
	strWorkorderID.Format(_T("%d"), m_unWorkorderID);

	auto row = m_lscWorkorderAddedPartList.GetItemCount();

	auto nIndex = m_lscWorkorderAddedPartList.InsertItem(row, strWorkorderID);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 1, m_strWorkorderPartDescription);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 2, m_strWorkorderPartAmount);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 3, m_strWorkorderPartUnitPrice);

	auto nAmount = _ttoi(m_strWorkorderPartAmount);
	auto dUnitPrice = _ttof(m_strWorkorderPartUnitPrice);
	//dUnitPrice = floor(dUnitPrice * 100 + 0.5) / 100; 
	auto nTotalPrice = nAmount * dUnitPrice;

	m_strWorkorderPartTotalPrice.Format(_T("%.2f"), nTotalPrice);
	m_lscWorkorderAddedPartList.SetItemText(nIndex, 4, m_strWorkorderPartTotalPrice);

	CalculateTotalPrice();
}

void CWorkorderPartsDialog::OnLvnItemChangedWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		m_btnWorkorderPartDelete.EnableWindow(TRUE);
	}
	else
	{
		m_btnWorkorderPartDelete.EnableWindow(FALSE);
	}
	*pResult = 0;
}

void CWorkorderPartsDialog::OnNMKillFocusWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult)
{	
	m_btnWorkorderPartDelete.EnableWindow(FALSE);
	*pResult = 0;
}

void CWorkorderPartsDialog::OnBnClickedWorkorderDeleteAddedPart()
{
	int nIndex = m_lscWorkorderAddedPartList.GetNextItem(-1, LVNI_SELECTED);
	m_lscWorkorderAddedPartList.DeleteItem(nIndex);
	//CalculateTotalPrice();
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
