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
* File: MainFrm.cpp is the implementation file of the main window of the application.
*
* This class is responsible for the main window of the application.
* It also contains the main menu, the ribbon bar and the status bar.
* The main menu is created in the resource file.
* The ribbon bar is created in the OnCreate method.
* The status bar is created in the OnCreate method.
* The caption bar is created in the OnCreate method.
*
* Target: Windows 10/11 64bit
* Version: 1.0.3.5 (beta)
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 16-09-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/

#include "pch.h"
#include "framework.h"
#include "RepairCafeCureApp.h"
#include "MainFrm.h"
#include "DatabaseTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace artvabas::sql;
using namespace artvabas::database::tables::employee;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

CMainFrame::CMainFrame() noexcept
	: m_bHighlightCategories(FALSE)
	, m_bHighlightAll(FALSE)
{
	m_pCmbCaptionBarEmployeeName = new CComboBox();
}

CMainFrame::~CMainFrame()
{
	//m_wndRibbonBar.HideAllContextCategories();
	if ( m_pCmbCaptionBarEmployeeName != nullptr ) {
		delete m_pCmbCaptionBarEmployeeName;
		m_pCmbCaptionBarEmployeeName = nullptr;
	}
	m_vecEmployeeList.clear();
}

/* Message handles bindings */
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CAPTION_BAR,
		&CMainFrame::OnViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS,
		&CMainFrame::OnOptions)
	ON_COMMAND(ID_FILE_PRINT,
		&CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,
		&CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,
		&CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW,
		&CMainFrame::OnUpdateIsPrintable)
	ON_CBN_SELCHANGE(IDC_CAPTION_COMBOBOX_EMPLOYEE_NAME,
		&CMainFrame::OnCaptionBarComboBoxEmployeeNameChange)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMER_VIEW,
		&CMainFrame::OnUpdateCustomerView)
	ON_UPDATE_COMMAND_UI(ID_WORKORDER_VIEW_OPEN, 
		&CMainFrame::OnUpdateWorkorderViewOpen)
	ON_UPDATE_COMMAND_UI(ID_WORKORDER_VIEW_REPAIRED, 
		&CMainFrame::OnUpdateWorkorderViewRepaired)
	ON_UPDATE_COMMAND_UI(ID_WORKORDER_VIEW_PROGRESS, 
		&CMainFrame::OnUpdateWorkorderViewProgress)
	ON_UPDATE_COMMAND_UI(ID_REPORT_VIEW_FINANCE_TAX, 
		&CMainFrame::OnUpdateReportViewFinanceTax)
	ON_UPDATE_COMMAND_UI(ID_REPORT_WORKORDER_PINTRANSACTION, 
		&CMainFrame::OnUpdateReportWorkorderPinTransaction)
	ON_UPDATE_COMMAND_UI(ID_REPORT_WORKORDER_CASHTRANSACTION,
		&CMainFrame::OnUpdateReportWorkorderCashTransaction)
	ON_UPDATE_COMMAND_UI(ID_REPORT_WORKORDER__TOTALFLOW,
		&CMainFrame::OnUpdateReportWorkorderTotalFlow)
	ON_UPDATE_COMMAND_UI(ID_REPORT_WORKORDER_CLOSED, 
		&CMainFrame::OnUpdateReportWorkorderClosed)
	ON_UPDATE_COMMAND_UI(ID_APP_VIEW,
		&CMainFrame::OnUpdateSearchHistory)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,
		&CMainFrame::OnUpdateIsPrintable)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT,
		&CMainFrame::OnUpdateIsPrintable)
	ON_UPDATE_COMMAND_UI(ID_WORKORDER_EXTRA_COMBI,
		&CMainFrame::OnUpdateWorkorderExtraCombi)
	ON_UPDATE_COMMAND_UI(ID_WORKORDER_EXTRA_INVOICE,
		&CMainFrame::OnUpdateWorkorderExtraInvoice)
	ON_COMMAND(ID_GENERAL_SHOW_LOGINBAR_CHECK,
		&CMainFrame::OnViewCaptionBar)
	ON_COMMAND(ID_GENERAL_ACCESIBILITY_HIGHLIGHT, 
		&CMainFrame::OnGeneralAccessibilityHighlight)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_SHOW_LOGINBAR_CHECK,
		&CMainFrame::OnUpdateGeneralShowLoginBarCheck)
	ON_UPDATE_COMMAND_UI(ID_APP_ADMIN,
		&CMainFrame::OnUpdateAppAdmin)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/* Overrides */

