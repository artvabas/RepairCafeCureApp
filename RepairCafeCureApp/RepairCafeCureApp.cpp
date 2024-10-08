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
/***************************************************
 At bottom of this file the CAbout class is defined
****************************************************/
/*
* This file is part of RepairCafeCureApp.
* File: RepairCafeCureApp.h, defines class CRepairCafeCureApp
*
* This class is the main application class. It is used to create the database connection and
* to switch between the views.
*
* Target: Windows 10/11 64bit
* Version: 1.0.4.5 (beta)
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 23-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/
#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include <afxpriv.h>

#include "MainFrm.h"
#include "RepairCafeCureApp.h"
#include "RepairCafeCureAppDoc.h"

#include "CAssetView.h"
#include "CCustomerView.h"
#include "CWorkorderView.h"
#include "CReportTaxView.h"
#include "CReportFinanceTotalView.h"
#include "CReportWorkorderClosedView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _DEBUG
#include "CAdminLogin.h"
#include "CAdminEmployeeMaintenance.h"
#include "CAdminSparepartsStockMaintenance.h"
#include "CAdminWorkorderToProgress.h"
#include "CAdminCombiReceipt.h"
using namespace artvabas::rcc::security::admin;
using namespace artvabas::rcc::dialog::admin;
#endif


CRepairCafeCureApp theApp; // The one and only CRepairCafeCureApp object

// TimerCallback is used for deciding if the computer where this application is running on
// is not used for period of time, if so it will lock the application
static void __stdcall TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {

	LASTINPUTINFO lii{ 0 };
	lii.cbSize = sizeof(LASTINPUTINFO);
	GetLastInputInfo(&lii);

	auto dwTimeNow = GetTickCount64();
	auto dwTimeIdle = dwTimeNow - lii.dwTime;

	if (dwTimeIdle > static_cast<unsigned long long>(1000 * 60) * 2 && !theApp.m_bIsIdle) {
		theApp.IsIdle();
		theApp.SetAdmin(false);
		MessageBoxW(theApp.m_pMainWnd->m_hWnd, theApp.ConvertIDToString(IDS_MSGBT_IDLE), theApp.ConvertIDToString(IDS_MSGBC_IDLE), MB_ICONINFORMATION);
	} 
}

CRepairCafeCureApp::CRepairCafeCureApp() noexcept
	: m_pAssetView{ NULL }
	, m_pCustomerView{ NULL }
	, m_pWorkorderView{ NULL }
	, m_pReportTaxView{ NULL }
	, m_pReportFinanceTotalView{ NULL }
	, m_pReportWorkorderClosedView{ NULL }
	, m_dbConnection{ new CDatabaseConnection() }
	, m_enuWorkorderViewType{ VIEW_WORKORDER_OPEN }
	, m_eFinanceTaxViewType{ VIEW_CONTRIBUTON_REPORT }
	, m_bIsIdle{ true }
	, m_bIsPrintPreview{ false }
	, m_bIsAdmin{ false }
{
	SetAppID(_T("RepairCafeCureApp.AppID.1.0.4.5"));
	SetTimer(NULL, 1, (1000 * 60), TimerCallback);
}

CRepairCafeCureApp::~CRepairCafeCureApp()
{
	KillTimer(NULL, 1);
	m_SplashScreen.DestroyWindow();
	if ( NULL != m_dbConnection ) {
		delete m_dbConnection;
		m_dbConnection = NULL;
	}
}

/* Message handles bindings */
BEGIN_MESSAGE_MAP(CRepairCafeCureApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CRepairCafeCureApp::OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CRepairCafeCureApp::OnFilePrintSetup)
	ON_COMMAND(ID_CUSTOMER_VIEW, &CRepairCafeCureApp::OnCustomerView)
	ON_COMMAND(ID_APP_VIEW, &CRepairCafeCureApp::OnAssetView)
	ON_COMMAND(ID_WORKORDER_VIEW_OPEN, &CRepairCafeCureApp::OnWorkorderViewOpen)
	ON_COMMAND(ID_WORKORDER_VIEW_PROGRESS, &CRepairCafeCureApp::OnWorkorderViewProgress)
	ON_COMMAND(ID_WORKORDER_VIEW_REPAIRED, &CRepairCafeCureApp::OnWorkorderViewRepaired)
	ON_COMMAND(ID_REPORT_VIEW_FINANCE_TAX, &CRepairCafeCureApp::OnReportViewFinanceTax)
	ON_COMMAND(ID_REPORT_WORKORDER_CLOSED, &CRepairCafeCureApp::OnReportWorkorderClosed)
	ON_COMMAND(ID_REPORT_WORKORDER_PINTRANSACTION, &CRepairCafeCureApp::OnReportWorkorderPinTransaction)
	ON_COMMAND(ID_REPORT_WORKORDER_CASHTRANSACTION, &CRepairCafeCureApp::OnReportWorkorderCashTransaction)
	ON_COMMAND(ID_REPORT_WORKORDER__TOTALFLOW, &CRepairCafeCureApp::OnReportWorkorderTotalFlow)
	ON_COMMAND(ID_APP_ADMIN, &CRepairCafeCureApp::OnAppAdmin)
	ON_COMMAND(ID_ADMIN_MAINTENANCE_EMPLOYEES, &CRepairCafeCureApp::OnAdminMaintenanceEmployees)
	ON_COMMAND(ID_ADMIN_MAINTENANCE_STOCK, &CRepairCafeCureApp::OnAdminMaintenanceStock)
	ON_COMMAND(ID_ADMIN_MAINTENANCE_WORKORDER_PROGRESS, &CRepairCafeCureApp::OnAdminMaintenanceWorkorderProgress)
	ON_COMMAND(ID_ADMIN_SETTINGS_COMBI, &CRepairCafeCureApp::OnAdminSettingsCombi)
