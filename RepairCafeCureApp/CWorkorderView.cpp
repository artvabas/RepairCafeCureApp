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
* File: CWorkorderView.cpp, implementation of class CWorkorderView
*
* This class is the view of the workorder form.
* With this form the user can search for existing workorders and update existing workorders.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.1.0 (beta)
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 08-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderView.h"
#include "CWorkorderPartsDialog.h"
#include "MainFrm.h"
#include "CPrintWorkorder.h"
#include "CContributionPaymentDialog.h"
#include "DatabaseTables.h"

using namespace artvabas::rcc::ui;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;
using namespace artvabas::rcc::support;
using namespace artvabas::database::tables::workorder;
using namespace artvabas::database::tables::workorderparts;
using namespace artvabas::database::tables::asset;
using namespace artvabas::database::tables::customer;

IMPLEMENT_DYNCREATE(CWorkorderView, CFormView)

CWorkorderView::CWorkorderView() noexcept
	: CFormView(IDD_WORKORDER_FORM)
	//, m_pDC{ NULL }
	, m_bWorkorderSelected{ false }
	, m_bResponsibleChanged{ false }
	, m_bPrintCombi{ false }
	, m_bPrintInvoice{ false }
	, m_bPinTransaction{ false }
	, m_ePrinterOrientation{ LANDSCAPE }
	, m_unWorkorderId{ 0 }
	, m_strCustomerSurname{ _T("") }
	, m_strCustomerName{ _T("") }
	, m_strCustomerCellPhone{ _T("") }
	, m_strCustomerPhone{ _T("") }
	, m_strCustomerEmail{ _T("") }
	, m_strCustomerComments{ _T("") }
	, m_strAssetDescription{ _T("") }
	, m_strAssetModelNumber{ _T("") }
	, m_strAssetBrand{ _T("") }
	, m_strAssetHistoryLog{ _T("") }
	, m_strWorkorderCreatedDate{ _T("") }
	, m_strWorkorderCreatedBy{ _T("") }
	, m_strWorkorderDescription{ _T("") }
	, m_strWorkorderStatus{ _T("") }
	, m_strWorkorderClosedDate{ _T("") }
	, m_strWorkorderNewLog{ _T("") }
	, m_strWorkorderHistoryLog{ _T("") }
	, m_strWorkorderTotalPartsPrice{ _T("") }
{}

CWorkorderView::~CWorkorderView()
{}
/* Message handles binders */
#pragma warning ( push )
#pragma warning ( disable : 26454 )
BEGIN_MESSAGE_MAP(CWorkorderView, CFormView)
	ON_WM_UPDATEUISTATE()
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDERVIEW_EXISTING,
		&CWorkorderView::OnNMDoubleClickWorkorderViewExisting)
	ON_CBN_SELCHANGE(IDC_WORKORDERVIEW_RESPONSIBLE,
		&CWorkorderView::OnCbnSelectChangeWorkorderViewResponsible)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_UPDATE,
		&CWorkorderView::OnBnClickedWorkorderViewUpdate)
	ON_EN_CHANGE(IDC_WORKORDERVIEW_LOG,
		&CWorkorderView::OnEnChangeWorkorderViewLog)
	ON_EN_CHANGE(IDC_WORKORDERVIEW_ASSET_DESCRIPTION, 
		&CWorkorderView::OnEnChangeWorkorderViewAsset)
	ON_EN_CHANGE(IDC_WORKORDERVIEW_ASSET_MODEL_NUMBER, 
		&CWorkorderView::OnEnChangeWorkorderViewAsset)
	ON_EN_CHANGE(IDC_WORKORDERVIEW_ASSET_BRAND, 
		&CWorkorderView::OnEnChangeWorkorderViewAsset)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_CUSTOMER_CONTACTED_CUSTOMER,
		&CWorkorderView::OnBnClickedWorkorderVewCustomerContactedCustomer)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_FINISHED,
		&CWorkorderView::OnBnClickedWorkorderViewFinished)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_ASSET_DISPOSED,
		&CWorkorderView::OnBnClickedWorkorderViewAssetDisposed)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_ASSETDETAILS_UPDATE, 
		&CWorkorderView::OnBnClickedWorkorderViewAssetDetailsUpdate)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_PARTS,
		&CWorkorderView::OnBnClickedWorkorderViewParts)
	ON_COMMAND(ID_FILE_PRINT,
		&CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,
		&CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,
		&CWorkorderView::OnFilePrintPreview)
	ON_COMMAND(ID_WORKORDER_EXTRA_COMBI,
		&CWorkorderView::OnWorkorderExtraCombi)
	ON_COMMAND(ID_WORKORDER_EXTRA_INVOICE,
		&CWorkorderView::OnWorkorderExtraInvoice)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_CLOSE,
		&CWorkorderView::OnBnClickedWorkorderViewClose)
END_MESSAGE_MAP()
#pragma warning ( pop )

/* Overrides methods */

// DoDataExchange is called by the framework to exchange and validate form data.
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
	DDX_Text(pDX, IDC_WORKORDERVIEW_TOTAL_PRICE, m_strWorkorderTotalPartsPrice);
	DDX_Control(pDX, IDC_WORKORDERVIEW_CLOSE, m_btnWorkorderClose);
	DDX_Control(pDX, IDC_WORKORDERVIEW_ASSETDETAILS_UPDATE, m_btnWorkorderAssetUpdate);
	DDX_Control(pDX, IDC_WORKORDERVIEW_ASSET_DESCRIPTION, m_edcAssetDescription);
	DDX_Control(pDX, IDC_WORKORDERVIEW_ASSET_MODEL_NUMBER, m_edcAssetModelNumber);
	DDX_Control(pDX, IDC_WORKORDERVIEW_ASSET_BRAND, m_edcAssetBrand);
}

