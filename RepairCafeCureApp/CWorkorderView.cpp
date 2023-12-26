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
* File: CWorkorderView.cpp, implementation of class CWorkorderView
*
* This class is the view of the workorder form.
* With this form the user can search for existing workorders and update existing workorders.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 02-12-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/

/****************************************************************************************
* REMARKS:
* 
* The controls in the view are enabled and disabled depending on the state of the form.
* 
* The states are:
*	VIEW_WORKORDER_OPEN
*		only Update button is enabled, depends on selected employee responsible.
*
* 	VIEW_WORKORDER_PROGRESS
*		Update and Finished buttons are enabled or disabled, depends on selected employee responsible, new log edit control, 
*		contacted customer check box and asset disposed check box.
*
*		Update is active when the contacted customer check box is unchecked and the asset disposed check box is unchecked,
* 		and the new log edit control is not empty or the employee responsible combo box is changed.
* 
*		Finished is active when the contacted customer check box is checked.
* 
******************************************************************************************/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderView.h"
#include "CWorkorderPartsDialog.h"
#include "MainFrm.h"

//using namespace artvabas::rcc::ui;
using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;

IMPLEMENT_DYNCREATE(CWorkorderView, CFormView)

CWorkorderView::CWorkorderView() : CFormView(IDD_WORKORDER_FORM)
	, m_unWorkorderId(0)
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
	, m_strWorkorderTotalPartsPrice(_T(""))
	, m_pDC(NULL)
{
}

CWorkorderView::~CWorkorderView()
{
	//delete lpDevMode;
	if (m_pDC)
	{
		m_pDC->Detach();
		m_pDC->DeleteDC();
		delete m_pDC;
	}
}

/* Overrides methods */

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

/// <summary>
/// DoDataExchange is called by the framework to exchange and validate dialog data.
/// </summary>
/// <param name="pDX"></param>
/// <returns></returns>
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
}

/// <summary>
/// OnInitialUpdate is called by the framework after the view is created.
/// Initialize the list controls and the employee responsible combo box.
/// </summary>
/// <returns></returns>
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

BOOL CWorkorderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standaard voorbereiding
	BOOL bRet = DoPreparePrinting(pInfo);

	// Controleer of de voorbereiding succesvol was
	if (bRet)
	{
		
		if (m_pDC == NULL)
		{
			// Verkrijg de huidige printerinstellingen
			HDC hDC = pInfo->m_pPD->GetPrinterDC();
			m_pDC = new CDC;
			m_pDC->Attach(hDC);
		}

		// Verkrijg de DEVMODE structuur via de CPrintInfo structuur
		DEVMODE* pDevMode = pInfo->m_pPD->GetDevMode();

		// Controleer of de DEVMODE pointer geldig is
		if (pDevMode != NULL)
		{
			// Zet de printer in landscape-modus
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;

			// Pas de instellingen toe
			::ResetDC(m_pDC->GetSafeHdc(), pDevMode);
		}
	}

	return bRet;
}

void CWorkorderView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//HDC hDC = pDC->GetSafeHdc();
	// Get the default DevMode for the current default printer
}

void CWorkorderView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Free the memory allocated for the DEVMODE structure
	//delete lpDevMode;
	//GlobalUnlock(lpDevMode);
	//GlobalFree(pInfo->m_pPD->m_pd.hDevMode);
	//lpDevMode = NULL
	
}

void CWorkorderView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	pDC->TextOut(100, 100, _T("Hello World!"));
}

/* Message handles */

BEGIN_MESSAGE_MAP(CWorkorderView, CFormView)
	ON_WM_UPDATEUISTATE()
	ON_NOTIFY(NM_DBLCLK, IDC_WORKORDERVIEW_EXISTING, &CWorkorderView::OnNMDoubleClickWorkorderViewExisting)
	ON_CBN_SELCHANGE(IDC_WORKORDERVIEW_RESPONSIBLE, &CWorkorderView::OnCbnSelectChangeWorkorderViewResponsible)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_UPDATE, &CWorkorderView::OnBnClickedWorkorderViewUpdate)
	ON_EN_CHANGE(IDC_WORKORDERVIEW_LOG, &CWorkorderView::OnEnChangeWorkorderViewLog)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_CUSTOMER_CONTACTED_CUSTOMER, &CWorkorderView::OnBnClickedWorkorderVewCustomerContactedCustomer)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_FINISHED, &CWorkorderView::OnBnClickedWorkorderViewFinished)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_ASSET_DISPOSED, &CWorkorderView::OnBnClickedWorkorderViewAssetDisposed)
	ON_BN_CLICKED(IDC_WORKORDERVIEW_PARTS, &CWorkorderView::OnBnClickedWorkorderViewParts)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWorkorderView::OnFilePrintPreview)
END_MESSAGE_MAP()

/* Message methods */