END_MESSAGE_MAP()

/* Overrides  methods */

// InitInstance, create document template, views and show the the application window
BOOL CRepairCafeCureApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(FALSE);

	m_SplashScreen.Create(IDD_SPLASHSCREEN);

	SetRegistryKey(_T("artvabas\\Repair cafe cure app"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRepairCafeCureAppDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCustomerView));
	if (!pDocTemplate) return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Un-regserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo)) return FALSE;

	// Get a pointer to the current view
	CView* pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	m_pCustomerView = pView;

	m_pAssetView = (CView*)new CAssetView;
	if (NULL == m_pAssetView) return FALSE;

	m_pWorkorderView = (CView*)new CWorkorderView;
	if (NULL == m_pWorkorderView) return FALSE;

	m_pReportTaxView = (CView*)new CReportTaxView;
	if (NULL == m_pReportTaxView) return FALSE;

	m_pReportFinanceTotalView = (CView*)new CReportFinanceTotalView;
	if (NULL == m_pReportFinanceTotalView) return FALSE;

	m_pReportWorkorderClosedView = (CView*)new CReportWorkorderClosedView;
	if (NULL == m_pReportWorkorderClosedView) return FALSE;

	// Get the active document
	CDocument* pDoc = pView->GetDocument();

	// Initialize a CCreateContext to point to the active document.
	// With this context, the new view is added to the document
	// when the view is created in CView::OnCreate().
	CCreateContext newContext;
	newContext.m_pNewViewClass = NULL;
	newContext.m_pNewDocTemplate = NULL;
	newContext.m_pLastView = NULL;
	newContext.m_pCurrentFrame = NULL;
	newContext.m_pCurrentDoc = pDoc;

	// The ID of the initial active view is AFX_IDW_PANE_FIRST.
	// Incrementing this value by one for additional views
	UINT viewAssetID = AFX_IDW_PANE_FIRST + 1;
	UINT viewWorkorderID = AFX_IDW_PANE_FIRST + 2;
	UINT viewReportTaxID = AFX_IDW_PANE_FIRST + 3;
	UINT viewReportFinanceTotalID = AFX_IDW_PANE_FIRST + 4;
	UINT viewReportWorkorderClosedID = AFX_IDW_PANE_FIRST + 5;
	CRect rect(0, 0, 0, 0); // Gets resized later.

	// Create the new view. The view persists for
	// the life of the application. The application automatically
	// deletes the view when the application is closed.
	m_pAssetView->Create(NULL, _T("Asset"), WS_CHILD, rect, m_pMainWnd, viewAssetID, &newContext);
	m_pWorkorderView->Create(NULL, _T("Workorder"), WS_CHILD, rect, m_pMainWnd, viewWorkorderID, &newContext);
	m_pReportTaxView->Create(NULL, _T("Report Tax"), WS_CHILD, rect, m_pMainWnd, viewReportTaxID, &newContext);
	m_pReportFinanceTotalView->Create(NULL, _T("Report Finance Total"), WS_CHILD, rect, m_pMainWnd, viewReportFinanceTotalID, &newContext);
	m_pReportWorkorderClosedView->Create(NULL, _T("Report Workorder Closed"), WS_CHILD, rect, m_pMainWnd, viewReportWorkorderClosedID, &newContext);

	// When a document template creates a view, the WM_INITIALUPDATE
	// message is sent automatically. However, this code must
	// explicitly send the message, as follows.
	m_pAssetView->SendMessage(WM_INITIALUPDATE, 0, 0);
	m_pWorkorderView->SendMessage(WM_INITIALUPDATE, 0, 0);
	m_pReportTaxView->SendMessage(WM_INITIALUPDATE, 0, 0);
	m_pReportFinanceTotalView->SendMessage(WM_INITIALUPDATE, 0, 0);
	m_pReportWorkorderClosedView->SendMessage(WM_INITIALUPDATE, 0, 0);

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->SetWindowTextW(ConvertIDToString(IDS_WT_CUSTOMER));
	m_pMainWnd->ShowWindow(SW_SHOW);
	
	return TRUE;
}