// OnInitialUpdate is called by the framework when the view is first activated.
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
		m_lscWorkorderExisting.InsertColumn(6, _T("RESPONSIBLE"), LVCFMT_LEFT, 100);
		m_lscWorkorderExisting.InsertColumn(7, _T("STATUS"), LVCFMT_LEFT, 100);
		m_lscWorkorderExisting.InsertColumn(8, _T("CLOSED DATE"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(9, _T("HISTORY"), LVCFMT_LEFT, 0);
		m_lscWorkorderExisting.InsertColumn(10, _T("ASSET"), LVCFMT_LEFT, 100);
		m_lscWorkorderExisting.InsertColumn(11, _T("DESCRIPTION"), LVCFMT_LEFT, 600);

		m_lscWorkorderSpareParts.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_lscWorkorderSpareParts.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 0);
		m_lscWorkorderSpareParts.InsertColumn(1, _T("DESCRIPTION"), LVCFMT_LEFT, 300);
		m_lscWorkorderSpareParts.InsertColumn(2, _T("AMOUNT"), LVCFMT_LEFT, 100);
		m_lscWorkorderSpareParts.InsertColumn(3, _T("UNIT PRICE"), LVCFMT_LEFT, 100);
		m_lscWorkorderSpareParts.InsertColumn(4, _T("TOTAL"), LVCFMT_LEFT, 100);

		SetCustomFocusButton(&m_btnWorkorderParts, ColorButton::BLUE, false);

		InitWorkorderEmployeeResponsibleComboBox();
}

// PreTranslateMessage is called by the framework before the message is dispatched.
// This method is used to handle the Enter key press event to virtual make mouse clicks on buttons
// - pMsg: A pointer to the message structure.
BOOL CWorkorderView::PreTranslateMessage(MSG* pMsg)
{

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN) {
			// Check if the Shift key is also down
			if (GetKeyState(VK_SHIFT) & 0x8000) // 0x8000 checks if the high-order bit is set
			{
				// Shift + Enter was pressed
				// Return to App for handling, means new line in edit Box

				return CFormView::PreTranslateMessage(pMsg);
			}
			if (m_btnWorkorderUpdate.IsWindowEnabled() && m_btnWorkorderFinished.IsWindowEnabled()) {
				OnBnClickedWorkorderViewFinished();
				return TRUE;
			} else if ( m_btnWorkorderUpdate.IsWindowEnabled() ) {
				OnBnClickedWorkorderViewUpdate();
				return TRUE;
			}
			if ( m_btnWorkorderClose.IsWindowEnabled() ) {
				OnBnClickedWorkorderViewClose();
				return TRUE;
			}
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

// OnPreparePrinting is called by the framework before the print job is started.
// - pInfo: A pointer to the print information structure.
BOOL CWorkorderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

// OnBeginPrinting is called by the framework when the print job is started.
// - pDC: A pointer to the device context.
// - pInfo: A pointer to the print information structure.
void CWorkorderView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	SetPrinterOrientation(theApp.GetDeviceMode(), pDC);
	CFormView::OnBeginPrinting(pDC, pInfo);
}

// OnEndPrinting is called by the framework when the print job is finished.
// - pDC: A pointer to the device context.
// - pInfo: A pointer to the print information structure.
void CWorkorderView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

// OnPrint: Called by the framework to print the view.
// - pDC: A pointer to the device context.
// - pInfo: A pointer to the print information structure.
void CWorkorderView::OnPrint(CDC* pDC, CPrintInfo* pInfo) {
	BeginWaitCursor();
	if (m_bPrintInvoice)	{
		CPrintWorkorder::WorkorderData workorderData{};
		workorderData.strWorkorderID.Format(_T("%d"), m_unWorkorderId);
		workorderData.strCustomerSurname = m_strCustomerSurname;
		workorderData.strCustomerName = m_strCustomerName;
		workorderData.strCustomerCellPhone = m_strCustomerCellPhone;
		workorderData.strCustomerPhone = m_strCustomerPhone;
		workorderData.strCustomerEmail = m_strCustomerEmail;
		workorderData.strCustomerComments = m_strCustomerComments;
		workorderData.strAssetDescription = m_strAssetDescription;
		workorderData.strAssetModelNumber = m_strAssetModelNumber;
		workorderData.strAssetBrand = m_strAssetBrand;
		workorderData.strWorkorderCreatedDate = m_strWorkorderCreatedDate;
		m_cbxWorkorderEmployeeResponsible.GetWindowTextW(workorderData.strEmployeeResponsible);
		workorderData.strWorkorderStatus = m_strWorkorderStatus;
		workorderData.strWorkorderDescription = m_strWorkorderDescription;
		workorderData.strWorkorderTotalPartsPrice = m_strWorkorderTotalPartsPrice;
		workorderData.m_bIsPinTransfer = m_bPinTransaction;

		auto nCount{ m_lscWorkorderSpareParts.GetItemCount() };
		for (auto i{ 0 }; i < nCount; i++) {
			
			workorderData.m_structSparePArts.strSparePartDescription = m_lscWorkorderSpareParts.GetItemText(i, 1);
			workorderData.m_structSparePArts.strSparePartQuantity = m_lscWorkorderSpareParts.GetItemText(i, 2);
			workorderData.m_structSparePArts.strSparePartPrice = m_lscWorkorderSpareParts.GetItemText(i, 3);
			workorderData.m_structSparePArts.strSparePartSubTotalPrice = m_lscWorkorderSpareParts.GetItemText(i, 4);
			workorderData.m_vecSpareParts.push_back(workorderData.m_structSparePArts);
		}

		workorderData.m_structWorkorderLog.strWorkorderLog = m_strWorkorderHistoryLog;
		workorderData.m_structWorkorderLog.strWorkorderRepairedDate = m_strWorkorderClosedDate;

		CPrintWorkorder printWorkorder{ &workorderData };
		printWorkorder.PrintInvoice(pDC);
		m_bPrintInvoice = false;
	}
	else if (m_bPrintCombi || m_bWorkorderSelected) {
		CPrintWorkorder::WorkorderData workorderData{};
		workorderData.strWorkorderID.Format(_T("%d"), m_unWorkorderId);
		workorderData.strCustomerSurname = m_strCustomerSurname;
		workorderData.strCustomerName = m_strCustomerName;
		workorderData.strCustomerCellPhone = m_strCustomerCellPhone;
		workorderData.strCustomerPhone = m_strCustomerPhone;
		workorderData.strCustomerEmail = m_strCustomerEmail;
		workorderData.strCustomerComments = m_strCustomerComments;
		workorderData.strAssetDescription = m_strAssetDescription;
		workorderData.strAssetModelNumber = m_strAssetModelNumber;
		workorderData.strAssetBrand = m_strAssetBrand;
		workorderData.strWorkorderCreatedDate = m_strWorkorderCreatedDate;
		m_cbxWorkorderEmployeeResponsible.GetWindowTextW(workorderData.strEmployeeResponsible);
		workorderData.strWorkorderStatus = m_strWorkorderStatus;
		workorderData.strWorkorderDescription = m_strWorkorderDescription;

		CPrintWorkorder printWorkorder{ &workorderData };
		printWorkorder.PrintCombi(pDC);
		m_bPrintCombi = false;
	}
	EndWaitCursor();
	CFormView::OnPrint(pDC, pInfo);
}