/// <summary>
/// OnUpdateUIState is called by the framework when the view is activated.
/// This method is used to enable and disable controls depending on the state of the view.
/// </summary>
/// <param name="nAction"></param>
/// <param name="nUIElement"></param>
/// <returns></returns>
void CWorkorderView::OnUpdateUIState(UINT nAction, UINT nUIElement)
{
	CWnd* pChild = GetWindow(GW_CHILD);

	switch (nAction)
	{
	case 1:	// UIS_SET. 1 means - Employee name is selected in the caption bar.
		// nUIElement = 0 means this method is called by the framework when the view is activated, controls are accessible.	
		if (0 == nUIElement)
		{
			while (pChild)	// Go through all child controls of the view and acitvate all.
			{
				pChild->EnableWindow(TRUE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}

			ResetAllControls();	// Reset all controls to their default state.
		}
		break;
		default:
			while (pChild)	// This view is activated, but employee name is not selected in the caption bar.
			{
				pChild->EnableWindow(FALSE);
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			break;
	}
}

/// <summary>
/// OnNMDoubleClickWorkorderViewExisting is called by the framework when the user double clicks on an item in the existing workorders list control.
/// This method is used to get the selected workorder's information from the database and display it in the view.
/// It also call the GetAssetInfo and GetCustomerInfo methods to get the asset and customer information.
/// And set the controls depending on the status of the workorder.
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
/// <returns></returns>
void CWorkorderView::OnNMDoubleClickWorkorderViewExisting(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1)
	{
		// Get the selected item's text.
		m_unWorkorderId = _ttoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 0));

		const unsigned int unAssetID = _ttoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 1));
		const unsigned int unCustomerID = _ttoi(m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 2));
	
		m_strWorkorderCreatedDate = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 4);
		m_strWorkorderCreatedBy = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 5);
		m_strWorkorderStatus = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 8);
		m_strWorkorderNewLog = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 9);
		m_strWorkorderHistoryLog = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 10);

		theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

		// Get the asset and customer information.
		if(GetAssetInfo(unAssetID) && GetCustomerInfo(unCustomerID))
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
		else
			theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);

		InitWorkorderSparePartsList();

		// Set the employee responsible combo box on this form.
		CString strValue = m_lscWorkorderExisting.GetItemText(pNMItemActivate->iItem, 7);

		if(!strValue.IsEmpty())
			m_cbxWorkorderEmployeeResponsible.SelectString(0, strValue.Trim());
		else
			m_cbxWorkorderEmployeeResponsible.SetCurSel(0);

		m_cbxWorkorderEmployeeResponsible.EnableWindow(TRUE);

		CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
		CMFCRibbonBar* ribbonBar = pMainFrame->GetRibbonBar();

		// Set the controls depending on the status of the workorder.
		if (theApp.GetWorkorderViewType() != VIEW_WORKORDER_OPEN)
		{
			m_edtWorkorderNewLog.EnableWindow(TRUE);
			m_btnWorkorderParts.EnableWindow(TRUE);
			m_chbWorkorderAssetDisposed.EnableWindow(TRUE);
			m_chbWorkorderContactedCustomer.EnableWindow(TRUE);
			m_btnWorkorderUpdate.EnableWindow(FALSE);
			m_bResponsibleChanged = false;
			ribbonBar->ShowContextCategories(ID_CONTEXT_WORKORDER);
			ribbonBar->ActivateContextCategory(ID_CONTEXT_WORKORDER);
		}
		else
		{
			ribbonBar->HideAllContextCategories();
			m_edtWorkorderNewLog.EnableWindow(FALSE);
		}

		m_bWorkorderSelected = true;
	}

	*pResult = 0;
}

/// <summary>
/// OnCbnSelectChangeWorkorderViewResponsible is called by the framework when the user selects or change an employee responsible from the combo box.
/// This method is used to enable and disable controls depending on the state of the form.
/// </summary>
/// <returns></returns>
void CWorkorderView::OnCbnSelectChangeWorkorderViewResponsible()
{
	switch (theApp.GetWorkorderViewType())
	{
		case VIEW_WORKORDER_OPEN:
			// Is a employee responsible selected and is a workorder selected?
			if (m_cbxWorkorderEmployeeResponsible.GetCurSel() && m_bWorkorderSelected)
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
			}
			else // No employee responsible selected or no workorder selected.
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_edtWorkorderNewLog.EnableWindow(FALSE);
			}
		break;
		case VIEW_WORKORDER_PROGRESS:
			// Is a employee responsible selected and is a workorder selected and is the 
			// contacted customer check box not checked and is the asset disposed check box not checked?
			if (m_cbxWorkorderEmployeeResponsible.GetCurSel() && m_bWorkorderSelected && 
				(!m_chbWorkorderContactedCustomer.GetCheck()) && (!m_chbWorkorderAssetDisposed.GetCheck()))
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
			}
			// But if the Workorder Contact Customer Checkbox is selected
			else if (m_chbWorkorderContactedCustomer.GetCheck())
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(TRUE);
				m_edtWorkorderNewLog.EnableWindow(TRUE);
			}
			else // No employee responsible selected.
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