// CRepairCafeCureApp exit instance 
int CRepairCafeCureApp::ExitInstance()
{
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

/*Messages handlers methods*/

// OnCustomerView is called when user select the customer button on the ribbon
// Change the view
void CRepairCafeCureApp::OnCustomerView() noexcept
{
	SwitchView(VIEW_CUSTOMER);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_CUSTOMER));

	// Get a pointer to the main frame window.
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	// Set controls state of view, depending in selected employee
	if ( pMainFrm != NULL ) pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
}

// OnAssetView is called when user select the Asset button on the ribbon
// Change the view
void CRepairCafeCureApp::OnAssetView() noexcept
{
	SwitchView(VIEW_ASSET);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_HISTORY));
}

// OnWorkorderViewOpen is called when user select the workorder-open button on the ribbon
// set type of view and change the view 
void CRepairCafeCureApp::OnWorkorderViewOpen() noexcept
{
	m_enuWorkorderViewType = VIEW_WORKORDER_OPEN;
	SwitchView(VIEW_WORKORDER);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_WORKORDER_OPEN));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if ( pMainFrm != NULL ) pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
}

// OnWorkorderViewProgress is called when user select the workorder-progress button on the ribbon
// set type of view and change the view 
void CRepairCafeCureApp::OnWorkorderViewProgress() noexcept
{
	m_enuWorkorderViewType = VIEW_WORKORDER_PROGRESS;
	SwitchView(VIEW_WORKORDER);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_WORKORDER_PROGRESS));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if ( pMainFrm != NULL ) pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
}