// PreCreateWindow is called before the window is created
// It is used to modify the window class or styles
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs)) return FALSE;
	return TRUE;
}
#ifdef _DEBUG
// AssertValid is called to check the validity of the object
void CMainFrame::AssertValid() const { CFrameWndEx::AssertValid(); }
// Dump is called to dump the object
void CMainFrame::Dump(CDumpContext& dc) const { CFrameWndEx::Dump(dc); }
#endif //_DEBUG

/* Messages */

// OnCreate is called when the window is created
// It is used to create the ribbon bar and the status bar
// - lpCreateStruct is a pointer to a CREATESTRUCT structure that contains information about the CWnd object being created
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CFrameWndEx::OnCreate(lpCreateStruct) == -1 ) return -1;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if ( !m_wndStatusBar.Create(this) ) {
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Create a caption bar:
	if ( !CreateCaptionBar() ) {
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	}

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
	m_wndRibbonBar.SetWindows7Look(TRUE);

	LoadSettings();

	theApp.m_SplashScreen.DestroyWindow();
	return 0;
}

// OnViewCaptionBar is called when the user clicks on hide show caption bar button
void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

// OnGeneralAccessibilityHighlight is called when the user selects an option from the combo box Highlight
// It is used to highlight the the ribbon button selection, there are 3 options:
// 1. Highlight ribbon buttons if there are more then one buttons in same panel
// 2. Highlight all ribbon buttons after selection
// 3. Use no highlight
void CMainFrame::OnGeneralAccessibilityHighlight()
{
	CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_GENERAL_ACCESIBILITY_HIGHLIGHT));
	auto selectedItem = pComboBox->GetCurSel();

	switch (selectedItem) {
	case 0:
		m_bHighlightCategories = true;
		m_bHighlightAll = false;
		break;
	case 1:
		m_bHighlightCategories = false;
		m_bHighlightAll = true;
		break;
	default:
		m_bHighlightCategories = false;
		m_bHighlightAll = false;
		break;
	}
	SaveSettings(selectedItem);
}

// OnOptions is called when the user clicks on the options button in the ribbon bar
void CMainFrame::OnOptions() {
	auto *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != nullptr);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

// OnFilePrint is called when the user clicks on the print button in the ribbon bar
void CMainFrame::OnFilePrint()
{
	if ( IsPrintPreview() ) PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
}

// OnFilePrintPreview is called when the user clicks on the print preview button in the ribbon bar
void CMainFrame::OnFilePrintPreview()
{
	if ( IsPrintPreview() ) PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);
}

// OnCaptionBarComboBoxEmployeeNameChange is called when the user selects an employee name from the combo box
void CMainFrame::OnCaptionBarComboBoxEmployeeNameChange()
{
	if ( m_pCmbCaptionBarEmployeeName == nullptr ) return;

	auto *pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();

	if ( m_pCmbCaptionBarEmployeeName->GetCurSel() != 0 ) {
		theApp.m_bIsIdle = false;
		pView->SendMessage(WM_UPDATEUISTATE, 1);
		m_wndStatusBar.SetInformation(theApp.ConvertIDToString(IDS_STATUSBAR_IDLE_UNLOCK));
	} else {
		theApp.m_bIsIdle = true;
		theApp.SetAdmin(false);
		pView->SendMessage(WM_UPDATEUISTATE, 0);
		m_wndStatusBar.SetInformation(theApp.ConvertIDToString(IDS_STATUSBAR_IDLE_LOCK));
		m_wndRibbonBar.HideAllContextCategories();
		m_wndRibbonBar.ForceRecalcLayout();
	}
}

/* Member methods*/

// CreateCaptionBar is called to create the caption bar
// It is used to create the caption bar and the combo box for the employee names
BOOL CMainFrame::CreateCaptionBar()
{
	if ( !m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE) ) {
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}

	m_wndCaptionBar.SetText(theApp.ConvertIDToString(IDS_CAPTION_TEXT), CMFCCaptionBar::ALIGN_LEFT);
	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	m_wndCaptionBar.SetImageToolTip(theApp.ConvertIDToString(IDS_CAPTION_IMAGE_TIP), 
		theApp.ConvertIDToString(IDS_CAPTION_IMAGE_TEXT));

	// Combo box for employee names
	m_pCmbCaptionBarEmployeeName->Create(WS_CHILD | WS_VISIBLE | EBS_READONLY | CBS_DROPDOWN, CRect(380, 6, 550, 100), &m_wndCaptionBar,
		IDC_CAPTION_COMBOBOX_EMPLOYEE_NAME);

	m_wndStatusBar.SetInformation(theApp.ConvertIDToString(IDS_STATUSBAR_LOADING));

	GetEmployeeList();

	m_wndStatusBar.SetInformation(theApp.ConvertIDToString(IDS_STATUSBAR_IDLE_LOCK));
	return TRUE;
}