#ifdef _DEBUG
// AssetValid: Asserts the validity of the view.
void CWorkorderView::AssertValid() const { CFormView::AssertValid(); }
#ifndef _WIN32_WCE
// Dump: Dumps the view.
// - dc: A reference to the dump context.
void CWorkorderView::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif
#endif //_DEBUG

/* Message methods */

// OnFilePrintPreview is called by the framework when the user clicks on the print preview button.
void CWorkorderView::OnFilePrintPreview() noexcept
{
	m_ePrinterOrientation = LANDSCAPE;
	AFXPrintPreview(this);
}

// OnUpdateUIState is called by the framework when the view is activated.
// This method is used to enable and disable controls depending on the state of the form.
// - nAction: The action to be taken.
// - nUIElement: The UI element to be updated.
void CWorkorderView::OnUpdateUIState(UINT nAction, UINT nUIElement) noexcept
{
	auto *pChild{ GetWindow(GW_CHILD) };

	switch (nAction) {
		case UIS_SET:	// 1 means - Employee name is selected in the caption bar.
			// nUIElement = 0 means this method is called by the framework when the view is activated, controls are accessible.	
			if ( 0 == nUIElement ) {
				// Go through all child controls of the view and activate all.
				while ( pChild ) {
					pChild->EnableWindow(TRUE);
					pChild = pChild->GetWindow(GW_HWNDNEXT);
				}
				ResetAllControls();	// Reset all controls to their default state.
			}
			break;
		default:
			// This view is activated, but employee name is not selected in the caption bar.
			while ( pChild ) {
				pChild->EnableWindow(FALSE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			break;
	}
}

// OnNMDoubleClickWorkorderViewExisting is called by the framework when the user double clicks on the existing workorder list control.
// This method is used to get the selected workorder information and set the form controls.
// - pNMHDR: A pointer to the notification message header.
// - pResult: A pointer to the result.
void CWorkorderView::OnNMDoubleClickWorkorderViewExisting(NMHDR* pNMHDR, LRESULT* pResult) noexcept
{
	auto pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if ( pNMItemActivate->iItem != -1 ) {
		// Get the selected item's text.
		m_unWorkorderId = _wtoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 0));

		const unsigned unAssetID = _wtoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 1));
		const unsigned unCustomerID = _wtoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 2));
	
		m_strWorkorderCreatedDate = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 4);
		m_strWorkorderCreatedBy = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 5);
		m_strWorkorderDescription = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 11);
		m_strWorkorderStatus = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 7);
		m_strWorkorderHistoryLog = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 9);

		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

		// Get the asset and customer information.
		if(GetAssetInfo(unAssetID) && GetCustomerInfo(unCustomerID))
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);

		InitWorkorderSparePartsList();

		// Set the employee responsible combo box on this form.
		auto strValue = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 6);

		if( !strValue.IsEmpty() )
			m_cbxWorkorderEmployeeResponsible.SelectString(0, strValue.Trim());
		else
			m_cbxWorkorderEmployeeResponsible.SetCurSel(0);

		auto *pMainFrame = reinterpret_cast<CMainFrame*>(GetParentFrame());
		auto *ribbonBar = pMainFrame->GetRibbonBar();

		switch ( theApp.GetWorkorderViewType() ) {
			case VIEW_WORKORDER_OPEN:
				m_edtWorkorderNewLog.EnableWindow(FALSE);
				m_cbxWorkorderEmployeeResponsible.EnableWindow(TRUE);
				m_cbxWorkorderEmployeeResponsible.SetFocus();
				theApp.ShowContextCategory(ID_CONTEXT_WORKORDER);
				break;
			case VIEW_WORKORDER_PROGRESS:
				m_edtWorkorderNewLog.EnableWindow(TRUE);
				m_edcAssetDescription.SetReadOnly(FALSE);
				m_edcAssetModelNumber.SetReadOnly(FALSE);
				m_edcAssetBrand.SetReadOnly(FALSE);
				m_btnWorkorderAssetUpdate.EnableWindow(FALSE);
				m_btnWorkorderParts.EnableWindow(TRUE);
				m_chbWorkorderAssetDisposed.EnableWindow(TRUE);
				m_chbWorkorderContactedCustomer.EnableWindow(TRUE);
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_cbxWorkorderEmployeeResponsible.EnableWindow(TRUE);
				m_bResponsibleChanged = false;
				theApp.ShowContextCategory(ID_CONTEXT_WORKORDER);
				break;
			case VIEW_WORKORDER_REPAIRED:
				m_edtWorkorderNewLog.EnableWindow(FALSE);
				m_btnWorkorderParts.EnableWindow(FALSE);
				m_chbWorkorderAssetDisposed.EnableWindow(FALSE);
				m_chbWorkorderContactedCustomer.EnableWindow(TRUE);
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderClose.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::RED, true);
				m_cbxWorkorderEmployeeResponsible.EnableWindow(FALSE);
				m_bResponsibleChanged = false;
				theApp.ShowContextCategory(ID_CONTEXT_WORKORDER);
				break;
		}

		m_bWorkorderSelected = true;
		m_bPrintCombi = true;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