// OnWorkorderViewProgress is called when user select the workorder-repaired button on the ribbon
// set type of view and change the view 
void CRepairCafeCureApp::OnWorkorderViewRepaired() noexcept
{
	m_enuWorkorderViewType = VIEW_WORKORDER_REPAIRED;
	SwitchView(VIEW_WORKORDER);
	theApp.m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_WORKORDER_REPAIRED));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if ( pMainFrm != NULL ) pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
}

// OnReportViewFinanceTax is called when user select the report-finance-tax button on the ribbon
// set type of view and change the view
void CRepairCafeCureApp::OnReportViewFinanceTax() noexcept
{
	m_bIsPrintPreview = false;
	m_eFinanceTaxViewType = VIEW_CONTRIBUTON_REPORT;
	SwitchView(VIEW_REPORT_FINANCE_TAX);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_REPORT_CONTRIBUTION));
}

// OnReportWorkorderPinTransaction is called when user select the report-workorder-pintransaction button on the ribbon
// set type of view and change the view
void CRepairCafeCureApp::OnReportWorkorderPinTransaction() noexcept
{
	m_bIsPrintPreview = false;
	m_eFinanceTaxViewType = VIEW_PIN_TRANSACTION_REPORT;
	SwitchView(VIEW_REPORT_FINANCE_TAX);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_REPORT_PIN));
}

void CRepairCafeCureApp::OnReportWorkorderCashTransaction() noexcept
{
	m_bIsPrintPreview = false;
	m_eFinanceTaxViewType = VIEW_CASH_TRANSACTION_REPORT;
	SwitchView(VIEW_REPORT_FINANCE_TAX);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_REPORT_CASH));
}

// OnReportWorkorderTotalFlow is called when user select the report-workorder-total-flow button on the ribbon
// set type of view and change the view
void CRepairCafeCureApp::OnReportWorkorderTotalFlow() noexcept
{
	SwitchView(VIEW_REPORT_FINANCE_TOTAL);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_REPORT_TOTAL));
}


// OnReportWorkorderClosed is called when user select the report-workorder-closed button on the ribbon
// set type of view and change the view
void CRepairCafeCureApp::OnReportWorkorderClosed() noexcept
{
	SwitchView(VIEW_REPORT_WORKORDER_CLOSED);
	m_pMainWnd->SetWindowText(ConvertIDToString(IDS_WT_WORKORDER_CLOSED));
}

// OnAppAdmin is called when user select the admin button on the ribbon
// Show the admin login dialog, if the user is admin, show the admin context category
void CRepairCafeCureApp::OnAppAdmin() noexcept
{
#ifdef _DEBUG
	AfxMessageBox(_T("App is in debug mode, no Admin!"));
#else
	CAdminLogin dlg;
	if (dlg.DoModal() == IDOK) {
		SetAdmin(true);
		ShowContextCategory(ID_CONTEXT_ADMIN);
	}
#endif
}

// OnAdminMaintenanceEmployees is called when user select the maintenance-employees button on the ribbon
// Show the admin employee maintenance dialog
// Update the employee list
void CRepairCafeCureApp::OnAdminMaintenanceEmployees() noexcept
{
#ifndef _DEBUG
	CAdminEmployeeMaintenance dlg;
	if (dlg.DoModal() == IDOK) {
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (pMainFrm != NULL) {
			pMainFrm->GetEmployeeList();
			pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
		}
	}
#endif
}

// OnAdminMaintenanceStock is called when user select the maintenance-stock button on the ribbon
// Show the admin spare parts stock maintenance dialog
void CRepairCafeCureApp::OnAdminMaintenanceStock() noexcept
{
#ifndef _DEBUG
	CAdminSparePartsStockMaintenance dlg;
	dlg.DoModal();
#endif
}

// OnAdminMaintenanceWorkorderProgress is called when user select the maintenance-workorder-progress button on the ribbon
// Show the admin workorder to progress dialog
void CRepairCafeCureApp::OnAdminMaintenanceWorkorderProgress() noexcept
{
#ifndef _DEBUG
	CAdminWorkorderToProgress dlg;
	dlg.DoModal();
#endif // !_DEBUG
}

