// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RepairCafeCureApp.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include <afxpriv.h>
#include "RepairCafeCureApp.h"
#include "MainFrm.h"

#include "RepairCafeCureAppDoc.h"
#include "RepairCafeCureAppView.h"

#include "CCustomerView.h"
#include "CWorkorderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace artvabas::rcc::ui;

// CRepairCafeCureApp

BEGIN_MESSAGE_MAP(CRepairCafeCureApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CRepairCafeCureApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_CUSTOMER_VIEW, &CRepairCafeCureApp::OnCustomerView)
	ON_COMMAND(ID_APP_VIEW, &CRepairCafeCureApp::OnAppView)
	ON_COMMAND(ID_WORKORDER_VIEW, &CRepairCafeCureApp::OnWorkorderView)
END_MESSAGE_MAP()


// CRepairCafeCureApp construction

CRepairCafeCureApp::CRepairCafeCureApp() noexcept
	: m_pAppView(NULL)
	, m_pCustomerView(NULL)
	, m_pWorkorderView(NULL)
	, m_dbConnection(new CDatabaseConnection())

{

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("RepairCafeCureApp.AppID.0.0.1.0"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	
}

CRepairCafeCureApp::~CRepairCafeCureApp()
{
	if (NULL != m_dbConnection)
	{
		delete m_dbConnection;
		m_dbConnection = NULL;
	}
}


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
		case VIEW_APP:
		default:
			pNewView = m_pAppView;
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
	return pActiveView;
}

// The one and only CRepairCafeCureApp object
CRepairCafeCureApp theApp;


// CRepairCafeCureApp initialization

BOOL CRepairCafeCureApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
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
		RUNTIME_CLASS(CRepairCafeCureAppView));
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
	m_pAppView = pView;

	
	m_pCustomerView = (CView*)new CCustomerView;
	if (NULL == m_pCustomerView) return FALSE;

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
	UINT viewCustomerID = AFX_IDW_PANE_FIRST + 1;
	UINT viewWorkorderID = AFX_IDW_PANE_FIRST + 2;
	CRect rect(0, 0, 0, 0); // Gets resized later.

	// Create the new view. The view persists for
	// the life of the application. The application automatically
	// deletes the view when the application is closed.
	m_pCustomerView->Create(NULL, _T("Customer"), WS_CHILD, rect, m_pMainWnd, viewCustomerID, &newContext);
	m_pWorkorderView->Create(NULL, _T("Workorder"), WS_CHILD, rect, m_pMainWnd, viewWorkorderID, &newContext);

	// When a document template creates a view, the WM_INITIALUPDATE
	// message is sent automatically. However, this code must
	// explicitly send the message, as follows.
	m_pCustomerView->SendMessage(WM_INITIALUPDATE, 0, 0);
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

int CRepairCafeCureApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CRepairCafeCureApp message handlers


// CAboutDlg dialog used for App About

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

// App command to run the dialog
void CRepairCafeCureApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
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

void CRepairCafeCureApp::LoadCustomState()
{
}

void CRepairCafeCureApp::SaveCustomState()
{
}

// CRepairCafeCureApp message handlers


void CRepairCafeCureApp::OnCustomerView()
{
	SwitchView(VIEW_CUSTOMER);
}


void CRepairCafeCureApp::OnAppView()
{
	SwitchView(VIEW_APP);
}


void CRepairCafeCureApp::OnWorkorderView()
{
	SwitchView(VIEW_WORKORDER);
}