// LoadSettings is called to load the settings from the ini file
void CMainFrame::LoadSettings() noexcept
{
	// Load init settings
	TCHAR szPath[MAX_PATH]{};
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
		PathAppendW(szPath, _T("artvabas\\repairCafeCureApp\\rcca.ini"));
		// Does destination exist?
		if (GetFileAttributesW(szPath)) {
			int selection = GetPrivateProfileIntW(_T("Settings"), _T("highlight"), 0, szPath);
			CMFCRibbonComboBox* pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_GENERAL_ACCESIBILITY_HIGHLIGHT));
			pComboBox->SelectItem(selection);
			PostMessageW(WM_COMMAND, ID_GENERAL_ACCESIBILITY_HIGHLIGHT);
		}
	}
}

// SaveSettings is called to save the settings to the ini file
void CMainFrame::SaveSettings(const int& nHighLight) const noexcept
{
	// Save the new init setting
	TCHAR szPath[MAX_PATH]{};
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
		PathAppendW(szPath, _T("artvabas"));
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(szPath)) CreateDirectoryW(szPath, NULL);
		PathAppendW(szPath, _T("repairCafeCureApp"));
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(szPath)) CreateDirectoryW(szPath, NULL);
		PathAppendW(szPath, _T("rcca.ini"));

		std::wstring selectedItemStr = std::to_wstring(nHighLight);
		WritePrivateProfileStringW(_T("Settings"), _T("highlight"), selectedItemStr.c_str(), szPath);
	}
}

// GetSelectedEmployee is called to get the selected employee name from the combo box
CString CMainFrame::GetSelectedEmployee() const noexcept {
	CString strEmployee;
	auto nIndex = m_pCmbCaptionBarEmployeeName->GetCurSel();
	if ( nIndex != CB_ERR && nIndex != 0 ) {
		auto nLength = m_pCmbCaptionBarEmployeeName->GetLBTextLen(nIndex);
		if ( nLength != CB_ERR ) {
			m_pCmbCaptionBarEmployeeName->GetLBText(nIndex, strEmployee.GetBuffer(nLength));
			strEmployee.ReleaseBuffer();
		}
	}
	return strEmployee;
}

// GetEmployeeList is called to get the employee names from the database
// It is used to get the employee names from the database and add them to the combo box
void CMainFrame::GetEmployeeList() noexcept
{
	theApp.BeginWaitCursor();
	
	m_pCmbCaptionBarEmployeeName->ResetContent();
	m_vecEmployeeList.clear();
	// Add names to comboBox

	m_pCmbCaptionBarEmployeeName->AddString(theApp.ConvertIDToString(IDS_CAPTION_SELECT_NAME));

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if (sql.CreateSQLConnection()) {

		SQLCHAR szName[50]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ _T("SELECT * FROM EMPLOYEE ORDER BY EMPLOYEE_NAME") };

		CString strName;
		bool bIsAdmin;

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if (retcode == SQL_SUCCESS) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
					MessageBoxW(theApp.ConvertIDToString(IDS_MSGBT_ERROR_FETCH_EMPLOYEE), theApp.ConvertIDToString(IDS_MSGBC_ERROR_CONNECT_DS), MB_ICONERROR);
					break;
				}
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					SQLGetData(hstmt, EMPLOYEE.EMPLOYEE_NAME, SQL_C_CHAR, szName, 50, &cbName);
					strName = static_cast<CString>(szName);
					SQLGetData(hstmt, EMPLOYEE.EMPLOYEE_ROLE_ADMIN, SQL_C_BIT, &bIsAdmin, 0, nullptr);
					m_pCmbCaptionBarEmployeeName->AddString(strName);
					m_vecEmployeeList.push_back(std::make_pair(strName, bIsAdmin));
				} else break;
			}
		}
		sql.CheckReturnCodeForClosing(retcode);
	}
	m_pCmbCaptionBarEmployeeName->SetCurSel(0);
	sql.CloseConnection();
	theApp.EndWaitCursor();
}

// OnUpdateCustomerView is called to highlight the customer view button in the ribbon bar,
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateCustomerView(CCmdUI* pCmdUI)
{
	if (m_bHighlightAll) {
		(theApp.GetActiveViewType() == VIEW_CUSTOMER)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);

}

// OnUpdateWorkorderViewOpen is called to highlight the workorder view open button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateWorkorderViewOpen(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetWorkorderViewType() == VIEW_WORKORDER_OPEN && theApp.GetActiveViewType() == VIEW_WORKORDER)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}

// OnUpdateWorkorderViewProgress is called to highlight the workorder view progress button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateWorkorderViewProgress(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetWorkorderViewType() == VIEW_WORKORDER_PROGRESS && theApp.GetActiveViewType() == VIEW_WORKORDER)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}