// OnAdminSettingsCombi is called when user select the settings-combi button on the ribbon
// Show the admin combi receipt dialog
void CRepairCafeCureApp::OnAdminSettingsCombi() noexcept
{
#ifndef _DEBUG
	CAdminCombiReceipt dlg;
	dlg.DoModal();
#endif // !_DEBUG
}

// OnFilePrintSetup is called when user select the print setup button on the ribbon
// Set the printer orientation for the selected view
void CRepairCafeCureApp::OnFilePrintSetup() noexcept
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame) {
		switch (GetActiveViewType()) {
			case VIEW_WORKORDER: {
				CWorkorderView* pView = (CWorkorderView*)pMainFrame->GetActiveView();
				pView->SetPrinterOrientation(GetDeviceMode());
			}
			break;
			case VIEW_REPORT_FINANCE_TAX: {
				CReportTaxView* pView = (CReportTaxView*)pMainFrame->GetActiveView();
				pView->SetPrinterOrientation(GetDeviceMode());
			}
			break;
			case VIEW_REPORT_FINANCE_TOTAL: {
				CReportFinanceTotalView* pView = (CReportFinanceTotalView*)pMainFrame->GetActiveView();
				pView->SetPrinterOrientation(GetDeviceMode());
			}
			break;
		}
	}
	CWinAppEx::OnFilePrintSetup();
}

/* Member methods */

// SwitchView is used for switching the views of this application
// vtView - enum with the view type yo switch to
CView* CRepairCafeCureApp::SwitchView(ViewType vtView) const noexcept
{
	CView* pActiveView = ((CFrameWndEx*)m_pMainWnd)->GetActiveView();
	CView* pNewView = pActiveView;// NULL;

	ShowContextCategory(ID_CONTEXT_WORKORDER, FALSE);

	switch ( vtView ) {
		case VIEW_CUSTOMER:
			pNewView = m_pCustomerView;
			break;
		case VIEW_WORKORDER:
			pNewView = m_pWorkorderView;
			break;
		case VIEW_ASSET:
			pNewView = m_pAssetView;
			break;
		case VIEW_REPORT_FINANCE_TAX:
			pNewView = m_pReportTaxView;
			break;
		case VIEW_REPORT_WORKORDER_CLOSED:
			pNewView = m_pReportWorkorderClosedView;
			break;
		case VIEW_REPORT_FINANCE_TOTAL:
			pNewView = m_pReportFinanceTotalView;
			break;
	}

#ifndef _WIN32
	if (pNewView != NULL) {
		((CFrameWndEx*)m_pMainWnd)->SetActiveView(pNewView);
		((CFrameWndEx*)m_pMainWnd)->RecalcLayout();
		pNewView->Invalidate();
	}
	UINT temp = ::GetWindowWord(pActiveView->m_hWnd, GWW_ID);
	::SetWindowWord(pActiveView->m_hWnd, GWW_ID, ::GetWindowWord(pNewView->m_hWnd, GWW_ID));
	::SetWindowWord(pNewView->m_hWnd, GWW_ID, temp);

#else
	if (pNewView != NULL) {
		UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
		::SetWindowLong(pActiveView->m_hWnd, GWL_ID, ::GetWindowLong(pNewView->m_hWnd, GWL_ID));
		::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
	}
#endif
	// Swap the views and attach the view
	pActiveView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_SHOW);
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->SetActiveView(pNewView);
	pMainFrm->RecalcLayout();
	return pNewView;// pActiveView;
}

CString CRepairCafeCureApp::ConvertIDToString(const UINT& nID) const noexcept
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(nID);
	ASSERT(bNameValid);
	return strName;
}