/// <summary>
/// OnEnChangeWorkorderViewLog is called by the framework when the user changes the log edit control.
/// This method is used to enable and disable controls depending on the state of the form.
/// </summary>
/// <returns></returns>
void CWorkorderView::OnEnChangeWorkorderViewLog()
{
	UpdateData(TRUE);
	switch (theApp.GetWorkorderViewType())
	{
		case VIEW_WORKORDER_OPEN:
			// Is the new log not empty and is a workorder selected?
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
			// Is the new log not empty and is a workorder selected and is the
			// contacted customer check box not checked and is the asset disposed check box not checked?
			if (!m_strWorkorderNewLog.IsEmpty() && m_bWorkorderSelected &&
				(!m_chbWorkorderContactedCustomer.GetCheck()) && (!m_chbWorkorderAssetDisposed.GetCheck()))
			{
				m_btnWorkorderUpdate.EnableWindow(TRUE);
				m_btnWorkorderFinished.EnableWindow(FALSE);
			}
			else if (m_chbWorkorderContactedCustomer.GetCheck())	// But if the Workorder Contact Customer Checkbox is selected
			{
				m_btnWorkorderUpdate.EnableWindow(FALSE);
				m_btnWorkorderFinished.EnableWindow(TRUE);
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

/// <summary>
/// OnBnClickedWorkorderViewAssetDisposed is called by the framework when the user clicks on the asset disposed check box.
/// This method is used to enable and disable controls depending on the state of the form.
/// </summary>
/// <returns></returns>
void CWorkorderView::OnBnClickedWorkorderViewAssetDisposed()
{
	// Is the asset disposed check box checked and is the contacted customer check box checked?
	if (m_chbWorkorderAssetDisposed.GetCheck() && m_chbWorkorderContactedCustomer.GetCheck())
	{
		m_btnWorkorderFinished.EnableWindow(TRUE);
		m_btnWorkorderUpdate.EnableWindow(FALSE);
	}
	// Is the asset disposed check box checked	and the contacted customer check box uncheked?
	else if (m_chbWorkorderAssetDisposed.GetCheck() && !m_chbWorkorderContactedCustomer.GetCheck())
	{
		m_btnWorkorderFinished.EnableWindow(FALSE);
		m_btnWorkorderUpdate.EnableWindow(FALSE);
	}
	// Is the asset disposed check box unchecked	and the contacted customer check box cheked?
	else if (!m_chbWorkorderAssetDisposed.GetCheck() && m_chbWorkorderContactedCustomer.GetCheck())
	{
		m_btnWorkorderFinished.EnableWindow(TRUE);
		m_btnWorkorderUpdate.EnableWindow(FALSE);
	}
	else
	{
		m_btnWorkorderFinished.EnableWindow(FALSE);

		if(m_bResponsibleChanged || !m_strWorkorderNewLog.IsEmpty())
			m_btnWorkorderUpdate.EnableWindow(TRUE);
	}
}

/// <summary>
/// OnBnClickedWorkorderViewParts is called by the framework when the user clicks on the parts button.
/// This method is used to enable and disable controls depending on the state of the form.
/// </summary>
/// <returns></returns>
void CWorkorderView::OnBnClickedWorkorderVewCustomerContactedCustomer()
{
	if (m_chbWorkorderContactedCustomer.GetCheck() == BST_CHECKED)
	{
		m_btnWorkorderFinished.EnableWindow(TRUE);
		m_btnWorkorderUpdate.EnableWindow(FALSE);
	}
	else
	{
		m_btnWorkorderFinished.EnableWindow(FALSE);

		if((m_bResponsibleChanged || !m_strWorkorderNewLog.IsEmpty()) && !m_chbWorkorderAssetDisposed.GetCheck())
			m_btnWorkorderUpdate.EnableWindow(TRUE);
	}
}

/// <summary>
/// OnBnClickedWorkorderViewParts is called by the framework when the user clicks on the update button.
/// This method set the status of the workorder depend on the state of the form.
/// And call the PerformWorkorderUpdate method, for processing the update.
/// </summary>
/// <returns></returns>
void CWorkorderView::OnBnClickedWorkorderViewUpdate()
{
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
	UpdateData(FALSE);
	PerformWorkorderUpdate();
}

/// <summary>
/// OnBnClickedWorkorderViewFinished is called by the framework when the user clicks on the finished button.
/// This method set the status of the workorder depend on the state of the form.
/// And call the PerformWorkorderUpdate method, for processing the update.
/// </summary>
/// <returns></returns>
void CWorkorderView::OnBnClickedWorkorderViewFinished()
{
	m_chbWorkorderAssetDisposed.GetCheck() == BST_CHECKED ?
		m_strWorkorderStatus = _T("Closed") : m_strWorkorderStatus = _T("Repaired");

	UpdateData(FALSE);
	PerformWorkorderUpdate();
}

/// <summary>
/// OnBnClickedWorkorderViewParts is called by the framework when the user clicks on the parts button.
/// This method opens the workorder parts dialog.	
/// </summary>
/// <returns></returns>
void CWorkorderView::OnBnClickedWorkorderViewParts()
{
	CWorkorderPartsDialog dlg(m_unWorkorderId);
	if (dlg.DoModal() == IDOK)
	{
		InitWorkorderSparePartsList();
	}
}

void CWorkorderView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

/* Custom methods */

/// <summary>
/// InitWorkorderExistingList is used to fill the existing workorders list control with the found workorders from the database.
/// </summary>
/// <returns></returns>
void CWorkorderView::InitWorkorderExistingList()
{
	m_lscWorkorderExisting.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strQuery;
	CString strWorkorderStatus;

	// Set the workorder status depending on the view type.
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
		static_cast<LPCTSTR>(strWorkorderStatus));

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

/// <summary>
/// InitWorkorderEmployeeResponsibleComboBox is used to fill the employee responsible combo box with the found employees from the database.
/// </summary>
/// <returns></returns>
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

void CWorkorderView::InitWorkorderSparePartsList()
{
	m_lscWorkorderSpareParts.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	CRecordset* rs = new CRecordset();
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM WORKORDER_PARTS WHERE WORKORDER_PARTS_WORKORDER_ID = %d"), m_unWorkorderId);

	if (theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		double dTotalPrice = 0.0;
		// Fill the existing customers list control with the found customers from the database.
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			CString strConvert = _T("");
			double dConvertToMoney = 0.0;
			rs->GetFieldValue(_T("WORKORDER_PARTS_WORKORDER_ID"), strValue);
			nIndex = m_lscWorkorderSpareParts.InsertItem(row++, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_DESCRIPTION"), strValue);
			m_lscWorkorderSpareParts.SetItemText(nIndex, 1, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_AMOUNT"), strValue);
			m_lscWorkorderSpareParts.SetItemText(nIndex, 2, strValue);

			rs->GetFieldValue(_T("WORKORDER_PARTS_UNIT_PRICE"), strValue);
			dConvertToMoney = _ttof(strValue);
			strConvert.Format(_T("% .2f"), dConvertToMoney);
			m_lscWorkorderSpareParts.SetItemText(nIndex, 3, strConvert);

			rs->GetFieldValue(_T("WORKORDER_PARTS_TOTAL_PRICE"), strValue);
			dTotalPrice += (dConvertToMoney = _ttof(strValue));
			
			strConvert.Format(_T("% .2f"), dConvertToMoney);
			m_lscWorkorderSpareParts.SetItemText(nIndex, 4, strConvert);

			rs->MoveNext();
		}
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);

		m_strWorkorderTotalPartsPrice.Format(_T("% .2f"), dTotalPrice);
	}
	else
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;

	UpdateData(FALSE);
}

/// <summary>
/// GetAssetInfo is used to get the asset information of the selected workorder from the database.
/// </summary>
/// <param name="nAssetId"> ID asset index number</param>
/// <returns></returns>
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

/// <summary>
/// GetCustomerInfo is used to get the customer information of the selected workorder from the database.
/// </summary>
/// <param name="nCustomerId"> ID customer index number</param>
/// <returns></returns>
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

/// <summary>
/// PerformWorkorderUpdate is used to update the selected workorder in the database.
/// </summary>
/// <returns></returns>
void CWorkorderView::PerformWorkorderUpdate()
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

	// If the employee responsible is changed, add a new log entry to the workorder history log.
	if (m_bResponsibleChanged)
	{
		m_strWorkorderHistoryLog += _T("\r\n[") + strCurDate + _T(", ") + theApp.GetSelectedEmployeeName() + _T("] Assigned to ") + strEmployee;
		m_bResponsibleChanged = false;
	}

	// If the new log is not empty, add a new log entry to the workorder history log.
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
		m_unWorkorderId);

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

	ResetAllControls();
}

/// <summary>
/// ResetAllControls is used to reset all controls to their default state.
/// </summary>
/// <returns></returns>
void CWorkorderView::ResetAllControls()
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
	m_btnWorkorderParts.EnableWindow(FALSE);
	m_chbWorkorderAssetDisposed.EnableWindow(FALSE);
	m_chbWorkorderContactedCustomer.EnableWindow(FALSE);

	InitWorkorderExistingList();

	UpdateData(FALSE);
}