// OnUpdateWorkorderViewRepaired is called to highlight the workorder view repaired button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateWorkorderViewRepaired(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetWorkorderViewType() == VIEW_WORKORDER_REPAIRED && theApp.GetActiveViewType() == VIEW_WORKORDER)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}

// OnUpdateSearchHistory is called to highlight the search history button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateSearchHistory(CCmdUI* pCmdUI)
{
	if (m_bHighlightAll) {
		(theApp.GetActiveViewType() == VIEW_ASSET)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateReportViewFinanceTax(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetFinanceTaxViewType() == VIEW_CONTRIBUTON_REPORT && theApp.GetActiveViewType() == VIEW_REPORT_FINANCE_TAX)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}

// OnUpdateReportWorkorderPinTransaction is called to highlight the report workorder pin transaction button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateReportWorkorderPinTransaction(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetFinanceTaxViewType() == VIEW_PIN_TRANSACTION_REPORT && theApp.GetActiveViewType() == VIEW_REPORT_FINANCE_TAX)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}

// OnUpdateReportWorkorderCashTransaction is called to highlight the report workorder cash transaction button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateReportWorkorderCashTransaction(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetFinanceTaxViewType() == VIEW_CASH_TRANSACTION_REPORT && theApp.GetActiveViewType() == VIEW_REPORT_FINANCE_TAX)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	}
	else pCmdUI->SetCheck(FALSE);
}

// OnUpdateReportWorkorderTotalFlow is called to highlight the report workorder total flow button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateReportWorkorderTotalFlow(CCmdUI* pCmdUI)
{
	if (m_bHighlightCategories || m_bHighlightAll) {
		(theApp.GetActiveViewType() == VIEW_REPORT_FINANCE_TOTAL)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	}
	else pCmdUI->SetCheck(FALSE);
}

// OnUpdateReportWorkorderClosed is called to highlight the report workorder closed button in the ribbon bar.
// Depending on the settings, the button is highlighted or not.
void CMainFrame::OnUpdateReportWorkorderClosed(CCmdUI* pCmdUI)
{
	if (m_bHighlightAll) {
		(theApp.GetActiveViewType() == VIEW_REPORT_WORKORDER_CLOSED)
			? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	} else pCmdUI->SetCheck(FALSE);
}


// OnUpdateIsPrintable is called to update the print button in the ribbon bar
void CMainFrame::OnUpdateIsPrintable(CCmdUI* pCmdUI) {
	switch (theApp.GetActiveViewType())	{
		case VIEW_ASSET:
			pCmdUI->Enable(FALSE);
			break;
		case VIEW_CUSTOMER:
			pCmdUI->Enable(FALSE);
			break;
		case VIEW_WORKORDER:
			pCmdUI->Enable(TRUE);
			break;
		case VIEW_REPORT_FINANCE_TAX:
			pCmdUI->Enable(TRUE);
			break;
		case VIEW_REPORT_WORKORDER_CLOSED:
			pCmdUI->Enable(TRUE);
			break;
	}
}

// OnUpdateWorkorderExtraCombi is called to update the print combi button in the ribbon bar
void CMainFrame::OnUpdateWorkorderExtraCombi(CCmdUI* pCmdUI) {
	switch (theApp.GetWorkorderViewType()) {
		case VIEW_WORKORDER_OPEN:
			pCmdUI->Enable(FALSE);
			break;
		case VIEW_WORKORDER_PROGRESS:
		case VIEW_WORKORDER_REPAIRED:
			pCmdUI->Enable(TRUE);
			break;
	}
}

// OnUpdateWorkorderExtraInvoice is called to update the print invoice button in the ribbon bar
void CMainFrame::OnUpdateWorkorderExtraInvoice(CCmdUI* pCmdUI) {
	switch (theApp.GetWorkorderViewType()) {
	case VIEW_WORKORDER_OPEN:
	case VIEW_WORKORDER_PROGRESS:
		pCmdUI->Enable(FALSE);
		break;
	case VIEW_WORKORDER_REPAIRED:
		pCmdUI->Enable(TRUE);
		break;
	}
}

// OnUpdateGeneralShowLoginBarCheck toggle for show hide caption bar
// trigged when user check or uncheck the checkbox on ribbon bar -> General
void CMainFrame::OnUpdateGeneralShowLoginBarCheck(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

// OnUpdateAppAdmin is called to enable the admin button in the ribbon bar.
// Depending on the user, the button is enabled or not.
void CMainFrame::OnUpdateAppAdmin(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
	for each (std::pair<CString, bool>  employee in m_vecEmployeeList) {
		if (employee.first == GetSelectedEmployee()) {
			pCmdUI->Enable(employee.second ? TRUE : FALSE);
			break;
		}
	}
}