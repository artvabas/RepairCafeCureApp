// CWorkorderView.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderView.h"

//using namespace artvabas::rcc::ui;

// CWorkorderView

IMPLEMENT_DYNCREATE(CWorkorderView, CFormView)

CWorkorderView::CWorkorderView() : CFormView(IDD_WORKORDER_FORM)
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
{

}

CWorkorderView::~CWorkorderView()
{
}

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
}

BEGIN_MESSAGE_MAP(CWorkorderView, CFormView)
END_MESSAGE_MAP()


// CWorkorderView diagnostics

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


// CWorkorderView message handlers
