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
* File: RepairCafeCureApp.cpp, defines class CRepairCafeCureApp
*
* This class is the main application class. It is used to create the database connection and
* to switch between the views.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 23-10-2023, (dd-mm-yyyy)
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
#include "RepairCafeCureApp.h"	// is Asset view
#include "MainFrm.h"

#include "RepairCafeCureAppDoc.h"
#include "CAssetView.h"

#include "CCustomerView.h"
#include "CWorkorderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace artvabas::rcc::ui;

/* Globals */
CRepairCafeCureApp theApp; // The one and only CRepairCafeCureApp object

/* Messages Maps */
BEGIN_MESSAGE_MAP(CRepairCafeCureApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CRepairCafeCureApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_CUSTOMER_VIEW, &CRepairCafeCureApp::OnCustomerView)
	ON_COMMAND(ID_APP_VIEW, &CRepairCafeCureApp::OnAssetView)
	ON_COMMAND(ID_WORKORDER_VIEW, &CRepairCafeCureApp::OnWorkorderView)
END_MESSAGE_MAP()

CRepairCafeCureApp::CRepairCafeCureApp() noexcept
	: m_pAssetView(NULL)
	, m_pCustomerView(NULL)
	, m_pWorkorderView(NULL)
	, m_dbConnection(new CDatabaseConnection())
{
	SetAppID(_T("RepairCafeCureApp.AppID.0.0.1.0"));
}

CRepairCafeCureApp::~CRepairCafeCureApp()
{
	if (NULL != m_dbConnection)
	{
		delete m_dbConnection;
		m_dbConnection = NULL;
	}
}

/* Overrides */
// CRepairCafeCureApp initialization
BOOL CRepairCafeCureApp::InitInstance()
{
	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("artvabas\\Repair cafe cure"));
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
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	/*************************************************************/

	// Get a pointer to the current view
	CView* pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	m_pCustomerView = pView;


	m_pAssetView = (CView*)new CAssetView;
	if (NULL == m_pAssetView) return FALSE;

	m_pWorkorderView = (CView*)new CWorkorderView;
	if (NULL == m_pWorkorderView) return FALSE;

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
	CRect rect(0, 0, 0, 0); // Gets resized later.

	// Create the new view. The view persists for
	// the life of the application. The application automatically
	// deletes the view when the application is closed.
	m_pAssetView->Create(NULL, _T("Asset"), WS_CHILD, rect, m_pMainWnd, viewAssetID, &newContext);
	m_pWorkorderView->Create(NULL, _T("Workorder"), WS_CHILD, rect, m_pMainWnd, viewWorkorderID, &newContext);

	// When a document template creates a view, the WM_INITIALUPDATE
	// message is sent automatically. However, this code must
	// explicitly send the message, as follows.
	m_pAssetView->SendMessage(WM_INITIALUPDATE, 0, 0);
	m_pWorkorderView->SendMessage(WM_INITIALUPDATE, 0, 0);

	/*************************************************************/

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

// CRepairCafeCureApp exit instance 
int CRepairCafeCureApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CRepairCafeCureApp customization load/save methods
void CRepairCafeCureApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

// CRepairCafeCureApp customization load/save methods
void CRepairCafeCureApp::LoadCustomState()
{
}

// CRepairCafeCureApp customization load/save methods
void CRepairCafeCureApp::SaveCustomState()
{
}

/*Messages Handlers*/

/// <summary>
/// This method is called when the user clicks on the Customer menu item.
/// It switches the view to the Customer view.
/// </summary>
void CRepairCafeCureApp::OnCustomerView()
{
	SwitchView(VIEW_CUSTOMER);

	// Get a pointer to the main frame window.
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrm != NULL)
	{
		// Update the Customer view controls, depending on the current selection of the employee name combo box on the caption bar.
		pMainFrm->OnCaptionBarComboBoxEmployeeNameChange();
	}
}

/// <summary>
/// This method is called when the user clicks on the App menu item.
/// It switches the view to the App view.
/// </summary>
void CRepairCafeCureApp::OnAssetView()
{
	SwitchView(VIEW_ASSET);
}

/// <summary>
/// This method is called when the user clicks on the Workorder menu item.
/// It switches the view to the Workorder view.
/// </summary>
void CRepairCafeCureApp::OnWorkorderView()
{
	SwitchView(VIEW_WORKORDER);
}

/*Custom methods*/

/// <summary>
/// This method is used to switch between the views.
/// </summary>
/// <param name="vtView">The view to switch to.</param>
/// <returns>CView* The new view.</returns>
CView* CRepairCafeCureApp::SwitchView(ViewType vtView)
{
	CView* pActiveView = ((CFrameWnd*)m_pMainWnd)->GetActiveView();
	CView* pNewView = NULL;

	switch (vtView)
	{
		case VIEW_CUSTOMER:
			pNewView = m_pCustomerView;
			break;
		case VIEW_WORKORDER:
			pNewView = m_pWorkorderView;
			break;
		case VIEW_ASSET:
		default:
			pNewView = m_pAssetView;
			break;
	}

#ifndef _WIN32
	UINT temp = ::GetWindowWord(pActiveView->m_hWnd, GWW_ID);
	::SetWindowWord(pActiveView->m_hWnd, GWW_ID, ::GetWindowWord(pNewView->m_hWnd, GWW_ID));
	::SetWindowWord(pNewView->m_hWnd, GWW_ID, temp);
#else
	UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
	::SetWindowLong(pActiveView->m_hWnd, GWL_ID, ::GetWindowLong(pNewView->m_hWnd, GWL_ID));
	::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
#endif

	pActiveView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_SHOW);
	((CFrameWnd*)m_pMainWnd)->SetActiveView(pNewView);
	((CFrameWnd*)m_pMainWnd)->RecalcLayout();
	pNewView->Invalidate();
	return pNewView;// pActiveView;
}
//**************************************************************************************************************

/*
* This class is part of RepairCafeCureApp.
* class: CAboutDlg, defines class CAboutDlg
*
* This class is used to show the about dialog.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 23-10-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/// <summary>
/// This method is called when the user clicks on the About menu item.
/// It shows the about dialog.
/// </summary>
void CRepairCafeCureApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