// OnCbnSelectChangeWorkorderViewResponsible is called by the framework when the user changes the employee responsible combo box.
// This method is used to enable and disable controls depending on the state of the form.
void CWorkorderView::OnCbnSelectChangeWorkorderViewResponsible() noexcept
{
	switch ( theApp.GetWorkorderViewType() ) {
		case VIEW_WORKORDER_OPEN:
			// Is a employee responsible selected and is a workorder selected?
			if ( m_cbxWorkorderEmployeeResponsible.GetCurSel() && m_bWorkorderSelected ) {
				m_edtWorkorderNewLog.EnableWindow(TRUE);
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::RED, true);
			} else { // No employee responsible selected or no workorder selected.
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_edtWorkorderNewLog.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
			}
		break;
		case VIEW_WORKORDER_PROGRESS:
			// Is a employee responsible selected and is a workorder selected and is the 
			// contacted customer check box not checked and is the asset disposed check box not checked?
			if ( m_cbxWorkorderEmployeeResponsible.GetCurSel() && m_bWorkorderSelected && 
				(!m_chbWorkorderContactedCustomer.GetCheck()) && (!m_chbWorkorderAssetDisposed.GetCheck()) ) {
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::RED, true);
			} else if ( m_chbWorkorderContactedCustomer.GetCheck() ) { // But if the Workorder Contact Customer Checkbox is selected
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::RED, true);
			} else { // No employee responsible selected.
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
				m_edtWorkorderNewLog.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::BLACK, false);
			}
	}
	m_bResponsibleChanged = true;
}

// OnEnChangeWorkorderViewLog is called by the framework when the user changes the new log edit control.
// This method is used to enable and disable controls depending on the state of the form.
void CWorkorderView::OnEnChangeWorkorderViewLog() noexcept
{
	UpdateData(TRUE);
	switch ( theApp.GetWorkorderViewType() ) {
		case VIEW_WORKORDER_OPEN:
			// Is the new log not empty and is a workorder selected?
			if ( !m_strWorkorderNewLog.IsEmpty() && m_bWorkorderSelected ) {
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::RED, false);
			} else if ( !m_bResponsibleChanged ) {
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
			}
			break;
		default:
			SetControlsAfterChangeContactedOrDisposed();
			break;
	}
	UpdateData(FALSE);
}

// OnEnChangeWorkorderViewAsset is called by the framework when the user changes the asset description edit control.
// This method is used to enable and disable controls depending on the state of the form.
void CWorkorderView::OnEnChangeWorkorderViewAsset() noexcept
{
	UpdateData(TRUE);
	m_edtWorkorderNewLog.EnableWindow(FALSE);
	m_btnWorkorderParts.EnableWindow(FALSE);
	m_chbWorkorderAssetDisposed.EnableWindow(FALSE);
	m_chbWorkorderContactedCustomer.EnableWindow(FALSE);
	m_btnWorkorderUpdate.EnableWindow(FALSE);
	m_cbxWorkorderEmployeeResponsible.EnableWindow(FALSE);
	m_lscWorkorderExisting.EnableWindow(FALSE);
	m_lscWorkorderSpareParts.EnableWindow(FALSE);

	if (m_strAssetDescription.IsEmpty()) m_btnWorkorderAssetUpdate.EnableWindow(FALSE);
	else {
		m_btnWorkorderAssetUpdate.EnableWindow(TRUE);
		SetCustomFocusButton(&m_btnWorkorderAssetUpdate, RED, false);
	}
}

// OnBnClickedWorkorderViewAssetDisposed is called by the framework when the user clicks on the asset disposed check box.
// This method is used to enable and disable controls depending on the state of the form.
void CWorkorderView::OnBnClickedWorkorderViewAssetDisposed() noexcept
{
	SetControlsAfterChangeContactedOrDisposed();
}

// OnBnClickedWorkorderViewAssetDetailsUpdate is called by the framework when the user clicks on the asset details update button.
// This method is used to update the asset information in the database.
void CWorkorderView::OnBnClickedWorkorderViewAssetDetailsUpdate() noexcept
{
	UpdateData(TRUE);
	CString strQuery;
	auto nAssetID  = _wtoi(m_lscWorkorderExisting.GetItemText(m_lscWorkorderExisting.GetSelectionMark(), 1));

	auto buildFieldValue = [](CString str) -> CString {
		CString strResult;
		if (str.IsEmpty()) return _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return strResult;
		};

	strQuery.Format(_T("UPDATE ASSET SET ASSET_DESCRIPTION = %s, ASSET_MODEL_NUMBER = %s,")
		_T("ASSET_BRAND = %s WHERE ASSET_ID = %d"),
		static_cast<LPCTSTR>(buildFieldValue(m_strAssetDescription)),
		static_cast<LPCTSTR>(buildFieldValue(m_strAssetModelNumber)),
		static_cast<LPCTSTR>(buildFieldValue(m_strAssetBrand)),
		nAssetID);

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		if (!sql.ExecuteQuery(strQuery.GetBuffer()))
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
		else {
			m_edtWorkorderNewLog.EnableWindow(TRUE);
			m_btnWorkorderParts.EnableWindow(TRUE);
			m_chbWorkorderAssetDisposed.EnableWindow(TRUE);
			m_chbWorkorderContactedCustomer.EnableWindow(TRUE);
			m_btnWorkorderAssetUpdate.EnableWindow(FALSE);
			m_cbxWorkorderEmployeeResponsible.EnableWindow(TRUE);
			m_lscWorkorderExisting.EnableWindow(TRUE);
			m_lscWorkorderSpareParts.EnableWindow(TRUE);
			OnEnChangeWorkorderViewLog();
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_OK);
		}
		strQuery.ReleaseBuffer();
	}

	sql.CloseConnection();
	theApp.EndWaitCursor();
}

