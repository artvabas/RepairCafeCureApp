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
* Version: 1.0.569.0
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 29-02-2024, (dd-mm-yyyy)
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

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

CMainFrame::CMainFrame() noexcept {
	m_pCmbCaptionBarEmployeeName = new CComboBox();
}

CMainFrame::~CMainFrame() {
	if (m_pCmbCaptionBarEmployeeName != nullptr) {
		delete m_pCmbCaptionBarEmployeeName;
		m_pCmbCaptionBarEmployeeName = nullptr;
	}
}

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
END_MESSAGE_MAP()

/* Overrides */

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs)) return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}
	
// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid() const { CFrameWndEx::AssertValid(); }
void CMainFrame::Dump(CDumpContext& dc) const { CFrameWndEx::Dump(dc); }
#endif //_DEBUG

/* Messages */

/// <summary>
/// This function is called when the main window is created.
/// It creates the ribbon bar and the status bar.
/// It also creates the caption bar, the ribbon bar 
/// and the status bar. 
/// </summary>
/// <param name="lpCreateStruct"></param>
/// <returns>int -1 not succeed, 0 succeed</returns>
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this)) {
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Create a caption bar:
	if (!CreateCaptionBar()) {
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	}

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
	m_wndRibbonBar.SetWindows7Look(TRUE);

	return 0;
}

/// <summary>
/// This function is called when the user clicks on the caption bar button.
/// It shows or hides the caption bar.
/// </summary>
void CMainFrame::OnViewCaptionBar() {
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

/// <summary>
/// This function is called when the user clicks on the caption bar button.
/// It shows or hides the caption bar.
/// </summary>
/*
void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}
*/

/// <summary>
/// This function is called when the user clicks on the options button in the ribbon bar.
/// It shows the options dialog.
/// </summary>
void CMainFrame::OnOptions() {
	auto *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != nullptr);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

/// <summary>
/// This function is called when the user clicks on the print button in the ribbon bar.
/// It shows the print dialog.
/// </summary>
void CMainFrame::OnFilePrint() {
	if (IsPrintPreview()) PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
}

/// <summary>
/// This function is called when the user clicks on the print preview button in the ribbon bar.
/// It shows the print preview dialog.
/// </summary>
void CMainFrame::OnFilePrintPreview() {
	if (IsPrintPreview()) PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);
}

//void CMainFrame::OnUpdateIsPrintable(CCmdUI* pCmdUI)
//{
//	pCmdUI->SetCheck(IsPrintPreview());
//}

/// <summary>
/// This function is called when the user changes the selected item in the combobox on the caption bar
/// Get the active view and update the data
/// </summary>
void CMainFrame::OnCaptionBarComboBoxEmployeeNameChange() {
	if (m_pCmbCaptionBarEmployeeName == nullptr) return;

	BOOL bNameValid;
	CString strTemp;
	auto *pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();

	if (m_pCmbCaptionBarEmployeeName->GetCurSel() != 0) {
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

/* General methods*/

/// <summary>
/// This function is called by the OnCreate method.
/// It creates the caption bar.
/// </summary>
/// <returns>BOOL TRUE if succeed, FALSE if not succeed</returns>
BOOL CMainFrame::CreateCaptionBar() {
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS , this, ID_VIEW_CAPTION_BAR, -1, TRUE))	{
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

	//*******************************************************************************
	// Make a new ComboBox
	// Make ComboBox visible
	m_pCmbCaptionBarEmployeeName->Create(WS_CHILD | WS_VISIBLE | EBS_READONLY | CBS_DROPDOWN, CRect(350, 6, 550, 100), &m_wndCaptionBar,
		IDC_CAPTION_COMBOBOX_EMPLOYEE_NAME);

	bNameValid = strTemp.LoadString(IDS_STATUSBAR_LOADING);
	ASSERT(bNameValid);
	m_wndStatusBar.SetInformation(strTemp);

	// Add items to comboBox
	m_pCmbCaptionBarEmployeeName->AddString(_T(">> Select your name <<"));
	auto *rs = new CRecordset();
	CString strQuery{ _T("SELECT EMPLOYEE_NAME FROM EMPLOYEE ORDER BY EMPLOYEE_NAME") };
	theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery);
	while (!rs->IsEOF()) {
		CString strValue{ _T("") };
		rs->GetFieldValue(_T("EMPLOYEE_NAME"), strValue);
		m_pCmbCaptionBarEmployeeName->AddString(strValue);
		rs->MoveNext();
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	m_pCmbCaptionBarEmployeeName->SetCurSel(0);

	bNameValid = strTemp.LoadString(IDS_STATUSBAR_IDLE_LOCK);
	ASSERT(bNameValid);
	m_wndStatusBar.SetInformation(strTemp);
	//*******************************************************************************

	return TRUE;
}

/// <summary>
/// This function is called by the App class, for giving back the selected
/// employee name from the combobox on the caption bar.
/// </summary>
/// <returns>CString the selected employee name</returns>
CString CMainFrame::GetSelectedEmployee() {
	CString strEmployee;
	auto nIndex = m_pCmbCaptionBarEmployeeName->GetCurSel();
	if (nIndex != CB_ERR && nIndex != 0) {
		auto nLength = m_pCmbCaptionBarEmployeeName->GetLBTextLen(nIndex);
		if (nLength != CB_ERR) {
			m_pCmbCaptionBarEmployeeName->GetLBText(nIndex, strEmployee.GetBuffer(nLength));
			strEmployee.ReleaseBuffer();
		}
	}
	return strEmployee;
}

void CMainFrame::OnUpdateIsPrintable(CCmdUI* pCmdUI) {
	switch (theApp.GetActiveViewType())	{
		case VIEW_ASSET:
			pCmdUI->Enable(TRUE);
			break;
		case VIEW_CUSTOMER:
			pCmdUI->Enable(FALSE);
			break;
		case VIEW_WORKORDER:
			pCmdUI->Enable(TRUE);
			break;
	}
}

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

void CMainFrame::OnUpdateGeneralShowLoginbarCheck(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}
