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
* Version: 0.0.1.0
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 02-05-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/

#include "pch.h"
#include "framework.h"
#include "RepairCafeCureApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace artvabas::sql;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

CMainFrame::CMainFrame() noexcept
{
	m_pCmbCaptionBarEmployeeName = new CComboBox();
}

CMainFrame::~CMainFrame()
{
	m_wndRibbonBar.HideAllContextCategories();
	if ( m_pCmbCaptionBarEmployeeName != nullptr ) {
		delete m_pCmbCaptionBarEmployeeName;
		m_pCmbCaptionBarEmployeeName = nullptr;
	}
}

/* Message handles bindings */
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CAPTION_BAR,
		&CMainFrame::OnViewCaptionBar)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
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
	ON_UPDATE_COMMAND_UI(ID_GENERAL_SHOW_LOGINBAR_CHECK,
		&CMainFrame::OnUpdateGeneralShowLoginbarCheck)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/* Overrides */

// PreCreateWindow is called before the window is created
// It is used to modify the window class or styles
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs)) return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
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

	theApp.m_SplashScreen.DestroyWindow();
	return 0;
}

// OnViewCaptionBar is called when the user clicks on hide show caption bar button
void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
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

// 
void CMainFrame::OnFilePrintPreview()
{
	if ( IsPrintPreview() ) PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);
}

// OnCaptionBarComboBoxEmployeeNameChange is called when the user selects an employee name from the combo box
void CMainFrame::OnCaptionBarComboBoxEmployeeNameChange()
{
	if ( m_pCmbCaptionBarEmployeeName == nullptr ) return;

	BOOL bNameValid;
	CString strTemp;
	auto *pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();

	if ( m_pCmbCaptionBarEmployeeName->GetCurSel() != 0 ) {
		pView->SendMessage(WM_UPDATEUISTATE, 1);
		bNameValid = strTemp.LoadString(IDS_STATUSBAR_IDLE_UNLOCK);
		ASSERT(bNameValid);
		m_wndStatusBar.SetInformation(strTemp);
	} else {
		pView->SendMessage(WM_UPDATEUISTATE, 0);
		bNameValid = strTemp.LoadString(IDS_STATUSBAR_IDLE_LOCK);
		ASSERT(bNameValid);
		m_wndStatusBar.SetInformation(strTemp);
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

	BOOL bNameValid;

	CString strTemp, strTemp2;

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	// Combo box for employee names
	m_pCmbCaptionBarEmployeeName->Create(WS_CHILD | WS_VISIBLE | EBS_READONLY | CBS_DROPDOWN, CRect(350, 6, 550, 100), &m_wndCaptionBar,
		IDC_CAPTION_COMBOBOX_EMPLOYEE_NAME);

	bNameValid = strTemp.LoadString(IDS_STATUSBAR_LOADING);
	ASSERT(bNameValid);
	m_wndStatusBar.SetInformation(strTemp);

	theApp.BeginWaitCursor();

	// Add names to comboBox
	m_pCmbCaptionBarEmployeeName->AddString(_T(">> Select your name <<"));

	CSqlNativeAVB sql{ theApp.GetDatabaseConnection()->ConnectionString() };

	if ( sql.CreateSQLConnection() ) {

		SQLCHAR szName[50]{};
		SQLLEN cbName{};
		SQLRETURN retcode{};
		SQLHSTMT hstmt{ sql.GetStatementHandle() };
		SQLWCHAR* strQuery{ _T("SELECT EMPLOYEE_NAME FROM EMPLOYEE ORDER BY EMPLOYEE_NAME") };

		retcode = SQLExecDirectW(hstmt, strQuery, SQL_NTS);

		if ( retcode == SQL_SUCCESS ) {
			while (TRUE) {
				retcode = SQLFetch(hstmt);
				if ( retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO )
					AfxMessageBox(_T("Error fetching data from Employee Table!"), MB_ICONEXCLAMATION);
				if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
					SQLGetData(hstmt, 1, SQL_C_CHAR, szName, 50, &cbName);
					m_pCmbCaptionBarEmployeeName->AddString(static_cast<CString>(szName));
				}
				else
					break;
			}
		}
		sql.CheckReturnCodeForClosing(retcode);
	}
	sql.CloseConnection();

	m_pCmbCaptionBarEmployeeName->SetCurSel(0);

	theApp.EndWaitCursor();

	bNameValid = strTemp.LoadString(IDS_STATUSBAR_IDLE_LOCK);
	ASSERT(bNameValid);
	m_wndStatusBar.SetInformation(strTemp);
	return TRUE;
}

// GetSelectedEmployee is called to get the selected employee name from the combo box
CString CMainFrame::GetSelectedEmployee() {
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
	}
}

// OnUpdateWorkorderExtraCombi is called to update the print combi button in the ribbon bar
void CMainFrame::OnUpdateWorkorderExtraCombi(CCmdUI* pCmdUI) {
	switch (theApp.GetWorkorderViewType()) {
		case VIEW_WORKORDER_OPEN:
			//pCmdUI->Enable(FALSE);
			//break;
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

// OnUpdateGeneralShowLoginbarCheck toggle for show hide caption bar
// trigged when user check or uncheck the checkbox on ribbinbar -> General
void CMainFrame::OnUpdateGeneralShowLoginbarCheck(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}