// OnBnClickedWorkorderVewCustomerContactedCustomer is called by the framework when the user clicks on the contacted customer check box.
// This method is used to enable and disable controls depending on the state of the form.
void CWorkorderView::OnBnClickedWorkorderVewCustomerContactedCustomer() noexcept
{
	SetControlsAfterChangeContactedOrDisposed();
}

// OnBnClickedWorkorderViewUpdate is called by the framework when the user clicks on the update button.
// This method set the status of the workorder depend on the state of the form.
void CWorkorderView::OnBnClickedWorkorderViewUpdate() noexcept
{
	switch ( theApp.GetWorkorderViewType() ) {
		case VIEW_WORKORDER_OPEN:
		case VIEW_WORKORDER_PROGRESS:
			m_strWorkorderStatus = _T("Progress");
			break;
		case VIEW_WORKORDER_REPAIRED:
			m_strWorkorderStatus = _T("Repaired");
			break;
		default:
			m_strWorkorderStatus = _T("Open");
			break;
	}
	UpdateData(FALSE);
	PerformWorkorderUpdate();
}

// OnBnClickedWorkorderViewFinished is called by the framework when the user clicks on the finished button.
// This method set the status of the workorder to repaired.
void CWorkorderView::OnBnClickedWorkorderViewFinished() noexcept
{
	m_strWorkorderStatus = _T("Repaired");
	UpdateData(FALSE);
	PerformWorkorderUpdate();
}

// OnBnClickedWorkorderViewClose is called by the framework when the user clicks on the close button.
// This method set the status of the workorder to closed and show the contribution payment dialog.
// If the asset is disposed, the workorder is updated and the contribution payment is added to the database.
void CWorkorderView::OnBnClickedWorkorderViewClose() {
	m_strWorkorderStatus = _T("Closed");
	UpdateData(FALSE);
	if( m_chbWorkorderAssetDisposed.GetCheck() )
		PerformWorkorderUpdate();
	else {
		CContributionPaymentDialog::InvoiceData invoiceData{};
		CContributionPaymentDialog::ContributionData contributionData{};

		invoiceData.unAssetID = _wtoi(m_lscWorkorderExisting.GetItemText(m_lscWorkorderExisting.GetSelectionMark(), 1));
		invoiceData.unCustomerID = _wtoi(m_lscWorkorderExisting.GetItemText(m_lscWorkorderExisting.GetSelectionMark(), 2));
		invoiceData.unWorkOrderID= m_unWorkorderId;
		invoiceData.strTotal = m_strWorkorderTotalPartsPrice;

		CContributionPaymentDialog dlg{ invoiceData, contributionData };
		if ( dlg.DoModal() == IDOK )
		{
			CString strQuery{};
			m_bPinTransaction = contributionData.bPaymentWithPin;
			auto strCurDate{ COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")) }; //time.Format(_T("%m/%d/%Y"));

			auto buildFieldValue = [](CString str) -> CString {
				CString strResult;
				if (str.IsEmpty()) return _T("NULL");
				strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
				return strResult;
			};

			CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

			if (sql.CreateSQLConnection() ) {
				if (contributionData.dContribution > 0.0) {
					strQuery.Format(_T("INSERT INTO [CONTRIBUTION] ([CONTRIBUTION_CUSTOMER_ID], [CONTRIBUITION_WORKORDER_ID], ")
						_T("[CONTRIBUTION_CREATEDATE], [CONTRIBUTION_AMOUNT], [CONTRIBUTION_PIN]) VALUES(%d, %d, %s, %f, %d)"),
						invoiceData.unCustomerID,
						invoiceData.unWorkOrderID,
						static_cast<LPCTSTR>(buildFieldValue(strCurDate)),
						contributionData.dContribution,
						contributionData.bPaymentWithPin ? 1 : 0);
					theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
				
					if (!sql.ExecuteQuery(strQuery.GetBuffer()))
						theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
					else
						theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
					strQuery.ReleaseBuffer();
				}

				auto dTotal = _wtof(invoiceData.strTotal);

				if (dTotal > 0.0) {
					strQuery.Format(_T("INSERT INTO [INVOICE] ([INVOICE_CUSTOMER_ID], [INVOICE_ASSET_ID], [INVOICE_WORKORDER_ID], ")
						_T("[INVOICE_CREATE_DATE], [INVOICE_PAYMENT_PIN], [INVOICE_TOTAL]) VALUES(%d, %d, %d, %s, %d, %f)"),
						invoiceData.unCustomerID,
						invoiceData.unAssetID,
						invoiceData.unWorkOrderID,
						static_cast<LPCTSTR>(buildFieldValue(strCurDate)),
						contributionData.bPaymentWithPin ? 1 : 0,
						dTotal);
					theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

					if (!sql.ExecuteQuery(strQuery.GetBuffer()))
						theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
					else
						theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
					strQuery.ReleaseBuffer();

					auto nID = sql.GetLastAddedID(_T("SELECT IDENT_CURRENT('INVOICE')"));

					if (nID > 0) {
						strQuery.Format(_T("UPDATE WORKORDER SET WORKORDER_INVOICE_ID = %d WHERE WORKORDER_ID = %d"),
							nID, invoiceData.unWorkOrderID);
						theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

						if (!sql.ExecuteQuery(strQuery.GetBuffer()))
							theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
						else
							theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_OK);
						strQuery.ReleaseBuffer();
					}
				}
			}
			sql.CloseConnection();
			PerformWorkorderUpdate();
		}
	}
}