// SetStatusBarText is used to set the status bar text
// - nStrID, the ID from string table
void CRepairCafeCureApp::SetStatusBarText(const UINT& nStrID) const noexcept
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if ( pMainFrame != NULL ) pMainFrame->m_wndStatusBar.SetInformation(ConvertIDToString(nStrID));
}

// GetSelectedEmployeeName is used to get teh selected employee,
// who unlock this application.
CString CRepairCafeCureApp::GetSelectedEmployeeName() const noexcept
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if ( pMainFrame != NULL ) return pMainFrame->GetSelectedEmployee();
	else return CString("");
}

// IsIdle is called by the timer trigger,
// Unselect employee and update current view controls
void CRepairCafeCureApp::IsIdle() const noexcept
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if ( pMainFrm != NULL ) {
		ShowContextCategory(ALL_CONTEXT_CATEGORY, FALSE);
		pMainFrm->EmployeeIdle();
		pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
	}
}

void CRepairCafeCureApp::ShowContextCategory(const UINT nID, const BOOL bShow) const noexcept
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrm != NULL) {
		CMFCRibbonBar* ribbonBar = pMainFrm->GetRibbonBar();
		
		switch (nID) {
		case ALL_CONTEXT_CATEGORY:
			if (bShow) {
				ribbonBar->ShowContextCategories(ID_CONTEXT_WORKORDER, bShow);
				ribbonBar->ShowContextCategories(ID_CONTEXT_ADMIN, bShow);
			} else {
				ribbonBar->HideAllContextCategories();
			}
			break;
		case ID_CONTEXT_WORKORDER:
			ribbonBar->ShowContextCategories(nID, bShow);
			break;
		case ID_CONTEXT_ADMIN:
			ribbonBar->ShowContextCategories(nID, bShow);
			break;
		}
		ribbonBar->ForceRecalcLayout();
	}
	//ribbonBar->ActivateContextCategory(ID_CONTEXT_ADMIN);
}

// GetDeviceMode is used to get the device mode for printing
// for the selected view.
HANDLE CRepairCafeCureApp::DefineDeviceMode() noexcept
{
	PRINTDLG pd{};
	pd.lStructSize = (DWORD)sizeof(PRINTDLG);
	return GetPrinterDeviceDefaults(&pd) ? pd.hDevMode : 0;
}

// GetActiveViewType is used to get the selected workorder view type
ViewType CRepairCafeCureApp::GetActiveViewType() const noexcept
{
	ViewType vtView{};
	CView* pActiveView = ((CFrameWnd*)m_pMainWnd)->GetActiveView();

	if (pActiveView != NULL) {
		if (pActiveView == m_pAssetView) vtView = VIEW_ASSET;
		else if (pActiveView == m_pCustomerView) vtView = VIEW_CUSTOMER;
		else if (pActiveView == m_pWorkorderView) vtView = VIEW_WORKORDER;
		else if (pActiveView == m_pReportTaxView) vtView = VIEW_REPORT_FINANCE_TAX;
		else if (pActiveView == m_pReportFinanceTotalView) vtView = VIEW_REPORT_FINANCE_TOTAL;
		else if (pActiveView == m_pReportWorkorderClosedView) vtView = VIEW_REPORT_WORKORDER_CLOSED;

		return vtView;
	}
	
	return ViewType::VIEW_CUSTOMER;
}

/*
* This class is part of RepairCafeCureApp.
* class: CAboutDlg, defines class CAboutDlg
*
* This class is used to show the about dialog.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.1 (alpha)
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 28-05-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/

class CAboutDlg : public CDialogEx
{
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
public:
	CAboutDlg() noexcept;

private:
	void DoDataExchange(CDataExchange* pDX) override;

private:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept 
	: CDialogEx(IDD_ABOUTBOX)
{}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// OnAppAbout is called when user click on the info button on the ribbon
void CRepairCafeCureApp::OnAppAbout() noexcept
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}