// OnBnClickedWorkorderViewParts is called by the framework when the user clicks on the parts button.
// This method opens the workorder parts dialog and if the user clicks on the OK button, the spare parts list is updated.
void CWorkorderView::OnBnClickedWorkorderViewParts() noexcept
{
	CWorkorderPartsDialog dlg{ m_unWorkorderId };
	if (dlg.DoModal() == IDOK) {
		InitWorkorderSparePartsList();
	}
}

// OnWorkorderExtraCombi is called by the framework when the user clicks on the combi button.
// This method is used to print the combi workorder.
void CWorkorderView::OnWorkorderExtraCombi() noexcept
{
	m_bPrintCombi = true;
	m_ePrinterOrientation = LANDSCAPE;
	this->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

// OnWorkorderExtraInvoice is called by the framework when the user clicks on the invoice button.
// This method is used to print the invoice workorder.
void CWorkorderView::OnWorkorderExtraInvoice() noexcept
{
	m_bPrintInvoice = true;
	m_ePrinterOrientation = PORTRAIT;
	this->SendMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);
}

/* Custom methods */

// SetPrinterOrientation is used to set the printer orientation.
bool CWorkorderView::SetPrinterOrientation(HANDLE h, CDC* dc)
{
	DEVMODE* devMode;
	if (!h) return false;
	devMode = (DEVMODE*)::GlobalLock(h);
	if (!devMode) return false;

	switch (m_ePrinterOrientation) {
	case PORTRAIT:
		devMode->dmOrientation = DMORIENT_PORTRAIT;  // portrait mode
		devMode->dmFields |= DM_ORIENTATION;
		break;

	case LANDSCAPE:
		devMode->dmOrientation = DMORIENT_LANDSCAPE; // landscape mode
		devMode->dmFields |= DM_ORIENTATION;
		break;
	}

	if (dc) dc->ResetDC(devMode);
	::GlobalUnlock(h); 
	return true;
}

// InitWorkorderExistingList is used to initialize the workorder existing list control.
// This method is used to get the workorder information from the database and set the list control.
void CWorkorderView::InitWorkorderExistingList() {
	m_lscWorkorderExisting.DeleteAllItems();

	int nIndex{ 0 };			// Index of the list control item.
	auto row{ 0 };		// Row of the list control item.
	CString strBuildQuery{};
	CString strWorkorderStatus{};

	// Set the workorder status depending on the view type.
	switch ( theApp.GetWorkorderViewType() ) {
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
	theApp.BeginWaitCursor();

	
	strBuildQuery.Format(_T("SELECT WORKORDER.*, ASSET_DESCRIPTION FROM WORKORDER, ASSET WHERE(WORKORDER_STATUS = N\'%s\' AND WORKORDER_ASSET_ID = ASSET_ID)"),
		static_cast<LPCTSTR>(strWorkorderStatus));

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if ( cbName == SQL_NULL_DATA ) {
							return _T("");
						}
						return static_cast<CString>(szName);
					};

					// Get data for columns 1, employee names
					SQLGetData(hstmt, WORKORDER.WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lscWorkorderExisting.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_ASSET_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 1, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CUSTOMER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_INVOICE_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 3, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 4, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CREATE_BY, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 5, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_DESCRIPTION, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 11, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_RESPONSIBLE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 6, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_STATUS, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 7, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_CLOSED_DATE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 8, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDER.WORKORDER_HISTORY, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 9, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, WORKORDER.ASSET_DESCRIPTION, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderExisting.SetItemText(nIndex, 10, CheckForNull(szName, cbName));
				}
				else 
					break;
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) )
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	UpdateData(FALSE);
}

// InitWorkorderEmployeeResponsibleComboBox is used to initialize the employee responsible combo box.
// This method is used to get the employee names from the database and set the combo box.
void CWorkorderView::InitWorkorderEmployeeResponsibleComboBox() {
	m_cbxWorkorderEmployeeResponsible.m_nFlags |= CBS_DROPDOWNLIST;
	m_cbxWorkorderEmployeeResponsible.ResetContent();
	m_cbxWorkorderEmployeeResponsible.AddString(_T(">> Select name <<"));

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery = _T("SELECT EMPLOYEE_NAME FROM EMPLOYEE ORDER BY EMPLOYEE_NAME");

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO )
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if ( cbName == SQL_NULL_DATA ) {
							return _T("");
						}
						return static_cast<CString>(szName);
					};

					// Get data for columns 1, employee names
					SQLGetData(hstmt, 1, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_cbxWorkorderEmployeeResponsible.AddString(CheckForNull(szName, cbName));
				}
				else 
					break;
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) ) 
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	m_cbxWorkorderEmployeeResponsible.SetCurSel(0);
}

// InitWorkorderSparePartsList is used to initialize the workorder spare parts list control.
// This method is used to get the spare parts information from the database and set the list control.
void CWorkorderView::InitWorkorderSparePartsList() {
	m_lscWorkorderSpareParts.DeleteAllItems();

	int nIndex{ 0 };			// Index of the list control item.
	auto row{ 0 };		// Row of the list control item.

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CString strBuildQuery;
	strBuildQuery.Format(_T("SELECT * FROM WORKORDER_PARTS WHERE WORKORDER_PARTS_WORKORDER_ID = %d"), m_unWorkorderId);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			auto dTotalPrice = 0.0;
			while ( TRUE ) {
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					CString strValue = _T("");
					CString strConvert = _T("");
					auto dConvertToMoney = 0.0;

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if ( cbName == SQL_NULL_DATA ) {
							return _T("");
						}
						return static_cast<CString>(szName);
					};

					// Get data for columns 1, employee names
					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_WORKORDER_ID, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					nIndex = m_lscWorkorderSpareParts.InsertItem(row++, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_DESCRIPTION, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_lscWorkorderSpareParts.SetItemText(nIndex, 1, CheckForNull(szNameLong, cbName));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_AMOUNT, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_lscWorkorderSpareParts.SetItemText(nIndex, 2, CheckForNull(szName, cbName));

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_UNIT_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					dConvertToMoney = _ttof(CheckForNull(szName, cbName));
					strConvert.Format(_T("% .2f"), dConvertToMoney);
					m_lscWorkorderSpareParts.SetItemText(nIndex, 3, strConvert);

					SQLGetData(hstmt, WORKORDERPARTS.WORKORDER_PARTS_TOTAL_PRICE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					dTotalPrice += (dConvertToMoney = _ttof(CheckForNull(szName, cbName)));
					strConvert.Format(_T("% .2f"), dConvertToMoney);
					m_lscWorkorderSpareParts.SetItemText(nIndex, 4, strConvert);
				}
				else 
					break;
			}
			m_strWorkorderTotalPartsPrice.Format(_T("% .2f"), dTotalPrice);
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) )
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	theApp.EndWaitCursor();
	UpdateData(FALSE);
}

// SetControlsAfterChangeContactedOrDisposed 
void CWorkorderView::SetControlsAfterChangeContactedOrDisposed() noexcept
{
	UpdateData(TRUE);
	switch ( theApp.GetWorkorderViewType() ) {
		case VIEW_WORKORDER_PROGRESS:
			if  (m_chbWorkorderContactedCustomer.GetCheck() && m_chbWorkorderAssetDisposed.GetCheck()
				&& !m_strWorkorderNewLog.IsEmpty() ) {
				m_btnWorkorderClose.EnableWindow(TRUE);
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::RED, false);
			} else if ( m_chbWorkorderContactedCustomer.GetCheck() && !m_chbWorkorderAssetDisposed.GetCheck()
				&& !m_strWorkorderNewLog.IsEmpty() ) {
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_btnWorkorderFinished.EnableWindow(TRUE);
				m_btnWorkorderClose.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLUE, false);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::RED, false);
			} else if ( !m_chbWorkorderContactedCustomer.GetCheck() && !m_chbWorkorderAssetDisposed.GetCheck()
				&& !m_strWorkorderNewLog.IsEmpty() ) {
				m_btnWorkorderClose.EnableWindow(FALSE);
				m_btnWorkorderUpdate.EnableWindow(TRUE);///////////////////////////////////////////////////////////
				m_btnWorkorderFinished.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::RED, false);
			} else {
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
				m_btnWorkorderClose.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderFinished, ColorButton::BLACK, false);
			}
			break;
		case VIEW_WORKORDER_REPAIRED:
			if ( m_chbWorkorderContactedCustomer.GetCheck() && !m_strWorkorderNewLog.IsEmpty() ) {
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_btnWorkorderClose.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::RED, false);
			} else if ( !m_chbWorkorderContactedCustomer.GetCheck() && !m_strWorkorderNewLog.IsEmpty() ) {
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_btnWorkorderClose.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::RED, false);
			} else if ( m_chbWorkorderContactedCustomer.GetCheck() && m_strWorkorderNewLog.IsEmpty() ) {
				m_btnWorkorderClose.EnableWindow(FALSE);
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
			} else {
				m_btnWorkorderClose.EnableWindow(TRUE);
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				SetCustomFocusButton(&m_btnWorkorderUpdate, ColorButton::BLACK, false);
				SetCustomFocusButton(&m_btnWorkorderClose, ColorButton::RED, false);
				m_edtWorkorderNewLog.EnableWindow(FALSE);
			}
			break;
	}
	UpdateData(FALSE);
}

// GetAssetInfo is used to get the asset information of the selected workorder from the database.
bool CWorkorderView::GetAssetInfo(const unsigned int& nAssetId) {
	CString strBuildQuery{};

	strBuildQuery.Format(_T("SELECT * FROM ASSET WHERE ASSET_ID = %d"), nAssetId);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO )
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if ( cbName == SQL_NULL_DATA ) {
							return _T("");
						}
						return static_cast<CString>(szName);
					};

					// Get data for columns 1, employee names
					SQLGetData(hstmt, ASSET.ASSET_DESCRIPTION, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strAssetDescription = CheckForNull(szNameLong, cbName);

					SQLGetData(hstmt, ASSET.ASSET_MODEL_NUMBER, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetModelNumber = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_BRAND, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strAssetBrand = CheckForNull(szName, cbName);

					SQLGetData(hstmt, ASSET.ASSET_HISTORY_LOG, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strAssetHistoryLog = CheckForNull(szNameLong, cbName);
				}
				else 
					break;
			}
		}
		if ( !sql.CheckReturnCodeForClosing(retcode) ) {
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
			sql.CloseConnection();
			return false;
		}
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	UpdateData(FALSE);
	return true;
}

// GetCustomerInfo is used to get the customer information of the selected workorder from the database.
bool CWorkorderView::GetCustomerInfo(const unsigned int& nCustomerId) {
	CString strBuildQuery;

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	strBuildQuery.Format(_T("SELECT * FROM CUSTOMER WHERE CUSTOMER_ID = %d"), nCustomerId);

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[SQLCHARVSMALL]{};
		SQLCHAR szNameLong[SQLCHARVMAX]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ strBuildQuery.GetBuffer() };
		strBuildQuery.ReleaseBuffer();

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while ( TRUE ) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO )
					AfxMessageBox(_T("Error fetching data from Asset Table!"), MB_ICONEXCLAMATION);
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

					auto CheckForNull = [](SQLCHAR* szName, SQLLEN cbName) -> CString {
						if ( cbName == SQL_NULL_DATA ) {
							return _T("");
						}
						return static_cast<CString>(szName);
					};

					// Get data for columns 1, employee names
					SQLGetData(hstmt, CUSTOMER.CUSTOMER_SURNAME, SQL_C_CHAR, szName, SQLCHARVMAX, &cbName);
					m_strCustomerSurname = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_NAME, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerName = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_CELL_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerCellPhone = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_PHONE, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerPhone = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_EMAIL, SQL_C_CHAR, szName, SQLCHARVSMALL, &cbName);
					m_strCustomerEmail = CheckForNull(szName, cbName);

					SQLGetData(hstmt, CUSTOMER.CUSTOMER_COMMENT, SQL_C_CHAR, szNameLong, SQLCHARVMAX, &cbName);
					m_strCustomerComments = CheckForNull(szNameLong, cbName);
				}
				else
					break;
			}
		}
		if (!sql.CheckReturnCodeForClosing(retcode)) {
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
			sql.CloseConnection();
			return false;
		}
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	sql.CloseConnection();
	UpdateData(FALSE);
	return true;
}

// PerformWorkorderUpdate is used to update the workorder information in the database.
void CWorkorderView::PerformWorkorderUpdate() {
	UpdateData(TRUE);

	auto time = CTime::GetCurrentTime();
	auto strCurDate = COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));
	CString strEmployee;

	auto nIndex = m_cbxWorkorderEmployeeResponsible.GetCurSel();
	if ( nIndex != CB_ERR && nIndex != 0 ) {
		auto nLength = m_cbxWorkorderEmployeeResponsible.GetLBTextLen(nIndex);
		if ( nLength != CB_ERR ) {
			m_cbxWorkorderEmployeeResponsible.GetLBText(nIndex, strEmployee.GetBuffer(nLength));
			strEmployee.ReleaseBuffer();
		}
	}

	// If the employee responsible is changed, add a new log entry to the workorder history log.
	if ( m_bResponsibleChanged ) {
		m_strWorkorderHistoryLog += _T("\r\n[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Assigned to ") + strEmployee;
		m_bResponsibleChanged = false;
	}

	// If the new log is not empty, add a new log entry to the workorder history log.
	if ( !m_strWorkorderNewLog.IsEmpty() ) {
		m_strWorkorderHistoryLog += _T("\r\n[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Comment: ") + m_strWorkorderNewLog;
		m_strWorkorderNewLog.Empty();
	}

	// If the asset is disposed, add a new log entry to the workorder history log.
	if (m_chbWorkorderAssetDisposed.GetCheck())
		m_strWorkorderHistoryLog += _T("\r\n[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Asset disposed.");

	if ( m_strWorkorderStatus == _T("Closed") /* || m_strWorkorderStatus == _T("Repaired")*/)
		m_strWorkorderClosedDate = strCurDate;
	else
		m_strWorkorderClosedDate.Empty();

	CString strQuery;

	auto buildFieldValue = [](CString str) -> CString {
		CString strResult;
		if (str.IsEmpty()) return _T("NULL");
		strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
		return strResult;
	};

	strQuery.Format(_T("UPDATE WORKORDER SET WORKORDER_RESPONSIBLE = %s, WORKORDER_STATUS = %s,")
		_T("WORKORDER_CLOSED_DATE = %s, WORKORDER_HISTORY = %s WHERE WORKORDER_ID = %d"),
		static_cast<LPCTSTR>(buildFieldValue(strEmployee)),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderStatus)),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderClosedDate)),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderHistoryLog)),
		m_unWorkorderId);

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	theApp.BeginWaitCursor();

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		if (!sql.ExecuteQuery(strQuery.GetBuffer()))
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_OK);
		strQuery.ReleaseBuffer();

		if ( m_chbWorkorderAssetDisposed.GetCheck() ) {
			auto nAssetID = _wtoi(m_lscWorkorderExisting.GetItemText(m_lscWorkorderExisting.GetSelectionMark(), 1));
			strQuery.Format(_T("UPDATE ASSET SET ASSET_DISPOSED = %d WHERE ASSET_ID = %d"),
				m_chbWorkorderAssetDisposed.GetCheck() ? 1 : 0,
				nAssetID);

			theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
			if (!sql.ExecuteQuery(strQuery.GetBuffer()))
				theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_FAIL);
			else
				theApp.SetStatusBarText(IDS_STATUSBAR_UPDATE_OK);
			strQuery.ReleaseBuffer();
		}
	}
		
	sql.CloseConnection();
	theApp.EndWaitCursor();

	// Do we want to print invoice when order is set to repaired?
	if ( m_strWorkorderStatus == _T("Closed") ) {
		if (_tstof(m_strWorkorderTotalPartsPrice) != 0.0f)
			OnWorkorderExtraInvoice();
	}
	ResetAllControls();
}

// ResetAllControls is used to reset all controls in the form.
void CWorkorderView::ResetAllControls() noexcept
{ 
	m_unWorkorderId = 0;
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
	m_strWorkorderTotalPartsPrice.Empty();

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
	m_btnWorkorderClose.EnableWindow(FALSE);
	m_btnWorkorderParts.EnableWindow(FALSE);
	m_btnWorkorderAssetUpdate.EnableWindow(FALSE);
	m_chbWorkorderAssetDisposed.EnableWindow(FALSE);
	m_chbWorkorderContactedCustomer.EnableWindow(FALSE);

	m_edcAssetDescription.SetReadOnly(TRUE);
	m_edcAssetModelNumber.SetReadOnly(TRUE);
	m_edcAssetBrand.SetReadOnly(TRUE);

	UpdateData(FALSE);
	InitWorkorderExistingList();
}

// SetCustomFocusButton is used to set the focus on a button and change the color of the text.
void CWorkorderView::SetCustomFocusButton(CMFCButton* pButton, ColorButton Color, bool bFocus) noexcept
{
	auto color = RGB(255, 0, 0);
	switch ( Color ) {
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
	if (bFocus) pButton->SetFocus();
}